/***************************************************************************
                          cprintitem.cpp  -  description
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


#include "cprintitem.h"
#include "cprintitemlist.h"
#include "cstyle.h"
#include "cstyleformat.h"
#include "cstyleformatframe.h"
#include "../backend/cswordmoduleinfo.h"
#include "../backend/cswordversekey.h"
#include "../backend/cswordldkey.h"
#include "../backend/chtmlentrydisplay.h"
#include "../frontend/optionsdialog/coptionsdialog.h"


//Qt includes
#include <qsimplerichtext.h>
#include <qlistview.h>
#include <qregexp.h>
#include <qstylesheet.h>

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <versekey.h>


#define PARAGRAPH_SPACE 10 // Space between different paragraphs
#define STYLE_PART_SPACE 1	//Space between the differnt parts (header, moduletext ...)
#define BORDER_SPACE 4 	//border between text and rectangle


CPrintItem::CPrintItem() {
	m_listViewItem = 0;
	m_module = 0;
	m_style = 0;
	m_startKey = 0;
	m_stopKey = 0;
	
	clearData();
}

CPrintItem::~CPrintItem(){
	if (m_startKey && m_startKey == m_stopKey)
		delete m_startKey;
	else {
		if (m_startKey) {
			delete m_startKey;
			m_startKey = 0;
		}
		if (m_stopKey) {
			delete m_stopKey;
			m_stopKey = 0;		
		}
	}
}

/** Returns the first key covered by this entry. */
CSwordKey* CPrintItem::getStartKey() const{
	return m_startKey;
}

/** Sets the startkey. */
void CPrintItem::setStartKey(CSwordKey* newKey) {
	if (m_startKey)
		delete m_startKey;
	m_startKey = newKey;

}

/** Sets the end key. */
void CPrintItem::setStopKey( CSwordKey* newKey ){
	if (m_stopKey)
		delete m_stopKey;	
	m_stopKey = newKey;
}

/** Returns the last covered key. */
CSwordKey* CPrintItem::getStopKey() const {
	return m_stopKey;
}

/** Returns the used module. */
CSwordModuleInfo* CPrintItem::getModule() const {
	return m_module;
}

/** Sets the used module. */
void CPrintItem::setModule( CSwordModuleInfo* newModule ){
	m_module = newModule;
}

/** Returns the description. Only valid for inserted bookmarks. */
const QString& CPrintItem::getDescription() const {
	return m_description;
}

/** Sets the decsription. */
void CPrintItem::setDescription( const QString& newDescription ){
	m_description = newDescription;
}

/** Returns the moduletext used by this item. */
const QString& CPrintItem::getModuleText() {
//	qDebug("const QString CPrintItem::getModuleText()");
	/**
	* If a special text is set use the text.
	* If the moduleText variable is empty use the CModuleInfo
	* object to retrieve the text,
	*/
	if (!m_moduleText.isEmpty())
		return m_moduleText;
	if (!m_startKey)
		return QString::null;
	
	CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(m_startKey);
//	CSwordKey* key = dynamic_cast<CSwordKey*>(m_startKey);
//	CSwordModuleInfo* sw = dynamic_cast<CSwordModuleInfo*>(m_module);
	
	m_moduleText = vk ? QString::fromLatin1("<FONT SIZE=\"-1\"><NOBR>(%1)</NOBR></FONT>").arg(vk->Verse()): QString::null;
	m_moduleText += m_startKey ? m_startKey->renderedText() : QString::null;
	if (m_module && m_stopKey && m_stopKey != m_startKey) { //range of entries
		if (m_module->getType() == CSwordModuleInfo::Bible  || m_module->getType() == CSwordModuleInfo::Commentary ) {
			CSwordVerseKey* vk_start = dynamic_cast<CSwordVerseKey*>(m_startKey);
			CSwordVerseKey* vk_stop = dynamic_cast<CSwordVerseKey*>(m_stopKey);			
			if (!vk_start && !vk_stop)
				return m_moduleText;
			
			CSwordVerseKey dummyKey(m_module);				
			dummyKey.key( vk_start->key() );
			while (dummyKey < *vk_stop) {
				dummyKey.NextVerse();
				m_moduleText += QString::fromLatin1("<FONT SIZE=\"-1\"><NOBR>(%1)</NOBR></FONT>").arg(dummyKey.Verse()) + dummyKey.renderedText();
			}
		}
		else if (m_module->getType() == CSwordModuleInfo::Lexicon )
			qWarning("implement for range of lexicon entries");
	}		
	m_moduleText.replace(QRegExp("$\n+"), "");
	m_moduleText.replace(QRegExp("$<BR>+"), "");	
	
	return m_moduleText;
}

/** Sets the module text. */
void CPrintItem::setModuleText( const QString& newText ){
	m_moduleText = newText;
}

/** Returns the text of the header. */
const QString& CPrintItem::getHeader() const {
	return m_headerText;
}

/**  */
void CPrintItem::setHeader( const QString& newText){
	m_headerText = newText;
}

/** Sets the style for this item. */
void CPrintItem::setStyle( CStyle* newStyle ) {
//	qDebug("CPrintItem::setStyle( CStyle* newStyle )");
	m_style = newStyle;
}

/** Returns the style used by this item. */
CStyle* CPrintItem::getStyle() const {
	return m_style;
}
/** Returns the listview item for this printitem. */
QListViewItem* CPrintItem::getListViewItem( CPrintItemList* list ) {
	deleteListViewItem();
	m_listViewItem = new QListViewItem( list );
	updateListViewItem();
	return m_listViewItem;
}

/** sets the variables back. */
void CPrintItem::clearData(){
	deleteListViewItem();
	setHeader(QString::null);
	setDescription(QString::null);
	setModuleText(QString::null);	
	setStyle(0);	
	setStartKey(0);
	setStopKey(0);
	setModule(0);
}

/** Updates the item. */
void CPrintItem::updateListViewItem(){
	if (m_module)
		m_listViewItem->setText(0, m_module->name() );

	if (m_startKey)
		m_listViewItem->setText(1,m_startKey->key());
	if (m_stopKey)
		m_listViewItem->setText(2,m_stopKey->key());
	else if (m_startKey)
		m_listViewItem->setText(2,m_startKey->key());
		
	if (getStyle())
		m_listViewItem->setText(3, getStyle()->getStyleName() );
}

/**  */
QListViewItem* CPrintItem::getListViewItem() const {
	return m_listViewItem;
}

/** Deletes the list view item. */
void CPrintItem::deleteListViewItem(){
	if (m_listViewItem)
		delete m_listViewItem;
	m_listViewItem = 0;
}

/** Draws the content of this item on the page. */
void CPrintItem::draw(QPainter* p, CPrinter* printer){
	/**
	* Print this item
	* Steps to do for the printing:
	*
	*	1. Get the settings (font, colors, alignment etc.) and generate the correct richtext for this (correct alignment)
	*	2. Apply the settings to the printer's painter object
	*/
	QFont font;
	QColor fgColor;
	QColor bgColor;
	CStyleFormat* format = 0;
	CStyleFormatFrame* frame = 0;
	int frameThickness = 0;
	CStyleFormat::alignement alignement;
	int identation = 0;
	CStyle::styleType type = CStyle::Unknown;
	QString text;
	QColorGroup cg;
	QPen pen;
	QBrush brush;
	
	//print the header
	for (int i = 0; i < 3; ++i) {		
		type = (CStyle::styleType)i;
		
		format = m_style->getFormatForType( type );
		ASSERT(format);
		fgColor = format->getFGColor();
		bgColor = format->getBGColor();	
		pen.setColor(fgColor);
		font = format->getFont();
		frame = format->hasFrame() ? format->getFrame() : 0;
		frameThickness = frame ? frame->getThickness() : 0;		
		alignement = format->getAlignement();
		identation = format->getIdentation();
		if (type == CStyle::Header)
			text = getHeaderText();
		else if (type == CStyle::Description)
			text = m_description;
		else
			text = getModuleText();

		p->setFont(font);
		p->setPen(pen);
		cg.setColor(QColorGroup::Text, format->getFGColor());
		
		if (!m_style->hasFormatTypeEnabled(type))
			continue;
		int arguments = Qt::WordBreak;		
			if (alignement == CStyleFormat::Left)
				arguments |= Qt::AlignLeft;
			else if (alignement == CStyleFormat::Center)
				arguments |= Qt::AlignHCenter;
			else if (alignement == CStyleFormat::Right)
				arguments |= Qt::AlignRight;
		QRect boundingRect;
		QRect br;
		if ((type == CStyle::Header || type == CStyle::Description) && !text.isEmpty()) {
			boundingRect = p->boundingRect( printer->leftMargin(), printer->getVerticalPos(),
				printer->getPageSize().width(), printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin(), arguments, text );
			
			//check if the new text fits into page
			if ( (boundingRect.height() + frameThickness + STYLE_PART_SPACE ) > printer->getPageSize().height()-printer->getVerticalPos() ) {
				//this part doesn't fit on the current page
				printer->newPage();
				boundingRect = p->boundingRect( printer->leftMargin(), printer->getVerticalPos(),
					printer->getPageSize().width(), printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin(), arguments, text );			
			}
			br = boundingRect;
			br.setX(printer->leftMargin());
			br.setWidth(printer->getPageSize().width());
			p->fillRect( br, bgColor );	
						
			p->drawText(boundingRect, arguments, text);
			printer->setVerticalPos( printer->getVerticalPos() + boundingRect.height() + 2*frameThickness + STYLE_PART_SPACE );

			if (frame) {
				QPen framePen = pen;
				framePen.setWidth( frameThickness );
				framePen.setColor( frame->getColor() );
				p->setPen( framePen );
					
				p->drawRect( br );
			}						
		}
		else if (type == CStyle::ModuleText) {		
			p->save();
			CSwordModuleInfo* m = dynamic_cast<CSwordModuleInfo*>(m_module);

			if (m && m->encoding() == QFont::Unicode )
				font = COptionsDialog::getBTFont( COptionsDialog::unicode );

			if (alignement == CStyleFormat::Center)		
				text = QString::fromLatin1("<CENTER>%1</CENTER>").arg(text);
			else if (alignement == CStyleFormat::Right)		
				text = QString::fromLatin1("<P ALIGN=\"RIGHT\">%1</P>").arg(text);
    	QSimpleRichText richText( text, font, QString::null, QStyleSheet::defaultSheet(), QMimeSourceFactory::defaultFactory(), printer->getPageSize().height()-printer->getVerticalPos()-frameThickness+printer->upperMargin());
    	richText.setWidth( p, printer->getPageSize().width()-2*frameThickness-BORDER_SPACE );
    	QRect view( printer->getPageSize() );
    	int translated = 0;
    	do {
    		if ((int)(richText.height()-translated + printer->getVerticalPos()) < (int)(printer->getPageSize().height()+printer->upperMargin()) )
    			br = QRect(printer->leftMargin(), printer->getVerticalPos(), printer->getPageSize().width(), richText.height()+frameThickness);
    		else { //fill to bottom of the page
    			br = QRect(printer->leftMargin(), printer->getVerticalPos(), printer->getPageSize().width(), printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin());
    			br.moveBy(0, translated);
    		}
    		p->setClipRect(printer->getPageSize());
   			p->fillRect(br,QBrush(bgColor));
				if (frame) {
					QPen framePen = pen;
					framePen.setWidth( frameThickness );
					framePen.setColor( frame->getColor() );
					p->setPen( framePen );

					p->drawRect(br);
				}						   			
    		p->setClipping(false);
        richText.draw(p,printer->leftMargin()+frameThickness+(int)((float)BORDER_SPACE/2),printer->getVerticalPos(),view,cg);
				const int movePixs = ( ((int)richText.height()-translated) > (int)(printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin())) ? ( printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin() ) : richText.height()-translated+frameThickness;

   			printer->setVerticalPos(printer->getVerticalPos()+movePixs);
		    view.moveBy( 0,movePixs);		
        p->translate( 0,-movePixs);
        translated+=movePixs;
        if ( view.top() >= richText.height() ) { //bottom or top(default)
    			break;
    		}
    		printer->newPage();
    	} while (true);
			p->restore();
    }
	}	
	printer->setVerticalPos(printer->getVerticalPos() + PARAGRAPH_SPACE);	
}

/** Updates and returns the header text. */
const QString& CPrintItem::getHeaderText() {
	if ( m_startKey ) {
		if ((m_startKey == m_stopKey) || (m_startKey && !m_stopKey))
			m_headerText = m_startKey->key();
		else if (m_startKey && m_stopKey) {//start and stop key do exist and are different
			m_headerText = QString::fromLatin1("%1 - %2").arg(m_startKey->key()).arg(m_stopKey->key());
		}
	}
	else
		m_headerText = QString::null;
	
	return m_headerText;
}
