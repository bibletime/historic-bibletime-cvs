/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2006 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



#include "csearchdialogpages.h"

#include "csearchdialog.h"
#include "csearchanalysis.h"
#include "crangechooser.h"
#include "cmoduleresultview.h"
#include "csearchresultview.h"

#include "backend/cswordversekey.h"
#include "backend/cdisplayrendering.h"

#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"

#include "util/cresmgr.h"
#include "util/ctoolclass.h"

//Qt includes
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qregexp.h>
#include <qeventloop.h>

//KDE includes
#include <klocale.h>
#include <kcombobox.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kprogress.h>
#include <kapplication.h>


namespace Search {
	namespace Result {

/********************************************
************  StrongsResulClass *************
********************************************/
void StrongsResultClass::initStrongsResults(void) {
   using namespace Rendering;
   
   CDisplayRendering render;
   ListCSwordModuleInfo modules;
   CTextRendering::KeyTreeItem::Settings settings;
   QString rText, lText, key;
   bool found;
   int sIndex;
   int count;
   int index;
   QString text;

   modules.append(srModule);
   sword::ListKey& result = srModule->searchResult();

   count = result.Count();
   if (!count)
      return;

   srList.clear();

	KProgressDialog* progress = new KProgressDialog(0, "progressDialog", i18n("Parsing Stong's Numbers"), QString::null, true);
	progress->setAllowCancel(false);

   for (index = 0; index < count; index++){
// 	  if (index % 100 == 0){
		progress->progressBar()->setProgress( int( (index*100) / count ) );
		KApplication::kApplication()->processEvents( QEventLoop::AllEvents );
		qWarning("percent: %d", int( (index*100) / count ) );
// 	  }
      key = QString::fromUtf8(result.GetElement(index)->getText());
      text = render.renderSingleKey(key, modules, settings);
      sIndex = 0;
      while ((rText = getStrongsNumberText(text, &sIndex)) != "")
         {
         StrongsResultList::iterator it;
         found = FALSE;
         for ( it = srList.begin(); it != srList.end(); ++it )
            {
            lText = (*it).keyText();
            if (lText == rText)
               {
               found = TRUE;
               (*it).addKeyName(key);
               break;
               }
            }
         if (found == FALSE)
            srList.append( StrongsResult(rText, key) );
         }
      }
	  delete progress;
	  progress = 0;
   //qHeapSort(srList);
   }

QString StrongsResultClass::getStrongsNumberText(const QString& verseContent, int *startIndex) {
   // get the strongs text
   int idx1, idx2, index;
   QString sNumber, strongsText;
   //const bool cs = CSwordModuleSearch::caseSensitive;
   const bool cs = false;

   if (*startIndex == 0) {
      index = verseContent.find("<body", 0);
   }
   else {
      index = *startIndex;
   }
   
	// find all the "lemma=" inside the the content
	while((index = verseContent.find("lemma=", index, cs)) != -1) {
		// get the strongs number after the lemma and compare it with the
		// strongs number we are looking for
		idx1 = verseContent.find("\"", index) + 1;
		idx2 = verseContent.find("\"", idx1 + 1);
		sNumber = verseContent.mid(idx1, idx2 - idx1);
		if (sNumber == lemmaText) {
			// strongs number is found now we need to get the text of this node
			// search right until the ">" is found.  Get the text from here to
			// the next "<".
			index = verseContent.find(">", index, cs) + 1;
			idx2  = verseContent.find("<", index, cs);
			strongsText = verseContent.mid(index, idx2 - index);
			index = idx2;
			*startIndex = index;
 
			return(strongsText);
		}
		else {
			index += 6; // 6 is the length of "lemma="
		}
   }
   return QString::null;
}

/********************************************
**********  CSearchDialogResultPage *********
********************************************/

CSearchResultPage::CSearchResultPage(QWidget *parent, const char *name ) : 
		SearchResultsForm(parent, name) {
	initView();
	initConnections();
}

CSearchResultPage::~CSearchResultPage() {}

/** Initializes the view of this widget. */
void CSearchResultPage::initView() {
	QVBoxLayout* frameLayout = new QVBoxLayout(m_displayFrame, 0, 6, "frameLayout");
	m_previewDisplay = CDisplay::createReadInstance(0, m_displayFrame);
	frameLayout->addWidget(m_previewDisplay->view());
}

/** Sets the modules which contain the result of each. */
void CSearchResultPage::setSearchResult(ListCSwordModuleInfo modules) {
   const QString searchedText = CSearchDialog::getSearchDialog()->searchText();
	reset(); //clear current modules

	m_modules = modules;
	m_moduleListBox->setupTree(modules, searchedText);

	//have a Bible or commentary in the modules?
	bool enable = false;
	//   for (modules.first(); !enable && modules.current(); modules.next()) {
	ListCSwordModuleInfo::iterator end_it = modules.end();
	for (ListCSwordModuleInfo::iterator it(modules.begin()); it != end_it; ++it) {
		if ((*it)->type() == CSwordModuleInfo::Bible ) {
			enable = true;
			break;
		};
	};

	m_analyseButton->setEnabled(enable);
}


/** Resets the current list of modules and the displayed list of found entries. */
void CSearchResultPage::reset() {
	m_moduleListBox->clear();
	m_resultListBox->clear();
	m_previewDisplay->setText(QString::null);
	m_analyseButton->setEnabled(false);
	//   m_modules.setAutoDelete(false); //make sure we don't delete modules accidentally
	m_modules.clear();
}


/** Update the preview of the selected key. */
void CSearchResultPage::updatePreview(const QString& key) {
	using namespace Rendering;

	CSwordModuleInfo* module = m_moduleListBox->activeModule();
	if ( module ) {
		const QString searchedText = CSearchDialog::getSearchDialog()->searchText();
		//const int searchFlags = CSearchDialog::getSearchDialog()->searchFlags();

		QString text;
		CDisplayRendering render;

		ListCSwordModuleInfo modules;
		modules.append(module);

		CTextRendering::KeyTreeItem::Settings settings;

		//for bibles only render 5 verses, for all other modules only one entry
		if (module->type() == CSwordModuleInfo::Bible) {
			CSwordVerseKey vk(module);
			vk.key(key);

			//first go back and then go forward the keys to be in context
			vk.previous(CSwordVerseKey::UseVerse);
			vk.previous(CSwordVerseKey::UseVerse);
			const QString startKey = vk.key();

			vk.key(key);

			vk.next(CSwordVerseKey::UseVerse);
			vk.next(CSwordVerseKey::UseVerse);
			const QString endKey = vk.key();

			//    qWarning("want to render from %s to %s", startKey.latin1(), endKey.latin1());
			//now render the range
			settings.keyRenderingFace = CTextRendering::KeyTreeItem::Settings::CompleteShort;
			text = render.renderKeyRange(startKey, endKey, modules, key, settings);
			//    qWarning(text.latin1());
		}
		else {
			text = render.renderSingleKey(key, modules, settings);
		}

		m_previewDisplay->setText( highlightSearchedText(text, searchedText/*, searchFlags*/) );
		m_previewDisplay->moveToAnchor( CDisplayRendering::keyToHTMLAnchor(key) );
	}
}

const QString CSearchResultPage::highlightSearchedText(const QString& content, const QString& searchedText/*, const int searchFlags*/) {
	QString ret = content;

	//const bool cs = (searchFlags & CSwordModuleSearch::caseSensitive);
	const bool cs = false;
	
	//   int index = 0;
	int index = ret.find("<body", 0);
	int length = searchedText.length();

	const QString rep1("<span style=\"background-color:#FFFF66;\">");
	const QString rep2("</span>");
	const unsigned int repLength = rep1.length() + rep1.length();
   	int sstIndex; // strong search text index for finding "strong:"

   //---------------------------------------------------------------------
   // find the strongs search lemma and highlight it
   //---------------------------------------------------------------------
   // search the searched text for "strong:" until it is not found anymore
   sstIndex = 0;
   while ((sstIndex = searchedText.find("strong:", sstIndex)) != -1)
      {
      int idx1, idx2, sTokenIndex;
      QString sNumber, lemmaText;
      const QString rep3("style=\"background-color:#FFFF66;\" ");
      const unsigned int rep3Length = rep3.length();
      int strongIndex = index;
      //--------------------------------------------------
      // get the strongs number from the search text
      //--------------------------------------------------
      // first find the first space after "strong:"
      //    this should indicate a change in search token
      sstIndex = sstIndex + 7;
      sTokenIndex = searchedText.find(" ", sstIndex);
      sNumber = searchedText.mid(sstIndex, sTokenIndex - sstIndex);
      // find all the "lemma=" inside the the content
      while((strongIndex = ret.find("lemma=", strongIndex, cs)) != -1)
         {
         // get the strongs number after the lemma and compare it with the
         // strongs number we are looking for
         idx1 = ret.find("\"", strongIndex) + 1;
         idx2 = ret.find("\"", idx1 + 1);
         lemmaText = ret.mid(idx1, idx2 - idx1);
         if (lemmaText == sNumber)
            {
            // strongs number is found now we need to highlight it
            // I believe the easiest way is to insert rep3 just before "lemma="
            ret = ret.insert(strongIndex, rep3);
            strongIndex += rep3Length;
            }
         strongIndex += 6; // 6 is the length of "lemma="
         }
      }
   //---------------------------------------------------------------------
   // now that the strong: stuff is out of the way continue with
   // other search options
   //---------------------------------------------------------------------
/*	if (searchFlags & CSwordModuleSearch::exactPhrase) { //exact phrase matching
		while ( (index = ret.find(searchedText, index, cs)) != -1 ) {
			if (!CToolClass::inHTMLTag(index, ret)) {
				ret = ret.insert( index+length, rep2 );
				ret = ret.insert( index, rep1 );
				index += repLength;
			}
			index += repLength;
		};
	}
	else if (searchFlags & CSwordModuleSearch::multipleWords) { //multiple words*/
		QStringList words = QStringList::split(" ", searchedText);
		for ( int wi = 0; (unsigned int)wi < words.count(); ++wi ) { //search for every word in the list
			QString word = words[ wi ];
			if ((word == "AND") || (word == "OR"))
				continue;
			length = word.length();
			//       index = 0; //for every word start at the beginning
			index = ret.find("<body", 0);
			while ( (index = ret.find(word, index, cs)) != -1 ) { //while we found the word
				if (!CToolClass::inHTMLTag(index, ret)) {
					ret = ret.insert( index+length, rep2 );
					ret = ret.insert( index, rep1 );
					index += repLength;
				}
				index += length;
			}
		}
// 	}
// 	else { //multiple words or regular expression
// 		//use re as regular expression and replace any occurences
// 		QRegExp regExp( searchedText, cs );
// 		regExp.setMinimal( true );
// 
// 		while ( (index = regExp.search(ret, index)) != -1 ) {
// 			if (!CToolClass::inHTMLTag(index, ret)) {
// 				ret = ret.insert( index + regExp.matchedLength(), rep2 );
// 				ret = ret.insert( index, rep1 );
// 				index += regExp.matchedLength() + repLength;
// 			}
// 			index += length;
// 		}
// 	}

	//   qWarning("\n\n\n%s", ret.latin1());

	return ret; //not implemented yet
};

/** Initializes the signal slot conections of the child widgets, */
void CSearchResultPage::initConnections() {
	connect(m_resultListBox, SIGNAL(keySelected(const QString&)),
			this, SLOT(updatePreview(const QString&)));
	connect(m_moduleListBox, SIGNAL(moduleSelected(CSwordModuleInfo*)),
			m_resultListBox, SLOT(setupTree(CSwordModuleInfo*)));
	connect(m_moduleListBox, SIGNAL(moduleChanged()),
			m_previewDisplay->connectionsProxy(), SLOT(clear()));
	connect(m_analyseButton, SIGNAL(clicked()), SLOT(showAnalysis()));
   // connect the strongs list
   connect(m_moduleListBox, SIGNAL(strongsSelected(CSwordModuleInfo*, QStringList*)),
         m_resultListBox, SLOT(setupStrongsTree(CSwordModuleInfo*, QStringList*)));
}

/** Shows a dialog with the search analysis of the current search. */
void CSearchResultPage::showAnalysis() {
	Analysis::CSearchAnalysisDialog dlg(m_modules, this);
	dlg.exec();
}

	} //end of namespace Search.Result

	/*************************/

	namespace Options {

CSearchOptionsPage::CSearchOptionsPage(QWidget *parent, const char *name ) :
	SearchOptionsForm(parent,name) {
	initView();
	readSettings();
}

CSearchOptionsPage::~CSearchOptionsPage() {
	saveSettings();
}

/** Returns the search text set in this page. */
const QString CSearchOptionsPage::searchText() {
	return m_searchTextCombo->currentText();
}

/** Sets the search text used in the page. */
void CSearchOptionsPage::setSearchText(const QString& text) {
	bool found = false;
	int i = 0;
	for (i = 0; !found && i < m_searchTextCombo->count(); ++i) {
		if (m_searchTextCombo->text(i) == text) {
			found = true;
		}
	}
   // This is needed because in the for loop i is incremented before the comparison (++i)
   // As a result the index i is actually one greater than expected.
   i--;
	if (!found) {
		i = 0;
		m_searchTextCombo->insertItem( text,0 );
	}

	m_searchTextCombo->setCurrentItem(i);
	m_searchTextCombo->reset();
	m_searchTextCombo->setFocus();
}

/** Initializes this page. */
void CSearchOptionsPage::initView() {
	connect( m_searchTextCombo, SIGNAL(activated( const QString& )),
				m_searchTextCombo, SLOT( addToHistory( const QString& ))
			);
	connect( m_searchTextCombo, SIGNAL(returnPressed ( const QString& )),
				m_searchTextCombo, SLOT(addToHistory(const QString&))
			);
	QToolTip::add(m_searchTextCombo, CResMgr::searchdialog::options::searchedText::tooltip);

	m_syntaxButton->setIconSet(SmallIconSet("contexthelp"));
	connect( m_syntaxButton, SIGNAL(clicked()), this, SLOT(syntaxHelp()));

	m_chooseModulesButton->setIconSet(SmallIconSet("wizard"));
	connect(m_chooseModulesButton, SIGNAL(clicked()),
			this, SLOT(chooseModules()));
	QToolTip::add
		(m_chooseModulesButton, CResMgr::searchdialog::options::moduleChooserButton::tooltip);
		
	QToolTip::add
		(m_rangeChooserCombo, CResMgr::searchdialog::options::chooseScope::tooltip);
	refreshRanges();

	m_chooseRangeButton->setIconSet(SmallIconSet("configure"));
	connect(m_chooseRangeButton, SIGNAL(clicked()),
			this, SLOT(setupRanges()));

	//set the initial focus
	m_searchTextCombo->setFocus();
}

/** Sets the modules used by the search. */
void CSearchOptionsPage::setModules( ListCSwordModuleInfo modules ) {
	QString t = i18n("Searching in: ");

	m_modules.clear(); //remove old modules
	ListCSwordModuleInfo::iterator end_it = modules.end();

	for (ListCSwordModuleInfo::iterator it(modules.begin()); it != end_it; ++it) {
		//ToDo:  Check for containsRef compat
		if (*it == 0) { //don't operate on null modules.
			continue;
		}
		
		if ( !m_modules.contains(*it) ) {
			m_modules.append( *it );
			t.append( (*it)->name() );
			if (*it != modules.last()) {
				t += QString::fromLatin1(", "); // so that it will become a readable list (WLC, LXX, GerLut...)
			}
		}
	};
	m_modulesLabel->setText(t);
	emit( sigSetSearchButtonStatus( (modules.count() != 0) ) );
}

/** Opens the modules chooser dialog. */
void CSearchOptionsPage::chooseModules() {
	CModuleChooserDialog* dlg = new CModuleChooserDialog(this, modules());
	connect(dlg, SIGNAL(modulesChanged(ListCSwordModuleInfo)),
			this, SLOT(setModules(ListCSwordModuleInfo)));
	dlg->exec();
}

/** Returns the list of used modules. */
const ListCSwordModuleInfo CSearchOptionsPage::modules() {
	return m_modules;
}

/** Sets all options back to the default. */
void CSearchOptionsPage::reset() {
	m_rangeChooserCombo->setCurrentItem(0); //no scope
	m_searchTextCombo->clearEdit();
}

/** Reads the settings for the searchdialog from disk. */
void CSearchOptionsPage::saveSettings() {
	CBTConfig::set
		(CBTConfig::searchCompletionTexts, m_searchTextCombo->completionObject()->items());
	CBTConfig::set
		(CBTConfig::searchTexts, m_searchTextCombo->historyItems());
}

/** Reads the settings of the last searchdialog session. */
void CSearchOptionsPage::readSettings() {
	m_searchTextCombo->completionObject()->setItems( CBTConfig::get
				(CBTConfig::searchCompletionTexts) );
	m_searchTextCombo->setHistoryItems( CBTConfig::get
											(CBTConfig::searchTexts) );
}

void CSearchOptionsPage::aboutToShow() {
	m_searchTextCombo->setFocus();
}

/** No descriptions */
void CSearchOptionsPage::setupRanges() {
	CRangeChooserDialog* chooser = new CRangeChooserDialog(this);
	chooser->exec();

	refreshRanges();
}

/** No descriptions */
void CSearchOptionsPage::syntaxHelp() {
	QString syntax = i18n (
			"<p>Enter search terms separated by spaces.  By default the search "
			"function will return results that match any of the search terms (OR). "
			"To search for all the terms separate the terms by AND.</p>") + i18n(
			"<p>You can use wildcards: '*' matches any sequence of characters, "
			"while '?' matches any single character. The use of brackets allows you to "
			"group your search terms, e.g. '(Jesus OR spirit) AND God'.</p>") + i18n(
			"<p>To search text other than the main text, enter the text type followed "
			"by \":\", and then the search term.  For example, to search for the Strong's "
			"number H8077, use 'strong:H8077'.</p>") + i18n(
			"<p>Available text types:<br><table>"
			"<tr><td>heading:</td><td>searches headings</td></tr>"
			"<tr><td>footnote:</td><td>searches footnotes</td></tr>"
			"<tr><td>strong:</td><td>searches Strong's Numbers</td></tr>"
			"<tr><td>morph:</td><td>searches morphology codes</td></tr></table></p>") + i18n(
			"<p>BibleTime uses the Lucene search engine to perform your searches. "
			"It has many advanced features, and you can read more about it here: "
			"<a href=\"http://lucene.apache.org/java/docs/queryparsersyntax.html\">"
			"http://lucene.apache.org/java/docs/queryparsersyntax.html</a></p>");

	KMessageBox::about( this, syntax, i18n("Basic Search Syntax Introduction"));
}


/** refreshes the listof ranges and the range combobox. */
void CSearchOptionsPage::refreshRanges() {
	//the first two options are fixed, the others can be edited using the "Setup ranges" button.
	m_rangeChooserCombo->clear();
	m_rangeChooserCombo->insertItem(i18n("No search scope"));
	//m_rangeChooserCombo->insertItem(i18n("Last search result"));

	//insert the user-defined ranges
	CBTConfig::StringMap map = CBTConfig::get
								   (CBTConfig::searchScopes);
	CBTConfig::StringMap::Iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
		m_rangeChooserCombo->insertItem(it.key());
	};
}

/** Returns the selected search scope if a search scope was selected. */
sword::ListKey CSearchOptionsPage::searchScope() {
	if (m_rangeChooserCombo->currentItem() > 0) { //is not "no scope"
		CBTConfig::StringMap map = CBTConfig::get(CBTConfig::searchScopes);
		
		QString scope = map[ m_rangeChooserCombo->currentText() ];
		if (!scope.isEmpty()) {
			return sword::VerseKey().ParseVerseList( (const char*)scope.utf8(), "Genesis 1:1", true);
		}
	}
	
	return sword::ListKey();
}

/*!
    \fn CSearchOptionsPage::hasSearchScope()
 */
bool CSearchOptionsPage::hasSearchScope() {
	return (searchScope().Count() > 0);
}

	} //end of namespace Options
} //end of namespace Search
