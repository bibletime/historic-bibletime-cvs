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

class KHTMLPart;
/**
@author The BibleTime team
*/
class CInfoDisplay : public QWidget
{
  Q_OBJECT
public:
	enum InfoType {
		Footnote = 0
	};
	
  CInfoDisplay(QWidget *parent = 0, const char *name = 0);
  virtual ~CInfoDisplay();
  void setInfo(InfoType, const QString& data);
		
private:
	KHTMLPart* m_htmlPart;
protected:
    const QString decodeFootnote( const QString& data );
};

#endif
