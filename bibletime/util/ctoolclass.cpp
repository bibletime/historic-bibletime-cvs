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
#include "ctoolclass.h"

#include "resource.h"
#include "backend/cswordmoduleinfo.h"

//QT includes
#include <qlabel.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qregexp.h>

//KDE includes
#include <klocale.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <kurl.h>
#include <kfiledialog.h>
#include <kapplication.h>


QString CToolClass::locatehtml(const QString &filename) {
  QString path = locate("html", KGlobal::locale()->language() + '/' + filename);
  if (path.isNull())
		path = locate("html", "default/" + filename);
  if (path.isNull())
		path = locate("html", "en/" + filename);
  return path;
}

/** Converts HTML text to plain text */
QString CToolClass::htmlToText(const QString& html) {
  QString newText = html;
  // convert some tags we need in code
	newText.replace( QRegExp(" "),"#SPACE#" );
	newText.replace( QRegExp("<BR>\\s*"), "<BR>\n" );
 	newText.replace( QRegExp("#SPACE#")," " );

  QRegExp re("<.+>");
  re.setMinimal(true);
  newText.replace( re,"" );
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
		
	if (saveFile.exists()) {
		if (KMessageBox::warningYesNo(0,
				QString::fromLatin1("<qt><B>%1</B><BR>%2</qt>")
					.arg( i18n("The file already exists!") )
					.arg( i18n("Do you want to overwrite it?")))
				== KMessageBox::No
			)
			return false;
		else
			saveFile.remove();
	}
	
	if ( saveFile.open(IO_ReadWrite) ) {
		QTextStream textstream( &saveFile );
		textstream << text;
		saveFile.close();
		ret = true;
	}
	else {
		KMessageBox::error(0, QString::fromLatin1("<qt>%1<BR><B>%2</B></qt>")
			.arg( i18n("Sorry! But the file couldn't be saved!") )
			.arg( i18n("Please check permissions etc. !")));
		saveFile.close();
		ret = false;		
	}	
	return ret;
}


/** Returns the icon used for the module given as aparameter. */
QPixmap CToolClass::getIconForModule( CSwordModuleInfo* module_info ){
  if (!module_info)
  	return QPixmap(BIBLE_ICON_SMALL);

  QPixmap img = QPixmap(BIBLE_ICON_SMALL);
  
 	switch (module_info->type()){
 	  case CSwordModuleInfo::Bible:
 	    if (module_info->isLocked())
 	      img = QPixmap(BIBLE_LOCKED_ICON_SMALL);
 	    else
 	      img = QPixmap(BIBLE_ICON_SMALL);
 	
 	  case CSwordModuleInfo::Lexicon:
 	    if (module_info->isLocked())
 	      img =  QPixmap(LEXICON_LOCKED_ICON_SMALL);
 	    else
 	      img= QPixmap(LEXICON_ICON_SMALL);
 	
 	  case CSwordModuleInfo::Commentary:
 	    if (module_info->isLocked())
 	      img = QPixmap(COMMENTARY_LOCKED_ICON_SMALL);
 	    else
 	      img = QPixmap(COMMENTARY_ICON_SMALL);
 	
 	  case CSwordModuleInfo::GenericBook:
 	    if (module_info->isLocked())
 	      img = QPixmap(BOOK_LOCKED_ICON_SMALL);
 	    else
 	      img = QPixmap(BOOK_ICON_SMALL); 	
 	
 	  case CSwordModuleInfo::Unknown: //fall though to default
 	  default:
 	    img = QPixmap(BIBLE_ICON_SMALL);
 	}

  if (module_info->category() == CSwordModuleInfo::Cult) {
    return SmallIcon("stop.png", 16);
  };
  
  return img;
}

QLabel* CToolClass::explanationLabel(QWidget* parent, const QString& heading, const QString& text ){
  QLabel* label = new QLabel( QString::fromLatin1("<B>%1</B><BR>%2").arg(heading).arg(text),parent );
  label->setAutoResize(true);
  label->setMargin(1);
  label->setFrameStyle(QFrame::Box | QFrame::Plain);
  return label;
}
/** No descriptions */
bool CToolClass::inHTMLTag(int pos, QString & text){
 int i1=text.findRev("<",pos);
 int i2=text.findRev(">",pos);
 int i3=text.find(">",pos);
 int i4=text.find("<",pos);


//	if ((i1>0) && (i2==-1))  //we're in th first html tag
//		i2=i1; // not ncessary, just for explanation

	if ((i3>0) && (i4==-1))  //we're in the last html tag
	  i4=i3+1;

//  qWarning("%d > %d && %d < %d",i1,i2,i3,i4);

 if ( (i1>i2) && (i3<i4) )
    return true; //yes, we're in a tag

  return false;
}
