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

#include "backend/cswordkey.h"
#include "backend/cswordmoduleinfo.h"
#include "backend/creferencemanager.h"
#include "backend/cswordversekey.h"
#include "backend/centrydisplay.h"

#include "frontend/ctoolclass.h"

#include "printing/cprintitem.h"
#include "printing/cprinter.h"

#include "util/scoped_resource.h"

//Qt includes
#include <qclipboard.h>
#include <qlist.h>
#include <qprogressdialog.h>

//KDE includes
#include <kprogress.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kapplication.h>

//Sword includes
#include <swkey.h>
#include <listkey.h>


CExportManager::CExportManager(const QString& caption, const bool showProgress, const QString& progressLabel, const CSwordBackend::FilterOptions filterOptions, const CSwordBackend::DisplayOptions displayOptions) {
  m_caption = !caption.isEmpty() ? caption : QString::fromLatin1("BibleTime");
  m_progressLabel = progressLabel;
  m_filterOptions = filterOptions;
  m_displayOptions = displayOptions;
  m_showProgress = showProgress;
};

const bool CExportManager::saveKey(CSwordKey* key, const Format format, const bool addText) {
  if (!key)
    return false;
  
  const QString filename = getSaveFileName(format);
  if (filename.isEmpty())
    return false;

  QString text = QString::null;
  if (addText) {
    CPointers::backend()->setFilterOptions(m_filterOptions);
    CPointers::backend()->setDisplayOptions(m_displayOptions);

    CSwordModuleInfo* module = key->module();    
  	if (CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(key) ) {
      CSwordVerseKey startKey(module);
      CSwordVerseKey stopKey(module);

  		startKey.key(vk->LowerBound());
  		stopKey.key(vk->UpperBound());
      QString entryText;
      if (format == HTML) {
        text = QString::fromLatin1("<HTML><HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><BODY>").arg(htmlCSS(module));
      };
      
  		while ( startKey < stopKey || startKey == stopKey ) {
        entryText = (format == HTML) ? startKey.renderedText() : startKey.strippedText();
      
  			text += ((bool)m_displayOptions.verseNumbers ? QString::fromLatin1("%1 ").arg(startKey.Verse()) : QString::null)
+ entryText + lineBreak(format);

        startKey.next(CSwordVerseKey::UseVerse);
  		}
  	}
    else {
      text = (format == HTML) ? key->renderedText() : key->strippedText();
    }
    text += "\n" + QString::fromLatin1("(%1, %1)").arg(key->key()).arg(module->name());
    if (format == HTML) {
      text += QString::fromLatin1("</BODY></HTML>");
    };    
  }
  else { //don't add text
    text = key ? key->key() : QString::null;
  	return true;
  }
  
	CToolClass::savePlainFile(filename, text);
	return true;
};

const bool CExportManager::saveKeyList(ListKey* list, CSwordModuleInfo* module, const Format format, const bool addText) {
  if (!list)
    return false;
  const QString filename = getSaveFileName(format);
  if (filename.isEmpty())
    return false;

 	QProgressDialog progress( m_progressLabel, i18n("Cancel"), list->Count(), 0,"progress", true );
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

 		if (addText)
 			text += QString::fromLatin1("%1:%2\t%3\n").arg( key->key() ).arg(lineBreak(format)).arg( (format == HTML) ? key->renderedText() : key->strippedText() );
 		else
 			text += key->key() + lineBreak(format);

 		(*list)++;
 	}
 	if (!progress.wasCancelled()) {
 		progress.setProgress(index);
 		CToolClass::savePlainFile(filename, text);
 		return true;
 	}
	return false;
};  

const bool CExportManager::copyKey(CSwordKey* key, const Format format, const bool addText) {
  if (!key)
    return false;

  QString text = QString::null;
  if (addText) {
    CPointers::backend()->setFilterOptions(m_filterOptions);
    CPointers::backend()->setDisplayOptions(m_displayOptions);

    CSwordModuleInfo* module = key->module();
  	if (CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(key) ) {
      CSwordVerseKey startKey(module);
      CSwordVerseKey stopKey(module);

  		startKey.key(vk->LowerBound());
  		stopKey.key(vk->UpperBound());
      QString entryText;
  		while ( startKey < stopKey || startKey == stopKey ) {
        entryText = (format == HTML) ? startKey.renderedText() : startKey.strippedText();

  			text += ((bool)m_displayOptions.verseNumbers ? QString::fromLatin1("%1 ").arg(startKey.Verse()) : QString::null)
+ entryText + lineBreak(format);

        startKey.next(CSwordVerseKey::UseVerse);
  		}
  	}
    else {
      text = (format == HTML) ? key->renderedText() : key->strippedText();
    }
    text += "\n" + QString::fromLatin1("(%1, %1)").arg(key->key()).arg(module->name());
  }
  else { //don't add text
    text = key ? key->key() : QString::null;
  	return true;
  }
  qWarning("copy now");
  KApplication::clipboard()->setText(text);
	return true;
};

const bool CExportManager::copyKeyList(ListKey* list, CSwordModuleInfo* module, const Format format, const bool addText) {
  if (!list)
    return false;

  QProgressDialog progress( m_progressLabel, i18n("Cancel"), list->Count()+1, 0,"progress", true );
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

 		if (addText)
 			text += QString::fromLatin1("%1:%2\t%3\n").arg( key->key() ).arg(lineBreak(format)).arg( (format == HTML) ? key->renderedText() : key->strippedText() );
 		else
 			text += key->key() + lineBreak(format);

 		(*list)++;
 	}

  if (!progress.wasCancelled()) {
    KApplication::clipboard()->setText(text);
 		progress.setProgress(index+1);    
   	return true;
 	}
	return false;
};

const bool CExportManager::printKeyList(ListKey* list, CSwordModuleInfo* module) {
	KProgressDialog progress( 0,"progress", m_caption, m_progressLabel, true );
  progress.progressBar()->setRange(0,list->Count()+1);
//	progress.progressBar()->setProgress(0);
	progress.setMinimumDuration(10);
	progress.show();

	int index = 0;
	QPtrList<CPrintItem> itemList;
	QString startKey, stopKey;

	(*list) = TOP;
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

  	progress.progressBar()->setProgress(0);
		KApplication::kApplication()->processEvents(); //do not lock the GUI!

		(*list)++;
	}

	//add all items to the queue
	if (progress.wasCancelled()) {
		itemList.setAutoDelete(true);
		itemList.clear();//delete all items
		return false;
	}

  printer()->appendItems(itemList);
	progress.progressBar()->setProgress(list->Count());

	return true;
};

const bool CExportManager::printKey( CSwordModuleInfo* module, const QString& startKey, const QString& stopKey, const QString& description ){
	printer()->appendItem( new CPrintItem(module, startKey, stopKey, description, m_displayOptions, m_filterOptions) );
	return true;
}

const bool CExportManager::printKey( CSwordKey* key, const QString& description ){
	printer()->appendItem( new CPrintItem(key->module(),key->key(), key->key(), description, m_displayOptions, m_filterOptions) );
	return true;
}

/** Prints a key using the hyperlink created by CReferenceManager. */
const bool CExportManager::printByHyperlink( const QString& hyperlink ){
 	QString moduleName;
  QString keyName;
  CReferenceManager::Type type;

  CReferenceManager::decodeHyperlink(hyperlink, moduleName, keyName, type);
  if (moduleName.isEmpty()) {
  	moduleName = CReferenceManager::preferredModule(type);
	}

 	if (CSwordModuleInfo* module = backend()->findModuleByName(moduleName)) {
//    qWarning(keyName.latin1());
    QString startKey = keyName;
    QString stopKey = keyName;

    //check if we have a range of entries or a single one
    if (module->type() == CSwordModuleInfo::Bible || module->type() == CSwordModuleInfo::Commentary) {
      ListKey verses = VerseKey().ParseVerseList((const char*)keyName.local8Bit(), "Genesis 1:1", true);
    	for (int i = 0; i < verses.Count(); ++i) {
    		VerseKey* element = dynamic_cast<VerseKey*>(verses.GetElement(i));
    		if (element)
        	CExportManager::printKey(module,QString::fromLocal8Bit((const char*)element->LowerBound()), QString::fromLocal8Bit((const char*)element->UpperBound()) );
    		else
					CExportManager::printKey(module,(const char*)*verses.GetElement(i),(const char*)*verses.GetElement(i));
    	}
		}
  	else {
			CExportManager::printKey(module,keyName,keyName);
    }
	}
  return true;
}

/** Returns the string for the filedialogs to show the correct files. */
const QString CExportManager::filterString( const Format format ){
  switch (format) {
    case HTML:
      return i18n("*.html *.htm | HTML files\n *.* | All files (*.*)");
    case Text:
      return i18n("*.txt | Text files\n *.* | All files (*.*)");
    default:
      return i18n("All files (*.*)");
  };
}

/** Returns a filename to save a file. */
const QString CExportManager::getSaveFileName(const Format format){
  return KFileDialog::getSaveFileName(QString::null, filterString(format), 0, m_caption);
}

/** Returns a string containing the linebreak for the current format. */
const QString CExportManager::lineBreak(const Format format){
  if ((bool)m_displayOptions.lineBreaks)
    return (format == HTML) ? QString::fromLatin1("<BR>\n") : QString::fromLatin1("\n");
  else
    return QString::null;
}

/** Returns the CSS string used in HTML pages. */
const QString CExportManager::htmlCSS(CSwordModuleInfo* module){
  CEntryDisplay* display = module ? module->getDisplay() : 0;
  if (!display)
    return QString::null;

  QString css = QString::null;
  for (int i = CEntryDisplay::MinType; i <= CEntryDisplay::MaxType; ++i) {
    CEntryDisplay::StyleType type = static_cast<CEntryDisplay::StyleType>(i);
    if (type != CEntryDisplay::Body && type != CEntryDisplay::Background) {
      css += display->cssString( type );
    }
  }
  return css;
}
