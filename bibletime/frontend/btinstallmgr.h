/***************************************************************************
                          btinstallmgr.h  -  description
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

#ifndef BTINSTALLMGR_H
#define BTINSTALLMGR_H

//sword includes
#include <installmgr.h>

//Qt includes
#include <qobject.h>
#include <qptrlist.h>
#include <qstring.h>
#include <qstringlist.h>

typedef QPtrList<sword::InstallSource> InstallSourceList;

/**Our own reimplementation to provide status bar updates.
  *@author The BibleTime team
  */  
class BTInstallMgr : public QObject, public sword::InstallMgr {
  Q_OBJECT
public:
  class Tool {
  public:
    static void initConfig();
    static const QString configPath();
    static const QString configFilename();

    static QStringList targetList();
    
    static QStringList sourceList( sword::InstallMgr* );
    static sword::InstallSource source( sword::InstallMgr*, const QString& name );
    static const bool isRemoteSource( sword::InstallSource* is );
    static void addSource( sword::InstallSource* );
    static void removeSource( sword::InstallMgr*, sword::InstallSource* );
    
  private:
    Tool() {};
  };

	BTInstallMgr();
	virtual ~BTInstallMgr();

protected:
  virtual void statusUpdate(double dltotal, double dlnow);
	virtual void preDownloadStatus(long totalBytes, long completedBytes, const char *message);

  long m_totalBytes;
  long m_completedBytes;
  
signals: // Signals
  /** No descriptions */
  void completed( const int, const int );
  /** Emitted when a new file gets downloaded. */
  void downloadStarted( const QString& );
};

#endif
