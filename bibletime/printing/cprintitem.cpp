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
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordversekey.h"
#include "backend/cswordldkey.h"
//#include "backend/chtmlentrydisplay.h"
#include "frontend/cbtconfig.h"
#include "util/scoped_resource.h"


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
#define STYLE_PART_SPACE 1	//Space between the different parts (header, moduletext ...)
#define BORDER_SPACE 3 	//border between text and rectangle


CPrintItem::ListViewItem::ListViewItem( QListView* parent, CPrintItem* printItem )
	: QListViewItem(parent), m_printItem(printItem) {
	
};

CPrintItem* const CPrintItem::ListViewItem::printItem() const {
	return m_printItem;
};

CStyle* const CPrintItem::ListViewItem::style() const {
	return (m_printItem) ? m_printItem->style() : 0;
};


CPrintItem::CPrintItem(CSwordModuleInfo* module, const QString& startKey, const QString& stopKey, const QString& description,const CSwordBackend::DisplayOptionsBool displayOptions, const CSwordBackend::FilterOptionsBool filterOptions) :
  m_style(0), m_headerText(QString::null), m_description(description),  m_moduleText(QString::null),
  m_startKey(startKey),  m_module(module), m_listViewItem(0),
  m_displayOptions(displayOptions), m_filterOptions(filterOptions)
{	
	m_startEmpty = startKey.isEmpty();	
	m_stopKey = (!m_startEmpty && (startKey != stopKey) ) ? stopKey : QString::null;
	m_stopEmpty  = m_stopKey.isEmpty();	
	
	headerText();
	moduleText(); //cache the module text, makes printing faster (at least the user thinks this :)
}

/** Returns the moduletext used by this item. */
const QString& CPrintItem::moduleText() {
	/**
	* If a special text is set use the text.
	* If the moduleText variable is empty use the CModuleInfo
	* object to retrieve the text,
	*/
	if (!m_moduleText.isEmpty()) //cached text
		return m_moduleText;

	if (m_startEmpty || !m_module)
		return QString::null;

	CPointers::backend()->setDisplayOptions(m_displayOptions);
	CPointers::backend()->setFilterOptions(m_filterOptions);	
	
	if (m_stopEmpty) {//only start key
		util::scoped_ptr<CSwordKey> key(CSwordKey::createInstance(m_module));
		key->key(m_startKey);
		m_moduleText = key->renderedText();			
	}
	else if (m_module->type() == CSwordModuleInfo::Bible || m_module->type() == CSwordModuleInfo::Commentary ) {
		util::scoped_ptr<CSwordVerseKey> startKey(dynamic_cast<CSwordVerseKey*>(CSwordKey::createInstance(m_module)));
		util::scoped_ptr<CSwordVerseKey> stopKey(dynamic_cast<CSwordVerseKey*>(CSwordKey::createInstance(m_module)));
		
		startKey->key(m_startKey);
		stopKey->key(m_stopKey);
	
//		const QString format = QString::fromLatin1(" <FONT SIZE=\"-2\"><NOBR>%1</NOBR></FONT>");
		const QString format = QString::fromLatin1(" <FONT SIZE=\"-3\"><NOBR>%1</NOBR></FONT> ");
		while ( (*startKey) < (*stopKey) || (*startKey) == (*stopKey) ) {
			m_moduleText += (m_displayOptions.verseNumbers ? format.arg(startKey->Verse()) : QString::null)
+ startKey->renderedText() + (/*m_displayOptions.lineBreaks ?*/ "<BR>\n"/* : QString::null*/);
			startKey->next(CSwordVerseKey::UseVerse);
		}
	}		
	
	m_moduleText.replace(QRegExp("$\n\n+"), "");
	m_moduleText.replace(QRegExp("$<BR><BR>+"), "");	
	return m_moduleText;
}

/** Sets the style for this item. */
void CPrintItem::setStyle( CStyle* newStyle ) {
	m_style = newStyle;
}

CStyle*const CPrintItem::style() const {
	return m_style;
}

/** Returns the listview item for this printitem. */
QListViewItem* CPrintItem::listViewItem( CPrintItemList* list ) {
	deleteListViewItem();
	m_listViewItem = new ListViewItem( list, this );
	updateListViewItem();
	return m_listViewItem;
}

/** Updates the item. */
void CPrintItem::updateListViewItem(){
	if (m_module)
		m_listViewItem->setText(0, m_module->name() );
	
	if (!m_startEmpty)
		m_listViewItem->setText(1,m_startKey);
	
	if (!m_stopEmpty)
		m_listViewItem->setText(2,m_stopKey);
	else if (!m_startEmpty)
		m_listViewItem->setText(2,m_startKey);
	
	if (m_style)
		m_listViewItem->setText(3, m_style->styleName() );
}

/**  */
QListViewItem* const CPrintItem::listViewItem() const {
	return m_listViewItem;
}

/** Deletes the list view item. */
void CPrintItem::deleteListViewItem(){
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
	CStyle::Format* format;
	CStyle::Format::Frame* frame = 0;
	int frameThickness = 0;
	CStyle::Format::Alignment alignment;	
	CStyle::StyleType type = CStyle::Unknown;
	QString text;
	
	QColorGroup cg;
	QPen pen;
	QBrush brush;

	const bool isUnicode 	= (m_module && m_module->isUnicode());
	const int leftMargin 	= printer->pageMargins().left;
	const int upperMargin = printer->pageMargins().top;	
	const QRect pageSize =  printer->contentSize();
	const int pageHeight 	= pageSize.height();
	const int pageWidth 	= pageSize.width();
	
//moved out of the loop for optimization	
	int verticalPos = printer->verticalPos();		
	int arguments	= 0;
	QRect boundingRect; //rectangle for the content
	QRect br;	
	QRect view;
	QPen framePen;
	int movePixs;
	
	for (int i = 0; i < 3; ++i) {		
		type = static_cast<CStyle::StyleType>(i);
		
		if (!m_style->hasFormatTypeEnabled(type)) //jump to next part if this is not enabled
			continue;
				
		format 	= m_style->formatForType( type );
		fgColor = format->color( CStyle::Format::Foreground );
		bgColor = format->color( CStyle::Format::Background );
		pen.setColor(fgColor);
		font = format->font();
		
		frame = format->frame();
		frameThickness = frame ? frame->thickness() : 0;		
		alignment = format->alignment();
		if (type == CStyle::Header)
			text = headerText();
		else if (type == CStyle::Description)
			text = m_description;
		else
			text = moduleText();

		p->setFont(font);
		p->setPen(pen);
		cg.setColor(QColorGroup::Text, fgColor);
				
		arguments = Qt::WordBreak;		
		if (alignment == CStyle::Format::Left)
			arguments |= Qt::AlignLeft;
		else if (alignment == CStyle::Format::Center)
			arguments |= Qt::AlignHCenter;
		else if (alignment == CStyle::Format::Right)
			arguments |= Qt::AlignRight;
				
		if ((type == CStyle::Header || type == CStyle::Description) && !text.isEmpty()) {
			boundingRect = p->boundingRect (
				leftMargin, //x of upper left corner
				verticalPos, //y of upper left corner
				pageWidth - 2*BORDER_SPACE - 2*frameThickness, //pixels to the right from the upper left corner
				pageHeight - 2*BORDER_SPACE - 2*frameThickness - verticalPos + upperMargin, //pixels down from upper left corner
				arguments, text
			);
			
			//check if the new text fits into the current page page
			//WARNING: use 2* or 1* frameThickness here??
			if ( 		( boundingRect.height() + 2*BORDER_SPACE + 2*frameThickness + STYLE_PART_SPACE )
						> ( pageHeight - verticalPos  + upperMargin ) )
			{
				//this part doesn't fit on the current page
				printer->newPage();
				verticalPos = printer->verticalPos();
				boundingRect = p->boundingRect(
					leftMargin,
					verticalPos,
					pageWidth - 2*BORDER_SPACE - 2*frameThickness,
					pageHeight- 2*BORDER_SPACE - 2*frameThickness - verticalPos + upperMargin,
					arguments, text
				);
			}
			br = boundingRect;	//rectangle for the background fill		
			br.setLeft(leftMargin);
			br.setWidth(pageWidth-frameThickness); //because we move in the next lines
			br.setHeight(br.height()+BORDER_SPACE); //because we move in the next lines
			br.moveBy(frameThickness, frameThickness);			
			p->fillRect(br, bgColor);

			br = boundingRect;	// the rect for the border		
			/**
			* we have to substract frameThickness/2,
			* because QPainter paints one half outline and the other part inside the rectangle.
			*/
			const int halfWidth = (int)((float)frameThickness/2);

//			br.setHeight( br.height() + 2*halfWidth  + BORDER_SPACE );						
			br.setLeft( leftMargin + halfWidth );
			br.setTop( br.top() + halfWidth ); //boundingRect is moved by 2*halfWidth down -> use +halfWidth !!
			br.setWidth( pageWidth - 2*halfWidth);
			br.setHeight( br.height() + 2*halfWidth  + BORDER_SPACE );						
			
//			boundingRect.setWidth(boundingRect.width()+5);//HACK to avoid cut letters
			boundingRect.moveBy(BORDER_SPACE + frameThickness, frameThickness);
//			arguments |= Qt::AlignVCenter; //WARNING: Right here? Will it change the boundingrect??

    		p->setClipRect(
    			upperMargin,
    			leftMargin,
    			pageWidth,
    			pageHeight
    		);

			p->drawText(boundingRect, arguments, text);
			
			p->setClipping(false);
			
			printer->setVerticalPos(boundingRect.top() + boundingRect.height() + 2*frameThickness + STYLE_PART_SPACE);
 			verticalPos = printer->verticalPos();
 			
			if (frame) {
				framePen = pen;
				framePen.setWidth( frameThickness );
				framePen.setColor( frame->color() );
				framePen.setStyle( frame->lineStyle() );
				p->setPen( framePen );

				p->drawRect( br );
			}						
		}
		else if (type == CStyle::ModuleText) {		
			p->save();
			
			if (isUnicode)
				font = CBTConfig::get( CBTConfig::unicode );

			if (alignment == CStyle::Format::Center)		
				text = QString::fromLatin1("<CENTER>%1</CENTER>").arg(text);
			else if (alignment == CStyle::Format::Right)		
				text = QString::fromLatin1("<P ALIGN=\"RIGHT\">%1</P>").arg(text);
    	QSimpleRichText richText(
    		text,
    		font,
    		QString::null,
    		QStyleSheet::defaultSheet(),
    		QMimeSourceFactory::defaultFactory(),
				pageHeight + upperMargin - verticalPos - 2*frameThickness - 2*BORDER_SPACE
			);

    	richText.setWidth( p, pageWidth-2*frameThickness-2*BORDER_SPACE );
    	view = QRect( //the content area of the whole page
    		leftMargin + frameThickness  + BORDER_SPACE,
    		upperMargin + frameThickness + BORDER_SPACE,
	    	pageWidth - 2*frameThickness - 2*BORDER_SPACE,
	    	pageHeight - 2*frameThickness - 2*BORDER_SPACE
    	);
    	
    	int translated = 0;
    	do {
    		if ((int)(verticalPos - upperMargin + richText.height() - translated) <= (int)(pageHeight) ) {
    			//text fits on current page
					br = QRect (
	    			leftMargin + frameThickness,
	    			verticalPos + frameThickness,
		    		pageWidth - frameThickness,
		    		richText.height() - translated + frameThickness + BORDER_SPACE
	    		);
				}
    		else { //doesn't fit completely on current page, fill to bottom of the page    		
		    	br = QRect(
		    		leftMargin + frameThickness,
		    		verticalPos + frameThickness,
			    	pageWidth - frameThickness,
			    	pageHeight - verticalPos + upperMargin - frameThickness
		    	);
    		}
				br.moveBy(0,translated); //we have to move down as far as the painter moved    		
    		p->setClipRect(
    			upperMargin,
    			leftMargin,
    			pageWidth,
    			pageHeight
    		);
   			p->fillRect(br,QBrush(bgColor)); //fill rect with background color
				if (frame) {
					br.moveBy( (int)(-(float)frameThickness/2), (int)(-(float)frameThickness/2) );	

					framePen = pen;
					framePen.setWidth( frameThickness );
					framePen.setColor( frame->color() );
					framePen.setStyle( frame->lineStyle() );
					p->setPen( framePen );

					p->drawRect(br);
				}						   			
    		p->setClipping(false);
    		
        richText.draw(p, leftMargin+frameThickness+BORDER_SPACE, verticalPos+frameThickness+BORDER_SPACE, view, cg);
				movePixs = //move down the painted area height
						(((int)richText.height()-translated) > (int)(pageHeight - verticalPos + upperMargin ))
					? (pageHeight - verticalPos + upperMargin) //move to bottom of page
					: richText.height() - translated + 2*frameThickness + 2*BORDER_SPACE; //only move the painted area

   			printer->setVerticalPos(verticalPos + movePixs);					

		    view.moveBy( 0,movePixs);
        p->translate( 0,-movePixs);
        translated += movePixs;
        if ( view.top() >= richText.height() ) //bottom or top(default)
    			break;

    		printer->newPage();
   			verticalPos = printer->verticalPos();    		
    	} while (true);
			p->restore();
    }
	}
	printer->setVerticalPos(printer->verticalPos() + PARAGRAPH_SPACE);	
}

/** Updates and returns the header text. */
const QString& CPrintItem::headerText() {
	if (!m_headerText.isEmpty())  // cached?
		return m_headerText;

  if (m_startEmpty)
  	return QString::null; 	
	if (m_stopEmpty)
		m_headerText = m_startKey;
	else {//start and stop key do exist and are different
		m_headerText = QString::fromLatin1("%1 - %2").arg(m_startKey).arg(m_stopKey);
	}	
	return m_headerText;
}
