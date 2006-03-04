/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2006 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



//BibleTime includes
#include "cswordmoduleinfo.h"
#include "cswordbackend.h"
#include "cswordmodulesearch.h"
#include "cswordkey.h"
#include "centrydisplay.h"
#include "clanguagemgr.h"

#include "util/scoped_resource.h"
#include "util/directoryutil.h"
#include "util/cpointers.h"
#include "frontend/cbtconfig.h"

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <dirent.h>
#include <regex.h>

//Qt includes
#include <qregexp.h>
#include <qdir.h>
#include <qvariant.h>
#include <qfileinfo.h>

//KDE includes
#include <klocale.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <kconfig.h>

//Sword includes
#include <swbuf.h>
#include <swkey.h>
#include <listkey.h>
#include <versekey.h>
#include <swconfig.h>
#include <rtfhtml.h>

//Lucence includes
#include "CLucene.h"
#include "CLucene/util/Reader.h"
#include "CLucene/util/Misc.h"
#include "CLucene/util/dirent.h"

using namespace std;
using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::util;
using namespace lucene::store;
using namespace lucene::document;
using namespace lucene::queryParser;
using namespace lucene::search;

using std::string;

#define INDEX_VERSION "1"

CSwordModuleInfo::CSwordModuleInfo(sword::SWModule * module, CSwordBackend * const usedBackend) {
	m_module = module;
	Q_ASSERT(module);

	m_searchResult.ClearList();
	m_backend = usedBackend ? usedBackend : CPointers::backend();
	m_dataCache.name = module ? QString(module->Name()) : QString::null;
	m_dataCache.isUnicode = module ? module->isUnicode() : false;
	m_dataCache.category = UnknownCategory;
	m_dataCache.language = 0;
	m_dataCache.hasVersion = !QString((*m_backend->getConfig())[module->Name()]["Version"]).isEmpty();

	if (backend()) {
		if (hasVersion() && (minimumSwordVersion() > sword::SWVersion::currentVersion)) {
			qWarning("The module \"%s\" requires a newer Sword library. Please update to \"Sword %s\".",
					 name().latin1(), (const char *)minimumSwordVersion());
		}
	}
}

CSwordModuleInfo::CSwordModuleInfo(const CSwordModuleInfo & m) {
	m_module = m.m_module;
	m_backend = m.m_backend;
	m_dataCache = m.m_dataCache;
	m_searchResult = m.m_searchResult;
}

/** No descriptions */
CSwordModuleInfo *CSwordModuleInfo::clone() {
	return new CSwordModuleInfo(*this);
}

CSwordModuleInfo::~CSwordModuleInfo() {
	m_searchResult.ClearList();
	m_module = 0;    //the Sword module object is deleted by the backend
}

/** Sets the unlock key of the modules and writes the key into the cofig file.*/
const bool CSwordModuleInfo::unlock(const QString & unlockKey) {
	if (!isEncrypted()) {
		return false;
	}

	CBTConfig::setModuleEncryptionKey(name(), unlockKey);
	backend()->setCipherKey(m_module->Name(), unlockKey.latin1());

	return true;
}

/** This function returns true if this module is locked, otherwise return false. */
const bool CSwordModuleInfo::isLocked() {
	//still works, but the cipherkey is stored in CBTConfig.
	//Works because it is set in sword on program startup.

	if (isEncrypted() && config(CipherKey).isEmpty()) {
		return true;
	}

	return false;
}

/** This functions returns true if this module is encrypted (locked or unlocked). */
const bool CSwordModuleInfo::isEncrypted() const {
	/**
	* If we have the CipherKey entry the module
	* is encrypted but not necessarily locked
	*/

	//This code is still right, though we do no longer write to the module config files any more
	sword::ConfigEntMap config = backend()->getConfig()->Sections.find(name().latin1())->second;
	sword::ConfigEntMap::iterator it = config.find("CipherKey");

	if (it != config.end()) {
		return true;
	}

	return false;
}

const bool CSwordModuleInfo::unlockKeyIsValid(){

	(*m_module) = sword::TOP;
	QString test = QString::fromLatin1( m_module->getRawEntryBuf().c_str() );
	if (test.isEmpty()){
		qWarning("unlock key of module %s is NOT valid", name().latin1());
		return false;
	}
	for (int i=0; i <= test.length(); i++){
		if ( !test[i].isPrint() && !test[i].isNull() ){
			qWarning("unlock key of module %s is NOT valid", name().latin1());
			return false;
		}
	}
	qWarning("unlock key of module %s is valid", name().latin1());
	return true;
}

const QString CSwordModuleInfo::getGlobalBaseIndexLocation(){
	return KGlobal::dirs()->saveLocation("data", "bibletime/indices");
}

const QString CSwordModuleInfo::getModuleBaseIndexLocation() const {
	return getGlobalBaseIndexLocation() + QString("/") + name().ascii();
}

const QString CSwordModuleInfo::getModuleStandardIndexLocation() const { //this for now returns the location of the main index
	 return getModuleBaseIndexLocation() + QString("/standard");
}

const bool CSwordModuleInfo::hasIndex() { //this will return true only 
	//if the index exists and has correct version information for both index and module

	QDir d;
	if (!d.exists( getModuleStandardIndexLocation() )) {
		return false;
	}
	
	//first check if the index version and module version are ok
	util::scoped_ptr<KConfig> indexconfig(
		new KConfig( getModuleBaseIndexLocation() + QString("/bibletime-index.conf") )
	);

	if (hasVersion()){
		if (indexconfig->readEntry("module-version") != QString(config(CSwordModuleInfo::ModuleVersion)) ){
			return false;
		}
	}
	if (indexconfig->readEntry("index-version") != QString( INDEX_VERSION )){
		qDebug("%s: INDEX_VERSION is not compatible with this version of BibleTime.", name().latin1());
		return false;
	}

	//then check if the index is there
	return IndexReader::indexExists(getModuleStandardIndexLocation().ascii());
}


void CSwordModuleInfo::buildIndex() {
	//Without this we don't get strongs, lemmas, etc
	backend()->setFilterOptions ( CBTConfig::getFilterOptionDefaults() );
	
	lucene::analysis::standard::StandardAnalyzer an;
	QString index = getModuleStandardIndexLocation();

	QDir dir;
	dir.mkdir( getGlobalBaseIndexLocation(), true );
	dir.mkdir( getModuleBaseIndexLocation(), true );
	dir.mkdir( getModuleStandardIndexLocation(), true );

	
	if (IndexReader::indexExists(index.ascii())){
		if (IndexReader::isLocked(index.ascii()) ){
			IndexReader::unlock(index.ascii());
		}
	}
	
	util::scoped_ptr<IndexWriter> writer( new IndexWriter(index.ascii(), &an, true) ); //always create a new index
	writer->setMaxFieldLength(IndexWriter::DEFAULT_MAX_FIELD_LENGTH);
	writer->setUseCompoundFile(true);
	writer->setMinMergeDocs(3200);
	
	long verseIndex, verseLowIndex, verseHighIndex = 1;
	*m_module = sword::TOP;
	verseLowIndex = m_module->Index();
	*m_module = sword::BOTTOM;
	verseHighIndex = m_module->Index();

	bool isVerseModule = (type() == CSwordModuleInfo::Bible) || (type() == CSwordModuleInfo::Commentary);
	
	m_indexingProgress.setValue( QVariant((int)0) );
	
	for (*m_module = sword::TOP; !m_module->Error(); (*m_module)++) {
		Document* doc = new Document();
		
		// index the key
		// we have to be sure to insert the english key into the index, otherwise we'd be in trouble if the
		//language changes
		QString key = QString::fromUtf8(m_module->getKey()->getText());
		if (isVerseModule) {
			VerseKey vk( m_module->getKey()->getText() );
			vk.setLocale("en_US");
			key = QString::fromUtf8(vk.getText());
		}
		//qWarning("key is %s", key.latin1());
		lucene_utf8towcs(m_wcharBuffer, (const char*)key.utf8(), MAX_CONV_SIZE);
		doc->add(*Field::UnIndexed(_T("key"), m_wcharBuffer));

		// index the main text
		lucene_utf8towcs(m_wcharBuffer, m_module->StripText(), MAX_CONV_SIZE);
		doc->add(*Field::UnStored(_T("content"), m_wcharBuffer));

		// index attributes
		AttributeList::iterator attListI;
		AttributeValue::iterator attValueI;
		// Footnotes
		for (attListI = m_module->getEntryAttributes()["Footnote"].begin();
			attListI != m_module->getEntryAttributes()["Footnote"].end();
			attListI++) {
				lucene_utf8towcs(m_wcharBuffer, attListI->second["body"], MAX_CONV_SIZE);
				doc->add(*Field::UnStored(_T("footnote"), m_wcharBuffer));
		} // for attListI
		
		// Headings
		for (attValueI = m_module->getEntryAttributes()["Heading"]["Preverse"].begin();
			attValueI != m_module->getEntryAttributes()["Heading"]["Preverse"].end();
			attValueI++) {
			lucene_utf8towcs(m_wcharBuffer, attValueI->second, MAX_CONV_SIZE);
			doc->add(*Field::UnStored(_T("heading"), m_wcharBuffer));
		} // for attValueI

		// Strongs/Morphs
		for (attListI = m_module->getEntryAttributes()["Word"].begin();
			attListI != m_module->getEntryAttributes()["Word"].end();
			attListI++) {
			// for each attribute
			if (attListI->second["LemmaClass"] == "strong") {
				lucene_utf8towcs(m_wcharBuffer, attListI->second["Lemma"], MAX_CONV_SIZE);
				doc->add(*Field::UnStored(_T("strong"), m_wcharBuffer));
				//qWarning("Adding strong %s", attListI->second["Lemma"].c_str());
			}
			if (attListI->second.find("Morph") != attListI->second.end()) {
				lucene_utf8towcs(m_wcharBuffer, attListI->second["Morph"], MAX_CONV_SIZE);
				doc->add(*Field::UnStored(_T("morph"), m_wcharBuffer));
			}
		} // for attListI
		
		writer->addDocument(doc);
		delete doc;
		verseIndex = m_module->Index();

		if (verseIndex % 200 == 0){
			if (verseHighIndex == verseLowIndex) { //prevent division by zero
				m_indexingProgress.setValue( QVariant(0) );
			}
			else {
				m_indexingProgress.setValue( QVariant((int)((100*(verseIndex-verseLowIndex))/(verseHighIndex-verseLowIndex))) );
			}
			
			m_indexingProgress.activate();
		}
	}

	writer->optimize();
	writer->close();

	QString configFilename = getModuleStandardIndexLocation() + QString("/../bibletime-index.conf");
	util::scoped_ptr<KConfig> indexconfig( new KConfig( configFilename ) );
	if (hasVersion()){
		indexconfig->writeEntry("module-version", config(CSwordModuleInfo::ModuleVersion) );
	}
	indexconfig->writeEntry("index-version", INDEX_VERSION);

}

void CSwordModuleInfo::deleteIndexForModule( QString name ) {
	util::filesystem::DirectoryUtil::removeRecursive( getGlobalBaseIndexLocation() + "/" + name );
}

unsigned long CSwordModuleInfo::indexSize() const {
	return util::filesystem::DirectoryUtil::getDirSizeRecursive( getModuleBaseIndexLocation() );
}

const bool CSwordModuleInfo::searchIndexed(const QString& searchedText, sword::ListKey& scope)
{
	// work around Swords thread insafety for Bibles and Commentaries
	util::scoped_ptr < CSwordKey > key(CSwordKey::createInstance(this));
	sword::SWKey* s = dynamic_cast < sword::SWKey * >(key.get());

	if (s) {
		m_module->SetKey(*s);
	}

	m_searchResult.ClearList();
	
	try {
		standard::StandardAnalyzer analyzer;
		IndexSearcher searcher(getModuleStandardIndexLocation().ascii());
		lucene_utf8towcs(m_wcharBuffer, searchedText.utf8(), MAX_CONV_SIZE);
		util::scoped_ptr<Query> q( QueryParser::parse(m_wcharBuffer, _T("content"), &analyzer) );

		util::scoped_ptr<Hits> h( searcher.search(q) );
		//TODO
		//h = searcher.search(q, Sort::INDEXORDER); //Should return keys in the right order, doesn't work properly with CLucene 0.9.10, will work with 0.9.11

		const bool useScope = (scope.Count() > 0);
		Document* doc = 0;
		util::scoped_ptr<SWKey> swKey( module()->CreateKey() );
		
		for (int i = 0; i < h->length(); ++i) {
			doc = &h->doc(i);
			lucene_wcstoutf8(m_utfBuffer, doc->get(_T("key")), MAX_CONV_SIZE);
			
			swKey->setText(m_utfBuffer);
			
		    // limit results based on scope
			//if (searchOptions & CSwordModuleSearch::useScope && scope.Count() > 0){
			if (useScope) {
				for (int j = 0; j < scope.Count(); j++) {
					VerseKey* vkey = dynamic_cast<VerseKey*>(scope.getElement(j));
					if (vkey->LowerBound().compare(*swKey) <= 0 && vkey->UpperBound().compare(*swKey) >= 0){
						m_searchResult.add(*swKey);
					}
				}
			}
			else { // no scope, give me all buffers
				m_searchResult.add(*swKey);
			}
		}	
	}
	catch (...) {
		qWarning("CLucene exception");
		return false;
	}
	
	return (m_searchResult.Count() > 0);
}

void CSwordModuleInfo::connectIndexingFinished(QObject* receiver, const char* slot) {
	m_indexingFinished.connect(receiver, slot);
}

void CSwordModuleInfo::connectIndexingProgress(QObject* receiver, const char* slot) {
	m_indexingProgress.connect(receiver, slot);
}

void CSwordModuleInfo::disconnectIndexingSignals(QObject* receiver) {
	m_indexingProgress.disconnect(receiver);
	m_indexingFinished.disconnect(receiver);
}

/** Returns the last search result for this module. */
sword::ListKey & CSwordModuleInfo::searchResult(const sword::ListKey * newResult) {
	if (newResult) {
		m_searchResult.copyFrom(*newResult);
	}

	return m_searchResult;
}

/** Clears the last search result. */
void CSwordModuleInfo::clearSearchResult() {
	m_searchResult.ClearList();
}

/** Returns the required Sword version for this module. Returns -1 if no special Sword version is required. */
const sword::SWVersion CSwordModuleInfo::minimumSwordVersion() {
	return sword::SWVersion(config(CSwordModuleInfo::MinimumSwordVersion).latin1());
}

const QString CSwordModuleInfo::config(const CSwordModuleInfo::ConfigEntry entry) const {
	switch (entry) {

	case AboutInformation: {
		return getFormattedConfigEntry("About");
		}

		case CipherKey: {
			if (CBTConfig::getModuleEncryptionKey(name()).isNull()) { //fall back!
				return QString(m_module->getConfigEntry("CipherKey"));
			}
			else {
				return CBTConfig::getModuleEncryptionKey(name());
			};
		}

		case AbsoluteDataPath: {
			QString path( getSimpleConfigEntry("AbsoluteDataPath") );
			path.replace(QRegExp("/./"), "/"); // make /abs/path/./modules/ looking better
			//make sure we have a trailing slash!

			if (path.right(1) != "/") {
				path.append('/');
			}

			return path;
		}

		case DataPath: { //make sure we remove the dataFile part if it's a Lexicon
			QString path(getSimpleConfigEntry("DataPath"));

			if ((type() == CSwordModuleInfo::GenericBook) || (type() == CSwordModuleInfo::Lexicon)) {
				int pos = path.findRev("/"); //last slash in the string

				if (pos != -1) {
					path = path.left(pos + 1); //include the slash
				}
			}

			return path;
		}

		case Description:
		return getFormattedConfigEntry("Description");

		case ModuleVersion: {
			QString version(getSimpleConfigEntry("Version"));

			if (version.isEmpty()) {
				version = "1.0";
			}

			return version;
		}

		case MinimumSwordVersion: {
			const QString minimumVersion(getSimpleConfigEntry("MinimumVersion"));
			return !minimumVersion.isEmpty()? minimumVersion : QString("0.0");
		}

		case TextDir: {
			const QString dir(getSimpleConfigEntry("Direction"));
			return !dir.isEmpty()? dir : QString("LtoR");
		}

		case DisplayLevel: {
			const QString level(getSimpleConfigEntry("DisplayLevel"));
			return !level.isEmpty()? level : QString("1");
		}

		case GlossaryFrom: {
			if (!category() == Glossary) {
				return QString::null;
			};

			const QString lang(getSimpleConfigEntry("GlossaryFrom"));

			return !lang.isEmpty()? lang : QString::null;
		}

		case GlossaryTo: {
			if (!category() == Glossary) {
				return QString::null;
			};

			const QString lang(getSimpleConfigEntry("GlossaryTo"));

			return !lang.isEmpty()? lang : QString::null;
		}

		case Markup: {
			const QString markup(getSimpleConfigEntry("SourceType"));
			return !markup.isEmpty()? markup : QString("Unknown");
		}

		case DistributionLicense:
		return getSimpleConfigEntry("DistributionLicense");

		case DistributionSource:
		return getSimpleConfigEntry("DistributionSource");

		case DistributionNotes:
		return getSimpleConfigEntry("DistributionNotes");

		case TextSource:
		return getSimpleConfigEntry("TextSource");

		case CopyrightNotes:
		return getSimpleConfigEntry("CopyrightNotes");

		case CopyrightHolder:
		return getSimpleConfigEntry("CopyrightHolder");

		case CopyrightDate:
		return getSimpleConfigEntry("CopyrightDate");

		case CopyrightContactName:
		return getSimpleConfigEntry("CopyrightContactName");

		case CopyrightContactAddress:
		return getSimpleConfigEntry("CopyrightContactAddress");

		case CopyrightContactEmail:
		return getSimpleConfigEntry("CopyrightContactEmail");

		default:
		return QString::null;
	}
}

/** Returns true if the module supports the feature given as parameter. */
const bool CSwordModuleInfo::has(const CSwordModuleInfo::Feature feature) const {
	switch (feature) {

// 		case StrongsNumbers:
// 		return m_module->getConfig().has("Feature", "StrongsNumber");

		case GreekDef:
		return m_module->getConfig().has("Feature", "GreekDef");

		case HebrewDef:
		return m_module->getConfig().has("Feature", "HebrewDef");

		case GreekParse:
		return m_module->getConfig().has("Feature", "GreekParse");

		case HebrewParse:
		return m_module->getConfig().has("Feature", "HebrewParse");
	}

	return false;
}

const bool CSwordModuleInfo::has(const CSwordModuleInfo::FilterTypes option) const {
	//BAD workaround to see if the filter is GBF, OSIS or ThML!
	const QString name = backend()->configOptionName(option);

	if (m_module->getConfig().has("GlobalOptionFilter", QString("OSIS").append(name).latin1())) {
		return true;
	}

	if (m_module->getConfig().has("GlobalOptionFilter", QString("GBF").append(name).latin1())) {
		return true;
	}

	if (m_module->getConfig().has("GlobalOptionFilter", QString("ThML").append(name).latin1())) {
		return true;
	}

	if (m_module->getConfig().has("GlobalOptionFilter", QString("UTF8").append(name).latin1())) {
		return true;
	}

	if (m_module->getConfig().has("GlobalOptionFilter", name.latin1())) {
		return true;
	}

	return false;
}

/** Returns the text direction of the module's text., */
const CSwordModuleInfo::TextDirection CSwordModuleInfo::textDirection() {
	if (config(TextDir) == "RtoL") {
		return CSwordModuleInfo::RightToLeft;
	}
	else {
		return CSwordModuleInfo::LeftToRight;
	}
}

/** Writes the new text at the given position into the module. This does only work for writable modules. */
void CSwordModuleInfo::write(CSwordKey * key, const QString & newText) {
	module()->KeyText((const char *)key->key().utf8());

	//don't store a pointer to the const char* value somewhere because QCString doesn't keep the value of it
	module()->setEntry(isUnicode()? (const char *)newText.utf8() : (const char *)newText.local8Bit());
}

/** Deletes the current entry and removes it from the module. */
const bool CSwordModuleInfo::deleteEntry(CSwordKey * const key) {
	module()->KeyText(isUnicode()? (const char *)key->key().utf8() : (const char *)key->key().local8Bit());

	if (module()) {
		module()->deleteEntry();
		return true;
	};

	return false;
}

/** Returns the category of this module. See CSwordModuleInfo::Category for possible values. */
const CSwordModuleInfo::Category CSwordModuleInfo::category() const {
	if (m_dataCache.category == CSwordModuleInfo::UnknownCategory) {
	const QString cat(m_module->getConfigEntry("Category"));

		if (cat == "Cults / Unorthodox / Questionable Material") {
			m_dataCache.category = Cult;
		}
		else if (cat == "Daily Devotional" || m_module->getConfig().has("Feature", "DailyDevotion")) {
			m_dataCache.category = DailyDevotional;
		}
		else if (cat == "Glossaries" || m_module->getConfig().has("Feature", "Glossary")) { //alow both
			m_dataCache.category = Glossary;
		};
	}

	return m_dataCache.category;
}

/** Returns the display object for this module. */
Rendering::CEntryDisplay * const CSwordModuleInfo::getDisplay() const {
	return dynamic_cast < Rendering::CEntryDisplay * >(m_module->Disp());
}

QString CSwordModuleInfo::aboutText() const {
	QString text;
	text += "<font size=\"-1\"><table>";

	text += QString("<tr><td><b>%1</b></td><td>%2</td><tr>")
			.arg(i18n("Version"))
			.arg(hasVersion()? config(CSwordModuleInfo::ModuleVersion) : i18n("unknown"));

	text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
			.arg(i18n("Markup"))
			.arg(!QString(m_module->getConfigEntry("SourceType")).isEmpty()? m_module->
				 getConfigEntry("SourceType") : i18n("unknown"));

	text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
			.arg(i18n("Location"))
			.arg(config(CSwordModuleInfo::AbsoluteDataPath));

	text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
			.arg(i18n("Language"))
			.arg(language()->translatedName());

	if (m_module->getConfigEntry("Category"))
	text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
			.arg(i18n("Category"))
			.arg(m_module->getConfigEntry("Category"));

	if (m_module->getConfigEntry("LCSH"))
		text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
				.arg(i18n("LCSH"))
				.arg(m_module->getConfigEntry("LCSH"));

		text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
				.arg(i18n("Writable"))
				.arg(isWritable()? i18n("yes") : i18n("no"));

		if (isEncrypted())
			text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
					.arg(i18n("Unlock key"))
					.arg(config(CSwordModuleInfo::CipherKey));

			QString options;

			unsigned int opts;

			for (opts = CSwordModuleInfo::filterTypesMIN; opts <= CSwordModuleInfo::filterTypesMAX; ++opts) {
				if (has(static_cast < CSwordModuleInfo::FilterTypes > (opts))) {
						if (!options.isEmpty()) {
							options += QString::fromLatin1(", ");
						}

						options += CSwordBackend::translatedOptionName(static_cast < CSwordModuleInfo::FilterTypes > (opts));
					}
				}

	if (!options.isEmpty()) {
	text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
				.arg(i18n("Features"))
				.arg(options);
	}

	text += "</table><hr>";

	if (category() == Cult) //clearly say the module contains cult/questionable materials
	text += QString("<br/><b>%1</b><br/><br/>")
			.arg(i18n("Take care, this work contains cult / questionable material!"));

	text += QString("<b>%1:</b><br><font size=\"-1\">%2</font>")
			.arg(i18n("About"))
			.arg(config(AboutInformation));

	typedef QValueList<CSwordModuleInfo::ConfigEntry> ListConfigEntry;

	ListConfigEntry entries;

	entries.append(DistributionLicense);

	entries.append(DistributionSource);

	entries.append(DistributionNotes);

	entries.append(TextSource);

	entries.append(CopyrightNotes);

	entries.append(CopyrightHolder);

	entries.append(CopyrightDate);

	entries.append(CopyrightContactName);

	entries.append(CopyrightContactAddress);

	entries.append(CopyrightContactEmail);

	typedef QMap<CSwordModuleInfo::ConfigEntry, QString> MapConfigEntry;

	MapConfigEntry entryMap;

	entryMap[DistributionLicense] = i18n("Distribution license");

	entryMap[DistributionSource] = i18n("Distribution source");

	entryMap[DistributionNotes] = i18n("Distribution notes");

	entryMap[TextSource] = i18n("Text source");

	entryMap[CopyrightNotes] = i18n("Copyright notes");

	entryMap[CopyrightHolder] = i18n("Copyright holder");

	entryMap[CopyrightDate] = i18n("Copyright date");

	entryMap[CopyrightContactName] = i18n("Copyright contact name");

	entryMap[CopyrightContactAddress] = i18n("Copyright contact address");

	entryMap[CopyrightContactEmail] = i18n("Copyright contact email");

	text += ("<hr><table>");

	for (ListConfigEntry::iterator it(entries.begin()); it != entries.end(); ++it) {
		QString t( config(*it) );

			if (!t.isEmpty()) {
				text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
						.arg(entryMap[*it])
						.arg(config(*it));
			}

		}


	text += "</table></font>";

	return text;
}

/** Returns the language of the module. */
const CLanguageMgr::Language * const CSwordModuleInfo::language() const {
	if (!m_dataCache.language) {
	if (module()) {
			if (category() == Glossary) {
				//special handling for glossaries, we use the "from language" as language for the module
				m_dataCache.language = (CPointers::languageMgr())->languageForAbbrev(config(GlossaryFrom));
			}
			else {
				m_dataCache.language = (CPointers::languageMgr())->languageForAbbrev(module()->Lang());
			}
		}
		else {
			m_dataCache.language = (CPointers::languageMgr())->defaultLanguage(); //default language
		}
	}

	return m_dataCache.language;
}


/*!
    \fn CSwordModuleInfo::getSimpleConfigEntry(char* name)
 */
QString CSwordModuleInfo::getSimpleConfigEntry(const QString& name) const {
	QString ret = isUnicode()
				  ? QString::fromUtf8(m_module->getConfigEntry(name.latin1()))
				  : QString::fromLatin1(m_module->getConfigEntry(name.latin1()));

	return ret.isEmpty() ? QString::null : ret;
}

QString CSwordModuleInfo::getFormattedConfigEntry(const QString& name) const {
	SWBuf RTF_Buffer(m_module->getConfigEntry(name.latin1()));
	sword::RTFHTML RTF_Filter;
	RTF_Filter.processText(RTF_Buffer, 0, 0);
	QString ret = isUnicode()
				  ? QString::fromUtf8(RTF_Buffer.c_str())
				  : QString::fromLatin1(RTF_Buffer.c_str());

	return ret.isEmpty() ? QString::null : ret;
}
