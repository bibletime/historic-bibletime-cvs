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

#include "frontend/ctoolclass.h"
#include "frontend/cbtconfig.h"
#include "frontend/ctooltipmanager.h"

#include "util/scoped_resource.h"

#include "resource.h"

//Qt includes
#include <qcursor.h>
#include <qwidget.h>
#include <qdragobject.h>
#include <qpopupmenu.h>
#include <qlayout.h>

//KDE includes
#include <kapplication.h>
#include <khtmlview.h>
#include <kglobalsettings.h>
#include <khtml_events.h>
#include <dom/dom2_range.h>
#include <dom/html_element.h>


CHTMLReadDisplay::CHTMLReadDisplay(CReadWindow* readWindow, QWidget* parentWidget) : KHTMLPart((m_view = new CHTMLReadDisplayView(this, parentWidget ? parentWidget : readWindow)), readWindow), CReadDisplay(readWindow) {
	qWarning("constructor of CHTMLReadDisplay");
  Q_ASSERT(readWindow);
  Q_ASSERT(parentWidget);
  setDNDEnabled(false);
  m_view->setDragAutoScroll(false);
}

CHTMLReadDisplay::~CHTMLReadDisplay(){
}

const QString CHTMLReadDisplay::text( const CDisplay::TextType format, const CDisplay::TextPart part) {
//  qWarning("CHTMLReadDisplay::text( const CDisplay::TextType format, const CDisplay::TextPart part)");
  switch (part) {
    case Document: {
      if (format == HTMLText)
        return document().toHTML();
      else {
        qWarning(htmlDocument().body().innerText().string().latin1());
        return htmlDocument().body().innerText().string().latin1();
      }
		}

    case SelectedText: {
			if (!hasSelection())
   			return QString::null;

      DOM::Range range = selection();
      if (range.isNull()) {
      	return QString::null;
      }
      else if (format == HTMLText) {
        return range.toHTML().string();
      }
      else {
        return range.toString().string();
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

      	return QString::fromLatin1("%1\n\t%2").arg(key->key()).arg(key->strippedText());
   		}
     	return QString::null;
    }


    default:
      return QString::null;
  }
}

void CHTMLReadDisplay::setText( const QString& newText ) {
  qWarning("CHTMLReadDisplay::view()");
  begin();
  write(newText);
  end();
}

/** No descriptions */
const bool CHTMLReadDisplay::hasSelection(){
  return KHTMLPart::hasSelection();
}


/** Reimplementation. */
QWidget* CHTMLReadDisplay::view(){
//  qWarning("CHTMLReadDisplay::view()");
  return KHTMLPart::view();
}

void CHTMLReadDisplay::selectAll() {
  KHTMLPart::selectAll();
}

/** No descriptions */
void CHTMLReadDisplay::moveToAnchor( const QString& anchor ){
//  qWarning("nove to anchor %s", anchor.latin1());
	gotoAnchor(anchor);
}

void CHTMLReadDisplay::urlSelected( const QString& url, int button, int state, const QString& _target, KParts::URLArgs args){
  qWarning("CHTMLReadDisplay::urlSelected");
  KHTMLPart::urlSelected(url, button, state, _target, args);
  if (!url.isEmpty() && CReferenceManager::isHyperlink(url)) {
    QString module;
    QString key;
    CReferenceManager::Type type;
    const bool ok = CReferenceManager::decodeHyperlink(url, module, key, type);
    if (module.isEmpty())
      module = CReferenceManager::preferredModule( type );

#warning Really bad bad work around! Otherwise the widget would scroll with the mouse moves afetr a link was clicked!
    QMouseEvent me( QEvent::MouseButtonRelease, QPoint(0,0), QPoint(0,0), QMouseEvent::LeftButton, QMouseEvent::NoButton);
    khtml::MouseReleaseEvent kme(&me, -1,-1, DOM::DOMString(), DOM::DOMString(), DOM::Node());
    KApplication::sendEvent( this, &kme );

		connectionsProxy()->emitReferenceClicked(module, key);
  }
}

/** Reimplementation. */
void CHTMLReadDisplay::khtmlMouseReleaseEvent( khtml::MouseReleaseEvent* event ){
//  qWarning("CHTMLReadDisplay::khtmlMouseReleaseEvent( khtml::MouseReleaseEvent* event )");
  KHTMLPart::khtmlMouseReleaseEvent(event);	

  m_dndData.mousePressed = false;
  m_dndData.isDragging = false;
  m_dndData.node = DOM::Node();
  m_dndData.anchor = DOM::DOMString();
}

void CHTMLReadDisplay::khtmlMousePressEvent( khtml::MousePressEvent* event ){
//  qWarning("CHTMLReadDisplay::khtmlMousePressEvent( khtml::MousePressEvent* event )");	
  KHTMLPart::khtmlMousePressEvent(event);

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
    if (!m_dndData.node.isNull()) { //we drag a valid link
      m_dndData.dragType = DNDData::Link;
    }
  }
}

/** Reimplementation for our drag&drop system. */
void CHTMLReadDisplay::khtmlMouseMoveEvent( khtml::MouseMoveEvent* e ){
//  qWarning("CHTMLReadDisplay::khtmlMouseMoveEvent( khtml::MouseMoveEvent* e )");
  if( !(e->qmouseEvent()->state() && LeftButton)) { //left mouse button not pressed
    KHTMLPart::khtmlMouseMoveEvent(e);
    return;
  }

  const int delay = KGlobalSettings::dndEventDelay();
  QPoint newPos = QPoint(e->x(), e->y());

  if(!m_dndData.anchor.isEmpty() &&
     (newPos.x() > m_dndData.startPos.x()+delay || newPos.x() < m_dndData.startPos.x()-delay ||
     newPos.y() > m_dndData.startPos.y()+delay || newPos.y() < m_dndData.startPos.y()-delay) &&
     !m_dndData.isDragging && m_dndData.mousePressed && m_dndData.dragType == DNDData::Link &&
     !m_dndData.node.isNull() /*&& !hasSelection()*/ )
  {
    QString module = QString::null;
  	QString key = QString::null;
  	CReferenceManager::Type type;
  	if ( !CReferenceManager::decodeHyperlink(m_dndData.anchor.string(), module, key, type) )
  		return;

  	QTextDrag *d = new QTextDrag(CReferenceManager::encodeReference(module,key),KHTMLPart::view()->viewport());
    d->setSubtype(REFERENCE);
    d->setPixmap(REFERENCE_ICON_SMALL);
    m_dndData.isDragging = true;
    m_dndData.mousePressed = false;
    d->drag();
    return;
  }

  KHTMLPart::khtmlMouseMoveEvent(e);
}
/* -------------------------- */
CHTMLReadDisplayView::ToolTip::ToolTip(CHTMLReadDisplayView* view) : CToolTip(view), m_view( view ) {
};

/** Decides whether a tooltip should be shown. */
void CHTMLReadDisplayView::ToolTip::maybeTip( const QPoint& p ){
  qWarning("CHTMLReadDisplayView::ToolTip::maybeTip( const QPoint& p )");
  DOM::Node node = m_view->part()->nodeUnderMouse();
  if (node.isNull())
  	return;

  DOM::Node linkNode = node;
  do {
    if (!linkNode.isNull() && linkNode.nodeName().string().upper() == "A" ) { //found right node
      if (linkNode.hasAttributes()) {
        QString link = QString::null;
        DOM::NamedNodeMap attributes = linkNode.attributes();
        for (unsigned int i = 0; i < attributes.length(); i++) {
          if (attributes.item(i).nodeName().string().upper() == "HREF") {
            link = attributes.item(i).nodeValue().string();
            break;
          }
        }

        const QString tooltipText = CTooltipManager::textForHyperlink( link );
        if (!tooltipText.isEmpty()) {
          QRect rect = linkNode.getRect();
          rect.setX( m_view->mapFromGlobal(QCursor::pos()).x() );
          rect.setY( m_view->mapFromGlobal(QCursor::pos()).y() );
	        tip( rect, tooltipText );
        }
        break;
      }
    }
  } while ( !(linkNode = linkNode.parentNode()).isNull() );
}

// ---------------------

CHTMLReadDisplayView::CHTMLReadDisplayView(CHTMLReadDisplay* displayWidget, QWidget* parent) : KHTMLView(displayWidget, parent), m_display(displayWidget) {
  qWarning("constructor of CHTMLReadDisplayView");
  Q_ASSERT(parent);
  viewport()->setAcceptDrops(true);
  setMarginWidth(4);
  setMarginHeight(4);
};


/** Opens the popupmenu at the given position. */
void CHTMLReadDisplayView::popupMenu( const QString& url, const QPoint& pos){	
//	qWarning("CHTMLReadDisplayView::popupMenu");
 	if (!url.isEmpty()) {
    qWarning(url.latin1());
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

  (void)new ToolTip(this);
}

/** Reimplementatiob from QScrollView. */
void CHTMLReadDisplayView::contentsDropEvent( QDropEvent* e ){
//  qWarning("CHTMLReadDisplayView::contentsDropEvent( QDropEvent* e )");
//  KHTMLView::contentsDropEvent(e);
  if (QTextDrag::canDecode(e)) {
//    if (e->provides(REFERENCE) || e->provides(BOOKMARK))
    {
      qWarning("ACCEPT DROP EVENT!");
      QString str;
      QCString submime;
      const bool accept = QTextDrag::decode(e, str);
      qWarning("DROP: %s", str.latin1());

      QString module;
      QString key;
      CReferenceManager::decodeReference(str, module, key );
//      qWarning("dropped key: %s", key.latin1());

      e->acceptAction();
      m_display->connectionsProxy()->emitReferenceDropped(key);
      return;
    }
  }
  e->acceptAction(false);
  e->ignore();
}

/** Reimplementation from QScrollView. */
void CHTMLReadDisplayView::contentsDragEnterEvent( QDragEnterEvent* e ){
//  qWarning("CHTMLReadDisplayView::contentsDragEnterEvent( QDragEnterEvent* e )");
//  KHTMLView::contentsDragEnterEvent(e);
  if (QTextDrag::canDecode(e)) {
//    if (e->provides(REFERENCE) || e->provides(BOOKMARK))
    {
//      qWarning("ACCEPT ENTER EVENT!");
      e->acceptAction();
      return;
    }
  }
  e->acceptAction(false);
  e->ignore();
}

