//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

//BibleTime includes
#include "kio_ftptransport.h"

//KDE includes
#include <kapplication.h>
#include <kdirlister.h>
#include <kfileitem.h>
#include <kio/jobclasses.h>
#include <kio/job.h>

namespace InstallationManager {
bool finishedDownload = false;

KIO_FTPTransport::KIO_FTPTransport(const char *host, sword::StatusReporter *statusReporter )
	: QObject(), 
		sword::FTPTransport(host, statusReporter),
		m_totalSize(-1)
{
}

 
KIO_FTPTransport::~KIO_FTPTransport() {}

char KIO_FTPTransport::getURL(const char *destPath, const char *sourceURL) {
	KIO::file_delete(
		KURL(QString::fromLocal8Bit(destPath)),
		false
	);	
	
// 	statusReporter->statusUpdate(100.0, 0.0); //completed
	
	KIO::Job* job = KIO::copy(
		KURL(QString::fromLocal8Bit(sourceURL)),
		KURL(QString::fromLocal8Bit(destPath)),
		false
	);
	
	//make sure to wait as long as the job is working
	finishedDownload = false;
	const int progressID = job->progressId();
	connect(
		job, SIGNAL(result(KIO::Job*)),
		this, SLOT(slotCopyResult(KIO::Job*))
	);
	connect(
		job, SIGNAL(totalSize(KIO::Job*, KIO::filesize_t)),
		this, SLOT(slotTotalSize(KIO::Job*, KIO::filesize_t))
	);
	connect(
		job, SIGNAL(processedSize(KIO::Job*, KIO::filesize_t)),
		this, SLOT(slotCopyProgress(KIO::Job*, KIO::filesize_t))
	);
	
	while (!finishedDownload) {
		KApplication::kApplication()->processEvents(1);
		if (term) {
			if (job)
				job->kill(false);
// 			break; //kill(false) emits an result signal
		}
	}

	statusReporter->statusUpdate(m_totalSize, m_totalSize); //completed

	if (!m_copyResults.contains(progressID)) {
		return 1; //Error
	}
	else if (m_copyResults[progressID] > 0) { //an error occurred
		return 1; //an error occured
	}
	return 0;
}

void KIO_FTPTransport::slotTotalSize(KIO::Job *job, KIO::filesize_t size) {
// 	qWarning("got total size of %d", (int)size);
	if (size > 0)
		m_totalSize = size;
}

void KIO_FTPTransport::slotCopyResult(KIO::Job *job) {
	m_copyResults.insert(job->progressId(),job->error());
	finishedDownload = true;
	  
	if ( job->error() ) {
//       job->showErrorDialog( 0 );
	}
}

void KIO_FTPTransport::slotCopyProgress(KIO::Job *job, KIO::filesize_t processedSize) {
// 	qWarning("%d progressed size", (int)processedSize);
	if (m_totalSize > 0) {
		statusReporter->statusUpdate(m_totalSize, processedSize);
	}
}

std::vector<struct ftpparse> KIO_FTPTransport::getDirList(const char *dirURL) {
// 	qWarning("dirlist %s", dirURL);
	KDirLister lister;
	lister.openURL(KURL(dirURL));
	while (!lister.isFinished()) {
		KApplication::kApplication()->processEvents(1);
	}
	
	std::vector< struct ftpparse > ret;
	
	KFileItemList items = lister.itemsForDir(KURL(dirURL));
	KFileItem* i = 0;
	for ( i = items.first(); i; i = items.next() ) {
		struct ftpparse s;
// 		qWarning("%s", i->name().latin1());
		
		int length = i->name().length();
		const char* t = i->name().latin1();
		
		s.name = new char[length];//i->name().latin1();
		strcpy(s.name, t);
		
		s.size = i->size();
		s.flagtrycwd = i->isDir(); //== 1 means a dir
		
		ret.push_back(s);	
	}
	
	return ret;
}

};

