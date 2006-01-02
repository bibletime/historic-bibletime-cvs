/********* Read the file LICENSE for license details. *********/

#include "csearchdialog.h"
#include "csearchanalysis.h"
#include "backend/cswordmodulesearch.h"


#include "backend/cswordkey.h"
#include "backend/cswordversekey.h"

#include "frontend/cbtconfig.h"

#include "util/cresmgr.h"
#include "util/ctoolclass.h"

//Qt includes
#include <qhbox.h>
#include <qvbox.h>
#include <qptrlist.h>
#include <qpainter.h>
#include <qlayout.h>
#include <qmap.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qsizepolicy.h>
#include <qpushbutton.h>
#include <qheader.h>
#include <qregexp.h>
#include <qmessagebox.h>

//KDE includes
#include <kapplication.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kiconloader.h>

static CSearchDialog* m_staticDialog = 0;

void CSearchDialog::openDialog(const ListCSwordModuleInfo modules, const QString& searchText, QWidget* parentDialog) {
	if (!m_staticDialog) {
		m_staticDialog = new CSearchDialog(parentDialog);
	};
	m_staticDialog->reset();

	if (modules.count()) {
		m_staticDialog->setModules(modules);
	}
	else {
		m_staticDialog->showModulesSelector();
	}

	m_staticDialog->setSearchText(searchText);
	if (m_staticDialog->isHidden()) {
		m_staticDialog->show();
	}

	m_staticDialog->raise();
	if (modules.count() && !searchText.isEmpty()) {
		m_staticDialog->startSearch();
	}
};

CSearchDialog* const CSearchDialog::getSearchDialog() {
	Q_ASSERT(m_staticDialog);
	return m_staticDialog;
};

CSearchDialog::CSearchDialog(QWidget *parent)
: KDialogBase(Plain, i18n("Search dialog"), Close | User1, User1, parent, "CSearchDialog", false, true, i18n("Search")) {

	setWFlags( getWFlags() | Qt::WStyle_MinMax );
	setIcon(CResMgr::searchdialog::icon);

	m_searcher.connectFinished( this, SLOT(searchFinished()));

	initView();
	initConnections();
}

// CSearchDialog::~CSearchDialog(){
// }

/** Starts the search with the set modules and the set search text. */
void CSearchDialog::startSearch() {
	QString searchText(m_searchOptionsPage->searchText());

	if (searchText.isEmpty()) {
		return;
	}

	// check that we have the indices we need for searching
	if (!m_searcher.modulesHaveIndices( modules() ) )	{
		int result = QMessageBox::question(this, i18n("Missing indices"),
			i18n("One or more modules need indexing before they can be searched.\n"
			"This could take a long time. Proceed with indexing?"),
			QMessageBox::Yes | QMessageBox::Default,
			QMessageBox::No | QMessageBox::Escape);
		if (result == QMessageBox::Yes) {
			// FIXME - add a proper progress dialog for indexing
			m_searcher.indexModules( modules() );
		}
		else {
			return;
		}
	}

	m_searchResultPage->reset();

	int searchFlags = m_searchOptionsPage->searchFlags();

	const CSwordModuleSearch::scopeType scopeType = m_searchOptionsPage->scopeType();
	if (scopeType == CSwordModuleSearch::Scope_LastSearch) {
		searchFlags |= CSwordModuleSearch::useLastResult;
	}
	else if ( (scopeType == CSwordModuleSearch::Scope_Bounds)
			  && strlen(m_searchOptionsPage->searchScope().getRangeText())) {
		//we need the scope flag and a valid scope!
		searchFlags |= CSwordModuleSearch::useScope;
		m_searcher.setSearchScope( m_searchOptionsPage->searchScope() );
	}

	m_searcher.setModules( modules() );
	m_searcher.setSearchedText(searchText);
	m_searcher.setSearchOptions(searchFlags);

	m_searcher.startSearch();
}

/** Starts the search with the given module list and given search text. */
void CSearchDialog::startSearch( const ListCSwordModuleInfo modules, const QString& searchText) {
	m_searchResultPage->reset();
	m_searchOptionsPage->reset();
	setModules(modules);
	setSearchText(searchText);

	startSearch();
}

/** Returns the list of used modules. */
const ListCSwordModuleInfo CSearchDialog::modules() {
	return m_searchOptionsPage->modules();
}

/** Sets the list of modules for the search. */
void CSearchDialog::setModules( const ListCSwordModuleInfo modules ) {
	m_searchOptionsPage->setModules(modules);
	resize( sizeHint() );
}

/** Returns the search text which is set currently. */
const QString CSearchDialog::searchText() {
	return m_searchOptionsPage->searchText();
}

sword::ListKey CSearchDialog::searchScope() {
	return m_searchOptionsPage->searchScope();
};

/** Returns true if the search used a scope, otherwise false. */
const CSwordModuleSearch::scopeType CSearchDialog::searchScopeType() const {
	return m_searchOptionsPage->scopeType();
}

/** Returns true if the search used a scope, otherwise false. */
const int CSearchDialog::searchFlags() const {
	return m_searchOptionsPage->searchFlags();
}

/** Returns the search text which is used for the search. */
void CSearchDialog::setSearchText( const QString searchText ) {
	m_searchOptionsPage->setSearchText(searchText);
}

/** Initializes this object. */
void CSearchDialog::initView() {
	setButtonTip(User1, CResMgr::searchdialog::searchButton::tooltip);

 	QVBoxLayout* box = new QVBoxLayout ( plainPage() );
	box->setSpacing(3);

	m_searchOptionsPage = new CSearchOptionsPage(plainPage());
 	box->addWidget( m_searchOptionsPage );

	m_searchResultPage = new CSearchResultPage(plainPage());
	box->addWidget( m_searchResultPage );
}

void CSearchDialog::searchFinished() {
// 	qWarning("CSearchDialog::searchFinished()");
	if ( m_searcher.foundItems() ) {
		m_searchResultPage->setSearchResult(modules());
	}
	else
		m_searchResultPage->reset();
}

void CSearchDialog::showModulesSelector() {
	m_searchOptionsPage->chooseModules();
}

/** Initializes the signal slot connections */
void CSearchDialog::initConnections() {
	connect(this, SIGNAL(user1Clicked()), SLOT(startSearch()));
	connect(this, SIGNAL(closeClicked()), SLOT(slotDelayedDestruct()));
}

/** Resets the parts to the default. */
void CSearchDialog::reset() {
	m_searchOptionsPage->reset();
	m_searchResultPage->reset();
}

/** Reimplementation. */
void CSearchDialog::slotClose() {
	delayedDestruct();
	m_staticDialog = 0;
}
