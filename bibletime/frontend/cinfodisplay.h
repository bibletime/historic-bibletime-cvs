//
// C++ Interface: cinfodisplay
//
// Description:
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef CINFODISPLAY_H
#define CINFODISPLAY_H

//Backend
#include  "backend/ctextrendering.h"

//Qt includes
#include <qwidget.h>
#include <qvaluelist.h>
#include <qpair.h>

//class forward declarations
class CReadDisplay;

/**
@author The BibleTime team
*/
class CInfoDisplay : public QWidget
{
  Q_OBJECT
public:
	enum InfoType {
		CrossReference,
		Footnote,
		Lemma,
		Morph,
		WordTranslation,
		WordGloss
	};

	typedef QPair<InfoType, QString> InfoData;
	typedef QValueList<InfoData> ListInfoData;
		
  CInfoDisplay(QWidget *parent = 0, const char *name = 0);
  virtual ~CInfoDisplay();
  
	void setInfo(const InfoType, const QString& data);
	void setInfo(const ListInfoData&);
	void clearInfo();

protected:
  const QString decodeCrossReference( const QString& data );
  const QString decodeFootnote( const QString& data );
	const QString decodeStrongs( const QString& data );
	const QString decodeMorph( const QString& data );
	const QString getWordTranslation( const QString& data );
 
private:
	CReadDisplay* m_htmlPart;
	
	class CrossRefRendering : public CHTMLExportRendering {
	public:
		CrossRefRendering( CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(), CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults() );
		
	protected:	
		virtual const QString entryLink( const KeyTreeItem& item, CSwordModuleInfo*  module );
		virtual const QString finishText( const QString&, KeyTree& tree );
	};
};

#endif
