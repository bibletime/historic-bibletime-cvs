/***************************************************************************
                 ctoolclass.cpp - some methods,that would be normal global,but
                 									I hate global function :-)

    begin                : 20 Jan 1999                        
    copyright            : (C) 1999 by Sandy Meier                         
    email                : smeier@rz.uni-potsdam.de                         
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
#include "../ressource.h"
#include "../backend/cswordmoduleinfo.h"

//QT includes
#include <qfile.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qregexp.h>

//KDE includes
#include <klocale.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <kmessagebox.h>
#include <kurl.h>
#include <kfiledialog.h>

#include "ctoolclass.h"

QString CToolClass::locatehtml(const QString &filename) {
    QString path = locate("html", KGlobal::locale()->language() + '/' + filename);
    if (path.isNull())
       path = locate("html", "default/" + filename);
    if (path.isNull())
       path = locate("html", "en/" + filename);
    return path;
}

/** Parses a URL and gives back a key. */
QString CToolClass::urlToKey(const QString& url) {
	if (url.left(8) == "sword://")
		return url.mid( 8, url.length()-9 );
	return url;	
}

/** Converts HTML text to plain text */
QString CToolClass::htmlToText(const QString& html) {
  QString newText = html;
  // convert some tags we need in code
	newText.replace( QRegExp(" "),"#SPACE#" );
	newText.replace( QRegExp("<BR>\\s*"), "<BR>\n" );
 	newText.replace( QRegExp("#SPACE#")," " );
	return newText;	
}

/** Converts text to HTML (\n to <BR>) */
QString CToolClass::textToHTML(const QString& text){
	QString newText = text;
	newText.replace( QRegExp("<BR>\n"),"#NEWLINE#" );
	newText.replace( QRegExp("\n"),"<BR>\n" );
	newText.replace( QRegExp("#NEWLINE#"),"<BR>\n");	
	return newText;
}

/** Creates the file filename and put text into the file.
 */
bool CToolClass::savePlainFile( const QString& filename, const QString& text){
	QFile saveFile(filename);
	bool ret;
		
	if (saveFile.exists())
		if (KMessageBox::warningYesNo(0, i18n("<qt><B>The file does already exist!</B><BR>Do you want to overwrite it?</qt>"), i18n("BibleTime - Save file as ...")) == KMessageBox::No)
			return false;
		else
			saveFile.remove();
	
	if ( saveFile.open(IO_ReadWrite) ) {
		QTextStream textstream( &saveFile );
		textstream << text;
		saveFile.close();
		ret = true;
	}
	else {
		KMessageBox::error(0, "<qt>Sory! But the file couldn't be saved!<BR><B>Please check permissions etc. !</B></qt>");
		saveFile.close();		
		ret = false;		
	}	
	return ret;
}


/** Returns the icon used for the module given as aparameter. */
QPixmap CToolClass::getIconForModule( CSwordModuleInfo* module_info ){
  if (!module_info)
  	return QPixmap(BIBLE_ICON_SMALL);
	
 	switch (module_info->getType()){
 	  case CSwordModuleInfo::Bible:
 	    if (module_info->isLocked())
 	      return QPixmap(BIBLE_LOCKED_ICON_SMALL);
 	    else
 	      return QPixmap(BIBLE_ICON_SMALL);
 	  case CSwordModuleInfo::Lexicon:
 	    if (module_info->isLocked())
 	      return QPixmap(LEXICON_LOCKED_ICON_SMALL);
 	    else
 	      return QPixmap(LEXICON_ICON_SMALL);
 	  case CSwordModuleInfo::Commentary:
 	    if (module_info->isLocked())
 	      return QPixmap(COMMENTARY_LOCKED_ICON_SMALL);
 	    else
 	      return QPixmap(COMMENTARY_ICON_SMALL);
 	  case CSwordModuleInfo::Unknown:
 	  default:
 	    return QPixmap(BIBLE_ICON_SMALL);
 	}
}

int CToolClass::makeLogicFontSize( const int s ) {
	const int defSize = QApplication::font().pointSize();
  if ( s < defSize - 4 )
		return 1;
	if ( s < defSize )
		return 2;
	if ( s < defSize + 4 )
		return 3;
	if ( s < defSize + 8 )
		return 4;
	if ( s < defSize + 12 )
		return 5;
	if (s < defSize + 16 )
		return 6;
	return 7;
}

/** No descriptions */
const QString CToolClass::getSaveFileName( const QString& startDir, const QString& filter, QWidget* parent, const QString& caption){
#ifdef STATIC_BUILD
	return QFileDialog::getSaveFileName(startDir, filter,parent,"CToolClass::getSaveFileName",caption);
#else
	return KFileDialog::getSaveFileName(startDir, filter, parent, caption);
#endif
}

/** No descriptions */
const QString CToolClass::getOpenFileName( const QString& startDir, const QString& filter, QWidget* parent, const QString& caption){
#ifdef STATIC_BUILD
	return QFileDialog::getOpenFileName(startDir, filter, parent,"CToolClass::getOpenileName",caption);
#else
	return KFileDialog::getOpenFileName(startDir, filter, parent, caption);
#endif
}
