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
#include "../util/scoped_resource.h"
#include "ctoolclass.h"

//Qt includes
#include <qprogressdialog.h>
#include <qclipboard.h>

//KDE includes
#include <kfiledialog.h>
#include <klocale.h>
#include <kapp.h>

//Sword includes
#include <swkey.h>
#include <listkey.h>

/** Saves the key to disk. */
const bool CExportManager::saveKey( CSwordKey* key, const bool withText ) {
//	const QString file = KFileDialog::getSaveFileName(QString::null, i18n("*.txt | Text files\n *.* | All files (*.*)"), 0, i18n("Save search result ..."));	
//  if (!file.isEmpty()) {
//		util::scoped_ptr<CSwordKey> key(CSwordKey::createInstance(module));
//		key->key();
//  }
	return true;
}

/** Saves the key to disk. */
const bool CExportManager::saveKeyList( ListKey* list, CSwordModuleInfo* module, const QString& label, const bool withText, const bool showProgress ){
	bool ret = false;
	const QString file = KFileDialog::getSaveFileName(QString::null, i18n("*.txt | Text files\n *.* | All files (*.*)"), 0, i18n("Save search result ..."));
	if (!file.isEmpty()) {
		QProgressDialog progress( label, i18n("Cancel"), list->Count(), 0,"progress", true );	
		progress.setProgress(0);	
		progress.setMinimumDuration(10);	
		progress.show();
	
		util::scoped_ptr<CSwordKey> key(CSwordKey::createInstance(module));
	
		QString text;	
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
		if (! progress.wasCancelled()) {
			progress.setProgress(index);	
			CToolClass::savePlainFile(file, text);
			ret = true;
		}
	}
	
	return ret;
}

const bool CExportManager::saveKeyList( QList<CSwordKey>& list, CSwordModuleInfo* module, const QString& label, const bool withText, const bool showProgress ){
	bool ret = false;
	const QString file = KFileDialog::getSaveFileName(QString::null, i18n("*.txt | Text files\n *.* | All files (*.*)"), 0, i18n("Save search result ..."));	
	if (!file.isEmpty()){
	
		QProgressDialog progress( label, i18n("Cancel"), list.count(), 0,"progress", true );	
		progress.setProgress(0);	
		progress.setMinimumDuration(10);	
		progress.show();

		QString text;	
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
		
		if (!progress.wasCancelled()) {
			progress.setProgress(list.count());	
			CToolClass::savePlainFile(file, text);
		}
	}
	
	return ret;
}


///////// copy functions
const bool CExportManager::copyKey( CSwordKey* key, const bool withText ) {
	bool ret = true;
	QString text = (withText) ? QString::fromLatin1("%1:\n\t%2").arg(key->key()).arg(key->strippedText()) : key->key()+"\n";

	KApplication::clipboard()->setText(text);

	return ret;
}

const bool CExportManager::copyKeyList( ListKey* list, CSwordModuleInfo* module, const QString& label, const bool withText, const bool showProgress ){
	QProgressDialog progress( label, i18n("Cancel"), list->Count(), 0,"progress", true );	
	progress.setProgress(0);	
	progress.setMinimumDuration(10);	
	progress.show();
	
	QString text;	
	int index = 0;	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
	*list = TOP;
	while (!list->Error() && !progress.wasCancelled()) {
		key->key((const char*)(*list));
		if (!key)
			break;
		progress.setProgress(index++);
		KApplication::kApplication()->processEvents(10); //do not lock the GUI!																		
		
		text += (withText) ? QString::fromLatin1("%1:\n\t%2\n").arg(key->key()).arg(key->strippedText()) : key->key()+"\n";
		(*list)++;
	}
	if (progress.wasCancelled())
		return false;
		
	progress.setProgress(index);	
	KApplication::clipboard()->setText(text);		
	return true;
}


//////// printing functions //////////

const bool CExportManager::printKey( CSwordModuleInfo* module, const QString& startKey, const QString& stopKey, const QString& description ){
	printer()->appendItem( new CPrintItem(module, startKey, stopKey, description) );	
	return true;
}

const bool CExportManager::printKeyList( ListKey* list, CSwordModuleInfo* module, const QString& label, const bool showProgress ){
	QProgressDialog progress( label, i18n("Cancel"), list->Count(), 0,"progress", true );	
	progress.setProgress(0);	
	progress.setMinimumDuration(10);	
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
			startKey = QString::fromLocal8Bit((const char*)*list);
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
