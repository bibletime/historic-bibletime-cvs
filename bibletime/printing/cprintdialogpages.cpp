/***************************************************************************
                          cstylelistpage.cpp  -  description
                             -------------------
    begin                : Fri Feb 8 2002
    copyright            : (C) 2002 by The BibleTime team
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

//BibleTime includes
#include "cprinter.h"
#include "cprintdialogpages.h"
#include "cprintitemlist.h"
#include "cprintitem.h"
#include "tooltipdef.h"
#include "resource.h"
#include "whatsthisdef.h"

//QT includes
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qlistview.h>

//KDE includes
#include <klocale.h>

CStyleListPage::CStyleListPage(CPrinter* printer, QWidget *parent, const char *name )
	: KPrintDialogPage(parent,name), m_printer(printer) {
	setTitle(i18n("Styles"));

//  QFrame *page = addPage( i18n("Layout"), i18n("Layout specific settings") );
  QVBoxLayout *topLayout = new QVBoxLayout( this, 5, 5 );
  	
  QButtonGroup *group = new QButtonGroup( i18n("Margins (in millimeter)"), this );
  QWhatsThis::add(group, WT_PD_LAYOUT_BORDER);
  topLayout->addWidget( group, 0 );

  QGridLayout *gbox = new QGridLayout( group, 4, 2, 5, 5 );
  gbox->addRowSpacing( 0, group->fontMetrics().height()-5 );

  const QString name[4] = {i18n("&Top"),i18n("&Bottom"),i18n("&Left"), i18n("&Right") };
  for( int i=0; i<4; i++ ) {
    m_marginSpin[i] = new QSpinBox( group );
    m_marginSpin[i]->setFixedHeight( m_marginSpin[i]->sizeHint().height() );
    m_marginSpin[i]->setMinimumWidth( m_marginSpin[i]->fontMetrics().width("M")*10 );
    m_marginSpin[i]->setSuffix(" mm");

    QLabel *label = new QLabel( m_marginSpin[i], name[i], group );
    label->setFixedHeight( m_marginSpin[i]->sizeHint().height() );
    label->setFixedWidth( label->sizeHint().width() );

    if( i < 2 )
    {
      gbox->addWidget( label, i+1, 0, AlignLeft );
      gbox->addWidget( m_marginSpin[i], i+1, 1, AlignLeft );
    }
    else
    {
      gbox->addWidget( label, i-1, 3, AlignLeft );
      gbox->addWidget( m_marginSpin[i], i-1, 4, AlignLeft );
    }
  }
  QWhatsThis::add(m_marginSpin[0], WT_PD_LAYOUT_BORDER_TOP);
  QWhatsThis::add(m_marginSpin[1], WT_PD_LAYOUT_BORDER_BOTTOM);
  QWhatsThis::add(m_marginSpin[2], WT_PD_LAYOUT_BORDER_LEFT);
  QWhatsThis::add(m_marginSpin[3], WT_PD_LAYOUT_BORDER_RIGHT);

  QToolTip::add(m_marginSpin[0], TT_PD_LAYOUT_BORDER_TOP);
  QToolTip::add(m_marginSpin[1], TT_PD_LAYOUT_BORDER_BOTTOM);
  QToolTip::add(m_marginSpin[2], TT_PD_LAYOUT_BORDER_LEFT);
  QToolTip::add(m_marginSpin[3], TT_PD_LAYOUT_BORDER_RIGHT);

  gbox->activate();	
  group->setFixedHeight( group->sizeHint().height() );
  //set minimum borders
  m_marginSpin[0]->setRange(0, 100);	//upper margin
  m_marginSpin[1]->setRange(0, 100);	//lower margin
  m_marginSpin[2]->setRange(0, 100);	//left margin
  m_marginSpin[3]->setRange(0, 100);	//right margin

  QHBoxLayout *entryLayout = new QHBoxLayout( 0, 5, 5 );
  QVBoxLayout *styleLayout = new QVBoxLayout( 0, 5, 5 );
  QVBoxLayout *buttonLayout = new QVBoxLayout( 0, 5, 5 );
  	
  m_styleList = new CStyleList( &(m_printer->styleList()), this, "CStyleList1");
	connect(m_styleList, SIGNAL(currentChanged(QListViewItem*)), SLOT(currentStyleChanged(QListViewItem*)));
	QToolTip::add(m_styleList, TT_PD_LAYOUT_STYLE_LIST);	
	QWhatsThis::add(m_styleList, WT_PD_LAYOUT_STYLE_LIST);
	
  QLabel* label = new QLabel(m_styleList, i18n("List of style items:"), this);
  styleLayout->addWidget(label,0);
	styleLayout->addWidget( m_styleList, 3);
	
	m_newStyleButton = new QToolButton( this, "newStyle Button");
	m_newStyleButton->setOnIconSet( SmallIcon(ICON_FILE_NEW));
	m_newStyleButton->setOffIconSet( SmallIcon(ICON_FILE_NEW));	
	QToolTip::add(m_newStyleButton, TT_PD_LAYOUT_STYLE_NEW);	
	QWhatsThis::add(m_newStyleButton, WT_PD_LAYOUT_STYLE_NEW);	
	connect( m_newStyleButton, SIGNAL(clicked()), m_styleList, SLOT(createNewStyle()));
		
	m_deleteStyleButton = new QToolButton( this, "deleteStyle Button");	
	m_deleteStyleButton->setOnIconSet( SmallIcon(ICON_FILE_DELETE));
	m_deleteStyleButton->setOffIconSet( SmallIcon(ICON_FILE_DELETE));	
	QToolTip::add(m_deleteStyleButton, TT_PD_LAYOUT_STYLE_DELETE);	
	QWhatsThis::add(m_deleteStyleButton, WT_PD_LAYOUT_STYLE_DELETE);		
	connect( m_deleteStyleButton, SIGNAL(clicked()), m_styleList, SLOT(deleteCurrentStyle()));

	m_editStyleButton = new QToolButton( this, "editStyle");
	m_editStyleButton->setOnIconSet( SmallIcon(ICON_EDIT));
	m_editStyleButton->setOffIconSet( SmallIcon(ICON_EDIT));	
	QToolTip::add(m_editStyleButton, TT_PD_LAYOUT_STYLE_EDIT);	
	QWhatsThis::add(m_editStyleButton, WT_PD_LAYOUT_STYLE_EDIT);		
	connect( m_editStyleButton, SIGNAL(clicked()), m_styleList, SLOT(editCurrentStyle()));
		
	buttonLayout->addWidget( m_newStyleButton );
	buttonLayout->addWidget( m_editStyleButton );
	buttonLayout->addWidget( m_deleteStyleButton );
	buttonLayout->addStretch(3);
		
	entryLayout->addLayout( styleLayout );
	entryLayout->addLayout( buttonLayout );
	
	topLayout->addLayout(entryLayout);
}

CStyleListPage::~CStyleListPage() {

}

void  CStyleListPage::getOptions (QMap<QString, QString>& opts, bool incldef) {
	qWarning("CStyleListPage::getOptions (const QMap<QString,QString>& opts, bool)");

	const QString leading = "kde-bibletime-";
	opts[ leading+"upper_margin" ] = QString::number(m_marginSpin[0]->value());
	qWarning(opts[leading+"upper_margin"].latin1());
	opts[ leading+"lower_margin" ] = QString::number(m_marginSpin[1]->value());
	opts[ leading+"left_margin"  ] = QString::number(m_marginSpin[2]->value());
	opts[ leading+"right_margin" ] = QString::number(m_marginSpin[3]->value());
}

void  CStyleListPage::setOptions (const QMap<QString,QString>& opts) {
	qWarning("CStyleListPage::setOptions (const QMap<QString,QString>& opts)");

	const QString leading = "kde-bibletime-";
	QString option = opts[ leading+"upper_margin" ];
	m_marginSpin[0]->setValue( option.toInt() );
	option = opts[ leading+"lower_margin" ];
	m_marginSpin[1]->setValue( option.toInt() );
	option = opts[ leading+"left_margin" ];
	m_marginSpin[2]->setValue( option.toInt() );
	option = opts[ leading+"right_margin" ];
	m_marginSpin[3]->setValue( option.toInt() );
}

bool  CStyleListPage::isValid (QString& errorMessage) {
	return true;
}

/** No descriptions */
void CStyleListPage::currentStyleChanged( QListViewItem* item ){
	if (item) {
		m_deleteStyleButton->setEnabled( item->text(0) != i18n("Standard") );
		m_editStyleButton->setEnabled( true );	
	}
	else {
		m_deleteStyleButton->setEnabled( false );
		m_editStyleButton->setEnabled( false );	
	}
}



// ------------------

CPrintItemListPage::CPrintItemListPage(CPrinter* printer, QWidget *parent, const char *name )
	: KPrintDialogPage(parent,name), m_printer( printer )
{
	setTitle(i18n("Print queue"));

  QVBoxLayout *topLayout = new QVBoxLayout( this, 5, 5 );
  QVBoxLayout *entryLayout = new QVBoxLayout( 0, 5, 5 );
  QLabel*	label = 0;

  m_styleComboBox = new QComboBox( this, "styleComboBox" );
	QToolTip::add(m_styleComboBox, TT_PD_ENTRIES_STYLE_COMBO);
	QWhatsThis::add(m_styleComboBox, WT_PD_ENTRIES_STYLE_COMBO);
  connect(m_styleComboBox, SIGNAL(activated(const QString&)), SLOT(slotListApplyStyle(const QString&)));

  label = new QLabel( m_styleComboBox, i18n("Choose a style"), this);
  QHBoxLayout*	hboxLayout = new QHBoxLayout( 0, 5, 5 );
  hboxLayout->addWidget( label );
  hboxLayout->addWidget( m_styleComboBox );
  topLayout->addLayout( hboxLayout );

  hboxLayout = new QHBoxLayout( 0, 5, 5 );

  m_printItemList = new CPrintItemList( &(m_printer->printQueue()), this, "CPrintItemList1");
	QToolTip::add(m_printItemList, TT_PD_ENTRIES_PI_LIST);
	QWhatsThis::add(m_printItemList, WT_PD_ENTRIES_PI_LIST);

  label = new QLabel(m_printItemList, i18n("Entries which should be printed:"), this);
  entryLayout->addWidget(label);
  entryLayout->addWidget( m_printItemList );
  hboxLayout->addLayout(entryLayout);

  QVBoxLayout *buttonLayout = new QVBoxLayout( 0, 5, 5 );
  m_moveUpButton = new QToolButton(this);
 	m_moveUpButton->setOnIconSet( SmallIcon("up"));
	m_moveUpButton->setOffIconSet( SmallIcon("up"));	
	QToolTip::add(m_moveUpButton, TT_PD_ENTRIES_PI_MOVE_UP);
	QWhatsThis::add(m_moveUpButton, WT_PD_ENTRIES_PI_MOVE_UP);	
	connect(m_moveUpButton, SIGNAL(clicked()),m_printItemList, SLOT(moveUp()));
	
  m_moveDownButton = new QToolButton(this);
 	m_moveDownButton->setOnIconSet( SmallIcon("down"));
	m_moveDownButton->setOffIconSet( SmallIcon("down"));	
	QToolTip::add(m_moveDownButton, TT_PD_ENTRIES_PI_MOVE_DOWN);
	QWhatsThis::add(m_moveDownButton, WT_PD_ENTRIES_PI_MOVE_DOWN);
	connect( m_moveDownButton, SIGNAL(clicked()),m_printItemList, SLOT(moveDown()));
	
  m_deleteButton = new QToolButton(this);
	m_deleteButton->setOnIconSet( SmallIcon("edittrash"));
	m_deleteButton->setOffIconSet( SmallIcon("edittrash"));	
	QToolTip::add(m_deleteButton, TT_PD_ENTRIES_PI_DELETE);
	QWhatsThis::add(m_deleteButton, WT_PD_ENTRIES_PI_DELETE);		
	connect( m_deleteButton, SIGNAL(clicked()),m_printItemList, SLOT(deleteSelectedItems()));

  buttonLayout->addWidget(m_moveUpButton);
  buttonLayout->addWidget(m_moveDownButton);
  buttonLayout->addWidget(m_deleteButton);
 	buttonLayout->addStretch(3);

  hboxLayout->addLayout( buttonLayout );
  topLayout->addLayout( hboxLayout );

	connect(m_printer, SIGNAL(sigStylesChanged()), this, SLOT(stylesChanged()));
	stylesChanged(); //fill the box for the first time
}

CPrintItemListPage::~CPrintItemListPage() {

}

void CPrintItemListPage::getOptions (QMap<QString, QString>& opts, bool incldef) {

}

void CPrintItemListPage::setOptions (const QMap<QString,QString>& opts) {

}

bool CPrintItemListPage::isValid(QString& errorMessage) {
	return true;
}

/** Applies the given styles to the print item list. */
void CPrintItemListPage::slotListApplyStyle(const QString& styleName ){
	StyleItemList& styleList = m_printer->styleList();	
	for(styleList.first(); styleList.current(); styleList.next()) {
		if (styleList.current()->styleName() == styleName) {
		 	m_printItemList->applyStyleToSelected( styleList.current() );
		 	break;
		}
	}
}

/** Is called when the CPrinter object sends a signal that the styles have changed. */
void CPrintItemListPage::stylesChanged(){
	m_styleComboBox->clear();
	//refill the combobox
	StyleItemList& styleList = m_printer->styleList();	
	for(styleList.first(); styleList.current(); styleList.next()) {
		m_styleComboBox->insertItem(styleList.current()->styleName());
	}	
}
