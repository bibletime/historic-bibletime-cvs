/***************************************************************************
                          cexportmanager.cpp  -  description
                             -------------------
    begin                : Mon Feb 25 2002
    copyright            : (C) 2002-2004 by The BibleTime team
    email                : info@bibletime.info
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
#include "backend/cdisplaytemplatemgr.h"

#include "printing/cprintitem.h"
#include "printing/cprinter.h"

#include "util/ctoolclass.h"
#include "util/scoped_resource.h"

//Qt includes
#include <qclipboard.h>
#include <qptrlist.h>
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
  m_progressDialog = 0;
};

const bool CExportManager::saveKey(CSwordKey* key, const Format format, const bool addText) {
  if (!key)
    return false;

  const QString filename = getSaveFileName(format);
  if (filename.isEmpty())
    return false;

  QString text = QString::null;
  bool hasBounds = false;
  if (addText) { //add the text of the key to the content of the file we save
    CPointers::backend()->setFilterOptions(m_filterOptions);
    CPointers::backend()->setDisplayOptions(m_displayOptions);

    CSwordModuleInfo* module = key->module();
  	if (CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(key) ) { //we can have a boundary
      if (vk->isBoundSet()) {//we have a valid boundary!
        hasBounds = true;
        CSwordVerseKey startKey(module);
        CSwordVerseKey stopKey(module);

    		startKey.key(vk->LowerBound());
    		stopKey.key(vk->UpperBound());

        QString entryText;

				//add the heading
        if (startKey < stopKey) { //we have a boundary
          QString bound = QString::fromLatin1("%1 - %2").arg(startKey.key()).arg(stopKey.key());
          text +=
            (format == HTML)
            ? QString::fromLatin1("<h3>%1</h3>").arg(bound)
            : QString::fromLatin1("%1\n\n").arg(bound);

         	while ( (startKey < stopKey) || (startKey == stopKey) ) {
            entryText = (format == HTML) ? startKey.renderedText(CSwordKey::HTMLEscaped) : startKey.strippedText();

         		text += ((bool)m_displayOptions.verseNumbers ? QString::fromLatin1("%1 ").arg(startKey.Verse()) : QString::null)
+ entryText + lineBreak(format);

            startKey.next(CSwordVerseKey::UseVerse);
          }
        }
        else {
          hasBounds = false;
        };
      }
  	}

    if (!hasBounds) { //no verse key, so we can't have a boundary!
    	if (format != HTML) {
					text = QString::fromLatin1("%1 (%2)\n\n%3") //plain text
            .arg(key->key())
            .arg(module->name())
            .arg(key->strippedText());
			}
      //we should only add the reference if the key has no bounds
      text +=
        lineBreak(format) +
        QString::fromLatin1("(%1, %1)")
            .arg(key->key())
            .arg(module->name());
    }

    if (format == HTML) {
			CDisplayTemplateMgr tMgr;
			text = tMgr.fillTemplate( CBTConfig::get(CBTConfig::displayStyle), QString::null, text );
    };
  }
  else { //don't add the text of the key
    text = key ? key->key() : QString::null;
  	return true;
  }

	CToolClass::savePlainFile(filename, text, false, (format==HTML) ? QTextStream::UnicodeUTF8 : QTextStream::Locale);
	return true;
};

const bool CExportManager::saveKeyList(sword::ListKey* list, CSwordModuleInfo* module, const Format format, const bool addText) {
  if (!list)
    return false;
  const QString filename = getSaveFileName(format);
  if (filename.isEmpty())
    return false;

  setProgressRange(list->Count());
 	util::scoped_ptr<CSwordKey> key(CSwordKey::createInstance(module));
 	QString text;
 	(*list) = sword::TOP;
 	while (!list->Error() && !progressWasCancelled()) {
 		if (!key)
 			break;
    key->key((const char*)(*list));

    if (addText) {
 			text += QString::fromLatin1("%1:%2\t%3\n").arg( key->key() ).arg(lineBreak(format)).arg( (format == HTML) ? key->renderedText(CSwordKey::HTMLEscaped) : key->strippedText() );
    }
 		else {
 			text += key->key() + lineBreak(format);
    }

    incProgress();
 		(*list)++;
 	}

  if (!progressWasCancelled()) {
 		CToolClass::savePlainFile(filename, text);
 		closeProgressDialog();
 		return true;
 	}
	return false;
};

const bool CExportManager::saveKeyList(QPtrList<CSwordKey> list, const Format format, const bool addText ) {
  if (!list.count())
    return false;

	const QString filename = getSaveFileName(format);
  if (filename.isEmpty())
    return false;

	QString text;
  setProgressRange(list.count());
  for (CSwordKey* k = list.first(); k && !progressWasCancelled(); k = list.next()) {
 		if (addText) {
 			text += QString::fromLatin1("%1:%2\t%3\n").arg( k->key() ).arg(lineBreak(format)).arg( (format == HTML) ? k->renderedText(CSwordKey::HTMLEscaped) : k->strippedText() );
		}
 		else {
 			text += k->key() + lineBreak(format);
		}
    incProgress();
  };

  if (!progressWasCancelled()) {
 		CToolClass::savePlainFile(filename, text);
 		closeProgressDialog();
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
//    text = key ? key->key() : QString::null;
    if (CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(key)) { //make sure VerseKeys are localized!
//      CSwordVerseKey vk( *key );
      vk->setLocale( backend()->booknameLanguage().latin1() );

      text = vk->key();      
    }
    else {
      text = key->key();
    }
  	return true;
  }
  KApplication::clipboard()->setText(text);
	return true;
};

const bool CExportManager::copyKeyList(sword::ListKey* list, CSwordModuleInfo* module, const Format format, const bool addText) {
  if (!list)
    return false;

  setProgressRange(list->Count());
 	util::scoped_ptr<CSwordKey> key(CSwordKey::createInstance(module));

 	QString text;
 	*list = sword::TOP;
 	while (!list->Error() && !progressWasCancelled()) {
 		if (!key)
 			break;

    key->key((const char*)(*list));
 		if (addText)
// 			text += QString::fromLatin1("%1:%2\t%3\n").arg( key->key() ).arg(lineBreak(format)).arg( (format == HTML) ? key->renderedText() : key->strippedText() );
 			text += QString::fromLatin1("%1\t%3\n").arg( key->key() ).arg( (format == HTML) ? key->renderedText() : key->strippedText() );

 		else
 			text += key->key() + lineBreak(format);
    incProgress();
 		(*list)++;
 	}

  if (!progressWasCancelled()) {
    KApplication::clipboard()->setText(text);
		closeProgressDialog();
   	return true;
 	}
	return false;
};


const bool CExportManager::copyKeyList(QPtrList<CSwordKey> list, const Format format, const bool addText ) {
  if (!list.count())
    return false;

  QString text;
  setProgressRange(list.count());
  for (CSwordKey* k = list.first(); k && !progressWasCancelled(); k = list.next()) {
 		if (addText)
// 			text += QString::fromLatin1("%1:%2\t%3\n").arg( k->key() ).arg(lineBreak(format)).arg( (format == HTML) ? k->renderedText() : k->strippedText() );
 			text += QString::fromLatin1("%1\t%3\n").arg( k->key() ).arg( (format == HTML) ? k->renderedText() : k->strippedText() );

 		else
 			text += k->key() + lineBreak(format);
    incProgress();
  };

  if (!progressWasCancelled()) {
    KApplication::clipboard()->setText(text);
 		closeProgressDialog();
 		return true;
 	}
  return false;
};

const bool CExportManager::printKeyList(sword::ListKey* list, CSwordModuleInfo* module) {
  setProgressRange(list->Count()+1);
	QPtrList<CPrintItem> itemList;
	QString startKey, stopKey;

	(*list) = sword::TOP;
	while (!list->Error() && !progressWasCancelled()) {
		sword::VerseKey* vk = dynamic_cast<sword::VerseKey*>(list);
		if (vk) {
			startKey = QString::fromLocal8Bit((const char*)(vk->LowerBound()) );
			stopKey = QString::fromLocal8Bit((const char*)(vk->UpperBound()) );
		}
		else {
			startKey = QString::fromLocal8Bit((const char*)*list);
			stopKey = QString::null;
		//add all items to the queue
	  }
    itemList.append( new CPrintItem(module, startKey, stopKey, QString::null, m_displayOptions, m_filterOptions) );
    incProgress();    
		(*list)++;
	}

	//add all items to the queue
	if (progressWasCancelled()) {
		itemList.setAutoDelete(true);
		itemList.clear();//delete all items
		return false;
	}

  printer()->appendItems(itemList);
  closeProgressDialog();    //close the dialog
	return true;
};

const bool CExportManager::printKeyList( const PrintItemList& list, CSwordModuleInfo* module ){
  if (!list.count() || !module)
    return false;
  setProgressRange(list.count()+1);
//  KApplication::kApplication()->processEvents(); //do not lock the GUI!
    
  PrintItemList::ConstIterator it;
  for ( it = list.begin(); (it != list.end()) && !progressWasCancelled(); ++it ) {
    printer()->appendItem( new CPrintItem(module,(*it).first,(*it).second, QString::null, m_displayOptions, m_filterOptions) );
//  	KApplication::kApplication()->processEvents(); //do not lock the GUI!    
  }
  
  closeProgressDialog(); //to close the dialog
	return true;
}

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
    QString startKey = keyName;
    QString stopKey = keyName;

    //check if we have a range of entries or a single one
    if (module->type() == CSwordModuleInfo::Bible || module->type() == CSwordModuleInfo::Commentary) {
     sword:: ListKey verses =sword:: VerseKey().ParseVerseList((const char*)keyName.local8Bit(), "Genesis 1:1", true);
    	for (int i = 0; i < verses.Count(); ++i) {
    		sword::VerseKey* element = dynamic_cast<sword::VerseKey*>(verses.GetElement(i));
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
  if (static_cast<bool>(m_displayOptions.lineBreaks))
    return (format == HTML) ? QString::fromLatin1("<BR>\n") : QString::fromLatin1("\n");
  else
    return QString::null;
}

/** No descriptions */
void CExportManager::setProgressRange( const int items ){
  if (QProgressDialog* dlg = progressDialog()) {
    dlg->setTotalSteps(items);
    dlg->setProgress(0);
  	dlg->setMinimumDuration(0);
    dlg->show();
    dlg->repaint();
  	KApplication::kApplication()->processEvents(); //do not lock the GUI!    
  }
}

/** Creates the progress dialog with the correct settings. */
QProgressDialog* const CExportManager::progressDialog(){
  if (!m_showProgress) {
    return 0;
  };
  if (!m_progressDialog) {
    m_progressDialog = new QProgressDialog( m_caption, m_progressLabel, 1, 0, "progress", true );
    m_progressDialog->setCaption("BibleTime");
  };
  return m_progressDialog;
}

/** Increments the progress by one item. */
void CExportManager::incProgress(){
  if (QProgressDialog* dlg = progressDialog()) {
    KApplication::kApplication()->processEvents(); //do not lock the GUI!
    dlg->setProgress( dlg->progress() + 1 );
    dlg->repaint();
    KApplication::kApplication()->processEvents(); //do not lock the GUI!
  }
}

/** No descriptions */
const bool CExportManager::progressWasCancelled(){
  if (QProgressDialog* dlg = progressDialog()) {
    return dlg->wasCancelled();
  };
  return true;
}

/** Closes the progress dialog immediatly. */
void CExportManager::closeProgressDialog(){
  if (QProgressDialog* dlg = progressDialog()) {
    dlg->repaint();
    dlg->close();
    dlg->reset();
  }
  KApplication::kApplication()->processEvents(); //do not lock the GUI!
}
