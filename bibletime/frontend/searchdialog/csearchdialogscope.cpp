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

#include <klocale.h>
#include <ksimpleconfig.h>

#include "versekey.h"

CSearchDialogScopeChooser::CSearchDialogScopeChooser(QWidget *parent, const char *name ) : QWidget(parent,name) {

  QHBoxLayout*	HLayout = new QHBoxLayout(this);

  noScope 				= new QRadioButton(i18n("No scope"	 ),this);
  lastResultScope = new QRadioButton(i18n("Last result"),this);
  useScope 				= new QRadioButton(i18n("Use scope"  ),this);

  group = new QButtonGroup();
  group->insert(noScope);
  group->insert(lastResultScope);
  group->insert(useScope);


  noScope->setChecked(true);

  RangeLabel = new QLabel(i18n("Choose custom range"),this);
  RangeChooser = new QComboBox(this);
  editButton = new QPushButton(i18n("Edit ranges"),this);
  editButton->setFixedWidth(editButton->sizeHint().width());

  RangeLabel->hide();
  RangeChooser->hide();
  editButton->hide();


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
  config->setGroup(QString("custom ranges"));

  QMap<QString,QString> entryMap = config->entryMap(QString("custom ranges"));
  QMap<QString,QString>::Iterator it;
	
	for ( it = entryMap.begin(); it != entryMap.end(); ++it) {
	  debug(it.key().local8Bit());
	  if (it.key() != QString::null)
		  RangeChooser->insertItem( it.key() );
	}
}

ListKey CSearchDialogScopeChooser::getScope(){
	qDebug("CSearchDialogScopeChooser::getScope()");
	return VerseKey().ParseVerseList( config->readEntry( RangeChooser->currentText() ).local8Bit(), "Genesis 1:1", true);
}

CSwordModuleSearch::scopeType CSearchDialogScopeChooser::getScopeType(){
  qDebug("CSwordModuleSearch::scopeType CSearchDialogScopeChooser::getScopeType()");
  if (noScope->isChecked())         return CSwordModuleSearch::Scope_NoScope;
  if (lastResultScope->isChecked()) return CSwordModuleSearch::Scope_LastSearch;
  if (useScope->isChecked())        return CSwordModuleSearch::Scope_Bounds;
  return CSwordModuleSearch::Scope_NoScope;
}

void CSearchDialogScopeChooser::editButtonClicked(){
	qDebug("CSearchDialogScopeChooser::editButtonClicked()");
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
  if (getScopeType() == CSwordModuleSearch::Scope_Bounds){
    RangeLabel->show();
    RangeChooser->show();
    editButton->show();
  }
  else{
    RangeLabel->hide();
    RangeChooser->hide();
    editButton->hide();
  }
}


CSearchDialogScopeChooser::~CSearchDialogScopeChooser(){	
	qDebug("destructor of CSearchDialogScopeChooser");	
	if (config) {
		config->sync();
		delete config;
	}
}

CSearchDialogScopeEdit::~CSearchDialogScopeEdit(){	
  //not delete config, because it points to the parent config
}

CSearchDialogScopeEdit::CSearchDialogScopeEdit(KSimpleConfig *parentconfig, QWidget *parent, const char *name ) : QDialog(parent,name,/*modal*/true) {

  QGridLayout*	Layout = new QGridLayout(this, 5, 5, /*border*/5, /*space*/0);

  RangeChooser = new QComboBox(this);
  RangeChooser->setEnabled(false);

  RangeEdit = new QLineEdit(this);
  RangeEdit->setEnabled(false);

  RangeDisp = new QListBox(this);

  newButton   = new QPushButton(i18n("Add"),this);
  deleteButton= new QPushButton(i18n("Delete"),this);
  closeButton = new QPushButton(i18n("Close"),this);
  deleteButton->setEnabled(false);

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
  delete this;
}
