/********* Read the file LICENSE for license details. *********/

//BibleTime includes
#include "kio_ftptransport.h"

//KDE includes
#include <kapplication.h>
#include <kdirlister.h>
#include <kfileitem.h>
#include <kio/jobclasses.h>
#include <kio/job.h>
#include <kprotocolmanager.h>

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
	qDebug("FTP: Copy %s -> %s", sourceURL, destPath);

	KIO::file_delete(
		KURL(QString::fromLocal8Bit(destPath)),
		false
	);

    KIO::Job* job = KIO::file_copy(
        KURL(QString::fromLocal8Bit(sourceURL)),
        KURL(QString::fromLocal8Bit(destPath)),
        -1, //no special permissions
        true, //overwrite
        false, //no resume
        false //no progress dialog
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
		if (term && job) {
			job->kill(false); //kill emits the result signal
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

void KIO_FTPTransport::slotTotalSize(KIO::Job* /*job*/, KIO::filesize_t size) {
	if (size > 0) {
		m_totalSize = size;
		statusReporter->statusUpdate(m_totalSize, 0); //emit that we just started
	}
}

void KIO_FTPTransport::slotCopyResult(KIO::Job *job) {
	m_copyResults.insert(job->progressId(),job->error());
	finishedDownload = true;

	if ( job->error() ) {

	}
}

void KIO_FTPTransport::slotCopyProgress(KIO::Job* /*job*/, KIO::filesize_t processedSize) {
	if (m_totalSize > 0) {
		statusReporter->statusUpdate(m_totalSize, processedSize);
	}
}

void KIO_FTPTransport::slotDirListingCanceled() {
	m_listingCancelled = true;
}

std::vector<struct ftpparse> KIO_FTPTransport::getDirList(const char *dirURL) {
	std::vector< struct ftpparse > ret;

//  	char* dirURL = const_cast<char*>(myDir);
// 	if (dirURL[strlen(dirURL)-1] == '/') {
// 		qWarning("setting end to 0");
// 		dirURL[strlen(dirURL)-1] = 0;
// 	}

	qDebug("KIO_FTPTransport: getDirList %s", dirURL);
    QString proxy = KProtocolManager::proxyForURL(KURL(dirURL));
    qDebug("  using proxy %s", proxy.latin1());


	Q_ASSERT(!term);
	if (term) {
		return ret;
	}

	m_listingCancelled = false;
 	KDirLister lister;
	connect(&lister, SIGNAL(canceled()), SLOT(slotDirListingCanceled()));
	lister.openURL(KURL(dirURL));

	while (!lister.isFinished() && !m_listingCancelled) {
		if (term) {
			lister.stop();
			break;
		}

		KApplication::kApplication()->processEvents(1);
	}

	Q_ASSERT(!term);
	if (term) {
		return ret;
	}

//     qWarning("  DirList: listing");
	KFileItemList items = lister.itemsForDir(KURL(dirURL));
// 	KFileItem* i = 0;
	for ( KFileItem* i = items.first(); i; i = items.next() ) {
		int length = i->name().length();
		const char* t = i->name().latin1();
//         qWarning("  DirList: got item %s", t);

		struct ftpparse s;
		s.name = new char[length+1];//i->name().latin1();
		bzero(s.name, length+1);
		strcpy(s.name, t);
		s.namelen = length+1;

		s.size = i->size();
		s.flagtrycwd = i->isDir(); //== 1 means a dir

		s.id = 0;
		s.idlen = 0;

		ret.push_back(s);
	}

    Q_ASSERT(ret.size() > 0);
	return ret;
}

};

