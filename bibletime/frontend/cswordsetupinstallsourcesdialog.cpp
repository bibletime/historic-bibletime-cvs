#include "cswordsetupinstallsourcesdialog.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qfiledialog.h>


#include <klocale.h>

#define PROTO_FILE i18n("Local")
#define PROTO_FTP  i18n("Remote")

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
	
	QVBoxLayout* mainLayout = new QVBoxLayout( this );
	mainLayout->setMargin( 10 );
	mainLayout->setSpacing( 5 );
	
	QGridLayout* layout = new QGridLayout( mainLayout, 3, 3 );
	layout->setSpacing( 5 );
	
	QLabel* label = new QLabel(i18n("Type"), this);
	layout->addWidget( label, 0, 0);
	m_serverLabel = new QLabel(i18n("Server"), this);
	layout->addWidget( m_serverLabel, 0, 1);
	label = new QLabel(i18n("Path"), this);
	layout->addWidget( label, 0, 2 );
	
	m_protocolCombo = new QComboBox( this );
	layout->addWidget(m_protocolCombo, 1, 0);
	m_protocolCombo->insertItem( PROTO_FTP  );
	m_protocolCombo->insertItem( PROTO_FILE );
	
	m_serverEdit = new QLineEdit( this );
	layout->addWidget( m_serverEdit, 1, 1 );
	m_serverEdit->setText("www.crosswire.org");

	m_pathEdit = new QLineEdit( this );
	layout->addWidget( m_pathEdit, 1, 2 );
	m_pathEdit->setText("/pub/sword");
	
	mainLayout->addSpacing( 10 );

	QHBoxLayout* buttonLayout = new QHBoxLayout( mainLayout );
	buttonLayout->addStretch();
	QPushButton* okButton = new QPushButton( i18n("Ok"), this);
	QPushButton* discardButton = new QPushButton( i18n("Discard"), this);
	buttonLayout->addWidget( discardButton);
	buttonLayout->addWidget( okButton);
	buttonLayout->addStretch();
	
	connect( okButton,				SIGNAL( clicked() ), this, SLOT( slotOk() ) );
	connect( discardButton, 	SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( m_protocolCombo, SIGNAL( activated(int) ), this, SLOT( slotProtocolChanged() ) );

}
void CSwordSetupInstallSourcesDialog::slotOk(){
	accept();
}

void CSwordSetupInstallSourcesDialog::slotProtocolChanged(){
	if (m_protocolCombo->currentText() == PROTO_FTP){ //REMOTE
		m_serverLabel->show();
		m_serverEdit->show();
	}
	else{ //LOCAL, no server needed
		m_serverLabel->hide();
		m_serverEdit->hide();
		m_pathEdit->setText( QFileDialog::getExistingDirectory() );
	}
	
}

QUrl CSwordSetupInstallSourcesDialog::getSourceURL(){

	CSwordSetupInstallSourcesDialog* dlg = new CSwordSetupInstallSourcesDialog();
	QUrl url; //empty, invalid URL
		
	if (dlg->exec() == QDialog::Accepted){
		if (dlg->m_protocolCombo->currentText() == PROTO_FTP){
			url.setProtocol( "ftp" );
			url.setHost( dlg->m_serverEdit->text() );
		}
		else{
			url.setProtocol( "file" );
		}
		url.setPath( dlg->m_pathEdit->text() );
	}
	return url;
}


} //namespace
