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
//#include "../frontend/thirdparty/qt3stuff/qt3stuff.h"
//#include "../frontend/thirdparty/qt3stuff/qrichtext_p.h"
//#include "../frontend/thirdparty/qt3stuff/qsimplerichtext.h"
#include <../backend/cmoduleinfo.h>
#include <../backend/sword_backend/cswordmoduleinfo.h>
#include <../backend/ckey.h>
#include <../backend/sword_backend/cswordversekey.h>
#include <../backend/sword_backend/cswordldkey.h>
#include <../backend/sword_backend/chtmlentrydisplay.h>

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
	m_startKey = m_stopKey = 0;
	
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
CKey* CPrintItem::getStartKey() const{
	return m_startKey;
}

/** Sets the startkey. */
void CPrintItem::setStartKey(CKey* newKey) {
	if (m_startKey)
		delete m_startKey;
	m_startKey = newKey;

	if ( dynamic_cast<SWKey*>(m_startKey) ) {
		SWKey* swStartKey = dynamic_cast<SWKey*>(m_startKey);
		if (dynamic_cast<SWKey*>(m_stopKey)) {
			if (m_startKey == m_stopKey) {
				m_headerText = QString::fromLocal8Bit( (const char*)*swStartKey );
			}
			else {	//start and stop key do exist and are different
				SWKey* swStopKey = dynamic_cast<SWKey*>(m_stopKey);
				m_headerText = QString("%1 - %2").arg(QString::fromLocal8Bit((const char*)*swStartKey)).arg(QString::fromLocal8Bit((const char*)*swStopKey));
			}
		}
	}
	else
		m_headerText = QString::null;			
}

/** Sets the end key. */
void CPrintItem::setStopKey( CKey* newKey ){
	if (m_stopKey)
		delete m_stopKey;	
	m_stopKey = newKey;
	
	if ( dynamic_cast<SWKey*>(m_startKey) ) {
		SWKey* swStartKey = dynamic_cast<SWKey*>(m_startKey);
		if (dynamic_cast<SWKey*>(m_stopKey)) {
			if (m_startKey == m_stopKey) {
				m_headerText = QString::fromLocal8Bit( (const char*)*swStartKey );
			}
			else {	//start and stop key do exist and are different
				SWKey* swStopKey = dynamic_cast<SWKey*>(m_stopKey);
				m_headerText = QString("%1 - %2").arg(QString::fromLocal8Bit((const char*)*swStartKey)).arg(QString::fromLocal8Bit((const char*)*swStopKey));
			}
		}
	}
	else
		m_headerText = QString::null;	
}

/** Returns the last covered key. */
CKey* CPrintItem::getStopKey() const {
	return m_stopKey;
}

/** Returns the used module. */
CModuleInfo* CPrintItem::getModule() const {
	return m_module;
}

/** Sets the used module. */
void CPrintItem::setModule( CModuleInfo* newModule){
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
const QString CPrintItem::getModuleText() {
	/**
	* If a special text is set use the text.
	* If the moduleText variable is empty use the CModuleInfo
	* object to retrieve the text,
	*/
	if (!m_moduleText.isEmpty())
		return m_moduleText;

//	#warning Todo: This function is incomplete. Implement for range between startKey and stopKey
	CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(m_startKey);
	CSwordLDKey* lk = dynamic_cast<CSwordLDKey*>(m_startKey);		
	QString text = QString::null;
	CSwordModuleInfo* sw = (CSwordModuleInfo*)m_module;
	text = QString("<FONT SIZE=\"-1\"><NOBR>(%1)</NOBR></FONT>").arg(vk->Verse())+ (vk ? vk->getRenderedText() : (lk ? lk->getRenderedText() : QString()));
	if (sw && m_stopKey && m_stopKey != m_startKey) {
		if (sw->getType() == CSwordModuleInfo::Bible  || sw->getType() == CSwordModuleInfo::Commentary ) {
			CSwordVerseKey dummyKey(sw);
			CSwordVerseKey* vk_start = dynamic_cast<CSwordVerseKey*>(m_startKey);			
			CSwordVerseKey* vk_stop = dynamic_cast<CSwordVerseKey*>(m_stopKey);						
			if (!vk_start && !vk_stop)
				return text;
			dummyKey.setKey( vk_start->getKey() );
			while (dummyKey < *vk_stop) {
				dummyKey.NextVerse();
				text += QString("<FONT SIZE=\"-1\"><NOBR>(%1)</NOBR></FONT>").arg(dummyKey.Verse()) + dummyKey.getRenderedText();
			}			
		}
//		else if (sw->getType() == CSwordModuleInfo::Lexicon ) {
//		}
	}
		
	text.replace(QRegExp("$\n+"), "");
	text.replace(QRegExp("$<BR>+"), "");	
	return text;
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
	if (m_listViewItem) {
		delete m_listViewItem;
		m_listViewItem = 0;			
	}	
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
	if (getModule() && (CSwordModuleInfo*)(getModule()) ) {
		CSwordModuleInfo* swModule = (CSwordModuleInfo*)(getModule());
		m_listViewItem->setText(0, swModule->module()->Name() );
	}

	if (getStartKey() && dynamic_cast<SWKey*>(getStartKey())) {
		SWKey* key = dynamic_cast<SWKey*>(getStartKey());
		m_listViewItem->setText(1, (const char*)*key);
	}
	if (getStopKey() && dynamic_cast<SWKey*>(getStopKey())) {
		SWKey* key = dynamic_cast<SWKey*>(getStopKey());
		m_listViewItem->setText(2, (const char*)*key);
	}
	if (getStyle()) {
		m_listViewItem->setText(3, getStyle()->getStyleName() );
	}
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
		fgColor = format->getFGColor();
		bgColor = format->getBGColor();	
		pen.setColor(fgColor);
		font = format->getFont();
		frame = format->hasFrame() ? format->getFrame() : 0;
		alignement = format->getAlignement();
		identation = format->getIdentation();
		if (type == CStyle::Header)
			text = m_headerText;
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
			if ( (boundingRect.height()+(frame ? 2*frame->getThickness() : 0) + STYLE_PART_SPACE ) > printer->getPageSize().height()-printer->getVerticalPos() ) {
				//this part doesn't fit on the current page
				printer->newPage();
				boundingRect = p->boundingRect( printer->leftMargin(), printer->getVerticalPos(),
					printer->getPageSize().width(), printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin(), arguments, text );			
			}
			br = boundingRect;
			br.setX(printer->leftMargin());
			br.setWidth(printer->getPageSize().width());
			p->fillRect( br, bgColor );	
//			p->setPen(pen);
						
			p->drawText(boundingRect, arguments, text);
			printer->setVerticalPos( printer->getVerticalPos() + boundingRect.height() + (frame ? 2*frame->getThickness() : 0) + STYLE_PART_SPACE );

			if (frame) {
				QPen framePen = pen;
				framePen.setWidth( frame->getThickness() );
				framePen.setColor( frame->getColor() );
				p->setPen( framePen );
					
				p->drawRect( br );
			}						
		}
		else if (type == CStyle::ModuleText) {		
			p->save();
			CSwordModuleInfo* m = (CSwordModuleInfo*)m_module;
			if (m && m->hasFont())
				font = m->getFont();
			if (alignement == CStyleFormat::Center)		
				text = QString("<CENTER>%1</CENTER>").arg(text);
			else if (alignement == CStyleFormat::Right)		
				text = QString("<P ALIGN=\"RIGHT\">%1</P>").arg(text);
			text = QString("%1").arg(text);
    	QSimpleRichText richText( text, font, QString::null, QStyleSheet::defaultSheet(), QMimeSourceFactory::defaultFactory(), printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin());
    	richText.setWidth( p, printer->getPageSize().width()-BORDER_SPACE );
    	QRect view( printer->getPageSize() );
    	int translated = 0;
    	do {				
    		if ((printer->getVerticalPos() + richText.height()) < (printer->getPageSize().height()+printer->upperMargin()) )
    			br = QRect(printer->leftMargin(), printer->getVerticalPos(), printer->getPageSize().width(), richText.height());    		
//    		else if ( (printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin()+translated) > richText.height() ) {
//    			br = QRect(printer->leftMargin(), printer->upperMargin(), printer->getPageSize().width(), printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin()+translated- richText.height() );
//    		}
    		else { //fill to bottom of the page
    			br = QRect(printer->leftMargin(), printer->getVerticalPos(), printer->getPageSize().width(), printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin());
    			br.moveBy(0, translated);
    		}
   			
    		p->setClipRect(printer->getPageSize());
   			p->fillRect(br,QBrush(bgColor));
				if (frame) {
					QPen framePen = pen;
					framePen.setWidth( frame->getThickness() );
					framePen.setColor( frame->getColor() );
					p->setPen( framePen );
						
					p->drawRect( br );
				}						   			
    		p->setClipping(false);
    		
        richText.draw(p,printer->leftMargin()+(int)((float)BORDER_SPACE/2),printer->getVerticalPos(),view,cg);
				const int movePixs = (richText.height() > (printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin())) ? ( printer->getPageSize().height()-printer->getVerticalPos()+printer->upperMargin() ) : richText.height();
   			printer->setVerticalPos(printer->getVerticalPos()+movePixs);		
		    view.moveBy( 0,movePixs);		
        p->translate( 0,-movePixs);
        translated+=movePixs;
        if ( view.top() >= richText.height() )
    			break;
    		printer->newPage();
    	} while (true);
			p->restore();
    }
	}	
	printer->setVerticalPos(printer->getVerticalPos() + PARAGRAPH_SPACE);	
}

