/***************************************************************************
                          cstyle.h  -  description
                             -------------------
    begin                : Sun Aug 20 2000
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

//BibleTime includes
#include "cprinter.h"

//Qt includes
#include <qlistview.h>
#include <qlist.h>


#ifndef CSTYLE_H
#define CSTYLE_H

//forward class decalarations
class CStyleList;
class CStyle;

class QListViewItem;
class QListView;

typedef QPtrList<CStyle> StyleItemList;

/** The class containing the properties requiredfor prinitng styles.
  * @author The BibleTime team
  */
class CStyle {
public:

	enum StyleType {
		Header = 0, /** The header displayed over each printed item */
		Description = 1 /** The description, used in bookmarks for example */,
		ModuleText = 2 /** The real text of the entry or the range of verses */,
		Unknown /** Unknown for us, should not be used */
	};

	/**	
  * A class which contains methods to support a format for the CStyle class.
  *	@author The BibleTime team
  * @version $Id$
  */
  class Format {
  public:
    class Frame  {
    public:
    	Frame();
			Frame(const Frame& f);
      /**
      * Sets the color of this frame.
      */
      void setColor( const QColor& );
      /**
      * Returns the color of this frame.
      */
      const QColor& color() const;
      /**
      * Sets the style how to paint the lines.
      */
      void setLineStyle( const Qt::PenStyle );
    	/**
      * Returns the style how to paint the lines of this frame.
      */
      const Qt::PenStyle& lineStyle() const;
      /**
      * Sets te thickness of this frame.
      */
      void setThickness( const int );
      /**
      * Returns the thickness of this frame.
      */
      const int& thickness() const;

    private:
    	QColor m_color;
    	Qt::PenStyle m_lineStyle;
    	int m_thickness;	
    };

  	enum Alignment { Left, Center, Right, Justification};
  	enum Color { Background, Foreground };
    	
  	Format(const CStyle::StyleType type = CStyle::Unknown);
  	Format(const Format& f);
  	~Format();
    const CStyle::StyleType type() const;
    /**
   	* Returns the foreground color of this format.
   	*/
  	const QColor& color( const Color type ) const;
    /**
   	* Sets the background color of this format.
   	*/
    void setColor( const Color type,  const QColor& );
    /**
    * sets the font of this format.
    */
    void setFont( QFont );
    /**
    * Returns the font of this style.
    */
    const QFont& font() const;
    /**
   	* Sets the alignement flags of this style format.
   	*/
    void setAlignment( const CStyle::Format::Alignment );
    /**
   	* Returns the alignement of this style format.
   	*/
    const CStyle::Format::Alignment& alignment() const;
    /**
   	* Sets the frame of this style.
   	*/
    void setFrameEnabled( const bool hasFrame );
    /**
   	* Returns the frame if we have one. Otherwise we return 0.
   	*/
    Frame* const frame();

  private:
    void clearData();
      	
  	QFont m_font;
  	QColor m_FGColor;
  	QColor m_BGColor;
  	bool m_hasFrame;
  	Frame* m_frame;
  	Alignment m_alignment;
   	CStyle::StyleType m_type;  	
  };
	
	CStyle(); //standard constructor, doesn't load from file
	virtual ~CStyle();
  /**
 	* Returns the proper CStyleFormat for the given type.
 	*/
  CStyle::Format* const formatForType( const CStyle::StyleType );
  /**
 	* Set the printing of the header (true enables it).
 	*/
  void setFormatTypeEnabled( const CStyle::StyleType, const bool );
  /**
 	* Returns true if we have the given type enabled.
 	*/
  const bool hasFormatTypeEnabled( const CStyle::StyleType ) const;
  /**
 	* Returns a QListViewItem for inserted in list.
 	*/
  QListViewItem* const listViewItem( CStyleList* const list = 0 );
  /**
 	* Sets the name of the style.
 	*/
  void setStyleName( const QString name );
  /**
 	* Returns the style name
 	*/
  const QString& styleName() const;
  /**
 	* Updates the Listview items
 	*/
  void updateListViewItem();
  /**
 	* Deletes the list view item.
 	*/
  void deleteListViewItem();
  /**
  * Loads the profile from a XML file.
  */
  const bool loadFromText(const QString& text);
  const bool loadFromFile(const QString& file);
  /**
  * Saves the style to the XML file.
  */
  const bool saveToFile( const QString& file );

protected:
	friend class CPrinter;
	CStyle( const QString filename );

private:
  /**
 	* Clears all variables and sets them back
 	*/
  void clearData();
	
	QString	m_name;
	
  Format* m_headerFormat;
	Format* m_descriptionFormat;		
	Format* m_moduleTextFormat;
	
	bool m_isHeaderFormatEnabled;
	bool m_isModuleTextFormatEnabled;
	bool m_isDescriptionFormatEnabled;
	
	QListViewItem*	m_listViewItem;
};

/** Set the printing of the header (true enables it). */
inline const bool CStyle::hasFormatTypeEnabled( const CStyle::StyleType type) const {
	switch (type) {
		case Header:
			return m_isHeaderFormatEnabled;
		case Description:
			return m_isDescriptionFormatEnabled;
		case ModuleText:
			return m_isModuleTextFormatEnabled;
		default:
			return false;
	}
}

#endif
