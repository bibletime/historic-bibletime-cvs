#include "cswordsetupinstallsourcesdialog.h"

// using namespace sword;

namespace InstallationManager {

// sword::InstallSource CSwordSetupInstallSourcesDialog::InstallSourceItem::swordInstallSource() {
// 	sword::InstallSource src("FTP");
// 	src.caption = m_caption.latin1();
// 	src.source = m_url.host().latin1();
// 	src.directory = m_url.path().latin1();
// 
// 	return src;
// }

CSwordSetupInstallSourcesDialog::CSwordSetupInstallSourcesDialog(/*QWidget *parent*/)
	: QDialog() {

// 	initLocalSourcesPage();
// 	initRemoteSourcesPage();
}

QString CSwordSetupInstallSourcesDialog::getSourceURL(){
	CSwordSetupInstallSourcesDialog* dlg = new CSwordSetupInstallSourcesDialog();
	
	if (dlg->exec() == QDialog::Accepted)
		//get result
	  ;
		
	return "";
}


} //namespace
