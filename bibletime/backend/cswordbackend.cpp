/***************************************************************************
                          cswordbackend.cpp  -  description
                             -------------------
    begin                : Thu Oct 5 2000
    copyright            : (C) 2000 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//BibleTime includes
#include "cswordbackend.h"
#include "chtmlentrydisplay.h"
#include "chtmlchapterdisplay.h"
#include "chtmlbookdisplay.h"
#include "cswordbiblemoduleinfo.h"
#include "cswordcommentarymoduleinfo.h"
#include "cswordlexiconmoduleinfo.h"
#include "cswordbookmoduleinfo.h"

#include "bt_thmlhtml.h"
#include "bt_gbfhtml.h"
#include <dirent.h>
#include <unistd.h>

//Qt includes
#include <qdir.h>

//KDE includes
#include <klocale.h>

//Sword includes
#include <swdisp.h>
#include <swfiltermgr.h>
#include <encfiltmgr.h>
#include <plainhtml.h>
#include <rawgbf.h>
#include <rtfhtml.h>
#include <filemgr.h>
#include <utilstr.h>

//static class-wide members
static QMap<QString, QString> moduleDescriptionMap;

CSwordBackend::CSwordBackend()
	: SWMgr(0,0,false,new EncodingFilterMgr( ENC_UTF8 ))
{	
	m_displays.entry = 0;
	m_displays.chapter = 0;
	m_displays.book = 0;

	m_filters.gbf = 0;
	m_filters.thml = 0;
	m_filters.plain = 0;
}

CSwordBackend::~CSwordBackend(){
	shutdownModules();	
	delete m_filters.gbf;
	delete m_filters.plain;	
	delete m_filters.thml;	
}

/** Initializes the Sword modules. */
const CSwordBackend::LoadError CSwordBackend::initModules() {
	LoadError ret = NoError;

	ModMap::iterator it;
	SWModule*	curMod = 0;
	CSwordModuleInfo* newModule = 0;
		 	
 	shutdownModules(); //remove previous modules
 	m_moduleList.clear();	

	ret = LoadError( Load() );
	
	for (it = Modules.begin(); it != Modules.end(); it++) {
		curMod = (*it).second;
		if (!strcmp(curMod->Type(), "Biblical Texts")) {
			newModule = new CSwordBibleModuleInfo(curMod);
			newModule->module()->Disp(m_displays.chapter ? m_displays.chapter : (m_displays.chapter = new CHTMLChapterDisplay));
		} else if (!strcmp(curMod->Type(), "Commentaries")) {
			newModule = new CSwordCommentaryModuleInfo(curMod);
			newModule->module()->Disp(m_displays.entry ? m_displays.entry : (m_displays.entry = new CHTMLEntryDisplay));
		} else if (!strcmp(curMod->Type(), "Lexicons / Dictionaries")) {
			newModule = new CSwordLexiconModuleInfo(curMod);
			newModule->module()->Disp(m_displays.entry ? m_displays.entry : (m_displays.entry = new CHTMLEntryDisplay));
		} else if (!strcmp(curMod->Type(), "Generic Books")) {
			newModule = new CSwordBookModuleInfo(curMod);
			newModule->module()->Disp(m_displays.book ? m_displays.book : (m_displays.book = new CHTMLBookDisplay));
		}
		if (newModule)	//append the new modules to our list
			m_moduleList.append( newModule );
	}

	for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
		moduleDescriptionMap.insert(m_moduleList.current()->config(CSwordModuleInfo::Description), m_moduleList.current()->name());
	}

	return ret;
}

void CSwordBackend::AddRenderFilters(SWModule *module, ConfigEntMap &section) {
	string sourceformat;
	string moduleDriver;
	ConfigEntMap::iterator entry;
	bool noDriver = true;

	sourceformat = ((entry = section.find("SourceType")) != section.end()) ? (*entry).second : (string) "";
	moduleDriver = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : (string) "";

	if (!stricmp(sourceformat.c_str(), "GBF")) {
		if (!m_filters.gbf)
			m_filters.gbf = new BT_GBFHTML();
		module->AddRenderFilter(m_filters.gbf);
		noDriver = false;
	}

	if (!stricmp(sourceformat.c_str(), "PLAIN")) {
		if (!m_filters.plain)
			m_filters.plain = new PLAINHTML();	
		module->AddRenderFilter(m_filters.plain);
		noDriver = false;
	}

	if (!stricmp(sourceformat.c_str(), "ThML")) {
		if (!m_filters.thml)
			m_filters.thml = new BT_ThMLHTML();
		module->AddRenderFilter(m_filters.thml);
		noDriver = false;
	}

	if (noDriver){
		if (!stricmp(moduleDriver.c_str(), "RawCom") || !stricmp(moduleDriver.c_str(), "RawLD")) {
			if (!m_filters.plain)
				m_filters.plain = new PLAINHTML();
			module->AddRenderFilter(m_filters.plain);
			noDriver = false;
		}
	}
}

/** This function deinitializes the modules and deletes them. */
const bool CSwordBackend::shutdownModules(){
	for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
		if (m_moduleList.current()) {
			CSwordModuleInfo* current = m_moduleList.current();
			m_moduleList.removeRef(current);
			delete current;
		}		
	}
	m_moduleList.clear();	
	return true;
}

/** Returns true if the given option is enabled. */
const bool CSwordBackend::isOptionEnabled( const CSwordBackend::FilterOptions type) {
	return (getGlobalOption( optionName(type).latin1() ) == "On");
}

/** Sets the given options enabled or disabled depending on the second parameter. */
void CSwordBackend::setOption( const CSwordBackend::FilterOptions type, const bool enable){
	string value;
	switch (type) {
		case textualVariants:
			value = enable ? "Secondary Reading" : "Primary Reading";
			//if on, user alternaticve (secondary) reading
			break;
		default:		
			value = enable ? "On": "Off";
			break;
	};
	setGlobalOption(optionName(type).latin1(), value.c_str());
}

void CSwordBackend::setFilterOptions( const CSwordBackend::FilterOptionsBool options){
  setOption( footnotes, 					options.footnotes );
  setOption( strongNumbers, 			options.strongNumbers );
  setOption( headings, 						options.headings );
  setOption( morphTags, 					options.morphTags );
	setOption( lemmas, 							options.lemmas );
	setOption( hebrewPoints, 				options.hebrewPoints );
	setOption( hebrewCantillation, 	options.hebrewCantillation );
	setOption( greekAccents, 				options.greekAccents);
	setOption( textualVariants,			options.textualVariants);	
}

void CSwordBackend::setDisplayOptions( const CSwordBackend::DisplayOptionsBool options){
  if (m_displays.entry)
		m_displays.entry->setDisplayOptions(options);	
  if (m_displays.chapter)
		m_displays.chapter->setDisplayOptions(options);	
}

/** This function searches for a module with the specified description */
CSwordModuleInfo* const CSwordBackend::findModuleByDescription(const QString& description){
  if (m_moduleList.count())
    for ( m_moduleList.first();m_moduleList.current();m_moduleList.next() )
      if ( m_moduleList.current()->config(CSwordModuleInfo::Description) == description )
        return m_moduleList.current();
  return 0;
}

/** This function searches for a module with the specified description */
const QString CSwordBackend::findModuleNameByDescription(const QString& description){
	if (moduleDescriptionMap.contains(description))
		return moduleDescriptionMap[description];
  return QString::null;
}

/** This function searches for a module with the specified name */
CSwordModuleInfo* const CSwordBackend::findModuleByName(const QString& name){
  if (m_moduleList.count())
    for ( m_moduleList.first(); m_moduleList.current(); m_moduleList.next() )
      if ( m_moduleList.current()->name() == name )
        return m_moduleList.current();
  return 0;
}

/** Returns our local config object to store the cipher keys etc. locally for each user. The values of the config are merged with the global config. */
const bool CSwordBackend::moduleConfig(const QString& module, SWConfig& moduleConfig) {
	SectionMap::iterator section;
	DIR *dir = opendir(configPath);
	struct dirent *ent;
	
	bool foundConfig = false;	
	QString modFile;	
	if (dir) {    // find and update .conf file
		rewinddir(dir);
		while ((ent = readdir(dir)) && !foundConfig) {
			if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {								
				modFile = QString::fromLocal8Bit(configPath);
				modFile += QString::fromLatin1("/");
				modFile += QString::fromLocal8Bit(ent->d_name);
				moduleConfig = SWConfig( (const char*)modFile.local8Bit() );
				section =	moduleConfig.Sections.find( (const char*)module.local8Bit() );
				foundConfig = ( section != moduleConfig.Sections.end() );
			}
		}
		closedir(dir);
	}
	else { //try to read mods.conf
		//moduleConfig = SWConfig( configPath + "/mods.conf" );
		moduleConfig = SWConfig("");//global config		
		section =	config->Sections.find( (const char*)module.local8Bit() );		
		foundConfig = ( section != config->Sections.end() );		
		
		ConfigEntMap::iterator entry;
		if (foundConfig) { //copy module section
			for (entry = (*section).second.begin(); entry != (*section).second.end(); entry++) {
					moduleConfig.Sections[(*section).first].insert(ConfigEntMap::value_type((*entry).first, (*entry).second));
			}
		}			
	}
	
	if (!foundConfig && configType != 2) { //search in $HOME/.sword/
		QString myPath = QString::fromLatin1("%1/.sword/mods.d").arg(getenv("HOME"));
		dir = opendir(myPath.latin1());
		if (dir) {
			rewinddir(dir);
			while ((ent = readdir(dir)) && !foundConfig) {
				if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {								
					modFile = myPath;
					modFile += "/";
					modFile += ent->d_name;
					moduleConfig = SWConfig( (const char*)modFile.local8Bit() );
					section =	moduleConfig.Sections.find( (const char*)module.local8Bit() );
					foundConfig = ( section != moduleConfig.Sections.end() );
				}
			}
			closedir(dir);
		}
	}
	return foundConfig;
}

/** Returns the text used for the option given as parameter. */
const QString CSwordBackend::optionName( const CSwordBackend::FilterOptions option ){
	switch (option) {
		case CSwordBackend::footnotes:
			return QString::fromLatin1("Footnotes");
		case CSwordBackend::strongNumbers:
			return QString::fromLatin1("Strong's Numbers");
		case CSwordBackend::headings:
			return QString::fromLatin1("Headings");
		case CSwordBackend::morphTags:
			return QString::fromLatin1("Morphological Tags");
  	case CSwordBackend::lemmas:
			return QString::fromLatin1("Lemmas");
		case CSwordBackend::hebrewPoints:
			return QString::fromLatin1("Hebrew Vowel Points");
		case CSwordBackend::hebrewCantillation:
			return QString::fromLatin1("Hebrew Cantillation");
		case CSwordBackend::greekAccents:
			return QString::fromLatin1("Greek Accents");
		case CSwordBackend::textualVariants:
			return QString::fromLatin1("Textual Variants");			
	}
	return QString::null;	
}

/** Returns the translated name of the option given as parameter. */
const QString CSwordBackend::translatedOptionName(const CSwordBackend::FilterOptions option){
	switch (option) {
		case CSwordBackend::footnotes:
			return i18n("Footnotes");
		case CSwordBackend::strongNumbers:
			return i18n("Strong's Numbers");
		case CSwordBackend::headings:
			return i18n("Headings");
		case CSwordBackend::morphTags:
			return i18n("Morphological Tags");
  	case CSwordBackend::lemmas:
			return i18n("Lemmas");
		case CSwordBackend::hebrewPoints:
			return i18n("Hebrew Vowel Points");
		case CSwordBackend::hebrewCantillation:
			return i18n("Hebrew Cantillation");
		case CSwordBackend::greekAccents:
			return i18n("Greek Accents");
		case CSwordBackend::textualVariants:
			return i18n("Textual Variants");			
	}
	return QString::null;
}


const QString CSwordBackend::configOptionName( const CSwordBackend::FilterOptions option ){
	switch (option) {
		case CSwordBackend::footnotes:
			return QString::fromLatin1("Footnotes");
		case CSwordBackend::strongNumbers:
			return QString::fromLatin1("Strongs");
		case CSwordBackend::headings:
			return QString::fromLatin1("Headings");
		case CSwordBackend::morphTags:
			return QString::fromLatin1("Morph");
  	case CSwordBackend::lemmas:
			return QString::fromLatin1("Lemma");
		case CSwordBackend::hebrewPoints:
			return QString::fromLatin1("HebrewPoints");
		case CSwordBackend::hebrewCantillation:
			return QString::fromLatin1("Cantillation");
		case CSwordBackend::greekAccents:
			return QString::fromLatin1("GreekAccents");
		case CSwordBackend::textualVariants:
			return QString::fromLatin1("TextualVariants");
	}
	return QString::null;	
}

const QString CSwordBackend::booknameLanguage( const QString& language ) {
	if (!language.isNull())
		LocaleMgr::systemLocaleMgr.setDefaultLocaleName( language.local8Bit() );
	return QString::fromLatin1(LocaleMgr::systemLocaleMgr.getDefaultLocaleName());
}

/** Returns the version of the Sword library. */
const SWVersion CSwordBackend::Version() {
	return SWVersion::currentVersion;
}
