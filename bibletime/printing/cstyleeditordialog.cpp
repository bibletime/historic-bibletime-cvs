/***************************************************************************
                          cstyleeditordialog.cpp  -  description
                             -------------------
    begin                : Sun Aug 20 2000
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

#include "cstyleeditordialog.h"

//KDE includes
#include <kcombobox.h>
#include <klocale.h>
#include <kcolorbutton.h>
#include <kfontdialog.h>

//Qt includes
#include <qlineedit.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qwidget.h>
#include <qbuttongroup.h>
#include <qvbuttongroup.h>
#include <qhbuttongroup.h>
#include <qradiobutton.h>
#include <qspinbox.h>

#define INNER_BORDER 4
#define OUTER_BORDER 4

CStyleEditorDialog::CStyleEditorDialog(CStyle* style, QWidget *parent, const char *name )
	: KDialogBase(parent,name, true, i18n("style editor")) {
	m_style = style;
	m_currentFormat = 0;
	m_formatEnabled =  false;
	
	initView();	
	readSettings();	
	styleTypeChanged("Header");
}

CStyleEditorDialog::~CStyleEditorDialog(){
	saveSettings();
}

/** Initializes te view. */
void CStyleEditorDialog::initView(){
  enableButtonSeparator( true );
  QFrame *mainWidget = makeMainWidget();
  QLabel* label = 0;

  QVBoxLayout *topLayout = new QVBoxLayout( mainWidget, OUTER_BORDER, INNER_BORDER );
	
	QHBoxLayout *hboxLayout = new QHBoxLayout(0, OUTER_BORDER, INNER_BORDER);
  m_styleNameEdit = new QLineEdit( mainWidget );
	label = new QLabel(m_styleNameEdit, i18n("Name of style:"), this);
	hboxLayout->addWidget(label);
	hboxLayout->addWidget(m_styleNameEdit);	

  topLayout->addLayout(hboxLayout);

	hboxLayout = new QHBoxLayout(0, OUTER_BORDER, INNER_BORDER);	
  m_styleTypeChooser = new KComboBox( mainWidget );
  label = new QLabel( m_styleTypeChooser, i18n("Choose the part of the style:"), mainWidget );
  hboxLayout->addWidget(label);
  m_styleTypeChooser->insertItem( i18n("Header") );
  m_styleTypeChooser->insertItem( i18n("Description") );
  m_styleTypeChooser->insertItem( i18n("Module text") );
  connect(m_styleTypeChooser, SIGNAL(activated(const QString&)), SLOT(styleTypeChanged(const QString&)));
  m_currentFormat = m_style->formatForType( CStyle::Header );
  hboxLayout->addWidget( m_styleTypeChooser );

  m_setEnabledBox = new QCheckBox( mainWidget );
  m_setEnabledBox->setText( i18n("Enable this part"));
  connect( m_setEnabledBox, SIGNAL(clicked()), SLOT(enableBoxClicked()));
  hboxLayout->addWidget( m_setEnabledBox );

  topLayout->addLayout( hboxLayout );


	hboxLayout = new QHBoxLayout(0, OUTER_BORDER, INNER_BORDER);
	topLayout->addLayout(hboxLayout);

  m_alignRadios.buttongroup = new QVButtonGroup( mainWidget );
	m_alignRadios.leftRB = new QRadioButton( i18n("Left"), m_alignRadios.buttongroup);
	m_alignRadios.centerRB = new QRadioButton( i18n("Center"), m_alignRadios.buttongroup);
	m_alignRadios.rightRB = new QRadioButton(i18n("Right"),m_alignRadios.buttongroup);
//	m_alignRadios.justificationRB = new QRadioButton(i18n("Justification"), m_alignRadios.buttongroup);
	m_alignRadios.buttongroup->setFixedHeight( m_alignRadios.buttongroup->sizeHint().height());
  hboxLayout->addWidget( m_alignRadios.buttongroup );

	QButtonGroup* box = new QButtonGroup( mainWidget );
	QVBoxLayout* boxLayout = new QVBoxLayout( box, OUTER_BORDER, INNER_BORDER );
	
	QHBoxLayout* layout1 = new QHBoxLayout(0, 1, 1);
	m_colors.foregroundChooser = new KColorButton( box );
	label = new QLabel(m_colors.foregroundChooser, "label", box);
	label->setText(i18n("Foreground:"));	
	layout1->addWidget(label);
  layout1->addStretch(2);	
	layout1->addWidget(m_colors.foregroundChooser);
  	
  boxLayout->addLayout(layout1);
  boxLayout->addStretch(3);
  	
	layout1 = new QHBoxLayout(0, 1, 1);
	m_colors.backgroundChooser = new KColorButton(box);
	label = new QLabel(m_colors.backgroundChooser, "label", box);
	label->setText(i18n("Background:"));
	layout1->addWidget(label);	
  layout1->addStretch(2);	
	layout1->addWidget(m_colors.backgroundChooser);
    	
  boxLayout->addLayout(layout1);
  boxLayout->addStretch(3);
  hboxLayout->addWidget(box);

  hboxLayout = new QHBoxLayout();
	m_font.fontButton = new QPushButton(mainWidget);
	m_font.fontButton->setText(i18n("Choose font"));
	connect(m_font.fontButton, SIGNAL(clicked()), SLOT(showFontChooser()));	
  m_font.fontDisplay = new QLabel(m_font.fontButton, "", mainWidget);	
  m_font.fontDisplay->setText(i18n("Font preview"));
  hboxLayout->addWidget(m_font.fontDisplay);
  hboxLayout->addWidget(m_font.fontButton);

//  m_font.identation = new QSpinBox(mainWidget);
//  label = new QLabel(m_font.identation, "identation", mainWidget);
//  label->setText(i18n("Identation (in mm):"));
//  hboxLayout->addStretch(2);
//  hboxLayout->addWidget(label);
//  hboxLayout->addWidget(m_font.identation);

  topLayout->addLayout(hboxLayout);

  m_frame.useFrame = new QCheckBox(mainWidget);
  m_frame.useFrame->setText( i18n("Use frame") );
  topLayout->addWidget(m_frame.useFrame);
  connect(m_frame.useFrame, SIGNAL(clicked()), SLOT(useFrameClicked()));

  m_frame.groupbox = new QGroupBox(mainWidget);
	boxLayout = new QVBoxLayout(m_frame.groupbox);
	
	hboxLayout = new QHBoxLayout();
  m_frame.lineThicknessChooser = new QSpinBox( m_frame.groupbox );
  m_frame.lineThicknessChooser->setMinValue(1);
  label = new QLabel(m_frame.lineThicknessChooser, "label", m_frame.groupbox);
  label->setText( i18n("Line thickness (in point):") );
  hboxLayout->addWidget(label);
	hboxLayout->addWidget(m_frame.lineThicknessChooser);	
	boxLayout->addLayout(hboxLayout);
	
	hboxLayout = new QHBoxLayout();
	m_frame.colorChooser = new KColorButton(m_frame.groupbox);
	label = new QLabel(m_frame.colorChooser, "", m_frame.groupbox);
	label->setText(i18n("Frame color:"));
	hboxLayout->addWidget(label);
	hboxLayout->addWidget(m_frame.colorChooser);
	boxLayout->addLayout(hboxLayout);

	hboxLayout = new QHBoxLayout();
	m_frame.lineStyleChooser = new QComboBox(m_frame.groupbox);
	label = new QLabel(m_frame.lineStyleChooser, "", m_frame.groupbox);
	label->setText(i18n("Line style:"));
	hboxLayout->addWidget(label);
	hboxLayout->addWidget(m_frame.lineStyleChooser);
	boxLayout->addLayout(hboxLayout);
			
	topLayout->addWidget(m_frame.groupbox);
	topLayout->addStretch(3);
	
	QPainter* p = new QPainter();
	QPen pen(Qt::black, 2, Qt::SolidLine);
	QBrush brush(Qt::white);
		
	for (int i=1; i <= 5; i++) {  // from Qt::SolidLine to Qt::DashDotDotLine
		QPixmap* pix = new QPixmap(/*m_frame.lineStyleChooser->width()*/150,15);
		pen.setStyle((Qt::PenStyle)i);
		p->begin(pix);
		p->setPen(pen);
		p->fillRect(0,0, pix->width(), pix->height(), brush);
		p->drawLine(0,(int)((float)pix->height()/2),pix->width(),(int)((float)pix->height()/2));
		p->end();
		m_frame.lineStyleChooser->insertItem(*pix);	
	}
	delete p;
	
	
	
	m_currentFormat = m_style->formatForType( CStyle::Header );	
	setupWithFormat( m_currentFormat );
	enableBoxClicked();
}

/** Reads settings from config file */
void CStyleEditorDialog::readSettings(){
	m_styleNameEdit->setText( m_style->styleName() );
	if (m_styleNameEdit->text() == i18n("Standard"))
		m_styleNameEdit->setEnabled(false);	//you can't change the name of the standard style
}

/** Saves settings to config file. */
void CStyleEditorDialog::saveSettings(){
	applySettingsToFormat( m_currentFormat );
  m_style->setStyleName( m_styleNameEdit->text() );	
}

/** Opens the font chooser dialog. */
void CStyleEditorDialog::showFontChooser(){
	int result = KFontDialog::getFont( m_font.font );
	if ( result == KFontDialog::Accepted ) {
		setupFontWidgets( m_font.font );
	}
}

/**  */
void CStyleEditorDialog::useFrameClicked(){
	m_frame.groupbox->setEnabled( m_frame.useFrame->isChecked() );
}

/** Sets up the states of the child widgets using the styl format given as parameter. */
void CStyleEditorDialog::setupWithFormat( CStyleFormat* format){
	if (!format)
		return;	

	CStyle::styleType styleType = CStyle::Unknown;
	if (m_currentFormat == m_style->formatForType(CStyle::Header))
		styleType = CStyle::Header;
	else if (m_currentFormat == m_style->formatForType(CStyle::Description))
		styleType = CStyle::Description;
	else if (m_currentFormat == m_style->formatForType(CStyle::ModuleText))
		styleType = CStyle::ModuleText;
	
	if (styleType == CStyle::Unknown)
		return;
	
	m_formatEnabled = m_style->hasFormatTypeEnabled(styleType);
	
  m_styleNameEdit->setText( m_style->styleName());	
	//setup alignement
	switch(format->alignement()) {
		case CStyleFormat::Left:
			m_alignRadios.leftRB->setChecked(true);
			break;
		case CStyleFormat::Center:
			m_alignRadios.centerRB->setChecked(true);
			break;
		case CStyleFormat::Right:
			m_alignRadios.rightRB->setChecked(true);
			break;
//		case CStyleFormat::Justification:
//			m_alignRadios.justificationRB->setChecked(true);
//			break;
		default:
			m_alignRadios.leftRB->setChecked(true);
			break;
	}
	m_alignRadios.buttongroup->setEnabled(m_formatEnabled);	
	m_setEnabledBox->setChecked(m_formatEnabled);
	
	//setup colors
	m_colors.backgroundChooser->setColor( format->color( CStyleFormat::Background ) );
	m_colors.backgroundChooser->setEnabled(m_formatEnabled);		
	
	m_colors.foregroundChooser->setColor( format->color( CStyleFormat::Foreground ) );
	m_colors.foregroundChooser->setEnabled(m_formatEnabled);
		
	//setup fonts
	m_font.font = format->getFont();
	setupFontWidgets( m_font.font );
//	m_font.identation->setValue( format->getIdentation() );
	
	//setup frame part
	CStyleFormatFrame* frame = format->frame();
	m_frame.useFrame->setChecked(frame);
	m_frame.useFrame->setEnabled(m_formatEnabled);		
	m_frame.groupbox->setEnabled(m_formatEnabled);		
	useFrameClicked();
	
	m_frame.colorChooser->setColor( frame->color() );
	m_frame.lineThicknessChooser->setValue( frame->thickness() );	
	m_frame.lineStyleChooser->setCurrentItem((int)(frame->lineStyle())-1);	
}

/** Setups the font widgets using the parameter. */
void CStyleEditorDialog::setupFontWidgets( QFont& font ){
	QFont dummy(font);
	dummy.setPointSize(12);
	m_font.fontDisplay->setFont( dummy );
	m_font.fontDisplay->setText( QString::fromLatin1("%1 - %2").arg(m_font.font.family()).arg(m_font.font.pointSize()) );
	m_font.fontDisplay->setEnabled(m_formatEnabled);
	
	m_font.fontButton->setEnabled(m_formatEnabled);	
}

/** Called when the type was changed in the combobox. */
void CStyleEditorDialog::styleTypeChanged( const QString& name ){
	applySettingsToFormat( m_currentFormat );
	
	if (name == i18n("Header")) {
		m_currentFormat = m_style->formatForType( CStyle::Header );
		m_formatEnabled = m_style->hasFormatTypeEnabled( CStyle::Header );
	}
	else 	if (name == i18n("Description")) {
		m_currentFormat = m_style->formatForType( CStyle::Description );
		m_formatEnabled = m_style->hasFormatTypeEnabled( CStyle::Description );
	}
	else 	if (name == i18n("Module text")) {
		m_currentFormat = m_style->formatForType( CStyle::ModuleText );	
		m_formatEnabled = m_style->hasFormatTypeEnabled( CStyle::ModuleText );	
	}
	setupWithFormat( m_currentFormat );
}

/** Sets the properties of the cuzrrent format which are changed in the editor. */
void CStyleEditorDialog::applySettingsToFormat( CStyleFormat* format ){
	if (!format)
		return;
	m_style->setStyleName(m_styleNameEdit->text());
	
	//apply alignement settings
	if (m_alignRadios.buttongroup->selected() == m_alignRadios.leftRB ) {
		format->setAlignement( CStyleFormat::Left);
	}
	else if (m_alignRadios.buttongroup->selected() == m_alignRadios.centerRB) {
		format->setAlignement( CStyleFormat::Center );
	}
	else if (m_alignRadios.buttongroup->selected() == m_alignRadios.rightRB ) {
		format->setAlignement( CStyleFormat::Right );
	}
//	else if (m_alignRadios.buttongroup->selected() == m_alignRadios.justificationRB ) {
//		format->setAlignement( CStyleFormat::Justification );
//	}
	
	//apply color settings
	format->setColor( CStyleFormat::Background, m_colors.backgroundChooser->color() );
	format->setColor( CStyleFormat::Foreground, m_colors.foregroundChooser->color() );
	
	//apply font settings
	format->setFont( m_font.font );
	
	//apply frame settings
	CStyleFormatFrame* frame = format->frame();
	if (frame) {
		frame->setColor( m_frame.colorChooser->color() );	
		frame->setThickness( m_frame.lineThicknessChooser->value() );
		//the position in the list equal to the position in Qt::PenStyle+1
		frame->setLineStyle((Qt::PenStyle)(m_frame.lineStyleChooser->currentItem()+1));
//		qWarning("%i",m_frame.lineStyleChooser->currentItem()+1);
	}
	
	format->setFrame( m_frame.useFrame->isChecked(), frame );
}

/** Is called when the enablePart box was clicked. */
void CStyleEditorDialog::enableBoxClicked() {
	//find the correct format type
	CStyle::styleType styleType = CStyle::Unknown;	
	if (m_currentFormat == m_style->formatForType(CStyle::Header))
		styleType = CStyle::Header;
	else if (m_currentFormat == m_style->formatForType(CStyle::Description))
		styleType = CStyle::Description;
	else if (m_currentFormat == m_style->formatForType(CStyle::ModuleText))
		styleType = CStyle::ModuleText;
	
	m_style->setFormatTypeEnabled(styleType, m_setEnabledBox->isChecked());	
	applySettingsToFormat(m_style->formatForType(styleType));
	setupWithFormat( m_currentFormat );
}
