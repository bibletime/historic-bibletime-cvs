/***************************************************************************
                          btinstallmgr.cpp  -  description
                             -------------------
    begin                : Mit Sep 24 2003
    copyright            : (C) 2003 by The BibleTime team
    email                : info@bibletime.info
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "btinstallmgr.h"

#include <qfile.h>

#include <kapplication.h>

#include <filemgr.h>
#include <swconfig.h>
#include <swbuf.h>
#include <map>
#include <utility>

using namespace sword;


QStringList BTInstallMgr::Tool::sourceList( sword::InstallMgr* mgr ) {
  qWarning("BTInstallMgr::Tool::sourceList( sword::InstallMgr* mgr )");
  Q_ASSERT(mgr);
  Q_ASSERT(mgr->installConf);
  
  QStringList names;

  //add Sword remote sources
	for (InstallSourceMap::iterator it = mgr->sources.begin(); it != mgr->sources.end(); it++) {    
    names << QString::fromLocal8Bit(it->second->caption);
  }  

  // Add local directory sources
  SWConfig config(Tool::configFilename().latin1());
  sword::SectionMap::iterator sourcesSection = config.Sections.find("Sources");
	if (sourcesSection != config.Sections.end()) {
		sword::ConfigEntMap::iterator sourceBegin = sourcesSection->second.lower_bound("DIRSource");
		sword::ConfigEntMap::iterator sourceEnd = sourcesSection->second.upper_bound("DIRSource");
    
		while (sourceBegin != sourceEnd) {
			InstallSource is("DIR", sourceBegin->second.c_str());
      names << QString::fromLatin1(is.caption.c_str());

      sourceBegin++;
		}
	}
  
  return names;
}

QStringList BTInstallMgr::Tool::targetList() {
  QStringList names;

  names.append("/usr/share/sword");
  names.append("$HOME/.sword");
  
  return names;
}

sword::InstallSource BTInstallMgr::Tool::source( sword::InstallMgr* mgr, const QString& name ) {
  Q_ASSERT(mgr);

  InstallSourceMap::iterator source = mgr->sources.find(name.latin1());
	if (source != mgr->sources.end()) {
  	return *(source->second);
  }
  else { //not found in Sword, may be a local DIR source
    SWConfig config(Tool::configFilename().latin1());
  	SectionMap::iterator sourcesSection = config.Sections.find("Sources");
  	if (sourcesSection != config.Sections.end()) {
  		ConfigEntMap::iterator sourceBegin = sourcesSection->second.lower_bound("DIRSource");
  		ConfigEntMap::iterator sourceEnd = sourcesSection->second.upper_bound("DIRSource");

      qWarning("looking for local source %s", name.latin1());
    
  		while (sourceBegin != sourceEnd) {
  			InstallSource is("DIR", sourceBegin->second.c_str());
        qWarning("found %s", is.caption.c_str());
        
        if (!strcmp(is.caption, name.latin1()) ) { //found local dir source
          qWarning("found it");

          return is;
        }

        sourceBegin++;
  		}
  	}
  }
  InstallSource is("EMPTY");   //default return value
  is.caption = "unknwon caption";
  is.source = "unknwon source";
  is.directory = "unknwon dir";
}

const bool BTInstallMgr::Tool::isRemoteSource( InstallSource* is ) {
  Q_ASSERT(is);
  if (is)
    return !strcmp(is->type, "FTP");
  else
    return false;
}

void BTInstallMgr::Tool::addSource( sword::InstallSource* is ) {
 	Q_ASSERT(is);

  SWConfig config(Tool::configFilename().latin1());

  if (!strcmp(is->type, "FTP")) {  
  	config["Sources"].insert( std::make_pair("FTPSource", is->getConfEnt()) );
  }
  else if (!strcmp(is->type, "DIR")) {
  	config["Sources"].insert( std::make_pair("DIRSource", is->getConfEnt()) );
  }
  
  config.Save();
}

void BTInstallMgr::Tool::initConfig() {
  QFile::remove(configFilename());
  
  
	InstallSource is("FTP");
	is.caption = "Crosswire";
	is.source = "ftp.crosswire.org";
	is.directory = "/pub/sword/raw";
  Tool::addSource(&is);

	is.type = "FTP";
	is.caption = "Crosswire Beta";
	is.source = "ftp.crosswire.org";
	is.directory = "/pub/sword/betaraw";
  Tool::addSource(&is);  

  //add default local sources
  is.type = "DIR";
	is.caption = "[CD-Rom] /media/cdrom";
	is.source = "file://";
	is.directory = "/media/cdrom/";
  Tool::addSource(&is);

  is.type = "DIR";
	is.caption = "[CD-Rom] /media/cdrom1";
	is.source = "file://";
	is.directory = "/media/cdrom1/";
  Tool::addSource(&is);

  SWConfig config(Tool::configFilename().latin1());
  config["General"]["PassiveFTP"] = "true";
  config.Save();
}

const QString BTInstallMgr::Tool::configPath() {
	const char *envhomedir  = getenv("HOME");
	QString confPath = QString::fromLatin1((envhomedir) ? envhomedir : ".");
	confPath += QString::fromLatin1("/.sword/InstallMgr");
//	qWarning(confPath.latin1());

  return confPath;
}

const QString BTInstallMgr::Tool::configFilename() {
  return (configPath() + "/InstallMgr.conf").latin1();
}

void BTInstallMgr::Tool::removeSource( sword::InstallMgr* mgr, sword::InstallSource* is) {
  Q_ASSERT(mgr);
  Q_ASSERT(is);

}

BTInstallMgr::BTInstallMgr() : InstallMgr(Tool::configPath().latin1()) {
}

BTInstallMgr::~BTInstallMgr(){
}

void BTInstallMgr::statusUpdate(double dltotal, double dlnow) {
  int totalPercent = (int)((float)(dlnow + m_completedBytes+1) / (float)(m_totalBytes) * 100);
  const int filePercent  = (int)((float)(dlnow + 1) / (float)(dltotal) * 100);
  int totalBytes;
  int fileBytes;
  
  
  emit completed(totalPercent, filePercent);
  KApplication::kApplication()->processEvents();
}

void BTInstallMgr::preDownloadStatus(long totalBytes, long completedBytes, const char *message) {
  emit downloadStarted( "unkown filename" );
  m_completedBytes = completedBytes;
  m_totalBytes = totalBytes;
  KApplication::kApplication()->processEvents();
}