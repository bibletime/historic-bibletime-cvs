/***************************************************************************
                          chtmlreaddisplay.cpp  -  description
                             -------------------
    begin                : Don Mai 9 2002
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

#include "chtmlreaddisplay.h"

#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/creadwindow.h"
#include "backend/creferencemanager.h"
#include "backend/cswordkey.h"

#include "util/ctoolclass.h"

#include "frontend/cbtconfig.h"
#include "frontend/ctooltipmanager.h"
#include "frontend/cdragdropmgr.h"
#include "frontend/cinfodisplay.h"

#include "util/cpointers.h"
#include "util/scoped_resource.h"

//Qt includes
#include <qcursor.h>
#include <qscrollview.h>
#include <qwidget.h>
#include <qdragobject.h>
#include <qpopupmenu.h>
#include <qlayout.h>
#include <qtimer.h>

//KDE includes
#include <kapplication.h>
#include <khtmlview.h>
#include <kglobalsettings.h>
#include <khtml_events.h>

#include <dom/dom2_range.h>
#include <dom/html_element.h>
#include <dom/dom2_traversal.h>

CHTMLReadDisplay::CHTMLReadDisplay(CReadWindow* readWindow, QWidget* parentWidget) : KHTMLPart((m_view = new CHTMLReadDisplayView(this, parentWidget ? parentWidget : readWindow)), readWindow ? readWindow : parentWidget), CReadDisplay(readWindow) {
  setDNDEnabled(false);
  m_view->setDragAutoScroll(false);
}

CHTMLReadDisplay::~CHTMLReadDisplay(){
}

const QString CHTMLReadDisplay::text( const CDisplay::TextType format, const CDisplay::TextPart part) {
  switch (part) {
    case Document: {
      if (format == HTMLText)
        return document().toHTML();
      else {
        return htmlDocument().body().innerText().string().latin1();
      }
		}

    case SelectedText: {
			if (!hasSelection())
   			return QString::null;

      if (!hasSelection()) {
      	return QString::null;
      }
      else if (format == HTMLText) {
        DOM::Range range = selection();
        return range.toHTML().string();
      }
      else { //plain text requested
        return selectedText();
      }
		}

  	case AnchorOnly: {
			QString moduleName;
   		QString keyName;
     	CReferenceManager::Type type;
      CReferenceManager::decodeHyperlink(activeAnchor(), moduleName, keyName, type);
      
     	return keyName;
    }

		case AnchorTextOnly: {
			QString moduleName;
   		QString keyName;
     	CReferenceManager::Type type;
      CReferenceManager::decodeHyperlink(activeAnchor(), moduleName, keyName, type);

      if (CSwordModuleInfo* module = backend()->findModuleByName(moduleName)) {
				util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
    		key->key( keyName );

      	return key->strippedText();
   		}
     	return QString::null;
    }

    case AnchorWithText: {
			QString moduleName;
   		QString keyName;
     	CReferenceManager::Type type;
      CReferenceManager::decodeHyperlink(activeAnchor(), moduleName, keyName, type);

      if (CSwordModuleInfo* module = backend()->findModuleByName(moduleName)) {
				util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
    		key->key( keyName );

      	return QString::fromLatin1("%1\n(%2, %3)")
          .arg(key->strippedText())
          .arg(key->key())
          .arg(key->module()->name());
   		}
     	return QString::null;
    }
    default:
      return QString::null;
  }
}

void CHTMLReadDisplay::setText( const QString& newText ) {
  begin();
  write(newText);
  end();
}

/** No descriptions */
const bool CHTMLReadDisplay::hasSelection(){
  return KHTMLPart::hasSelection();
}


/** Reimplementation. */
QScrollView* CHTMLReadDisplay::view(){
  return KHTMLPart::view();
}

void CHTMLReadDisplay::selectAll() {
  KHTMLPart::selectAll();
}

/** No descriptions */
void CHTMLReadDisplay::moveToAnchor( const QString& anchor ){
	gotoAnchor(anchor);
}

void CHTMLReadDisplay::urlSelected( const QString& url, int button, int state, const QString& _target, KParts::URLArgs args){
  KHTMLPart::urlSelected(url, button, state, _target, args);
  m_urlWorkaroundData.doWorkaround = false;

  if (!url.isEmpty() && CReferenceManager::isHyperlink(url)) {
    QString module;
    QString key;
    CReferenceManager::Type type;
    CReferenceManager::decodeHyperlink(url, module, key, type);
    if (module.isEmpty()) {
      module = CReferenceManager::preferredModule( type );
		}

    // we have to use this workaround, otherwise the widget would scroll because it was interrupted
    // between mouseClick and mouseRelease (I guess)
    m_urlWorkaroundData.doWorkaround = true;
    m_urlWorkaroundData.url = url;
    m_urlWorkaroundData.state =  state;
		m_urlWorkaroundData.button = button;
		m_urlWorkaroundData.target = _target;
		m_urlWorkaroundData.args = args;
		m_urlWorkaroundData.module = module;
		m_urlWorkaroundData.key = key;
  }
  else if (!url.isEmpty() && url.left(1) == "#") { //anchor
    moveToAnchor(url.mid(1));
  }
}

/** Reimplementation. */
void CHTMLReadDisplay::khtmlMouseReleaseEvent( khtml::MouseReleaseEvent* event ){
  KHTMLPart::khtmlMouseReleaseEvent(event);

  m_dndData.mousePressed = false;
  m_dndData.isDragging = false;
  m_dndData.node = DOM::Node();
  m_dndData.anchor = DOM::DOMString();

  if (m_urlWorkaroundData.doWorkaround) {
    m_urlWorkaroundData.doWorkaround = false;
    connectionsProxy()->emitReferenceClicked(m_urlWorkaroundData.module, m_urlWorkaroundData.key);
  }
}

void CHTMLReadDisplay::khtmlMousePressEvent( khtml::MousePressEvent* event ){
  m_dndData.node = DOM::Node();
  m_dndData.anchor = DOM::DOMString();
  m_dndData.mousePressed = false;
  m_dndData.isDragging = false;

  if (event->qmouseEvent()->button() == Qt::RightButton) {
    setActiveAnchor( event->url().string() );
  }
  else if (event->qmouseEvent()->button() == Qt::LeftButton) {
    m_dndData.node = event->innerNode();
    m_dndData.anchor = event->url();
    m_dndData.mousePressed = true;
    m_dndData.isDragging = false;
    m_dndData.startPos = QPoint(event->x(), event->y());
    m_dndData.selection = selectedText();

    if (!m_dndData.node.isNull()) { //we drag a valid link
      m_dndData.dragType = DNDData::Link;
    }
  }

  KHTMLPart::khtmlMousePressEvent(event);
}

/** Reimplementation for our drag&drop system. */
void CHTMLReadDisplay::khtmlMouseMoveEvent( khtml::MouseMoveEvent* e ){
  if( e->qmouseEvent()->state() & LeftButton ) { //left mouse button pressed
		const int delay = KGlobalSettings::dndEventDelay();
		QPoint newPos = QPoint(e->x(), e->y());
	
		if ( (newPos.x() > m_dndData.startPos.x()+delay || newPos.x() < (m_dndData.startPos.x()-delay) ||
				newPos.y() > m_dndData.startPos.y()+delay || newPos.y() < (m_dndData.startPos.y()-delay)) &&
				!m_dndData.isDragging && m_dndData.mousePressed  )
		{
			QDragObject* d = 0;
			if (!m_dndData.anchor.isEmpty() && m_dndData.dragType == DNDData::Link && !m_dndData.node.isNull() ) {
			// create a new bookmark drag!
				QString module = QString::null;
				QString key = QString::null;
				CReferenceManager::Type type;
				if ( !CReferenceManager::decodeHyperlink(m_dndData.anchor.string(), module, key, type) )
					return;
				
				CDragDropMgr::ItemList dndItems;
				dndItems.append( CDragDropMgr::Item(module, key, QString::null) ); //no description!
				d = CDragDropMgr::dragObject(dndItems, KHTMLPart::view()->viewport());
			}
			else if (m_dndData.dragType == DNDData::Text && !m_dndData.selection.isEmpty()) {    // create a new plain text drag!
				CDragDropMgr::ItemList dndItems;
				dndItems.append( CDragDropMgr::Item(m_dndData.selection) ); //no description!
				d = CDragDropMgr::dragObject(dndItems, KHTMLPart::view()->viewport());
			}
	
			if (d) {
				m_dndData.isDragging = true;
				m_dndData.mousePressed = false;
	
				//first make a virtual mouse click to end the selection, it it's in progress
				QMouseEvent e(QEvent::MouseButtonRelease, QPoint(0,0), Qt::LeftButton, Qt::LeftButton);
				KApplication::sendEvent(view()->viewport(), &e);
				d->drag();
			}
		}
	}
 	else { //no mouse button pressed
		KHTMLPart::khtmlMouseMoveEvent(e);
	  DOM::Node node = e->innerNode();//m_view->part()->nodeUnderMouse();
		QString infoText;
		
		//if no link was under the mouse try to find a title attrivute
		if (!node.isNull()) {
			DOM::Node currentNode = node;
			do {
				if (!currentNode.isNull() && currentNode.hasAttributes()) { //found right node
					DOM::Node attr = currentNode.attributes().getNamedItem("class");
					if (!attr.isNull() && (attr.nodeValue().string() == "footnote")) {
						DOM::Node footnote = currentNode.attributes().getNamedItem("footnote");
						CPointers::infoDisplay()->setInfo( CInfoDisplay::Footnote, footnote.nodeValue().string() );
						//infoText = QString("Footnote: ") + footnote.nodeValue().string();
						break;
					}
				}
			} while ( !(currentNode = currentNode.parentNode()).isNull() );
		}
	} 
	KHTMLPart::khtmlMouseMoveEvent(e);
}
/* -------------------------- */
CHTMLReadDisplayView::ToolTip::ToolTip(CHTMLReadDisplayView* view) : CToolTip(view), m_view( view ) {
};

/** Decides whether a tooltip should be shown. */
void CHTMLReadDisplayView::ToolTip::maybeTip( const QPoint& /*p*/ ){
  DOM::Node node = m_view->part()->nodeUnderMouse();
  if (node.isNull()) { //WARNING: Return already here
  	return;
	}

	QString tooltipText;
  DOM::Node currentNode = node;
  do {
    if (!currentNode.isNull() && currentNode.nodeName().string().upper() == "A" && currentNode.hasAttributes()) { //found right node
			DOM::Node hrefAttr = currentNode.attributes().getNamedItem("href");
			if (!hrefAttr.isNull()) {
				tooltipText = CTooltipManager::textForHyperlink(
					hrefAttr.nodeValue().string() 
				);
				break;
			}
		}
	} while ( !(currentNode = currentNode.parentNode()).isNull() );
  
	//if no link was under the mouse try to find a title attrivute
	if (tooltipText.isEmpty()) {
		currentNode = node;
		do {
			if (!currentNode.isNull() && currentNode.hasAttributes()) { //found right node
				DOM::Node titleAttr = currentNode.attributes().getNamedItem("title");
				if (!titleAttr.isNull()) {
					tooltipText = titleAttr.nodeValue().string();
					break;
				}
			}
		} while ( !(currentNode = currentNode.parentNode()).isNull() );
	}
	
	//display tooltip if the text is not empty
	if (!tooltipText.isEmpty()) {
		QRect rect = currentNode.getRect();
		rect.setX( m_view->mapFromGlobal(QCursor::pos()).x() );
		rect.setY( m_view->mapFromGlobal(QCursor::pos()).y() );
		tip( m_view->mapFromGlobal(QCursor::pos()), rect, tooltipText );
	}
}

// ---------------------

CHTMLReadDisplayView::CHTMLReadDisplayView(CHTMLReadDisplay* displayWidget, QWidget* parent) : KHTMLView(displayWidget, parent), m_display(displayWidget) {
  viewport()->setAcceptDrops(true);
  setMarginWidth(4);
  setMarginHeight(4);
};


/** Opens the popupmenu at the given position. */
void CHTMLReadDisplayView::popupMenu( const QString& url, const QPoint& pos){
 	if (!url.isEmpty()) {
//    qWarning(url.latin1());
  	m_display->setActiveAnchor(url);
	}
 	if (QPopupMenu* popup = m_display->installedPopup()) {
	 	popup->exec(pos);
  }
}

/** Reimplementation from QScrollView. Sets the right slots */
void CHTMLReadDisplayView::polish(){
  KHTMLView::polish();
  connect( part(), SIGNAL(popupMenu(const QString&, const QPoint&)),
    this, SLOT(popupMenu(const QString&, const QPoint&)));

//  (void)new ToolTip(this);
}

/** Reimplementatiob from QScrollView. */
void CHTMLReadDisplayView::contentsDropEvent( QDropEvent* e ){
  if (CDragDropMgr::canDecode(e) && CDragDropMgr::dndType(e) == CDragDropMgr::Item::Bookmark) {
    CDragDropMgr::ItemList dndItems = CDragDropMgr::decode(e);
    CDragDropMgr::Item item = dndItems.first();  
    e->acceptAction();
    m_display->connectionsProxy()->emitReferenceDropped(item.bookmarkKey());
    return;
  };

  //don't accept the action!
  e->acceptAction(false);
  e->ignore();
}

/** Reimplementation from QScrollView. */
void CHTMLReadDisplayView::contentsDragEnterEvent( QDragEnterEvent* e ){
  if (CDragDropMgr::canDecode(e) && CDragDropMgr::dndType(e) == CDragDropMgr::Item::Bookmark) {
    e->acceptAction();
    return;
  }
  e->acceptAction(false);
  e->ignore();
}

