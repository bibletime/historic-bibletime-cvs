/***************************************************************************
                   csearchdialogresultview.cpp  -  description
                             -------------------
    begin                : Wed Jan 19 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//own includes
#include "csearchdialogresultview.h"
#include "csearchdialog.h"
#include "../../resource.h"
#include "../../whatsthisdef.h"
#include "../../tooltipdef.h"
#include "../../backend/cswordbiblemoduleinfo.h"
#include "../../backend/cswordlexiconmoduleinfo.h"
#include "../../backend/cswordversekey.h"
#include "../../backend/cswordldkey.h"
#include "../../backend/creferencemanager.h"
#include "../cexportmanager.h"
#include "../ctoolclass.h"
#include "../cbtconfig.h"
#include "../../printing/cprintitem.h"
#include "../../printing/cprinter.h"

#include "../../util/scoped_resource.h"

//QT includes
#include <qlist.h>
#include <qheader.h>
#include <qevent.h>
#include <qmessagebox.h>
#include <qdragobject.h>
#include <qwidget.h>
#include <qclipboard.h>
#include <qprogressdialog.h>
#include <qwhatsthis.h>
#include <qtooltip.h>

//KDE includes
#include <kfiledialog.h>
#include <klocale.h>
#include <kstddirs.h>
#include <kiconloader.h>
#include <kapp.h>
#include <kpopupmenu.h>
#include <kglobalsettings.h>
#include <kaction.h>

//Sword includes
#include <listkey.h>
#include <swmodule.h>

CSearchDialogResultModuleView::CSearchDialogResultModuleView(QWidget *parent, const char *name)
	: QListView ( parent, name )	{	
	initView();
	adjustSize();
}

CSearchDialogResultModuleView::~CSearchDialogResultModuleView() {
	qDebug("CSearchDialogResultModuleView::~CSearchDialogResultModuleView()");
}

/** Initializes the tree of this ResultView */
void CSearchDialogResultModuleView::setupTree() {
	ListKey moduleSearchResult;
	QListViewItem	*module = 0;

	for (moduleList.first(); moduleList.current(); moduleList.next()) {
		moduleSearchResult = moduleList.current()->searchResult();
		module = new QListViewItem(this, QString("%1 [%2]").arg( moduleList.current()->name() ).arg( moduleSearchResult.Count() ));
		module->setPixmap(0,CToolClass::getIconForModule(moduleList.current()) );
	}	
	setFixedWidth( sizeHint().width() );
//	resize(sizeHint());
}

void CSearchDialogResultModuleView::viewportMousePressEvent(QMouseEvent *e) {
	qDebug("CSearchDialogResultModuleView::viewportMousePressEvent");
	QListView::viewportMousePressEvent(e);

	m_currentItem = (QListViewItem*)itemAt(e->pos());
	if (!m_currentItem)
	{
		qDebug("item is NULL");
		return;
	}
	if ((e->state() & ControlButton) || (e->state() & ShiftButton))
			return;
	for (moduleList.first();moduleList.current();moduleList.next()){
		QString modName = m_currentItem->text(0);
		modName = modName.left( modName.find(" [") );
		m_currentModule = backend()->findModuleByName(modName);
		if (m_currentModule)
			emit moduleSelected(m_currentModule);
	}	
	if (e->button() == RightButton)
		m_popup->popup( mapToGlobal(e->pos()) );
}

/** Initializes this widget */
void CSearchDialogResultModuleView::initView(){
	QWhatsThis::add(this, WT_SD_RESULT_MODULE_VIEW);
	
	setFocusPolicy(QWidget::WheelFocus);
	setAllColumnsShowFocus( true );
	setRootIsDecorated( false );
	addColumn( i18n( "Modules" ), -1 );
	header()->hide();
	setSorting( -1 );
	
	m_popup = new KPopupMenu(this);
	
	m_copyPopup = new KPopupMenu(m_popup);	
	m_copyPopup->insertItem(i18n("Search result"), this, SLOT(slotCopySearchResult()));	
	m_copyPopup->insertItem(i18n("Search result with text"), this, SLOT(slotCopySearchResultWithKeytext()));
	
	m_printPopup = new KPopupMenu(m_popup);		
	m_printPopup->insertItem(i18n("Search result"), this, SLOT(printSearchResult()));
	
	m_savePopup = new KPopupMenu(m_popup);		
	m_savePopup->insertItem(i18n("Search result"), this, SLOT(slotSaveSearchResult()));
	m_savePopup->insertItem(i18n("Search result with text"), this, SLOT(slotSaveSearchResultWithKeytext()));
	
	m_popup->insertItem(SmallIcon(ICON_EDIT_COPY), i18n("Copy..."), 	m_copyPopup);
	m_popup->insertItem(SmallIcon(ICON_FILE_PRINT), i18n("Add to printing queue..."), m_printPopup);
	m_popup->insertItem(SmallIcon(ICON_FILE_SAVE), i18n("Save..."), 	m_savePopup);
}

/** Returns the module belonging to the selected item. */
CSwordModuleInfo* CSearchDialogResultModuleView::getCurrentModule() {
	return m_currentModule;
}

/** Reimplementation. Sets the cortrect QHeader width. */
void CSearchDialogResultModuleView::resizeEvent( QResizeEvent* e){
	QListView::resizeEvent(e);	
	header()->resize( visibleWidth(), header()->height() );
}

/** Adds all items  of the search result of this module to the printing queue of BibleTime. */
void CSearchDialogResultModuleView::printSearchResult(){
	CExportManager::printKeyList( &(m_currentModule->searchResult()), m_currentModule, i18n("Appending keys to the printing queue ...") );		
}

/** This function copies the search result into the clipboard */
void CSearchDialogResultModuleView::slotCopySearchResult(){
	//get the searched text
//	QString searchedText;
//	for (QObject* w = parent(); w; w = w->parent()) {
//		if (w->isA("CSearchDialog")) {
//			CSearchDialog*	dlg = (CSearchDialog*)w;
//			searchedText = dlg->getSearchedText().stripWhiteSpace();
//			break;
//		}		
//	}
	CExportManager::copyKeyList(&m_currentModule->searchResult(), m_currentModule, i18n("Copying search result to clipboard..."), false);
}

/** This slot saves the search result to disk. */
void CSearchDialogResultModuleView::slotSaveSearchResult(){
	//get the searched text
	QString searchedText;
	for (QObject* w = parent(); w; w = w->parent()) {
		if (w->isA("CSearchDialog")) {
			CSearchDialog*	dlg = dynamic_cast<CSearchDialog*>(w);
			searchedText = dlg->getSearchedText().stripWhiteSpace();
			break;
		}		
	}
	ListKey& searchResult = m_currentModule->searchResult();
	CExportManager::saveKeyList(&searchResult, m_currentModule, i18n("Saving the search result ..."), false, true);	
}


void CSearchDialogResultModuleView::slotCopySearchResultWithKeytext(){
	//get the searched text
	QString searchedText;
	for (QObject* w = parent(); w; w = w->parent()) {
		if (w->isA("CSearchDialog")) {
			CSearchDialog*	dlg = (CSearchDialog*)w;
			searchedText = dlg->getSearchedText().stripWhiteSpace();
			break;
		}		
	}
	CExportManager::copyKeyList(&m_currentModule->searchResult(), m_currentModule, i18n("Copying search result to clipboard..."), true );
}

/** This slot saves the search result to disk. */
void CSearchDialogResultModuleView::slotSaveSearchResultWithKeytext(){
	//get the searched text
	QString searchedText;
	for (QObject* w = parent(); w; w = w->parent()) {
		if (w->isA("CSearchDialog")) {
			CSearchDialog*	dlg = (CSearchDialog*)w;
			searchedText = dlg->getSearchedText().stripWhiteSpace();
			break;
		}		
	}
	CExportManager::saveKeyList(&m_currentModule->searchResult(), m_currentModule,i18n("Saving the search result..."), true, true);
}

//------------class CSearchDialofResultView-----------//
CSearchDialogResultView::CSearchDialogResultView(QWidget *parent, const char *name)
	: QListBox(parent,name),
	m_currentItem(0),
	m_module(0),
	m_pressedPos()
{
	initView();
	initConnections();
}

CSearchDialogResultView::~CSearchDialogResultView() {
	qDebug("CSearchDialogResultView::~CSearchDialogResultView()");
}

/** Initializes the tree of this ResultView */
void CSearchDialogResultView::setupTree() {
	ListKey& moduleSearchResult = m_module->searchResult();
	const int count = moduleSearchResult.Count();
		
	clear();	
	setUpdatesEnabled(false);
	for (int index = 0; index < count; index++) {
		insertItem( QString::fromLocal8Bit((const char*)*moduleSearchResult.GetElement(index)), -1);
	}
	setUpdatesEnabled(true);	
}

/** Initializes the connections of this class */
void CSearchDialogResultView::initConnections() {
	connect(m_popup, SIGNAL(aboutToShow()),
		this, SLOT(popupAboutToShow()));
//	connect(this, SIGNAL(pressed(QListBoxItem*)),
//		this, SLOT(mousePressed(QListBoxItem*)));	
		
	connect(this, SIGNAL(currentChanged(QListBoxItem*)),
		this, SLOT(mousePressed(QListBoxItem*)));	
		
	connect(this, SIGNAL(rightButtonPressed(QListBoxItem*, const QPoint&)),
		this, SLOT(rightButtonPressed(QListBoxItem*, const QPoint&)));
}

/** Initializes this widget */
void CSearchDialogResultView::initView(){
	QWhatsThis::add(this, WT_SD_RESULT_RESULT_VIEW);
	setSelectionMode(QListBox::Extended);
	
	m_popup = new KPopupMenu(this);		
	m_copyPopup = new KPopupMenu(m_popup);	
	m_copyPopup->insertItem(i18n("Key"), this, SLOT(slotCopyCurrent()));	
	m_copyPopup->insertItem(i18n("Key with text"), this, SLOT(slotCopyCurrentWithKeytext()));
	
	m_printPopup = new KPopupMenu(m_popup);		
	m_printPopup->insertItem(i18n("Key with text"), this, SLOT(printItem()));
	
	m_savePopup = new KPopupMenu(m_popup);		
	m_savePopup->insertItem(i18n("Key"), this, SLOT(slotSaveCurrent()));
	m_savePopup->insertItem(i18n("Key with text"), this, SLOT(slotSaveCurrentWithKeytext()));
	
	m_popup->insertItem(SmallIcon(ICON_EDIT_COPY), i18n("Copy..."), m_copyPopup);
	m_popup->insertItem(SmallIcon(ICON_FILE_PRINT),i18n("Add to printing queue..."),m_printPopup);
	m_popup->insertItem(SmallIcon(ICON_FILE_SAVE), i18n("Save..."), m_savePopup);		
	
	setFocusPolicy(QWidget::WheelFocus);
}


/** Is called before the menu is shown */
void CSearchDialogResultView::popupAboutToShow(){
}

/**  */
void CSearchDialogResultView::viewportMouseMoveEvent(QMouseEvent *e){
//	qDebug("CSearchDialogResultView::viewportMouseMoveEvent(QMouseEvent *e)");
	if ( !(e->state() & QMouseEvent::LeftButton) ){
	  QListBox::viewportMouseMoveEvent(e);
		return;
	}
		
	//Is it time to start a drag?
 	if (m_currentItem && (abs(e->pos().x() - m_pressedPos.x()) > KGlobalSettings::dndEventDelay() ||
 		abs(e->pos().y() - m_pressedPos.y()) > KGlobalSettings::dndEventDelay()) )
 	{
		QString mod;
		QString ref;
		mod = m_module->name();
		ref = m_currentItem->text();

		QTextDrag *d = new QTextDrag(CReferenceManager::encodeReference(mod,ref), viewport());
		if (d){
			d->setSubtype(REFERENCE);
			d->setPixmap(REFERENCE_ICON_SMALL);
			d->drag();
			return;
		}
 	}
  else
  	QListBox::viewportMouseMoveEvent(e);
}

/**  */
void CSearchDialogResultView::printItem() {
	QList<QListBoxItem> list = selectedItems();	
	if (!list.count())
		return;
	for (list.first(); list.current(); list.next()) {
		CExportManager::printKey(m_module,list.current()->text(),QString::null);
	}
}

/** This slot is called when the current item changed. */
void CSearchDialogResultView::itemChanged(QListBoxItem* item){
	m_currentItem = item;
}

/** Opens the popup menu. */
void CSearchDialogResultView::rightButtonPressed( QListBoxItem* item, const QPoint& p){
	m_currentItem = item;	
	if (m_currentItem) {
		mousePressed(item);
		m_popup->popup(p);
	}
}

/**  */
void CSearchDialogResultView::mousePressed(QListBoxItem* item){
	qDebug("CSearchDialogResultView::mousePressed(QListBoxItem* item)");
	if (!( m_currentItem = item ))
		return;	
	QString text = QString::null;
	
	//we have to set the standard module view options for the module!!	
	util::scoped_ptr<CSwordKey> key(CSwordKey::createInstance(m_module));
	if ( key ) {
		backend()->setFilterOptions( CBTConfig::getFilterOptionDefaults() );				
		key->key(item->text());
		emit keySelected(key->renderedText());
	}		
}

/** This slot copies the current active item into the clipboard. */
void CSearchDialogResultView::slotCopyCurrent(){
	QString text;
	QList<QListBoxItem> list = selectedItems();
	for (list.first(); list.current(); list.next()) {
		text += list.current()->text()+"\n";
	}
	KApplication::clipboard()->setText(text);
}

/** This slot copies the current active item into the clipboard. */
void CSearchDialogResultView::slotCopyCurrentWithKeytext(){
	QList<QListBoxItem> list = selectedItems();
	QString text;
	QString keyText, keyName;
	for (list.first(); list.current(); list.next()) {
		CSwordKey* key = CSwordKey::createInstance(m_module);
		if (!key)
			continue;
			
		key->key(list.current()->text());
		keyName = key->key();
		keyText = key->strippedText();
		
		delete key;
		
		text += QString("%1\n%2\n\n").arg(keyName).arg(keyText);
	}
	
	KApplication::clipboard()->setText(text);
}

/** This slot copies the current active item into the clipboard. */
void CSearchDialogResultView::slotSaveCurrent(){
 	const QString file = KFileDialog::getSaveFileName (QString::null, i18n("*.txt | Text files\n *.* | All files (*.*)"), 0, i18n("Save key ..."));	
	if (!file.isNull()) {
		QList<QListBoxItem> list = selectedItems();
		QString text;
		for (list.first(); list.current(); list.next())
			text += list.current()->text()+"\n";
		CToolClass::savePlainFile(file, text);
	}
}

/** This slot copies the current active item into the clipboard. */
void CSearchDialogResultView::slotSaveCurrentWithKeytext(){
	QList<QListBoxItem> list = selectedItems();
	QString text;
	QString keyText, keyName;
	for (list.first(); list.current(); list.next()) {
		CSwordKey* key = CSwordKey::createInstance(m_module);
		if (!key)
			continue;
			
		key->key(list.current()->text());
		keyName = key->key();
		keyText = key->strippedText();
		
		delete key;
		
		text += QString("%1\n%2\n\n").arg(keyName).arg(keyText);
	}
	
 	const QString file = KFileDialog::getSaveFileName (QString::null, i18n("*.txt | Text files\n *.* | All files (*.*)"), 0, i18n("Save key ..."));	
	if (!file.isNull()) {
		CToolClass::savePlainFile( file, text);
	}
}

/** Returns the selected items of this listbox. */
QList<QListBoxItem> CSearchDialogResultView::selectedItems(){
	QList<QListBoxItem> list;
	QListBoxItem* item = firstItem();
	while (item) {
		if (item->selected())
			list.append(item);
		item = item->next();
	}
	return list;
}

/** Reimplementation to make Drag&Drop work. */
void CSearchDialogResultView::viewportMousePressEvent( QMouseEvent* e ){
	qDebug("CSearchDialogResultView::viewportMousePressEvent( QMouseEvent* e )");
	m_pressedPos = e->pos();	
	QListBox::viewportMousePressEvent(e);
}
