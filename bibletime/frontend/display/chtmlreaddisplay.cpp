/********* Read the file LICENSE for license details. *********/

#include "chtmlreaddisplay.h"

#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/creadwindow.h"
#include "backend/creferencemanager.h"
#include "backend/cswordkey.h"

#include "frontend/cbtconfig.h"
#include "frontend/cdragdropmgr.h"
#include "frontend/cinfodisplay.h"

#include "util/ctoolclass.h"
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

using namespace InfoDisplay;

CHTMLReadDisplay::CHTMLReadDisplay(CReadWindow* readWindow, QWidget* parentWidget) 
	: KHTMLPart((m_view = new CHTMLReadDisplayView(this, parentWidget ? parentWidget : readWindow)), readWindow ? readWindow : parentWidget),
	CReadDisplay(readWindow),
	m_currentAnchorCache(QString::null)
{
  setDNDEnabled(false);
  setJavaEnabled(false);
	setJScriptEnabled(false);
	setPluginsEnabled(false);
	
	m_view->setDragAutoScroll(false);
	
}

CHTMLReadDisplay::~CHTMLReadDisplay(){
}

const QString CHTMLReadDisplay::text( const CDisplay::TextType format, const CDisplay::TextPart part) {
  switch (part) {
    case Document: {
      if (format == HTMLText) {
        return document().toHTML();
			}
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
				
				//TODO: This is a BAD HACK, we have to fnd a better solution to manage the settings now
				CSwordBackend::FilterOptions filterOptions;
				filterOptions.footnotes = false;
				filterOptions.strongNumbers = false;
				filterOptions.morphTags = false;
				filterOptions.lemmas = false;
				filterOptions.scriptureReferences = false;
				filterOptions.textualVariants = false;

				CPointers::backend()->setFilterOptions(filterOptions);
				
      	return QString(key->strippedText()).append("\n(")
						.append(key->key())
						.append(", ")
						.append(key->module()->name())
						.append(")");
/*				("%1\n(%2, %3)")
          .arg()
          .arg(key->key())
          .arg(key->module()->name());*/
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
	m_currentAnchorCache = anchor;
	
	//This is an ugly hack to work around a KDE problem in KDE including 3.3.1 (no later versions tested so far)
 	QTimer::singleShot(0, this, SLOT(slotGoToAnchor()));
	
// instead of:
// 	slotGoToAnchor();
}

void CHTMLReadDisplay::urlSelected( const QString& url, int button, int state, const QString& _target, KParts::URLArgs args){
  KHTMLPart::urlSelected(url, button, state, _target, args);
  m_urlWorkaroundData.doWorkaround = false;
// 	qWarning("clicked: %s", url.latin1());
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
    m_urlWorkaroundData.state = state;
		m_urlWorkaroundData.button = button;
		m_urlWorkaroundData.target = _target;
		m_urlWorkaroundData.args = args;
		m_urlWorkaroundData.module = module;
		m_urlWorkaroundData.key = key;
  }
  else if (!url.isEmpty() && (url.left(1) == "#")) { //anchor
    moveToAnchor(url.mid(1));
  }
	else if (url.left(7) == "http://"){ //open the bowser configured by kdeb
		KApplication::kApplication()->invokeBrowser( url ); //ToDo: Not yet tested
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
    connectionsProxy()->emitReferenceClicked(
    	m_urlWorkaroundData.module,
    	m_urlWorkaroundData.key
    );
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

/** Reimplementation for our drag&drop system. Also needed for the mouse tracking */
void CHTMLReadDisplay::khtmlMouseMoveEvent( khtml::MouseMoveEvent* e ){
  if( e->qmouseEvent()->state() & LeftButton == LeftButton) { //left mouse button pressed
		const int delay = KGlobalSettings::dndEventDelay();
		QPoint newPos = QPoint(e->x(), e->y());
	
		if ( (newPos.x() > m_dndData.startPos.x()+delay || newPos.x() < (m_dndData.startPos.x()-delay) ||
				newPos.y() > m_dndData.startPos.y()+delay || newPos.y() < (m_dndData.startPos.y()-delay)) &&
				!m_dndData.isDragging && m_dndData.mousePressed  )
		{
			QDragObject* d = 0;
			if (!m_dndData.anchor.isEmpty() && (m_dndData.dragType == DNDData::Link) && !m_dndData.node.isNull() ) {
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
			else if ((m_dndData.dragType == DNDData::Text) && !m_dndData.selection.isEmpty()) {    // create a new plain text drag!
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
 	else if (getMouseTracking() && !(e->qmouseEvent()->state() & Qt::ShiftButton == Qt::ShiftButton)) { //no mouse button pressed and tracking enabled
	  DOM::Node node = e->innerNode();
//		bool setInfo = false;

		//if no link was under the mouse try to find a title attrivute
		if (!node.isNull() && (m_previousEventNode != node) ) { //we want to avoid precessing the node again
			DOM::Node currentNode = node;
			DOM::Node attr;
			
			CInfoDisplay::ListInfoData infoList;
			do {
				if (!currentNode.isNull() && (currentNode.nodeType() == DOM::Node::ELEMENT_NODE) && currentNode.hasAttributes()) { //found right node
					attr = currentNode.attributes().getNamedItem("note");
					if (!attr.isNull()) {
						infoList.append( qMakePair(CInfoDisplay::Footnote, attr.nodeValue().string()) );
					}
				
					attr = currentNode.attributes().getNamedItem("lemma");
					if (!attr.isNull()) {
						infoList.append( qMakePair(CInfoDisplay::Lemma, attr.nodeValue().string()) );
					}
					
					attr = currentNode.attributes().getNamedItem("morph");
					if (!attr.isNull()) {
						infoList.append( qMakePair(CInfoDisplay::Morph, attr.nodeValue().string()) );
					}
					
					attr = currentNode.attributes().getNamedItem("expansion");
					if (!attr.isNull()) {
						infoList.append( qMakePair(CInfoDisplay::Abbreviation, attr.nodeValue().string()) );
					}
/*					attr = currentNode.attributes().getNamedItem("pos");
					if (!attr.isNull()) {
						//infoList.append( qMakePair(CInfoDisplay::Morph, attr.nodeValue().string()) );
					}*/
					
/*					attr = currentNode.attributes().getNamedItem("gloss");
					if (!attr.isNull()) {
						infoList.append( qMakePair(CInfoDisplay::WordGloss, attr.nodeValue().string()) );
					}*/
				
/*					attr = currentNode.attributes().getNamedItem("xlit");
					if (!attr.isNull()) {
						//infoList.append( qMakePair(CInfoDisplay::Morph, attr.nodeValue().string()) );
					}*/
					
					attr = currentNode.attributes().getNamedItem("crossrefs");
					if (!attr.isNull()) {
						infoList.append( qMakePair(CInfoDisplay::CrossReference, attr.nodeValue().string()) );
					}
				}

				currentNode = currentNode.parentNode();
				if (!currentNode.isNull() && currentNode.hasAttributes()) {
					attr = currentNode.attributes().getNamedItem("class");
					if (!attr.isNull() && (attr.nodeValue().string() == "entry") || (attr.nodeValue().string() == "currententry") ) {
						break;
					}
				}
			} while ( !currentNode.isNull() );
			
			//Code part to show a translation of the hovered word, only works with KDE 3.3
/*			if (!infoList.count()) { //translate the text under the mouse, find the lowest node containing the mouse
			
				DOM::Node node = nonSharedNodeUnderMouse();

				if (!node.isNull() && node.nodeName().string() == "#text") {
 					infoList.append( qMakePair(
							CInfoDisplay::WordTranslation, 
							node.nodeValue().string()
						)
					);
				}
			}*/
			
			if ( !(e->qmouseEvent()->state() & Qt::ShiftButton) ) { //SHIFT key not pressed, so we display
				CPointers::infoDisplay()->setInfo(infoList);
			}
			
			m_previousEventNode = node;
		}
	} 
	
	KHTMLPart::khtmlMouseMoveEvent(e);
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

/*!
    \fn CHTMLReadDisplay::slotPageLoaded()
 */
void CHTMLReadDisplay::slotGoToAnchor() {
 	if (!m_currentAnchorCache.isEmpty()) {
		if (!gotoAnchor( m_currentAnchorCache ) ) {
			qDebug("anchor %s not present!", m_currentAnchorCache.latin1());
		}
 	}
	m_currentAnchorCache = QString::null;
}

void CHTMLReadDisplay::zoomIn() {
// 	qWarning("zooming in");
 	setZoomFactor( (int)((float)zoomFactor()*1.1) );
}

void CHTMLReadDisplay::zoomOut() {
// 	qWarning("zooming out");
 	setZoomFactor( (int)((float)zoomFactor()*(1.0/1.1)) );
}
