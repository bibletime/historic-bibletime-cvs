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
      // The result is 0 for Yes/Ok and 1 for No/Cancel.
      // It does not seem to matter what the two button constants are.
      // I have tried Yes/No, Ok/Cancel and they both show a Yes/No dialog with
      // Yes on the left and No on the right.  When Yes is clicked the result is 0,
      // when No is clicked the result is 1.
		if ( (result == QMessageBox::Yes) or (result == QMessageBox::Default) ) {
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

   QVBoxLayout *box = new QVBoxLayout( plainPage(), 0, spacingHint() );

	m_searchOptionsPage = new CSearchOptionsPage(plainPage());
	box->addWidget( m_searchOptionsPage );

	m_searchResultPage = new CSearchResultPage(plainPage());
	box->addWidget( m_searchResultPage );

	// The dialog doesn't resize properly if the minimum size of the
	// plain page is lower than the minimumsize of our two widgets.
	// You can resize the dialog, but it just starts covering up the
	// button bar and the two widgets instead of stopping at the
	// minimum size.  The following code sets the minimum with some
	// margin.  If you know of a better way to do this, do it!
	int w = m_searchOptionsPage->minimumWidth();
	int h = m_searchOptionsPage->minimumHeight() +
		m_searchResultPage->minimumHeight();
   plainPage()->setMinimumSize(w+10, h+100);
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
