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
#include "util/cpointers.h"

#include <qfile.h>
#include <qfileinfo.h>

#include <kapplication.h>

#include <filemgr.h>
#include <swconfig.h>
#include <swbuf.h>
#include <map>
#include <utility>

using namespace sword;

const QString BTInstallMgr::Tool::LocalConfig::swordConfigFilename() {
  Q_ASSERT( CPointers::backend()->sysconfig );
  if (CPointers::backend()->globalConfPath) {
    QStringList paths = QStringList::split(":", CPointers::backend()->globalConfPath);
    if (paths.count()) {
      return paths.first();
    }
  }
  return QString("Unknown path");
}

QStringList BTInstallMgr::Tool::RemoteConfig::sourceList( sword::InstallMgr* mgr ) {
  qWarning("BTInstallMgr::Tool::sourceList( sword::InstallMgr* mgr )");
  Q_ASSERT(mgr);
  Q_ASSERT(mgr->installConf);
  
  QStringList names;

  //add Sword remote sources
	for (InstallSourceMap::iterator it = mgr->sources.begin(); it != mgr->sources.end(); it++) {    
    names << QString::fromLocal8Bit(it->second->caption);
  }  

  // Add local directory sources
  SWConfig config(Tool::RemoteConfig::configFilename().latin1());
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

QStringList BTInstallMgr::Tool::LocalConfig::targetList() {
  QStringList names = CPointers::backend()->swordDirList();
  return names;
}

void BTInstallMgr::Tool::LocalConfig::setTargetList( const QStringList& targets ) {
  //saves a new Sworc config using the provided target list
  QFileInfo i(LocalConfig::swordConfigFilename());
  if (i.exists() && i.isWritable()) {
    bool setDataPath = false;

    SWConfig conf(swordConfigFilename().local8Bit());
    conf.Sections.clear();
    
    for (QStringList::const_iterator it = targets.begin(); it != targets.end(); ++it) {
      QString t = *it;
      if (t.contains( QString::fromLatin1("%1/.sword").arg(getenv("HOME")) )) {
        //we don't want HOME/.sword in the config
        continue;
      }
      else {
        conf["Install"][!setDataPath ? "DataPath" : "AugmentPath"] = t.local8Bit();
        setDataPath = true;
      }
      
    }
    conf.Save();
  }
}

sword::InstallSource BTInstallMgr::Tool::RemoteConfig::source( sword::InstallMgr* mgr, const QString& name ) {
  Q_ASSERT(mgr);

  InstallSourceMap::iterator source = mgr->sources.find(name.latin1());
	if (source != mgr->sources.end()) {
  	return *(source->second);
  }
  else { //not found in Sword, may be a local DIR source
    SWConfig config(Tool::RemoteConfig::configFilename().latin1());
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

const bool BTInstallMgr::Tool::RemoteConfig::isRemoteSource( InstallSource* is ) {
  Q_ASSERT(is);
  if (is)
    return !strcmp(is->type, "FTP");
  else
    return false;
}

void BTInstallMgr::Tool::RemoteConfig::addSource( sword::InstallSource* is ) {
 	Q_ASSERT(is);

  SWConfig config(Tool::RemoteConfig::configFilename().latin1());

  if (!strcmp(is->type, "FTP")) {  
  	config["Sources"].insert( std::make_pair("FTPSource", is->getConfEnt()) );
  }
  else if (!strcmp(is->type, "DIR")) {
  	config["Sources"].insert( std::make_pair("DIRSource", is->getConfEnt()) );
  }
  
  config.Save();
}

void BTInstallMgr::Tool::RemoteConfig::initConfig() {
  QFile::remove(configFilename());
  
  
	InstallSource is("FTP");
	is.caption = "Crosswire";
	is.source = "ftp.crosswire.org";
	is.directory = "/pub/sword/raw";
  Tool::RemoteConfig::addSource(&is);

	is.type = "FTP";
	is.caption = "Crosswire Beta";
	is.source = "ftp.crosswire.org";
	is.directory = "/pub/sword/betaraw";
  Tool::RemoteConfig::addSource(&is);  

  //add default local sources
  is.type = "DIR";
	is.caption = "[CD-Rom] /media/cdrom";
	is.source = "file://";
	is.directory = "/media/cdrom/";
  Tool::RemoteConfig::addSource(&is);

  is.type = "DIR";
	is.caption = "[CD-Rom] /media/cdrom1";
	is.source = "file://";
	is.directory = "/media/cdrom1/";
  Tool::RemoteConfig::addSource(&is);

  SWConfig config(Tool::RemoteConfig::configFilename().latin1());
  config["General"]["PassiveFTP"] = "true";
  config.Save();
}

const QString BTInstallMgr::Tool::RemoteConfig::configPath() {
	const char *envhomedir  = getenv("HOME");
	QString confPath = QString::fromLatin1((envhomedir) ? envhomedir : ".");
	confPath += QString::fromLatin1("/.sword/InstallMgr");
//	qWarning(confPath.latin1());

  return confPath;
}

const QString BTInstallMgr::Tool::RemoteConfig::configFilename() {
  return (configPath() + "/InstallMgr.conf").latin1();
}

void BTInstallMgr::Tool::RemoteConfig::removeSource( sword::InstallMgr* mgr, sword::InstallSource* is) {
  Q_ASSERT(mgr);
  Q_ASSERT(is);

}

CSwordBackend* BTInstallMgr::Tool::backend( sword::InstallSource* const is) {
  if (!is)
    return 0;

  CSwordBackend* ret = 0;
  if (RemoteConfig::isRemoteSource(is))
    ret = new CSwordBackend( QString::fromLatin1(is->localShadow.c_str()) );
  else
    ret = new CSwordBackend( QString::fromLatin1(is->directory.c_str()) );
  if (ret)
    ret->initModules();

  return ret;
}



BTInstallMgr::BTInstallMgr() : InstallMgr(Tool::RemoteConfig::configPath().latin1()) {
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
