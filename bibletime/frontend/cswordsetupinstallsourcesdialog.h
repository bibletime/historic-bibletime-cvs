#ifndef CSWORDSETUPINSTALLSOURCESDIALOG_H
#define CSWORDSETUPINSTALLSOURCESDIALOG_H

// #include "cbtconfig.h"
// #include "util/cpointers.h"
// #include "btinstallmgr.h"

//QT includes
// #include <qstring.h>
// #include <qmap.h>
// #include <qlistview.h>
// #include <qurl.h>
#include <qdialog.h>

//KDE includes
// #include <kdialogbase.h>
// #include <klistview.h>

// class QWidget;
// class QLabel;
// class QComboBox;
// class QWidgetStack;
// class QButton;
// class QPushButton;
// class QLineEdit;

namespace InstallationManager {

class CSwordSetupInstallSourcesDialog : public QDialog  {
   Q_OBJECT

public:
	static QString getSourceURL();
	
protected:
	CSwordSetupInstallSourcesDialog(/*QWidget *parent=0*/);

protected slots:
//  	void slotOk();

private:

};

} //namespace

#endif //CSWORDSETUPINSTALLSOURCESDIALOG_H
