/***************************************************************************
                          csearchdialogscope.cpp  -  description
                             -------------------
    begin                : Sun Nov 12 2000
    copyright            : (C) 2000 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "csearchdialogscope.h"
#include "../../whatsthisdef.h"
#include "../../tooltipdef.h"

#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qgroupbox.h>
#include <qinputdialog.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qwhatsthis.h>
#include <qtooltip.h>

#include <klocale.h>
#include <ksimpleconfig.h>

#include "versekey.h"

CSearchDialogScopeChooser::CSearchDialogScopeChooser(QWidget *parent, const char *name ) : QWidget(parent,name) {

  QHBoxLayout*	HLayout = new QHBoxLayout(this);

  noScope = new QRadioButton(i18n("No scope"	 ),this);
	QToolTip::add(noScope, TT_SD_SCOPE_NO_SCOPE);
	QWhatsThis::add(noScope, WT_SD_SCOPE_NO_SCOPE);
	
  lastResultScope = new QRadioButton(i18n("Last result"),this);
	QToolTip::add(lastResultScope, TT_SD_SCOPE_NO_SCOPE);
	QWhatsThis::add(lastResultScope, WT_SD_SCOPE_NO_SCOPE);

  useScope = new QRadioButton(i18n("Use scope"  ),this);
	QToolTip::add(useScope, TT_SD_SCOPE_OWN_SCOPE);
	QWhatsThis::add(useScope, WT_SD_SCOPE_OWN_SCOPE);

  group = new QButtonGroup();
  group->insert(noScope);
  group->insert(lastResultScope);
  group->insert(useScope);


  noScope->setChecked(true);

  RangeLabel = new QLabel(i18n("Choose custom range"),this);
  RangeChooser = new QComboBox(this);
	QToolTip::add(RangeChooser, TT_SD_SCOPE_CHOOSER);
  QWhatsThis::add(RangeChooser, WT_SD_SCOPE_CHOOSER);

  editButton = new QPushButton(i18n("Edit ranges"),this);
  editButton->setFixedWidth(editButton->sizeHint().width());
	QToolTip::add(editButton, TT_SD_SCOPE_EDIT_SCOPE_BUTTON);
	QWhatsThis::add(editButton, WT_SD_SCOPE_EDIT_SCOPE_BUTTON);	
  connect(editButton,SIGNAL(clicked()),this,SLOT(editButtonClicked()));
  connect(group,SIGNAL(clicked(int)),this,SLOT(scopeChanged()));

  QVBoxLayout*	VLayout_1 = new QVBoxLayout(HLayout);
  VLayout_1->addWidget(noScope, 				Qt::AlignTop);
  VLayout_1->addWidget(lastResultScope, Qt::AlignTop);
  VLayout_1->addWidget(useScope, 				Qt::AlignTop);

  HLayout->addStretch();

  QVBoxLayout*	VLayout_2 = new QVBoxLayout(HLayout);
  VLayout_2->addWidget(RangeLabel);
  VLayout_2->addWidget(RangeChooser);
  VLayout_2->addWidget(editButton, Qt::AlignHCenter);

  HLayout->addStretch();

  config = new KSimpleConfig("bt-custom_ranges", false);
  //we use this to use the global standard
  config->setGroup(QString("main"));
  if (config->readBoolEntry("firstUsage", true)) {		
  	config->setGroup(QString("custom ranges"));
  	
	  QMap<QString,QString> entryMap;
	  entryMap.insert(i18n("Old testament"),          QString("Gen - Mal"));
	  entryMap.insert(i18n("Moses/Pentateuch/Torah"), QString("Gen - Deut"));				
	  entryMap.insert(i18n("History"),                QString("Jos - Est"));		
	  entryMap.insert(i18n("Prophets"),               QString("Isa - Mal")); 	
 	  entryMap.insert(i18n("New testament"),          QString("Mat - Rev"));
	  entryMap.insert(i18n("Gospels"), 								QString("Mat - Joh"));
	  entryMap.insert(i18n("Letters/Epistels"), 			QString("Rom - Jude"));
	  entryMap.insert(i18n("Paul's Epistels"), 				QString("Rom - Phile"));

	  QMap<QString,QString>::Iterator it;			
				
		for ( it = entryMap.begin(); it != entryMap.end(); ++it) {
		  if (it.key() != QString::null) {
				QString text = QString::null;
		   	ListKey lk = VerseKey().ParseVerseList(it.data().local8Bit(),"Genesis 1:1",true);
				for (int i = 0; i < lk.Count(); ++i) {
					if (i)
						text.append(";");											
					VerseKey* element = dynamic_cast<VerseKey*>(lk.GetElement(i));
					if (element) 	
						text +=	QString("%1 - %2")
							.arg( QString::fromLocal8Bit((const char*)element->LowerBound()) )
							.arg( QString::fromLocal8Bit((const char*)element->UpperBound()) );
					else
						text += QString::fromLocal8Bit((const char*)*lk.GetElement(i));			
				}
				config->writeEntry(it.key(),text);
			}	
  	}
  }

  config->setGroup(QString("custom ranges"));
  QMap<QString,QString> entryMap = config->entryMap(QString("custom ranges"));
  QMap<QString,QString>::Iterator it;	
	for ( it = entryMap.begin(); it != entryMap.end(); ++it)
	  if (it.key() != QString::null)
		  RangeChooser->insertItem( it.key() );
	scopeChanged(); //set initial state
}

ListKey CSearchDialogScopeChooser::getScope(){
	return VerseKey().ParseVerseList( config->readEntry( RangeChooser->currentText() ).local8Bit(), "Genesis 1:1", true);
}

const CSwordModuleSearch::scopeType CSearchDialogScopeChooser::getScopeType() const {
  if (noScope->isChecked())         return CSwordModuleSearch::Scope_NoScope;
  if (lastResultScope->isChecked()) return CSwordModuleSearch::Scope_LastSearch;
  if (useScope->isChecked())        return CSwordModuleSearch::Scope_Bounds;

  return CSwordModuleSearch::Scope_NoScope;
}

void CSearchDialogScopeChooser::editButtonClicked(){
  CSearchDialogScopeEdit *dialog = new CSearchDialogScopeEdit(this->config, this, "ranges dialog");
  dialog->exec();

  //refresh the rangechooser
  RangeChooser->clear();

  QMap<QString,QString> entryMap = config->entryMap(QString("custom ranges"));
  QMap<QString,QString>::Iterator it;
  for ( it = entryMap.begin(); it != entryMap.end(); ++it) {
	  debug(it.key().local8Bit());
	  if (it.key() != QString::null)
		  RangeChooser->insertItem( it.key() );
	}
}

/** No descriptions */
void CSearchDialogScopeChooser::scopeChanged(){
	RangeLabel->setEnabled  (getScopeType() == CSwordModuleSearch::Scope_Bounds ? true : false);
	RangeChooser->setEnabled(getScopeType() == CSwordModuleSearch::Scope_Bounds ? true : false);
	editButton->setEnabled  (getScopeType() == CSwordModuleSearch::Scope_Bounds ? true : false);
}


CSearchDialogScopeChooser::~CSearchDialogScopeChooser(){
	if (config) {
	  config->setGroup(QString("main"));	
	  config->writeEntry("firstUsage", true);
		config->sync();
		delete config;
	}
}

CSearchDialogScopeEdit::~CSearchDialogScopeEdit(){	
  //not delete config, because it points to the parent config
}

CSearchDialogScopeEdit::CSearchDialogScopeEdit(KSimpleConfig *parentconfig, QWidget *parent, const char *name ) : QDialog(parent,name,/*modal*/true) {

  QGridLayout*	Layout = new QGridLayout(this, 5, 5, 5, 0);

  RangeChooser = new QComboBox(this);
	QToolTip::add(RangeChooser, TT_SD_SCOPE_CHOOSER);
  QWhatsThis::add(RangeChooser, WT_SD_SCOPE_CHOOSER);
  RangeChooser->setEnabled(false);

  RangeEdit = new QLineEdit(this);
	QToolTip::add(RangeChooser, TT_SD_SCOPE_EDIT_LINE);
  QWhatsThis::add(RangeChooser, WT_SD_SCOPE_EDIT_LINE);
  RangeEdit->setEnabled(false);

  RangeDisp = new QListBox(this);
	QToolTip::add(RangeDisp, TT_SD_SCOPE_PARSED_LIST);
  QWhatsThis::add(RangeDisp, WT_SD_SCOPE_PARSED_LIST);

  newButton   = new QPushButton(i18n("Add"),this);
	QToolTip::add(newButton, TT_SD_SCOPE_NEW_SCOPE);
  QWhatsThis::add(newButton, WT_SD_SCOPE_NEW_SCOPE);

  deleteButton= new QPushButton(i18n("Delete"),this);
  deleteButton->setEnabled(false);	
	QToolTip::add(deleteButton, TT_SD_SCOPE_DELETE_SCOPE);
  QWhatsThis::add(deleteButton, WT_SD_SCOPE_DELETE_SCOPE);

  closeButton = new QPushButton(i18n("Close"),this);
	QToolTip::add(closeButton, TT_SD_SCOPE_CLOSE);
  QWhatsThis::add(closeButton, WT_SD_SCOPE_CLOSE);

  QLabel* RangeLabel = new QLabel(i18n("Choose custom range"),this);
  QLabel* RangeEditLabel = new QLabel(i18n("Define range text"),this);
  QLabel* RangeDispLabel = new QLabel(i18n("Range parsing results"),this);


  Layout->addMultiCellWidget(RangeLabel,  0,0,0,2);
  Layout->addMultiCellWidget(RangeChooser,1,1,0,2);

  Layout->addMultiCellWidget(RangeEditLabel,0,0,4,4);
  Layout->addMultiCellWidget(RangeEdit,     1,1,4,4);

  Layout->addMultiCellWidget(RangeDispLabel,2,2,4,4);
  Layout->addMultiCellWidget(RangeDisp,     3,4,4,4);

  Layout->addMultiCellWidget(newButton,   3,3,0,0, Qt::AlignCenter);
  Layout->addMultiCellWidget(deleteButton,3,3,2,2, Qt::AlignCenter);
  Layout->addMultiCellWidget(closeButton, 4,4,0,2, Qt::AlignCenter);

  Layout->addRowSpacing(0, RangeLabel->sizeHint().height());
  Layout->addRowSpacing(1, RangeChooser->sizeHint().height());
  Layout->addRowSpacing(2, RangeDispLabel->sizeHint().height());
  Layout->addRowSpacing(3, newButton->sizeHint().height());
  Layout->addRowSpacing(4, RangeEdit->sizeHint().height() * 4);// 4 lines
  Layout->setRowStretch(4, 1); //only 4 will be stretched

  Layout->addColSpacing(0, newButton->sizeHint().width());
  Layout->addColSpacing(1, 2);
  Layout->addColSpacing(2, deleteButton->sizeHint().width());
  Layout->addColSpacing(3, 10);
  Layout->addColSpacing(4, RangeLabel->sizeHint().width() * 2);
  Layout->setColStretch(4, 1);//only 4 ill be stretched


  connect(newButton,SIGNAL(clicked()),
           this,SLOT(NewButtonClicked()));
	connect(deleteButton, SIGNAL(clicked()),
	         this,SLOT(DeleteButtonClicked()));
	connect(closeButton, SIGNAL(clicked()),
	         this,SLOT(closeButtonClicked()));
  connect(RangeEdit,SIGNAL(textChanged(const QString &)),
           this,SLOT(RangeEditTextChanged(const QString &)));
	connect(RangeChooser,SIGNAL(activated(int)),
	         this,SLOT(RangeChooserActivated(int)));


	config = parentconfig;
	
  config->setGroup(QString("custom ranges"));
  QMap<QString,QString> entryMap = config->entryMap(QString("custom ranges"));
  QMap<QString,QString>::Iterator it;
	for ( it = entryMap.begin(); it != entryMap.end(); ++it) {
  	debug(it.key().local8Bit());
	  if (it.key() != QString::null)
		  RangeChooser->insertItem( it.key() );
	}
	if (RangeChooser->count()>0){
	  RangeChooser->setEnabled(true);
	  RangeEdit->setEnabled(true);
	  deleteButton->setEnabled(true);
 	  RangeEdit->setFocus();
	  emit RangeChooserActivated(0);
	}
}

void CSearchDialogScopeEdit::NewButtonClicked(){
  bool ok = false;
  QString text = QInputDialog::getText( i18n( "Range name" ), i18n( "Please enter a name for the new range" ), QString::null, &ok, this );
  if ( ok && !text.isEmpty() /*&& !config->hasKey( text )*/ )
    config->writeEntry(text, QString::null);// user entered something and pressed ok
  //refresh the rangechooser
  RangeChooser->clear();

  QMap<QString,QString> entryMap = config->entryMap(QString("custom ranges"));
  QMap<QString,QString>::Iterator it;
  for ( it = entryMap.begin(); it != entryMap.end(); ++it) {
	  debug(it.key().local8Bit());
	  if (it.key() != QString::null)
		  RangeChooser->insertItem( it.key() );
	}
	if (RangeChooser->count()>0){
 	  RangeChooser->setEnabled(true);
	  RangeEdit->setEnabled(true);
	  deleteButton->setEnabled(true);
	  RangeChooser->setCurrentItem( RangeChooser->count()-1 );
	  emit RangeChooserActivated( RangeChooser->currentItem() );
 	  RangeEdit->setFocus();
	}
}

void CSearchDialogScopeEdit::DeleteButtonClicked(void){
  int index = RangeChooser->currentItem();
  QString text=RangeChooser->currentText();
  if ( !text.isEmpty() ){
  	config->deleteEntry(text,false);
				
		//refresh the rangechooser
    RangeChooser->clear();

    QMap<QString,QString> entryMap = config->entryMap(QString("custom ranges"));
    QMap<QString,QString>::Iterator it;
    for ( it = entryMap.begin(); it != entryMap.end(); ++it) {
  	  debug(it.key().local8Bit());
  	  if (it.key() != QString::null)
  		  RangeChooser->insertItem( it.key() );
  	}
  	if (RangeChooser->count()>0){
  	  RangeChooser->setCurrentItem( (index>0)?(index-1):0 );
  	  emit RangeChooserActivated( RangeChooser->currentItem() );
  	}
  	else{
 	  RangeChooser->setEnabled(false);
	  RangeEdit->setEnabled(false);
	  deleteButton->setEnabled(false);
 	  //clear
 	  RangeEdit->setText(QString::null);
	  RangeDisp->clear();

	  }
  }
}

void CSearchDialogScopeEdit::RangeChooserActivated(int index){
	QString value = config->readEntry(RangeChooser->text(index));
  RangeEdit->setText(value);

  //a workaround, maybe emitted twice, which is no problem
  emit RangeEditTextChanged(value);
}

void CSearchDialogScopeEdit::RangeEditTextChanged(const QString &text){
	config->writeEntry(RangeChooser->currentText(), text);
  RangeDisp->clear();
	VerseKey key;
  ListKey verses = key.ParseVerseList((const char*)text.local8Bit(), key, true);
	for (int i = 0; i < verses.Count(); ++i) {
		VerseKey* element = dynamic_cast<VerseKey*>(verses.GetElement(i));
		if (element) 	
			RangeDisp->insertItem(QString("%1 - %2").arg(QString::fromLocal8Bit((const char*)element->LowerBound())).arg(QString::fromLocal8Bit((const char*)element->UpperBound())));
		else
			RangeDisp->insertItem(QString::fromLocal8Bit((const char*)*verses.GetElement(i)));
	}
}


/** No descriptions */
void CSearchDialogScopeEdit::closeButtonClicked(){
	close();
}
