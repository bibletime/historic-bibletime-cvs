/***************************************************************************
                          ctextdisplay.cpp  -  description
                             -------------------
    begin                : Fre Mai 3 2002
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
#include "cdisplaywidget.h"

#include "backend/creferencemanager.h"
#include "backend/cswordkey.h"

#include "frontend/cbtconfig.h"
#include "frontend/ctoolclass.h"

#include "resource.h"

#include "util/scoped_resource.h"

//Qt includes
#include <qapplication.h>
#include <qclipboard.h>
#include <qpopupmenu.h>
#include <qtooltip.h>
#include <qdragobject.h>

//KDE includes
#include <kapplication.h>
#include <kglobalsettings.h>
#include <khtml_events.h>
#include <klocale.h>
#include <kfiledialog.h>

#include <dom/dom2_range.h>

CDisplayWidgetView::ToolTip::ToolTip(CDisplayWidgetView* view) : QToolTip(view) {
  m_view = view;
};

/** Decides whether a tooltip should be shown. */
void CDisplayWidgetView::ToolTip::maybeTip( const QPoint& p ){
  /*
    1. Find the node which contains pos
  */

//  QPoint pos = m_view->viewportToContents(p);

  DOM::Node node = m_view->part()->nodeUnderMouse();
  qWarning("node under mouse: %s", node.nodeName().string().latin1());

  DOM::Node linkNode = node;
  do {
    if (!linkNode.isNull() && linkNode.nodeName().string().upper() == "A" ) { //found right node
      if (linkNode.hasAttributes()) {
        QString link = QString::null;
        DOM::NamedNodeMap attributes = linkNode.attributes();
        for (unsigned int i = 0; i < attributes.length(); i++) {
          if (attributes.item(i).nodeName().string().upper() == "HREF") {
            link = attributes.item(i).nodeValue().string();
            qWarning("link is %s", link.latin1());
            break;
          }
        }

        QString module = QString::null;
        QString ref = QString::null;
        CReferenceManager::Type type;
        const bool ok = CReferenceManager::decodeHyperlink(link, module, ref, type);
        qWarning("decoded hyperlink!");
        if (!ok || ref.isEmpty())
          return;

        CSwordModuleInfo* m = 0;
        if (module.isEmpty()) {
          module = CReferenceManager::preferredModule( type );
        }

        if (( m = backend()->findModuleByName(module) )) {
//        if (m->type() == CSwordModuleInfo::Bible || m->type() == CSwordModuleInfo::Commentary) {
//          if (CSwordModuleInfo* module = m_view->modules().first()) {
//            ref = CReferenceManager::parseVerseReference(ref);
//          }
//        }
          util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance( m ) );
          if (key) {
            backend()->setFilterOptions( CBTConfig::getFilterOptionDefaults() );
            key->key(ref);

            QRect rect = linkNode.getRect();
//            rect.setX( m_view->contentsToViewport(p).x() );
//            rect.setY( m_view->contentsToViewport(p).y() );
            rect.setX( p.x() );
            rect.setY( p.y() );
            rect.setWidth( linkNode.getRect().width() );
            rect.setHeight( linkNode.getRect().height() );

            qWarning("rect is %i / %i / %i / %i", rect.x(), rect.y(), rect.width(), rect.height());
            tip( rect, key->renderedText() );
          }
        }
        break;
      }
    }
  } while ( !(linkNode = linkNode.parentNode()).isNull() );
}

// ---------------------

CDisplayWidgetView::CDisplayWidgetView(CDisplayWidget* displayWidget, QWidget* parent) : KHTMLView(displayWidget, parent) {
  Q_ASSERT(displayWidget);
  Q_ASSERT(part());

  m_displayWidget = displayWidget;
  viewport()->setAcceptDrops(true);
};


/** Installs the popup menu which should occur when the right mouse button was pressed. Set the parametzer to 0 if yu want to disable the popup. */
void CDisplayWidgetView::installPopup( const PopupType type, QPopupMenu* popup ){
  switch (type) {
    case Anchor:
      m_popups.anchor = popup;
      break;
    case Normal:
      m_popups.normal = popup;
      break;
  };
}

/** Opens the popupmenu at the given position. */
void CDisplayWidgetView::popupMenu( const QString& url, const QPoint& pos){
  QPopupMenu* popup = 0;
  if (url.isEmpty()) {
    popup = m_popups.normal;
  }
  else {
    popup = m_popups.anchor;
  };
  if (popup)
    popup->exec(pos);
}

/** Reimplementation from QScrollView. Sets the right slots */
void CDisplayWidgetView::polish(){
  KHTMLView::polish();
  connect( part(), SIGNAL(popupMenu(const QString&, const QPoint&)),
    this, SLOT(popupMenu(const QString&, const QPoint&)));

  (void)new ToolTip(this);
}


/** Returns the view object. */
CDisplayWidgetView* CDisplayWidget::view() const {
  return m_view;
}


/** Reimplementatiob from QScrollView. */
void CDisplayWidgetView::contentsDropEvent( QDropEvent* e ){
  qWarning("CDisplayWidgetView::contentsDropEvent( QDropEvent* e )");
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
      qWarning("dropped key: %s", key.latin1());

      e->acceptAction();
      m_displayWidget->emitReferenceDropped(module,key);
      return;
    }
  }
  e->acceptAction(false);
  e->ignore();
}

/** Reimplementation from QScrollView. */
void CDisplayWidgetView::contentsDragEnterEvent( QDragEnterEvent* e ){
  qWarning("CDisplayWidgetView::contentsDragEnterEvent( QDragEnterEvent* e )");
//  KHTMLView::contentsDragEnterEvent(e);
  if (QTextDrag::canDecode(e)) {
//    if (e->provides(REFERENCE) || e->provides(BOOKMARK))
    {
      qWarning("ACCEPT ENTER EVENT!");
      e->acceptAction();
      return;
    }
  }
  e->acceptAction(false);
  e->ignore();
}

// ---------------------

CDisplayWidget::CDisplayWidget(QWidget *parent, const char *name) : KHTMLPart((m_view = new CDisplayWidgetView(this, parent)),parent,name) {
  setDNDEnabled(false);
}

CDisplayWidget::~CDisplayWidget(){

}

/** Returns the content of this display widget. */
const QString CDisplayWidget::text( CDisplayWidget::TextFormat format ) const {
  switch (format) {
    case HTMLText:
      return document().toHTML();
    case PlainText:
      return document().nodeValue().string();
    default:
      return QString::null;
  };

}

/** Set the content of this display widget to the parameter "text". */
void CDisplayWidget::setText( const QString& text ){
  begin();
  write(text);
  end();
}

/** Copies the given type into the clipboard. */
void CDisplayWidget::copy( const TextType type ){
  QString t;
  switch (type) {
    case AllText:
      t = text();
      break;
    case SelectedText:
      t = selectedText();
      break;
  };
  if (!t.isEmpty()) {
    QClipboard* cb = KApplication::clipboard();
    cb->setText(t);
  }
}

/** Copies the whole text into the clipboard. */
void CDisplayWidget::copyAll(){
  copy(AllText);
}

/** Copies the selection into the clipboard. */
void CDisplayWidget::copySelection(){
  copy(SelectedText);
}

/** Selects all text. */
void CDisplayWidget::selectAll(){
  KHTMLPart::selectAll();
}

/** Saves the document in the given format. */
void CDisplayWidget::save( const TextFormat format ){
  QString filter = QString::null;
  const QString content = text(format);

  switch (format) {
    case PlainText: {
      filter = QString::fromLatin1("*.txt | ") + i18n("Text files") + QString::fromLatin1("\n *.* | All files (*.*)");
      break;
    };
    case HTMLText: {
      filter = QString::fromLatin1("*.html | ") + i18n("HTML files") + QString::fromLatin1("\n *.* | All files (*.*)");
      break;
    };
  }
  const QString filename = KFileDialog::getSaveFileName(QString::null, filter, 0, i18n("Save document ..."));
  if (!filename.isEmpty()) {
    CToolClass::savePlainFile(filename, content);
  }
}

/** Saves the document as HTML file. */
void CDisplayWidget::saveAsHTML(){
  save(HTMLText);
}

/** Saves the document as plain text. */
void CDisplayWidget::saveAsPlain(){
  save(PlainText);
}

/** Returns the node which was activated by a RMB. */
const QString CDisplayWidget::activeURLNode(){
  return m_activatedURLNode.string();
}

/** Returns true if the active URL node is valid and false if it's not valid. */
const bool CDisplayWidget::hasActiveURLNode() const {
  return !m_activatedURLNode.isNull();
}

/** Reimplemented for current anchor methods. */
void CDisplayWidget::khtmlMousePressEvent( khtml::MousePressEvent* event ){
  m_activatedURLNode = DOM::DOMString();
  m_dndData.node = DOM::Node();
  m_dndData.anchor = DOM::DOMString();
  m_dndData.mousePressed = false;
  m_dndData.isDragging = false;

  if (event->qmouseEvent()->button() == Qt::RightButton) {
    m_activatedURLNode = event->url();
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

//  //select current word
//  if (event && event->qmouseEvent()->button() == Qt::RightButton) {
//    qWarning("set selection: %s", event->innerNode().nodeName().string().latin1());
//
//    DOM::Node node = event->innerNode();
//    while (!node.isNull() && node.childNodes().length() && node.nodeName().upper() != "#TEXT") {
//      qWarning(node.nodeName().string().latin1());
//      node = node.firstChild();
//    };
//    qWarning(node.nodeName().string().latin1());

//    DOM::Range range = selection();
//    qWarning("node is from %s to %s", range.startContainer().nodeName().string().latin1(),range.endContainer().nodeName().string().latin1());
//    qWarning("node offsets are %i and %i", range.startOffset(),range.endOffset());

//    DOM::Range range = document().createRange();
//    DOM::Range range( node, event->offset(), node, event->offset() );
//  range.setStart(node,event->offset());
//  range.setEnd(node,event->offset()+5);
//  range.setEndAfter(node);
//    range.selectNode(node);

//  range.setEndAfter(node,);
//
//    qWarning("select now %s!", range.toHTML().string().latin1());
//    setSelection(range);
//  }

  KHTMLPart::khtmlMousePressEvent(event);
}

/** Reimplemented for current anchor methods. */
void CDisplayWidget::khtmlMouseReleaseEvent( khtml::MouseReleaseEvent* e ){
  m_dndData.mousePressed = false;
  m_dndData.isDragging = false;
  m_dndData.node = DOM::Node();
  m_dndData.anchor = DOM::DOMString();

  KHTMLPart::khtmlMouseReleaseEvent(e);
}

/** Reimplementation for our drag&drop system. */
void CDisplayWidget::khtmlMouseMoveEvent( khtml::MouseMoveEvent* e ){
  if( !(e->qmouseEvent()->state() && LeftButton)) {
    KHTMLPart::khtmlMouseMoveEvent(e);
    return;
  }

  const int delay = KGlobalSettings::dndEventDelay();
  QPoint newPos = QPoint(e->x(), e->y());

  if (m_dndData.anchor.isEmpty()) {
    KHTMLPart::khtmlMouseMoveEvent(e);
  }
  else if((newPos.x() > m_dndData.startPos.x()+delay || newPos.x() < m_dndData.startPos.x()-delay ||
     newPos.y() > m_dndData.startPos.y()+delay || newPos.y() < m_dndData.startPos.y()-delay) &&
     !m_dndData.isDragging && m_dndData.mousePressed && m_dndData.dragType == DNDData::Link &&
     !m_dndData.node.isNull() /*&& !hasSelection()*/ )
  {
    QString module = QString::null;
  	QString key = QString::null;
  	CReferenceManager::Type type;
  	if ( !CReferenceManager::decodeHyperlink(m_dndData.anchor.string(), module, key, type) )
  		return;

  	QTextDrag *d = new QTextDrag(CReferenceManager::encodeReference(module,key),view()->viewport());
    d->setSubtype(REFERENCE);
    d->setPixmap(REFERENCE_ICON_SMALL);
    m_dndData.isDragging = true;
    m_dndData.mousePressed = false;
    d->drag();
//    return;
  }
}

/** Emits the correct signal when a ref was dropped on the widget. */
void CDisplayWidget::emitReferenceDropped( const QString& module, const QString& key ){
  if (!key.isEmpty()) {
    qWarning("emit signal!");
    emit referenceDropped(module, key);
  }
}

/** Reimplementation. */
void CDisplayWidget::urlSelected( const QString& url, int button, int state, const QString& _target, KParts::URLArgs args){
  KHTMLPart::urlSelected(url, button, state, _target, args);
//  qWarning("URL %s chosen", url.latin1());

  if (!url.isEmpty() && CReferenceManager::isHyperlink(url)) {
    QString module;
    QString ref;
    CReferenceManager::Type type;
    const bool ok = CReferenceManager::decodeHyperlink(url, module, ref, type);

    if (module.isEmpty()) {
      module = CReferenceManager::preferredModule( type );
    }

    emit referenceClicked(module, ref);
  }
}
