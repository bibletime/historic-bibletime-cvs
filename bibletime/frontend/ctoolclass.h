/***************************************************************************
                 ctoolclass.h - some methods,that would be normal global, but
                  I hate global function :-)
                             -------------------                                         

    begin                : 20 Jan 1999
    copyright            : (C) 1999 by Joachim Ansorg
    email                : Jockel123@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

//BibleTime includes


//Qt includes
#include <qstring.h>

//KDE includes
#include <kapp.h>

#ifndef CTOOLCLASS_H
#define CTOOLCLASS_H

class CSwordModuleInfo;
/**
 * Provides some useful functions which would be normally global.
 *
 * Some methods,that would be normaly global, but I hate global functions :-)
 * (the function locateHTML is from Sandy Meier (KDevelop))
 *
 * @short A class which contains static members to do small things.
 * @author Joachim Ansorg <jansorg@gmx.de>
 */
class CToolClass {

public:
 /**
  * @return The path of the HTML file "filename". This function searches only in $KDEDIR.
  * @author Sandy Meier of the KDevelop team.
  */
  static QString locatehtml(const QString &filename);
 /**
  * Parses a unicode URL and gives back a SWORD key as QString.
  * For example it gets "Genesis%201:1" and returns "Genesis 1:1".
  * @return The key we got after parsing the URL
  * @author Joachim Ansorg
  */
 static QString urlToKey(const QString&);
 /**
  * Converts HTML text to plain text.
 	* This function converts some HTML tags in text (e.g. <BR> to \n)
 	* @return The text withput HTML tags and with converted <BR> to \n
 	* @author Joachim Ansorg 	
 	*/
  static QString htmlToText(const QString&);
  /**
   *Converts text to HTML converting some text commands into HTML tags (e.g. \n to <BR>)
   * @return The HTML formatted text we got after changing \n to <BR>
   * @author Joachim Ansorg
   */
  static QString textToHTML(const QString&);
  /**
   * Creates the file filename and put the text of parameter "text" into the file.
   * @return True if saving was sucessful, otherwise false
   * @author Joachim Ansorg
	 */
  static bool savePlainFile( const QString& filename, const QString& text);
  /**
   * Puts a module Name and a Reference together in the 'draggable' form
   * (module)reference
   * @param module The name of the module
   * @param reference The key reference as text
   * @return The encoded reference using module and reference
   * @author Martin Gruner
   */
  static QString encodeReference(QString &module, QString &reference);
  /**
   * decodes a 'draggable' reference into a modulename and a reference
   * @author Martin Gruner
   */
  static void decodeReference(QString &dragreference, QString &module, QString &reference);
  /**
  	* Returns the icon used for the module given as aparameter.
  	*/
  static QPixmap getIconForModule( CSwordModuleInfo* );
  /**
  	* Returns the logical font size which should
  	* be used for the absolute size s
  	*/
  static int makeLogicFontSize( const int s );
  /**
  * No descriptions
  */
  static const QString getSaveFileName( const QString& startDir = QString::null, const QString& filter = QString::null, QWidget* parent = 0, const QString& caption = QString::null);
  /**
  * No descriptions
  */
  static const QString getOpenFileName( const QString& startDir = QString::null, const QString& filter = QString::null, QWidget* parent = 0, const QString& caption = QString::null);

};

#endif
