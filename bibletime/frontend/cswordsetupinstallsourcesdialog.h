#ifndef CSWORDSETUPINSTALLSOURCESDIALOG_H
#define CSWORDSETUPINSTALLSOURCESDIALOG_H

#include <qdialog.h>
#include <qurl.h>

class QLabel;
class QComboBox;
class QLineEdit;

namespace InstallationManager {

class CSwordSetupInstallSourcesDialog : public QDialog  {
   Q_OBJECT

public:
	static QUrl getSourceURL();
	
protected:
	CSwordSetupInstallSourcesDialog();

protected slots:
	void slotOk();
	void slotProtocolChanged();

private:
	QLabel*    m_serverLabel;
	QLineEdit* m_serverEdit;
	QLineEdit* m_pathEdit;
	QComboBox* m_protocolCombo;
};

} //namespace

#endif //CSWORDSETUPINSTALLSOURCESDIALOG_H
