/***************************************************************************
                          cexportmanager.cpp  -  description
                             -------------------
    begin                : Mon Feb 25 2002
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

#include "cexportmanager.h"
#include "../backend/cswordkey.h"
#include "../backend/cswordmoduleinfo.h"
#include "../printing/cprintitem.h"
#include "../printing/cprinter.h"
#include "ctoolclass.h"

//Qt includes
#include <qprogressdialog.h>

//KDE includes
#include <kfiledialog.h>
#include <klocale.h>
#include <kapp.h>

//Sword includes
#include <swkey.h>
#include <listkey.h>

//CExportManager::CExportManager(){
//}
//
//CExportManager::~CExportManager(){
//}

/** Saves the key to disk. */
const bool CExportManager::saveKey( CSwordKey* key, const QString& caption, const QString& description,const bool withText, const bool showProgress ) {

}

/** Saves the key to disk. */
const bool CExportManager::saveKeyList( ListKey* list, CSwordModuleInfo* module, const QString& caption, const QString& description, const bool withText, const bool showProgress ){
	qWarning("CExportManager::saveKeyList( ListKey* keys, const QString& caption, const QString& description, const bool withText, const bool showProgress )");	
	ASSERT(list);
	const QString file = KFileDialog::getSaveFileName(QString::null, i18n("*.txt | Text files\n *.* | All files (*.*)"), 0, i18n("Save search result ..."));	
	if (file.isEmpty())
		return false;
	
	QProgressDialog progress( i18n("Saving..."), i18n("Cancel"), list->Count(), 0,"progress", true );	
	progress.show();
	progress.setProgress(0);	
	progress.setMinimumDuration(0);
	
	QString text = i18n("Search result for \"%1\" in module \"%2\"\n")/*.arg(searchedText).arg( m_currentModule->name() )*/;
	text += i18n("Entries found:") + QString::fromLatin1(" %1\n\n").arg(list->Count());	
	
	CSwordKey* key = CSwordKey::createInstance(module);
	ASSERT(key);
	int index = 0;
	*list = TOP;
	while (!list->Error() && !progress.wasCancelled()) {
		key->key((const char*)(*list));
		if (!key)
			break;
		progress.setProgress(index++);
		KApplication::kApplication()->processEvents(10); //do not lock the GUI!																		
		
		if (withText)
			text += QString::fromLatin1("%1:\n\t%2\n").arg( key->key() ).arg( key->strippedText() );
		else
			text += key->key() + "\n";

		(*list)++;
	}
	delete key;
	if (progress.wasCancelled())
		return false;
	progress.setProgress(index);	
	CToolClass::savePlainFile(file, text);
	
	return true;
}

const bool CExportManager::saveKeyList( QList<CSwordKey>& list, CSwordModuleInfo* module, const QString& caption, const QString& description, const bool withText, const bool showProgress ){
	qWarning("CExportManager::saveKeyList( ListKey* keys, const QString& caption, const QString& description, const bool withText, const bool showProgress )");	
	const QString file = KFileDialog::getSaveFileName(QString::null, i18n("*.txt | Text files\n *.* | All files (*.*)"), 0, i18n("Save search result ..."));	
	if (file.isEmpty())
		return false;
	
	QProgressDialog progress( i18n("Saving..."), i18n("Cancel"), list.count(), 0,"progress", true );	
	progress.show();
	progress.setProgress(0);	
	progress.setMinimumDuration(0);
	
	QString text = i18n("Search result for \"%1\" in module \"%2\"\n")/*.arg(searchedText).arg( m_currentModule->name() )*/;
	text += i18n("Entries found:") + QString::fromLatin1(" %1\n\n").arg(list.count());	
	
	CSwordKey* key = 0;
	for (list.first(); list.current(); list.next()) {
		key = list.current();
		if (!key)
			break;
		progress.setProgress(list.at());
		KApplication::kApplication()->processEvents(10); //do not lock the GUI!																		
		
		if (withText)
			text += QString::fromLatin1("%1:\n\t%2\n").arg( key->key() ).arg( key->strippedText() );
		else
			text += key->key() + "\n";
	}
		
	if (progress.wasCancelled())
		return false;
	progress.setProgress(list.count());	
	CToolClass::savePlainFile(file, text);
	
	return true;
}



//////// printing functions //////////

const bool CExportManager::printKey( CSwordModuleInfo* module, const QString& startKey, const QString& stopKey, const QString& description ){
	qWarning("CExportManager::printKeyList( ListKey* keys, const QString& caption, const QString& description, const bool withText, const bool showProgress )");	
	qWarning("start = %s, stop = %s", startKey.latin1(), stopKey.latin1());
	printer()->appendItem( new CPrintItem(module, startKey, stopKey, description) );	
	return true;
}

const bool CExportManager::printKeyList( ListKey* list, CSwordModuleInfo* module, const QString& caption, const QString& description, const bool showProgress ){
	qWarning("CExportManager::printKeyList( ListKey* keys, const QString& caption, const QString& description, const bool withText, const bool showProgress )");	

	QProgressDialog progress( caption, i18n("Cancel"), list->Count(), 0,"progress", true );	
	progress.setProgress(0);	
	progress.setMinimumDuration(0);	
	progress.show();
	
	int index = 0;	
	QList<CPrintItem> itemList;
	QString startKey, stopKey;
	
	*list = TOP;
	while (!list->Error() && !progress.wasCancelled()) {
		VerseKey* vk = dynamic_cast<VerseKey*>(list);
		if (vk) {
			startKey = QString::fromLocal8Bit((const char*)(vk->LowerBound()) );
			stopKey = QString::fromLocal8Bit((const char*)(vk->UpperBound()) );
		}
		else {
			startKey = (const char*)(*list);
			stopKey = QString::null;
		}
		itemList.append( new CPrintItem(module, startKey, stopKey) );
		
		progress.setProgress(index++);
		KApplication::kApplication()->processEvents(10); //do not lock the GUI!
				
		(*list)++;
	}
	
	//add all items to the queue
	if (progress.wasCancelled()) {
		itemList.setAutoDelete(true);	
		itemList.clear();//delete all items
		
		return false;
	}	
	progress.setProgress(list->Count());	
	printer()->appendItems(itemList);	
	
	return true;
}
