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
#include <qfont.h>

//Sword includes
#include <swdisp.h>
#include <swmodule.h>


class CHTMLChapterDisplay;
class CSwordModuleInfo;
/**	
	* The display class for entry based modules.
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
  * Renders the current entry of the module using HTML tags.
  * The rendered text is cached so it's faster if it's called for the
  * same key next time.
  */
  virtual char Display(CSwordModuleInfo* module);
  /**
  * This function returns the last looked up HTML text.
  */
  const QString getHTML() const;  	
  /**
  * Sets the standard font fot this display object.
  */
  void setStandardFont( const QString& family, const short int logicalSize );
  /**
  * Call this with false as argument to switch off the headers (module anme and current position)
  */
  void setIncludeHeader( const bool includeHeader );
  /**
  * Generates code to display the given modules side by side.
  */
  virtual char Display( QList<CSwordModuleInfo>* moduleList);

protected:
	bool 		m_useLineBreaks;
	bool 		m_includeHeader;

	QString m_htmlText;
	QString m_htmlHeader;
	QString m_htmlBody;

	QString m_highlightedVerseColorName;
	QString m_standardFontColorName;

  QString m_standardFontName;
	QString m_unicodeFontName;
  int 		m_standardFontSize;
  int 		m_unicodeFontSize;

	void updateSettings(void);

};

/** Returns the generated HTML text. */
inline const QString CHTMLEntryDisplay::getHTML() const {
	return m_htmlText;
}

/** Call this with false as argument to switch off the headers (module anme and current position) */
inline void CHTMLEntryDisplay::setIncludeHeader( const bool includeHeader ){
	m_includeHeader = includeHeader;
}

#endif
