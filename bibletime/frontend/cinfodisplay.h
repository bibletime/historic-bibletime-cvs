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

#include <qwidget.h>
#include <qvaluelist.h>
#include <qpair.h>

class KHTMLPart;
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
	const QString decodeLemma( const QString& data );
	const QString decodeMorph( const QString& data );
	const QString getWordTranslation( const QString& data );
 
private:
	KHTMLPart* m_htmlPart;
};

#endif
