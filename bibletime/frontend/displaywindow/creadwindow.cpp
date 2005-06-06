/********* Read the file LICENSE for license details. *********/

//BibleTime includes
#include "creadwindow.h"

#include "backend/centrydisplay.h"
#include "backend/cdisplayrendering.h"
#include "backend/cswordkey.h"

#include "frontend/cbtconfig.h"
#include "frontend/cexportmanager.h"
#include "frontend/cmdiarea.h"
#include "frontend/cprofilewindow.h"
#include "frontend/displaywindow/cmodulechooserbar.h"
#include "frontend/keychooser/ckeychooser.h"

#include "util/cresmgr.h"

//KDE includes
#include <kpopupmenu.h>
#include <kaction.h>
#include <kaccel.h>
#include <kstdaccel.h>
#include <klocale.h>

CReadWindow::CReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name )
	: CDisplayWindow(modules,parent,name),
		m_displayWidget(0)
{
}

CReadWindow::~CReadWindow() {
// 	qWarning("destructor of CReadWindow");
}

/** Returns the display widget of this window. */
CReadDisplay* const CReadWindow::displayWidget() {
	return m_displayWidget;
}

/** Sets the display widget of this display window. */
void CReadWindow::setDisplayWidget( CReadDisplay* newDisplay ) {
	CDisplayWindow::setDisplayWidget(newDisplay);
	if (m_displayWidget) {
		disconnect(m_displayWidget->connectionsProxy(), SIGNAL(referenceClicked(const QString&, const QString&)), this, SLOT(lookup(const QString&, const QString&)));
		
		disconnect(m_displayWidget->connectionsProxy(), SIGNAL(referenceDropped(const QString&)), this, SLOT(lookup(const QString&)));
	}

	m_displayWidget = newDisplay;
	connect(
		m_displayWidget->connectionsProxy(),
		SIGNAL(referenceClicked(const QString&, const QString&)),
		this,
		SLOT(lookup(const QString&, const QString&))
	);
	
	connect(
		m_displayWidget->connectionsProxy(),
		SIGNAL(referenceDropped(const QString&)),
		this,
		SLOT(lookup(const QString&))
	);
}

/** Lookup the given entry. */
void CReadWindow::lookup( CSwordKey* newKey ) {
	Q_ASSERT(newKey);

	using namespace Rendering;

 	Q_ASSERT(isReady() && newKey && modules().first());
	if (!isReady() || !newKey || !modules().first()) {
		return;
	}

	Q_ASSERT(modules().first()->getDisplay());
	if (CEntryDisplay* display = modules().first()->getDisplay()) {	//do we have a display object?
		displayWidget()->setText(
			display->text(
				modules(),
				newKey->key(),
				displayOptions(),
				filterOptions()
			)
		);
	}
	
	if (key() != newKey) {
		key()->key(newKey->key());
	}

	setCaption( windowCaption() );	

// 	qDebug("[CReadWindow::lookup] Moving to anchor %s", CDisplayRendering::keyToHTMLAnchor(key()->key()).latin1());
	
	displayWidget()->moveToAnchor( CDisplayRendering::keyToHTMLAnchor(key()->key()) );
}

/** Reimplementation to use the popup menu. */
const bool CReadWindow::init( const QString& keyName ) {
	CDisplayWindow::init(keyName);
	
	setReady(true);
	keyChooser()->setKey(key());

	return true;
}

/** Store the settings of this window in the given CProfileWindow object. */
void CReadWindow::storeProfileSettings(CProfileWindow * const settings) {
	QRect rect;
	rect.setX(parentWidget()->x());
	rect.setY(parentWidget()->y());
	rect.setWidth(width());
	rect.setHeight(height());
	settings->setGeometry(rect);

	//	settings->setScrollbarPositions( m_htmlWidget->view()->horizontalScrollBar()->value(), m_htmlWidget->view()->verticalScrollBar()->value() );
	settings->setType(modules().first()->type());
	settings->setMaximized(isMaximized() || parentWidget()->isMaximized());
	settings->setFocus( (this == mdi()->activeWindow()) ); //set property to true if this window is the active one.

	if (key()) {
		sword::VerseKey* vk = dynamic_cast<sword::VerseKey*>(key());
		QString oldLang;
		if (vk) {
			oldLang = QString(vk->getLocale());
			vk->setLocale("en"); //save english locale names as default!
		}
		settings->setKey( key()->key() );
		if (vk) {
			vk->setLocale(oldLang.latin1());
		}
	}

	QStringList mods;
	
	ListCSwordModuleInfo allMods = modules();
	ListCSwordModuleInfo::iterator end_it = allMods.end();
	for (ListCSwordModuleInfo::iterator it(allMods.begin()); it != end_it; ++it) {
		mods.append((*it)->name());
	}
	settings->setModules(mods);
}

void CReadWindow::applyProfileSettings(CProfileWindow * const settings) {
	//  parentWidget()->setUpdatesEnabled(false);
	setUpdatesEnabled(false);

	if (settings->maximized()) { //maximize this window
		// Use parentWidget() to call showMaximized. Otherwise we'd get lot's of X11 errors
		parentWidget()->showMaximized();
	} else {
		const QRect rect = settings->geometry();
		resize(rect.width(), rect.height());
		parentWidget()->move(rect.x(), rect.y());
	}

	setUpdatesEnabled(true);
}

void CReadWindow::insertKeyboardActions( KActionCollection* const ){
}

/** No descriptions */
void CReadWindow::copyDisplayedText() {
	CExportManager mgr(QString::null);
	mgr.copyKey(key(), CExportManager::Text, true);
}
