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
#include <kglobal.h>
#include <kstandarddirs.h>
#include <kprocess.h>

#include <filemgr.h>
#include <swconfig.h>
#include <swbuf.h>
#include <map>
#include <utility>

using namespace sword;

const QString BTInstallMgr::Tool::LocalConfig::swordConfigFilename() {
  Q_ASSERT( CPointers::backend()->sysconfig );

//	qWarning(CPointers::backend()->globalConfPath);
	//in Sword 1.5.7 we can use $HOME/.sword/sword.conf instead of the global ones
//	qWarning(sword::SWVersion::currentVersion);

	if ((sword::SWVersion::currentVersion > sword::SWVersion("1.5.7")) || (sword::SWVersion::currentVersion == sword::SWVersion("1.5.7"))) { // use sword.conf in HOME/.sword
		const QString file = QString::fromLatin1("%1/.sword/sword.conf").arg(getenv("HOME"));
		//qWarning("file is %s", file.latin1());
		return file;
	}
	else if (CPointers::backend()->globalConfPath) {
    QStringList paths = QStringList::split(":", CPointers::backend()->globalConfPath);
    if (paths.count()) { // more than one path
      return paths.first();
    }
  }

  return QString::null;
}

QStringList BTInstallMgr::Tool::RemoteConfig::sourceList( sword::InstallMgr* mgr ) {
//  qWarning("BTInstallMgr::Tool::sourceList( sword::InstallMgr* mgr )");
  Q_ASSERT(mgr);
  Q_ASSERT(mgr->installConf);

  QStringList names;

  //add Sword remote sources
	for (InstallSourceMap::iterator it = mgr->sources.begin(); it != mgr->sources.end(); it++)
	{
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
  QString filename =  KGlobal::dirs()->saveLocation("data", "bibletime/") + "sword.conf"; //default is to assume the real location isn't writable
  bool directAccess = false;

  QFileInfo i(LocalConfig::swordConfigFilename());
	QFileInfo dirInfo(i.dirPath(true));

  if ( i.exists() && i.isWritable() ) { //we can write to the file ourself
    filename = LocalConfig::swordConfigFilename();
    directAccess = true;
 	}
	else if ( !i.exists() && dirInfo.isWritable() ) { // if the file doesn't exist but th eparent is writable for us, create it
    filename = LocalConfig::swordConfigFilename();
    directAccess = true;
	}

  bool setDataPath = false;
  SWConfig conf(filename.local8Bit());
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

  if (!directAccess) { //use kdesu to move the file to the right place
   KProcess *proc = new KProcess;
   *proc << "kdesu";
   *proc << QString::fromLatin1("-c") << QString("mv %1 %2").arg(filename).arg(LocalConfig::swordConfigFilename());
//   KApplication::connect(proc, SIGNAL(processExited(KProcess *)),
//      this, SLOT(slot_swordConfigWritten(KProcess *)));
   proc->start(KProcess::Block);
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
			ConfigEntMap::iterator sourceBegin =
					sourcesSection->second.lower_bound("DIRSource");
			ConfigEntMap::iterator sourceEnd =
					sourcesSection->second.upper_bound("DIRSource");

			while (sourceBegin != sourceEnd) {
      	InstallSource is("DIR", sourceBegin->second.c_str());
        if (!strcmp(is.caption, name.latin1()) ) { //found local dir source
        	return is;
				}

				sourceBegin++;//next source
			}
		}
  }

  InstallSource is("EMPTY");   //default return value
  is.caption = "unknwon caption";
  is.source = "unknwon source";
  is.directory = "unknown dir";
	qWarning("no source with name %s available!", name.latin1());
	return is;
}

const bool BTInstallMgr::Tool::RemoteConfig::isRemoteSource( sword::InstallSource* is ) {
  Q_ASSERT(is);
  if (is)
    return !strcmp(is->type, "FTP");
  else
    return false;
}

void BTInstallMgr::Tool::RemoteConfig::addSource( sword::InstallSource* is ) {
  if (!is) {
    return;
  }

  SWConfig config(Tool::RemoteConfig::configFilename().latin1());
  if (!strcmp(is->type, "FTP")) {
		//make sure the path doesn't have a trailing slash, sword doesn't like it
		if (is->directory[ is->directory.length()-1 ] == '/') {
			is->directory--; //make one char shorter
		}

    config["Sources"].insert( std::make_pair("FTPSource", is->getConfEnt()) );
  }
  else if (!strcmp(is->type, "DIR")) {
    config["Sources"].insert( std::make_pair("DIRSource", is->getConfEnt()) );
  }
  config.Save();
}

void BTInstallMgr::Tool::RemoteConfig::initConfig() {
  //Do only continue if the config does not yet exist...
  if (QFile::exists(configFilename())) {
    return;
  }

  QFile::remove(configFilename());

//remote sources will be setup by the manage sources dialog

	SWConfig config(Tool::RemoteConfig::configFilename().latin1());
  config["General"]["PassiveFTP"] = "true";
	config.Save();
}

const QString BTInstallMgr::Tool::RemoteConfig::configPath() {
	const char *envhomedir  = getenv("HOME");
	QString confPath = QString::fromLatin1((envhomedir) ? envhomedir : ".");
	confPath += QString::fromLatin1("/.sword/InstallMgr");

  return confPath;
}

const QString BTInstallMgr::Tool::RemoteConfig::configFilename() {
  return (configPath() + "/InstallMgr.conf").latin1();
}

void BTInstallMgr::Tool::RemoteConfig::removeSource( sword::InstallMgr* mgr, sword::InstallSource* is) {
  Q_ASSERT(mgr);
  Q_ASSERT(is);

}

void BTInstallMgr::Tool::RemoteConfig::resetRemoteSources() {
  SWConfig config(Tool::RemoteConfig::configFilename().latin1());
  config["Sources"].erase( //remove all FTP sources
		config["Sources"].lower_bound("FTPSource"),
		config["Sources"].upper_bound("FTPSource")
	);
	config.Save();
}

void BTInstallMgr::Tool::RemoteConfig::resetLocalSources() {
  SWConfig config(Tool::RemoteConfig::configFilename().latin1());
  config["Sources"].erase( //remove all FTP sources
		config["Sources"].lower_bound("DIRSource"),
		config["Sources"].upper_bound("DIRSource")
	);
	config.Save();
}

CSwordBackend* BTInstallMgr::Tool::backend( sword::InstallSource* const is) {
  if (!is)
    return 0;

  CSwordBackend* ret = 0;
  if (RemoteConfig::isRemoteSource(is)) {
    ret = new CSwordBackend( QString::fromLatin1(is->localShadow.c_str()) );
	}
  else {
//		qWarning("constructing a backend in %s", is->directory.c_str());
    ret = new CSwordBackend( QString::fromLatin1(is->directory.c_str()) );
	}

	if (ret) {
    ret->initModules();
	}

  return ret;
}



BTInstallMgr::BTInstallMgr() : InstallMgr(Tool::RemoteConfig::configPath().latin1()) {
}

BTInstallMgr::~BTInstallMgr(){
}

void BTInstallMgr::statusUpdate(double dltotal, double dlnow) {
	//qWarning("total: %d; now: %d", dltotal, dlnow);
  int totalPercent = (int)((float)(dlnow + m_completedBytes+1) / (float)(m_totalBytes) * 100);

	if (totalPercent > 100) {
		totalPercent = 100;
	}
	else if (totalPercent < 0) {
		totalPercent = 0;
	}

  int filePercent  = (int)((float)(dlnow + 1) / (float)(dltotal) * 100);
	if (filePercent > 100) {
		filePercent = 100;
	}
	else if (filePercent < 0) {
		filePercent = 0;
	}

  emit completed(totalPercent, filePercent);
  KApplication::kApplication()->processEvents();
}

void BTInstallMgr::preDownloadStatus(long totalBytes, long completedBytes, const char *message) {
  emit downloadStarted( "unkown filename" );

	m_completedBytes = completedBytes;
  m_totalBytes = totalBytes;
  KApplication::kApplication()->processEvents();
}
