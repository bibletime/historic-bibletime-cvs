//
// C++ Interface: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INSTALLATIONMANAGERKIO_FTPTRANSPORT_H
#define INSTALLATIONMANAGERKIO_FTPTRANSPORT_H

//Sword includes
#include <ftptrans.h>
// extern "C" {
// #include <ftpparse.h>
// }

//Qt includes
#include <qobject.h>
#include <qmap.h>

//KDE includes
#include <kio/job.h>
// class StatusReporter;

//
#include <vector>

namespace InstallationManager {

/**
This is a reimplementation of Sword's FTP transport class which uses KDE's network functions.
 
@author The BibleTime team
*/
class KIO_FTPTransport : public QObject, public sword::FTPTransport {
	Q_OBJECT
public:
	KIO_FTPTransport(const char *host, sword::StatusReporter *statusReporter = 0);
	virtual ~KIO_FTPTransport();
	virtual char getURL(const char *destPath, const char *sourceURL);

// 		int copyDirectory(const char *urlPrefix, const char *dir, const char *dest, const char *suffix);

		// probably change to not expose struct ftpparse.  We probably need our
		// own FTPFile class or something that contains things like file name,
		// size, type (dir, file, special).  Then change to vector of this class
		// instead of ftpparse
	virtual std::vector<struct ftpparse> getDirList(const char *dirURL);
	
protected slots:
	void slotCopyResult(KIO::Job*);
	void slotCopyPercent(KIO::Job*, unsigned long);
// 	void slotListResult(KIO::Job *, const KIO::UDSEntryList &);
	
private:
	QMap<int, int> m_copyResults;
	QMap<int, std::vector< struct ftpparse > > m_dirListResults;
};

};

#endif
