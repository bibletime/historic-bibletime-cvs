/********* Read the file LICENSE for license details. *********/#ifndef KSTARTUPLOGO_H
#define KSTARTUPLOGO_H

#include <qwidget.h>

//forward declarations
class QLabel;


/**
	* This class provides a startuplogo.
  * @author The team of BibleTime
  */
class KStartupLogo : public QWidget {
public:
	static void createSplash();
	static void showSplash();
	static void hideSplash();
	static void deleteSplash();		
	static void setStatusMessage(const QString& message);
  /** Makes the splashscreen the toplevel window. */
  static void raiseSplash();
  /** Lowers the splash screen one window down, so it's possible to make it not to hide tip windows or other startup stuff. */
  static void lowerSplash();

private:
	KStartupLogo();
  void setText(const QString text);
  QLabel* textLabel;
	
	static KStartupLogo* startupLogo;
};

#endif






