/***************************************************************************
                          cstyle.cpp  -  description
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

#include "cstyle.h"
#include "cstylelist.h"


//Qt includes
#include <qapp.h>
#include <qdom.h>
#include <qfile.h>
#include <qlistview.h>
#include <qstring.h>
#include <qtextstream.h>

//KDE includes
#include <klocale.h>

#define CURRENT_SYNTAX_VERSION 1

CStyle::Format::Format(const CStyle::StyleType type) : m_frame(0), m_type(type) {
	clearData();
}

CStyle::Format::Format(const Format& f) {
	m_BGColor = f.m_BGColor;
	m_FGColor = f.m_FGColor;
	m_alignment = f.m_alignment;
	m_font = f.m_font;
	m_hasFrame = f.m_hasFrame;			
	m_frame = 0;
	if (m_hasFrame) {
		Frame* newFrame = frame();//create
		*newFrame = *f.m_frame; //copy content
	}
	m_type = f.m_type;
};

CStyle::Format::~Format(){
	delete m_frame;
}

/** Returns the font of this style. */
const QFont& CStyle::Format::font() const {
	return m_font;
}

/** sets the font of this format. */
void CStyle::Format::setFont( QFont newFont ) {
	m_font = newFont;
}

/** Returns the background color of this format. */
const QColor& CStyle::Format::color( const Color type ) const {
	switch (type) {
		case Foreground:
			return m_FGColor;
		case Background:
			return m_BGColor;		
	};
	return Qt::white; //shouldn't be executed
}

/** Sets the foreground color of this format. */
void CStyle::Format::setColor( const Color type, const QColor& newColor ) {
	switch (type) {
		case Foreground:
			m_FGColor = newColor;
			break;
		case Background:
			m_BGColor = newColor;
			break;
	};
}

/** Returns the frame if we have one. Otherwise we return 0. */
CStyle::Format::Frame* const CStyle::Format::frame() {
	if (m_hasFrame)
		return !m_frame ? (m_frame = new CStyle::Format::Frame) : m_frame;
	return 0;
}

/** Sets the frame of this style. */
void CStyle::Format::setFrameEnabled( const bool hasFrame ) {
	m_hasFrame = hasFrame;
	if (!m_hasFrame) {
		delete m_frame;
		m_frame = 0;
	};
}

/** Returns the alignement of this style format. */
const CStyle::Format::Alignment& CStyle::Format::alignment() const {
	return m_alignment;
}

/** Sets the alignement flags of this style format. */
void CStyle::Format::setAlignment( const Alignment newAlignment) {
	m_alignment = newAlignment;
}

/** Resets the data variables. */
void CStyle::Format::clearData(){
	m_BGColor = Qt::white;
	m_FGColor = Qt::black;	
	delete m_frame;
	m_frame = 0;	
	m_hasFrame = false;
	m_font = QApplication::font();
	m_alignment = Format::Left;
}

const CStyle::StyleType CStyle::Format::type() const {
	return m_type;
}

CStyle::Format::Frame::Frame() {
	m_thickness = 1;
	m_color = Qt::black;
	m_lineStyle = Qt::SolidLine;
}

CStyle::Format::Frame::Frame(const Frame& f) {
	m_thickness = f.m_thickness;
	m_color = f.m_color;
	m_lineStyle = f.m_lineStyle;
}

/** Returns the thickness of this frame. */
const int& CStyle::Format::Frame::thickness() const {
	return m_thickness;
}

/** Sets te thickness of this frame. */
void CStyle::Format::Frame::setThickness( const int newThickness ) {
	m_thickness = newThickness;
}

/** Returns the style how to paint the lines of this frame. */
const Qt::PenStyle& CStyle::Format::Frame::lineStyle() const {
	return m_lineStyle;
}

/** Sets the style how to paint the lines. */
void CStyle::Format::Frame::setLineStyle( const Qt::PenStyle newLineStyle ){
	m_lineStyle = newLineStyle;
}

/** Returns the color of this frame. */
const QColor& CStyle::Format::Frame::color() const {
	return m_color;
}

/** Sets the color of this frame. */
void CStyle::Format::Frame::setColor( const QColor& newColor ) {
	m_color = newColor;
}

CStyle::CStyle() :
	m_headerFormat(new Format(CStyle::Header)),
	m_descriptionFormat(new Format(CStyle::Description)),
	m_moduleTextFormat(new Format(CStyle::ModuleText))
{

	//dummy command to translate names of standard styles
//	i18n("Standard");
//	i18n("Colored style for larger text portions");
//18n("Black&White for larger text portions");
	
	m_listViewItem = 0;
	m_isHeaderFormatEnabled = m_isDescriptionFormatEnabled = m_isModuleTextFormatEnabled = true;

	setStyleName( i18n("Unknown") );
};

CStyle::CStyle( const QString text ) :
	m_headerFormat(new Format(CStyle::Header)),
	m_descriptionFormat(new Format(CStyle::Description)),
	m_moduleTextFormat(new Format(CStyle::ModuleText))
{
	m_listViewItem = 0;
	m_isHeaderFormatEnabled = m_isDescriptionFormatEnabled = m_isModuleTextFormatEnabled = true;
	
	loadFromText(text);	
}

CStyle::~CStyle(){
}

/** Returns the proper CStyleFormat for the given type. */
CStyle::Format* const CStyle::formatForType( const CStyle::StyleType type) {
	switch (type) {
		case Header:
			return m_headerFormat;
		case Description:
			return m_descriptionFormat;		
		case ModuleText:
			return m_moduleTextFormat;		
		default:
			return 0;
	}
}

/** Set the printing of the header (true enables it). */
void CStyle::setFormatTypeEnabled( const CStyle::StyleType type, const bool setEnabled) {
	switch (type) {
		case Header:
			m_isHeaderFormatEnabled = setEnabled;
			break;
		
		case Description:
			m_isDescriptionFormatEnabled = setEnabled;		
			break;
		
		case ModuleText:
			m_isModuleTextFormatEnabled = setEnabled;		
			break;
		
		default:
			break;
	}
}

/** Returns a QListViewItem for inserted in list. */
QListViewItem* const CStyle::listViewItem( CStyleList* const list ){
	if (!list) {
		if (!m_listViewItem)
			return 0;
		else
			return m_listViewItem;
	}
	deleteListViewItem();
	m_listViewItem = new QListViewItem( list );
	updateListViewItem();	
	return m_listViewItem;
}

/** Returns the style name */
const QString& CStyle::styleName() const{
	return m_name;
}

/** Sets the name of the style. */
void CStyle::setStyleName( const QString name ){
	m_name = name;
}

/** Clears all variables and sets them back */
void CStyle::clearData(){
//	qDebug("CStyle::clearData()");
	deleteListViewItem();
	m_name = QString::null;
	m_isDescriptionFormatEnabled = m_isHeaderFormatEnabled = m_isModuleTextFormatEnabled = true;		
}

/** Updates the Listview items */
void CStyle::updateListViewItem(){
//	qWarning("CStyle::updateListViewItem() for %s", m_name.latin1());
	m_listViewItem->setText(0, styleName() );
}

/** Deletes the list view item. */
void CStyle::deleteListViewItem(){
	delete m_listViewItem;
	m_listViewItem = 0;
}

/** Loads the profile from a XML file. */
const bool CStyle::loadFromFile(const QString& filename){
	QFile file(filename);
	if (!file.exists()) {
		qWarning("file doesn't exist");
		return false;
	}
	
	if (file.open(IO_ReadOnly)) {	
		QTextStream t(&file);
		t.setEncoding(QTextStream::UnicodeUTF8);		
		loadFromText( t.read() );
		file.close();	
	}
	else {
		qWarning("CStyle::load: unable to open file %s", filename.latin1());
		return false;
	};
  return true;
}

const bool CStyle::loadFromText(const QString& text){
/**
* The XML file should look like this:
* 	
 <?xml version="1.0" encoding="UTF-8"?><!DOCTYPE DOC >
 <BibleTimePrintingStyle syntaxVersion="1" name="Standard" translate="1">
  <HEADER enabled="1" alignment="center">
    <COLORS bgcolor="black" fgcolor="white">
    <FONT family="Arial" charset="2" pointsize="10" weight="1" italic="0">
    <FRAME enabled="1" color="blue" style="1" thickness="2">
  </HEADER>
 </BibleTimePrintingStyle>
*/
	QDomDocument doc;		
  doc.setContent(text);
	
  QDomElement document = doc.documentElement();
  if(document.tagName() != "BibleTimePrintingStyle") {
		qWarning("CProfile::load: Missing BibleTime doc");
		return false;
	}
	if (document.hasAttribute("name")) { //name of the printing style
		m_name = document.attribute("name");
		if (document.hasAttribute("translate") && document.attribute("translate").toInt()) {
			m_name = i18n(m_name.local8Bit()); //standard styles should be translated
		}
	}
	
	QDomElement elem = document.firstChild().toElement();
	while (!elem.isNull()) {
		StyleType type = Unknown;
		if (elem.tagName() == "HEADER") {
			type = Header;
		}
		else if (elem.tagName() == "DESCRIPTION") {
			type = Description;
		}
		else if (elem.tagName() == "MODULETEXT") {
			type = ModuleText;
		}
		Format* p = formatForType(type);

		if (!p || type == Unknown)
			continue;
		
		if (elem.hasAttribute("enabled")) {
			setFormatTypeEnabled(type, elem.attribute("enabled").toInt());
		}
		if (elem.hasAttribute("alignment")) {		
			const int align = elem.attribute("alignment").toInt();
			p->setAlignment( static_cast<Format::Alignment>(align) );
		}
		
		QDomElement object = elem.namedItem("COLORS").toElement();
		if (!object.isNull()) {
			if (object.hasAttribute("bgcolor"))
				p->setColor(Format::Background, QColor(object.attribute("bgcolor")));
			if (object.hasAttribute("fgcolor"))
				p->setColor(Format::Foreground, QColor(object.attribute("fgcolor")));				
		}

		object = elem.namedItem("FONT").toElement();
		if (!object.isNull()) {
			QString family = object.hasAttribute("family") ? object.attribute("family") : QApplication::font().family();
			unsigned int size = object.hasAttribute("pointsize") ? object.attribute("pointsize").toInt() : QApplication::font().pointSize();
			const int weight = object.hasAttribute("weight") ? object.attribute("weight").toInt() : QFont::Normal;
			const bool italic = object.hasAttribute("italic") ? static_cast<bool>(object.attribute("italic").toInt()) : false;
			
			if (family.isEmpty())
				family = QApplication::font().family();
			if (!size)
				size = QApplication::font().pointSize();
			
			p->setFont( QFont(family, size, weight, italic) );			
		}	

		object = elem.namedItem("FRAME").toElement();
		if (!object.isNull()) { //frame-section exists
			const bool enabled = object.hasAttribute("enabled") ? object.attribute("enabled").toInt() : false;			
			p->setFrameEnabled(enabled);
			if (Format::Frame* frame = p->frame()) {
				const QColor color = object.hasAttribute("color") ? QColor(object.attribute("color")) : Qt::white;
				Qt::PenStyle style = object.hasAttribute("style") ? static_cast<Qt::PenStyle>(object.attribute("style").toInt()) : Qt::SolidLine;
				const int thickness = object.hasAttribute("thickness") ? object.attribute("thickness").toInt() : 1;			
				
				frame->setColor(color);
				frame->setLineStyle(style);
				frame->setThickness(thickness);
			}
		}
		elem = elem.nextSibling().toElement();	
	}
	return true;
}

const bool CStyle::saveToFile( const QString& filename ){
/* Saved file should look like this:

 <?xml version="1.0" encoding="UTF-8"?><!DOCTYPE DOC >
 <BibleTimePrintingStyle syntaxVersion="1" name="Standard" translate="1">
  <HEADER enabled="1" alignment="center">
    <COLORS bgcolor="black" fgcolor="white">
    <FONT family="Arial" charset="2" pointsize="10" weight="1" italic="0">
    <FRAME enabled="1" color="blue" style="1" thickness="2">
  </HEADER>
  // same for DESCRIPTION and MODULETEXT

 </BibleTimePrintingStyle>
*/
	bool ret = false;
	
  QDomDocument doc("DOC");
  doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );

  QDomElement content = doc.createElement("BibleTimePrintingStyle");
  content.setAttribute("syntaxVersion", CURRENT_SYNTAX_VERSION);
  content.setAttribute("name", m_name);
  content.setAttribute("translate", 0); //don't trabslate styles except of prinstalled ones
  doc.appendChild(content);

  for (unsigned int i = Header; i <= ModuleText; ++i) {
		QDomElement elem;
			
		const StyleType type = static_cast<StyleType>(i);
		switch (type) {
			case Header:
				elem = doc.createElement("HEADER");
				break;
			case Description:
				elem = doc.createElement("DESCRIPTION");			
				break;
			case ModuleText:
				elem = doc.createElement("MODULETEXT");
				break;
			default:
				break;
		};		
		if (elem.isNull()) //shouldn't happen
			continue;
		content.appendChild(elem);
			
		Format* format = formatForType(type);			
		
		elem.setAttribute("enabled", hasFormatTypeEnabled(type));
		elem.setAttribute("alignment", static_cast<int>(format->alignment()));
		
		QDomElement object = doc.createElement("COLORS");
		object.setAttribute("bgcolor", format->color(Format::Background).name());
		object.setAttribute("fgcolor", format->color(Format::Foreground).name());
		elem.appendChild(object);		
				
		QFont font = format->font();
		object = doc.createElement("FONT");
		object.setAttribute("family", font.family());
		object.setAttribute("pointsize", font.pointSize());		
		object.setAttribute("weight", font.weight());
		object.setAttribute("italic", font.italic());
		elem.appendChild(object);
						
		object = doc.createElement("FRAME");		
		Format::Frame* frame = format->frame();
		object.setAttribute("enabled", static_cast<bool>(frame) );
		if (frame) {
			object.setAttribute("color", frame->color().name());
			object.setAttribute("thickness", frame->thickness());
			object.setAttribute("style", static_cast<int>(frame->lineStyle()) );
    }		
		elem.appendChild(object);
  };
  ret = false;


	QFile file(filename);
	if (file.open(IO_WriteOnly)) {
		ret = true;
		QTextStream t( &file );        // use a text stream
		t.setEncoding(QTextStream::UnicodeUTF8);
		t << doc.toString();
		file.close();
	}
	return ret;
}
