/***************************************************************************
                          cprinterdialog.cpp  -  description
                             -------------------
    begin                : Sat Aug 5 2000
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

#include "cprinterdialog.h"
#include "cprinter.h"
#include "cprintitemlist.h"
#include "cstylelist.h"
#include "cstyleeditordialog.h"
#include "../../config.h"
#include "../whatsthisdef.h"
#include "../tooltipdef.h"
#include "../ressource.h"

#include <qtextstream.h>
#include <qmessagebox.h>
#include <qpaintdevice.h>
#include <qpaintdevicemetrics.h>
#include <qprinter.h>
#include <qcombobox.h>
#include <qhbuttongroup.h>
#include <qbuttongroup.h>
#include <qfile.h>
#include <qframe.h>
#include <qwidget.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qiodevice.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qwhatsthis.h>
#include <qtooltip.h>


#include <values.h>
#include <ctype.h>


#include <klocale.h>
#include <kconfig.h>
#include <klineedit.h>
#include <kfiledialog.h>

#define INNER_BORDER 4
#define OUTER_BORDER 4

CPrinterDialog::CPrinterDialog(CPrinter* printer, QWidget *parent, const char *name )
	: KDialogBase(KDialogBase::Tabbed, i18n("Printdialog"), Help | Cancel | User1 | User2,
			User1, parent, name, true, true, i18n("Preview"), i18n("Print")) {	
	m_printer = printer;
	initView();
	parsePrintcap();
	readSettings();
}

CPrinterDialog::~CPrinterDialog(){
	saveSettings();
}

/** Initializes the widgets. */
void CPrinterDialog::initView(){
	initGeneralPage();
	initListPage();
	initLayoutPage();		
	if (m_entryWidgets.styleComboBox)
		m_layout.styleList->setStyleComboBox( m_entryWidgets.styleComboBox );
}

/** Initializes the general printing page */
void CPrinterDialog::initGeneralPage(){
	QFrame* page = addPage( i18n("General"), i18n("The general options for printing") );
	QVBoxLayout *topLayout = new QVBoxLayout( page, OUTER_BORDER, INNER_BORDER );  	

	m_general.printerListLabel = new QLabel(i18n("Available printers:"), page);
	topLayout->add( m_general.printerListLabel );
		
	m_general.printerList = new KListView(page);
  m_general.printerList->setAllColumnsShowFocus( TRUE );
  m_general.printerList->addColumn( i18n("Printer"), 125 );
  m_general.printerList->addColumn( i18n("Host"), 125 );
  m_general.printerList->addColumn( i18n("Comment"), 125 );
  m_general.printerList->setFrameStyle( QFrame::WinPanel + QFrame::Sunken );
  m_general.printerList->setMinimumSize( 380, // (380 == 125 * 3 + 5)
  	m_general.printerList->fontMetrics().height() * 5 + 4 );  	
	QWhatsThis::add(m_general.printerList, WT_PD_GENERAL_PRINTER_LIST);
  topLayout->add(m_general.printerList);

  m_general.fileCheck = new QCheckBox( i18n("Print to file:"), page );
  m_general.fileCheck->setFixedHeight( m_general.fileCheck->sizeHint().height() );
  topLayout->addWidget( m_general.fileCheck );
  connect( m_general.fileCheck, SIGNAL(clicked()), SLOT(slotPrintFileCheck()) );
	QWhatsThis::add(m_general.fileCheck, WT_PD_GENERAL_PRINT_TO_FILE);

  QHBoxLayout *hbox = new QHBoxLayout();
  if( hbox == 0 )
  	return;
  topLayout->addLayout( hbox );

  m_general.fileInput = new KLineEdit( page );
  hbox->addWidget( m_general.fileInput,0,AlignTop );

  m_general.browseButton = new QPushButton( i18n("Search"), page, "browse" );
  m_general.browseButton->setFixedSize( m_general.browseButton->sizeHint() );
  m_general.fileInput->setFixedHeight( m_general.browseButton->sizeHint().height() );
  hbox->addWidget( m_general.browseButton, 0, AlignTop );
  connect( m_general.browseButton, SIGNAL(clicked()), SLOT(slotFileBrowserClicked()) );
	QWhatsThis::add(m_general.browseButton, WT_PD_GENERAL_BROWSE_FILE );


  QGridLayout *gbox = new QGridLayout( 1, 1, INNER_BORDER );
	if( gbox == 0 )
		return;
  topLayout->addLayout( gbox );

	QVBoxLayout* vbox = new QVBoxLayout();
	gbox->addLayout( vbox, 0,0);

  hbox = new QHBoxLayout();
	m_general.pageSpin = new QSpinBox( page );
	m_general.pageSpin->setFixedHeight( m_general.pageSpin->sizeHint().height() );
	m_general.pageSpin->setMinimumWidth( m_general.pageSpin->fontMetrics().width("M")*5 );
	m_general.pageSpin->setRange( 1, MAXINT );
	QLabel *pageLabel = new QLabel( m_general.pageSpin, i18n("&Number of copies"), page );
	pageLabel->setFixedHeight( m_general.pageSpin->sizeHint().height() );
	pageLabel->setMinimumWidth( pageLabel->sizeHint().width() );
	hbox->addWidget( pageLabel );
	hbox->addWidget( m_general.pageSpin );
	
  vbox->addLayout( hbox );	
	
	m_general.oriGroup = new QHButtonGroup( i18n("Orientation"), page );
  if( m_general.oriGroup == 0 )
  	return;

  m_general.portraitRadio = new QRadioButton( i18n("Por&trait"), m_general.oriGroup );
  m_general.portraitRadio->setFixedSize( m_general.portraitRadio->sizeHint() );
  m_general.oriGroup->insert( m_general.portraitRadio, 0 );

  m_general.landscapeRadio = new QRadioButton( i18n("L&andscape"), m_general.oriGroup);
  m_general.landscapeRadio->setFixedSize( m_general.landscapeRadio->sizeHint() );
  m_general.oriGroup->insert( m_general.landscapeRadio, 1 );
  m_general.oriGroup->setFixedHeight( m_general.oriGroup->sizeHint().height());

	vbox->addWidget( m_general.oriGroup );


  hbox = new QHBoxLayout();
  vbox->addLayout( hbox );

  m_general.paperSelector = new QComboBox( false, page );
  m_general.paperSelector->setFixedHeight( m_general.paperSelector->sizeHint().height() );
  m_general.paperSelector->setMinimumWidth( m_general.paperSelector->fontMetrics().width("M")*10 );
  connect( m_general.paperSelector, SIGNAL(activated(int)), SLOT(paperTypeChanged(int)) );

  QLabel *paperLabel = new QLabel( m_general.paperSelector, i18n("Paper t&ype"), page );
  paperLabel->setFixedHeight( m_general.paperSelector->sizeHint().height() );
  paperLabel->setMinimumWidth( paperLabel->sizeHint().width() );

  m_general.paperSizeLabel = new QLabel( i18n("Size: 8888mm x 8888mm"), page );
  m_general.paperSizeLabel->setFixedHeight( m_general.paperSelector->sizeHint().height() );
  m_general.paperSizeLabel->setMinimumWidth( m_general.paperSizeLabel->sizeHint().width() );
  m_general.paperSizeLabel->hide();

  hbox->addWidget( paperLabel );
  hbox->addWidget( m_general.paperSelector );
  vbox->addWidget( m_general.paperSizeLabel );


  QStringList list;
  paperType( list );
  m_general.paperSelector->insertStringList( list );

  paperTypeChanged(0);
}

const bool CPrinterDialog::parsePrintcap() {
  bool emptyPrintcap, noPrintcap;
  QString tmp, name;

  char *path[] =
  {
    "/etc/printers.conf",
    "/etc/printcap",
    "/usr/local/etc/printcap",
    0
  };

  emptyPrintcap = noPrintcap = true;

	for( uint i=0; path[i] != 0; i++ )
	{
		QFile file( path[i] );
		if( file.open( IO_ReadOnly ) == false ) {
      continue;
    }

		noPrintcap = false;

		QTextStream st( (QIODevice *)&file );
		while( !st.eof() ) {
			tmp = st.readLine();
      tmp = tmp.stripWhiteSpace();
      if( tmp[0] == '#' || tmp.isEmpty() ) {
				continue;
      }
      name = tmp.left( tmp.find(':') );
      if( name.isEmpty() ) {
				continue;
      }
      if( name.contains('|') ) {
				name = name.left( name.find('|') );
      }
      name = name.stripWhiteSpace();
      if( name.isEmpty() ) {
				continue;
      }
      else {
				emptyPrintcap = false;
      }
      addPrinterName( name );
    }
    file.close();
  }

  setSelectedPrinter( "" ); // Select the first
  return (emptyPrintcap == true ? false : true);
}

void CPrinterDialog::addPrinterName( const QString &printer ) {
  ASSERT(m_general.printerList);
  const QListViewItem *entry = m_general.printerList->firstChild();
  ASSERT(entry);
  if (entry) {
	  for( ; entry; entry = entry->nextSibling() ) {
	     if( entry->text(0) == printer )
	       return;
	  }
	}
  (void)new QListViewItem( m_general.printerList, printer, "", "" );
}

void CPrinterDialog::setSelectedPrinter( const QString &name ) {
  QListViewItem *entry = m_general.printerList->firstChild();
  for( ; entry != 0; entry = entry->nextSibling() ) {
    if( entry->text(0) == name ) {
      m_general.printerList->setSelected ( entry, true );
      return;
    }
  }
  entry = m_general.printerList->firstChild();
  if( entry != 0 )
  	m_general.printerList->setSelected ( entry, true );
}

/** Is called when the print to file checkbox was clicked. */
void CPrinterDialog::slotPrintFileCheck(){
  bool state = m_general.fileCheck->isChecked();
  m_general.fileInput->setEnabled( state );
  m_general.browseButton->setEnabled( state );
  state = !state;
  m_general.printerListLabel->setEnabled( state );
  m_general.printerList->setEnabled( state );
}

/** Is called when "Browse for file was clicked." */
void CPrinterDialog::slotFileBrowserClicked(){
  QString url = KFileDialog::getSaveFileName( 0, i18n("*.ps | Postscript files"), topLevelWidget() );
  if( url.isEmpty() )
    return;
  m_general.fileInput->setText( url );
}

/** Initializes the widgets from config file. */
void CPrinterDialog::readSettings(){
	//setup general widgets
	m_general.fileCheck->setChecked(false);	
	slotPrintFileCheck();
	
	//setup layout widgets	
	m_layout.marginSpin[0]->setValue(m_printer->margins().height());
	m_layout.marginSpin[1]->setValue(m_printer->margins().height());
	m_layout.marginSpin[2]->setValue(m_printer->margins().width());
	m_layout.marginSpin[3]->setValue(m_printer->margins().width());

//read the saved things of our config	file
	KConfig* config = m_printer->getConfig();	
	KConfigGroupSaver gs(config, "Printerdialog");
	
	const QString printerName = config->readEntry("Printername");
	QListViewItemIterator it( m_general.printerList );
    for ( ; it.current(); ++it ) {
      if( it.current()->text(0) == printerName ) {
      	m_general.printerList->setSelected(it.current(),true);
      	break;
      }
		}

	m_general.pageSpin->setValue( m_printer->numCopies() );
	if (m_printer->orientation() == QPrinter::Portrait)
		m_general.portraitRadio->setChecked(true);
	else
		m_general.landscapeRadio->setChecked(true);
}

/** Saves the states of the widgets to config file. */
void CPrinterDialog::saveSettings(){
	KConfig* config = m_printer->getConfig();
	KConfigGroupSaver gs(config, "Printerdialog");
	config->writeEntry("Printername", m_general.printerList->currentItem()->text(0));	
	
	config->sync();	
}

void CPrinterDialog::paperType( QStringList &list )
{
  list.append(i18n("A4"));
  list.append(i18n("A5"));
  list.append(i18n("Letter"));
  list.append(i18n("Legal"));
  list.append(i18n("Executive"));

//  list.append(i18n("A0"));
//  list.append(i18n("A1"));
//  list.append(i18n("A2"));
//  list.append(i18n("A3"));
//  list.append(i18n("B5"));
//  list.append(i18n("A6"));
//  list.append(i18n("A7"));
//  list.append(i18n("A8"));
//  list.append(i18n("A9"));
//  list.append(i18n("B0"));
//  list.append(i18n("B1"));
//  list.append(i18n("B10"));
//  list.append(i18n("B2"));
//  list.append(i18n("B3"));
//  list.append(i18n("B4"));
//  list.append(i18n("B6"));
//  list.append(i18n("B7"));
//  list.append(i18n("B8"));
//  list.append(i18n("B9"));
//  list.append(i18n("C5E"));
//  list.append(i18n("Comm10E"));
//  list.append(i18n("DLE"));
//  list.append(i18n("Folio"));
//  list.append(i18n("Ledger"));
//  list.append(i18n("Tabloid"));
}

/** Initializes the layout page of the printerdialog. */
void CPrinterDialog::initLayoutPage(){
  QFrame *page = addPage( i18n("Layout"), i18n("Layout specific settings") );
  QVBoxLayout *topLayout = new QVBoxLayout( page, OUTER_BORDER, INNER_BORDER );
  	
  QButtonGroup *group = new QButtonGroup( i18n("Margins (in millimeter)"), page );
  QWhatsThis::add(group, WT_PD_LAYOUT_BORDER);
  topLayout->addWidget( group, 0 );

  QGridLayout *gbox = new QGridLayout( group, 4, 2, OUTER_BORDER, INNER_BORDER );
  gbox->addRowSpacing( 0, group->fontMetrics().height()-INNER_BORDER );

  QString name[4];
  name[0] = i18n("&Top");
  name[1] = i18n("&Bottom");
  name[2] = i18n("&Left");
  name[3] = i18n("&Right");
  for( int i=0; i<4; i++ ) {
    m_layout.marginSpin[i] = new QSpinBox( group );
    m_layout.marginSpin[i]->setFixedHeight( m_layout.marginSpin[i]->sizeHint().height() );
    m_layout.marginSpin[i]->setMinimumWidth( m_layout.marginSpin[i]->fontMetrics().width("M")*10 );

    QLabel *label = new QLabel( m_layout.marginSpin[i], name[i], group );
    label->setFixedHeight( m_layout.marginSpin[i]->sizeHint().height() );
    label->setFixedWidth( label->sizeHint().width() );

    if( i < 2 )
    {
      gbox->addWidget( label, i+1, 0, AlignLeft );
      gbox->addWidget( m_layout.marginSpin[i], i+1, 1, AlignLeft );
    }
    else
    {
      gbox->addWidget( label, i-1, 3, AlignLeft );
      gbox->addWidget( m_layout.marginSpin[i], i-1, 4, AlignLeft );
    }
  }
  QWhatsThis::add(m_layout.marginSpin[0], WT_PD_LAYOUT_BORDER_TOP);
  QWhatsThis::add(m_layout.marginSpin[1], WT_PD_LAYOUT_BORDER_BOTTOM);
  QWhatsThis::add(m_layout.marginSpin[2], WT_PD_LAYOUT_BORDER_LEFT);
  QWhatsThis::add(m_layout.marginSpin[3], WT_PD_LAYOUT_BORDER_RIGHT);

  QToolTip::add(m_layout.marginSpin[0], TT_PD_LAYOUT_BORDER_TOP);
  QToolTip::add(m_layout.marginSpin[1], TT_PD_LAYOUT_BORDER_BOTTOM);
  QToolTip::add(m_layout.marginSpin[2], TT_PD_LAYOUT_BORDER_LEFT);
  QToolTip::add(m_layout.marginSpin[3], TT_PD_LAYOUT_BORDER_RIGHT);

  gbox->activate();	
  group->setFixedHeight( group->sizeHint().height() );
  //set minimum borders
  m_layout.marginSpin[0]->setRange( m_printer->upperMargin(), MAXINT );	//upper margin
  m_layout.marginSpin[1]->setRange( m_printer->lowerMargin(), MAXINT );	//lower margin
  m_layout.marginSpin[2]->setRange( m_printer->leftMargin(), MAXINT );	//left margin
  m_layout.marginSpin[3]->setRange( m_printer->rightMargin(), MAXINT );	//right margin

  QHBoxLayout *entryLayout = new QHBoxLayout( 0, OUTER_BORDER, INNER_BORDER );
  QVBoxLayout *styleLayout = new QVBoxLayout( 0, OUTER_BORDER, INNER_BORDER );
  QVBoxLayout *buttonLayout = new QVBoxLayout( 0, OUTER_BORDER, INNER_BORDER );
  	
  m_layout.styleList = new CStyleList( m_printer->getStyleList(), page, "CStyleList1");
	connect(m_layout.styleList, SIGNAL(selectionChanged(QListViewItem*)), SLOT(currentStyleChanged(QListViewItem*)));
	QToolTip::add(m_layout.styleList, TT_PD_LAYOUT_STYLE_LIST);	
	QWhatsThis::add(m_layout.styleList, WT_PD_LAYOUT_STYLE_LIST);

  QLabel* label = new QLabel(m_layout.styleList, i18n("List of style items:"), page);
  styleLayout->addWidget(label,0);
	styleLayout->addWidget( m_layout.styleList, 3);
	
	m_layout.newStyleButton = new QToolButton( page, "newStyle Button");
	m_layout.newStyleButton->setOnIconSet( SmallIcon(ICON_FILE_NEW));
	m_layout.newStyleButton->setOffIconSet( SmallIcon(ICON_FILE_NEW));	
	QToolTip::add(m_layout.newStyleButton, TT_PD_LAYOUT_STYLE_NEW);	
	QWhatsThis::add(m_layout.newStyleButton, WT_PD_LAYOUT_STYLE_NEW);	
	connect( m_layout.newStyleButton, SIGNAL(clicked()),m_layout.styleList, SLOT(createNewStyle()));
		
	m_layout.deleteStyleButton = new QToolButton( page, "deleteStyle Button");	
	m_layout.deleteStyleButton->setOnIconSet( SmallIcon(ICON_FILE_DELETE));
	m_layout.deleteStyleButton->setOffIconSet( SmallIcon(ICON_FILE_DELETE));	
	QToolTip::add(m_layout.deleteStyleButton, TT_PD_LAYOUT_STYLE_DELETE);	
	QWhatsThis::add(m_layout.deleteStyleButton, WT_PD_LAYOUT_STYLE_DELETE);		
	connect( m_layout.deleteStyleButton, SIGNAL(clicked()), m_layout.styleList, SLOT(deleteCurrentStyle()));

	m_layout.editStyleButton = new QToolButton( page, "editStyle");
	m_layout.editStyleButton->setOnIconSet( SmallIcon(ICON_EDIT));
	m_layout.editStyleButton->setOffIconSet( SmallIcon(ICON_EDIT));	
	QToolTip::add(m_layout.editStyleButton, TT_PD_LAYOUT_STYLE_EDIT);	
	QWhatsThis::add(m_layout.editStyleButton, WT_PD_LAYOUT_STYLE_EDIT);		
	connect( m_layout.editStyleButton, SIGNAL(clicked()), m_layout.styleList, SLOT(editCurrentStyle()));
		
	buttonLayout->addWidget( m_layout.newStyleButton );
	buttonLayout->addWidget( m_layout.editStyleButton );
	buttonLayout->addWidget( m_layout.deleteStyleButton );
	buttonLayout->addStretch(3);
		
	entryLayout->addLayout( styleLayout );
	entryLayout->addLayout( buttonLayout );
	
	topLayout->addLayout(entryLayout);
}

/** Initialitzes the page which contains the two lists of CStyles and PrintItems. */
void CPrinterDialog::initListPage(){
  QFrame *page = addPage( i18n("Entries"), i18n("Management of BibleTimes printing queue") );
  QVBoxLayout *topLayout = new QVBoxLayout( page, OUTER_BORDER, INNER_BORDER );
  QVBoxLayout *entryLayout = new QVBoxLayout( 0, OUTER_BORDER, INNER_BORDER );
  QLabel*	label = 0;

  m_entryWidgets.styleComboBox = new QComboBox( page, "styleComboBox" );
	QToolTip::add(m_entryWidgets.styleComboBox, TT_PD_ENTRIES_STYLE_COMBO);
	QWhatsThis::add(m_entryWidgets.styleComboBox, WT_PD_ENTRIES_STYLE_COMBO);
  connect(m_entryWidgets.styleComboBox, SIGNAL(activated(const QString&)), SLOT(slotListApplyStyle(const QString&)));

  label = new QLabel( m_entryWidgets.styleComboBox, i18n("Choose a style"), page);
  QHBoxLayout*	hboxLayout = new QHBoxLayout( 0, OUTER_BORDER, INNER_BORDER );
  hboxLayout->addWidget( label );
  hboxLayout->addWidget( m_entryWidgets.styleComboBox );
  topLayout->addLayout( hboxLayout );

  hboxLayout = new QHBoxLayout( 0, OUTER_BORDER, INNER_BORDER );

  m_entryWidgets.printItemList = new CPrintItemList( m_printer->getPrintQueue(), page, "CPrintItemList1");
	QToolTip::add(m_entryWidgets.printItemList, TT_PD_ENTRIES_PI_LIST);
	QWhatsThis::add(m_entryWidgets.printItemList, WT_PD_ENTRIES_PI_LIST);

  label = new QLabel(m_entryWidgets.printItemList, i18n("Entries which should be printed:"), page);
  entryLayout->addWidget(label);
  entryLayout->addWidget( m_entryWidgets.printItemList );
  hboxLayout->addLayout(entryLayout);

  QVBoxLayout *buttonLayout = new QVBoxLayout( 0, OUTER_BORDER, INNER_BORDER );
  m_entryWidgets.moveUpButton = new QToolButton(page);
 	m_entryWidgets.moveUpButton->setOnIconSet( SmallIcon("up"));
	m_entryWidgets.moveUpButton->setOffIconSet( SmallIcon("up"));	
	QToolTip::add(m_entryWidgets.moveUpButton, TT_PD_ENTRIES_PI_MOVE_UP);
	QWhatsThis::add(m_entryWidgets.moveUpButton, WT_PD_ENTRIES_PI_MOVE_UP);	
	connect(m_entryWidgets.moveUpButton, SIGNAL(clicked()),m_entryWidgets.printItemList, SLOT(moveUp()));
	
  m_entryWidgets.moveDownButton = new QToolButton(page);
 	m_entryWidgets.moveDownButton->setOnIconSet( SmallIcon("down"));
	m_entryWidgets.moveDownButton->setOffIconSet( SmallIcon("down"));	
	QToolTip::add(m_entryWidgets.moveDownButton, TT_PD_ENTRIES_PI_MOVE_DOWN);
	QWhatsThis::add(m_entryWidgets.moveDownButton, WT_PD_ENTRIES_PI_MOVE_DOWN);
	connect( m_entryWidgets.moveDownButton, SIGNAL(clicked()),m_entryWidgets.printItemList, SLOT(moveDown()));
	
  m_entryWidgets.deleteButton = new QToolButton(page);
	m_entryWidgets.deleteButton->setOnIconSet( SmallIcon("edittrash"));
	m_entryWidgets.deleteButton->setOffIconSet( SmallIcon("edittrash"));	
	QToolTip::add(m_entryWidgets.deleteButton, TT_PD_ENTRIES_PI_DELETE);
	QWhatsThis::add(m_entryWidgets.deleteButton, WT_PD_ENTRIES_PI_DELETE);		
	connect( m_entryWidgets.deleteButton, SIGNAL(clicked()),m_entryWidgets.printItemList, SLOT(deleteCurrentItem()));
	
  m_entryWidgets.newPageButton = new QToolButton(page);
 	m_entryWidgets.newPageButton->setOnIconSet( SmallIcon("filenew"));
	m_entryWidgets.newPageButton->setOffIconSet( SmallIcon("filenew"));	
	QToolTip::add(m_entryWidgets.newPageButton, TT_PD_ENTRIES_PI_PAGE_BREAK);
	QWhatsThis::add(m_entryWidgets.newPageButton, WT_PD_ENTRIES_PI_PAGE_BREAK);		
	connect( m_entryWidgets.newPageButton, SIGNAL(clicked()),m_entryWidgets.printItemList, SLOT(newPage()));

  buttonLayout->addWidget(m_entryWidgets.moveUpButton);
  buttonLayout->addWidget(m_entryWidgets.moveDownButton);
  buttonLayout->addWidget(m_entryWidgets.deleteButton);
  buttonLayout->addWidget(m_entryWidgets.newPageButton);
 	buttonLayout->addStretch(3);

  hboxLayout->addLayout( buttonLayout );
  topLayout->addLayout( hboxLayout );
}

/** Is called when the papertype was changed. */
void CPrinterDialog::paperTypeChanged(const int id){
  char buf[30];

  CPrinter::CPageSize size = paperSize( (QPrinter::PageSize)id );
  QString msg = i18n("Size: %1mm x %2mm").arg(size.width).arg(size.height);

  m_general.paperSizeLabel->setText( msg );
  m_general.paperSizeLabel->show();
}

const CPrinter::CPageSize CPrinterDialog::paperSize( QPrinter::PageSize pageSize ) {
  m_printer->setPageSize( pageSize );
  QPaintDeviceMetrics paintMetric( m_printer );

  CPrinter::CPageSize size = { paintMetric.widthMM(), paintMetric.heightMM() };
  return size;
}

/**  */
void CPrinterDialog::slotUser1(){
	applySettingsToPrinter(true);
	m_printer->printQueue();
}

/**  */
void CPrinterDialog::slotUser2(){
	applySettingsToPrinter(false);
	m_printer->printQueue();	
	
	close();
}

/** Calls the CPrinter methods to set settings. */
const bool CPrinterDialog::applySettingsToPrinter( const bool preview ){
	m_printer->setPreview( preview );	
	if (preview)
		m_printer->setPreviewApplication("kghostview");
	else
		m_printer->setPreviewApplication("");

	m_printer->setFullPage(true);
	m_printer->setCreator( i18n("BibleTime version %1").arg(VERSION) );
	
	//apply general settings
	if (!m_general.printerList->currentItem() )
		m_printer->setPrinterName(m_general.printerList->currentItem()->text(0));

  m_printer->setNumCopies( m_general.pageSpin->value() );
  if ( m_general.portraitRadio->isChecked() )
    m_printer->setOrientation( QPrinter::Portrait );
  else
    m_printer->setOrientation( QPrinter::Landscape );
   m_printer->setPageSize((QPrinter::PageSize)m_general.paperSelector->currentItem());

  if (m_general.fileCheck->isChecked()) {	//print to file
		m_printer->setOutputToFile( true );		
		if (m_general.fileInput->text().isEmpty()) {
			return false;
		}
		else {
			m_printer->setOutputFileName( m_general.fileInput->text() );
		}
  }
  else { // do not print into file
		m_printer->setOutputToFile(false);
  }
		
	//apply layout settings
	m_printer->setUpperMargin(m_layout.marginSpin[0]->value());	
	m_printer->setLowerMargin(m_layout.marginSpin[1]->value());	
	m_printer->setLeftMargin(m_layout.marginSpin[2]->value());
	m_printer->setRightMargin(m_layout.marginSpin[3]->value());			

	return true;
}

/** Applies the given styles to the print item list. */
void CPrinterDialog::slotListApplyStyle(const QString& styleName ){
	styleItemList* styleList = m_printer->getStyleList();
	ASSERT(styleList);
	
	for(styleList->first(); styleList->current(); styleList->next()) {
		if (styleList->current()->getStyleName() == styleName)
		 	m_entryWidgets.printItemList->applyStyleToSelected( styleList->current() );
	}
}

/** No descriptions */
void CPrinterDialog::currentStyleChanged( QListViewItem* item){
	m_layout.deleteStyleButton->setEnabled( (bool)item );
	m_layout.editStyleButton->setEnabled( (bool)item );
}
