/***************************************************************************
                          chtmlentrydisplay.h  -  description
                             -------------------
    begin                : Wed Oct 11 2000
    copyright            : (C) 2000 by The BibleTime team
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

#ifndef CHTMLENTRYDISPLAY_H
#define CHTMLENTRYDISPLAY_H

//BibleTime includes

//Qt includes
#include <qstring.h>
#include <qlist.h>
#include <qdict.h>
#include <qasciidict.h>
#include <qmap.h>
#include <qfont.h>

//Sword includes
#include <swdisp.h>
#include <swmodule.h>


class CHTMLChapterDisplay;
class CSwordModuleInfo;
/**	
	* The display class for entrie based modules.
  *	@author The BibleTime team
  * @version $Id$
  */
class CHTMLEntryDisplay : public SWDisplay  {
public:
	/**
	* The constructor of CHTMLEntryDisplay.
	* This is the constructor of CHTMLEntryDisplay. It initializes the members
	* and creates the objects we need to create the required HTML formatted text.
	*/
	CHTMLEntryDisplay();
	/**
	* The destructor of this display object
	*/
	virtual ~CHTMLEntryDisplay();
  /**
  * Renders the current entry of the module using HTML tags.
  * The rendered text is cached so it's faster if it's called for the
  * same key next time.
  */
  virtual char Display(CSwordModuleInfo* module);
  /**
  * This function returns the last looked up HTML text.
  */
  QString getHTML() const;  	
  /**
  * Sets the standard font fot this display object.
  */
  void setStandardFont( QString family, short int logicalSize );
  /**
  * Call this with false as argument to switch off the headers (module anme and current position)
  */
  void setIncludeHeader( const bool includeHeader );
  /**
  * Generates code to display the given modules side by side.
  */
  virtual char Display( QList<CSwordModuleInfo>* moduleList);
	/**
	* This QString variable does contain
	* the color of the current highlighted verse
	* formatted as HTML text (with a leading #)
	*/
  QString m_highlightedVerseColor;
	/**
	* This QString variable does contain
	* the textcolor formatted as HTML text (with a leading #)
	*/
	QString m_textColor;
	/**
	* This QString variable does contain
	* the textcolor formatted as HTML text (with a leading #)
	*/
	QString m_linkColor;

		
private:
	/**
	*
	*/
	friend class CHTMLChapterDisplay;	
	/**
	*
	*/
	QString m_htmlText;
	/**
	*
	*/
	QString m_htmlHeader;
	/**
	*
	*/
	QString m_htmlBody;
	/**
	*
	*/
	QString m_standardFontName;
	/**
	*
	*/
	short int m_standardFontSize;	
	/**
	* This bool is used to store the state of headers
	*/
	bool m_includeHeader;
};

#endif
