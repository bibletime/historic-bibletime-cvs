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

//#include <qwidget.h>
//#include <qobject.h>
#include <qlistview.h>
#include <qlist.h>

#include "cprinter.h"


#ifndef CSTYLE_H
#define CSTYLE_H

//forward class decalarations
class CStyleList;
class CStyle;

class QListViewItem;
class QListView;

typedef QList<CStyle> StyleItemList;

/** The class containing the properties requiredfor prinitng styles.
  * @author The BibleTime team
  */
class CStyle {
public:
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
      void setThickness( const unsigned short int );
      /**
      * Returns the thickness of this frame.
      */
      const unsigned short int& thickness() const;

    protected:
    	QColor m_color;
    	Qt::PenStyle m_lineStyle;
    	unsigned short int m_thickness;	
    };

  	enum Alignement { Left, Center, Right, Justification};
  	enum Color { Background, Foreground };
    	
  	Format();
  	~Format();

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
    void setAlignement( const CStyle::Format::Alignement );
    /**
   	* Returns the alignement of this style format.
   	*/
    const CStyle::Format::Alignement& alignement() const;
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
  	CStyle::Format::Alignement m_alignement;
  };

	enum styleType { Header=0, /** The header displayed over each printed item */
		Description=1 /** The description, used in bookmarks for example */,
		ModuleText=2 /** The real text of the entry or the range of verses */,
		Unknown /** Unknown for us, should not be used */
	};
	
	CStyle();
	virtual ~CStyle();
  /**
 	* Returns the proper CStyleFormat for the given type.
 	*/
  CStyle::Format* formatForType( const CStyle::styleType ) const;
  /**
 	* Sets the format for the given type.
 	*/
  void setFormatForType( const CStyle::styleType type, CStyle::Format* format);
  /**
 	* Set the printing of the header (true enables it).
 	*/
  void setFormatTypeEnabled( const CStyle::styleType, const bool );
  /**
 	* Returns true if we have the given type enabled.
 	*/
  const bool hasFormatTypeEnabled( const CStyle::styleType ) const;
  /**
 	* Returns a QListViewItem for inserted in list.
 	*/
  QListViewItem* listViewItem( CStyleList* list = 0 );
  /**
 	* Sets the name of the style.
 	*/
  void setStyleName( const QString name);
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

private:
  /**
 	* Clears all variables and sets them back
 	*/
  void clearData();
	
	QString	m_styleName;
	CStyle::Format*	m_headerFormat;
	CStyle::Format*	m_moduleTextFormat;
	CStyle::Format*	m_descriptionFormat;	
	
	bool m_isHeaderFormatEnabled;
	bool m_isModuleTextFormatEnabled;
	bool m_isDescriptionFormatEnabled;
	
	QListViewItem*	m_listViewItem;
};

#endif
