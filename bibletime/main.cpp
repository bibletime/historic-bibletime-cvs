/***************************************************************************
                          main.cpp  -  The file containing main ;-)
                             -------------------
    begin                : Mit Jan 19 12:57:15 CET 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
 ***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

	
/* $Header$ */
/* $Revision$ */

//own includes
#include "ressource.h"
#include "bibletime.h"
#include "frontend/kstartuplogo.h"
#include "frontend/chtmldialog.h"
#include "frontend/cbtconfig.h"
#include "config.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


//KDE includes
#include <kcmdlineargs.h>
#include <kcrash.h>
#include <kglobal.h>
#include <kapp.h>
#include <kstddirs.h>
#include <kinstance.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kiconloader.h>

#include <stdio.h>
#include <stdlib.h>

//Qt includes
#include <qfont.h>

bool showDebugMessages = false;
BibleTime* bibletime = 0;

void myMessageOutput( QtMsgType type, const char *msg ) {	
//we use this messagehandler to switch debugging off in final releases
	switch ( type ) {
		case QtDebugMsg:
		 	if (showDebugMessages)	//only show messages if they are enabled!
		 		fprintf( stderr,"(BibleTime %s) Debug: %s\n",VERSION, msg );
			break;
		case QtWarningMsg:
			fprintf( stderr,"(BibleTime %s) WARNING: %s\n",VERSION, msg );
			break;
		case QtFatalMsg:
			fprintf( stderr,"(BibleTime %s) _FATAL_: %s\nPlease contact info@bibletime.de and report this bug!",VERSION, msg );
			abort();	// dump core on purpose
	}
}

extern "C" {
  static void setSignalHandler(void (*handler)(int));

  // Crash recovery signal handler
  static void signalHandler(int sigId) {
    setSignalHandler(SIG_DFL);
    fprintf(stderr, "*** BibleTime got signal %d (Exiting)\n", sigId);
    // try to cleanup all windows
  	if (bibletime) {
  		bibletime->saveSettings();
  	  fprintf(stderr, "Saving seems to be succesful\n");		
  	}
    ::exit(-1); //
  }

  // Crash recovery signal handler
  static void crashHandler(int sigId) {
    setSignalHandler(SIG_DFL);
    fprintf(stderr, "*** BibleTime got signal %d (Crashing). Trying to save settings.\n", sigId);
  	if (bibletime) {
  		bibletime->saveSettings();
  	  fprintf(stderr, "Saving seemed to be succesful\n");		
  	}
    // Return to DrKonqi.
  }

  static void setSignalHandler(void (*handler)(int)) {
    signal(SIGKILL, handler);
    signal(SIGTERM, handler);
    signal(SIGHUP,  handler);
    KCrash::setEmergencySaveFunction(crashHandler);
  }
}

int main(int argc, char* argv[]) {
	qInstallMsgHandler( myMessageOutput );
	
	//create about data for this application
	static KCmdLineOptions options[] =
	{
		{"debug", I18N_NOOP("Enable debug messages"),0},
		{0,0,0}
	};	
		
	KAboutData aboutData(
		PACKAGE,
		"BibleTime",
		VERSION,
		I18N_NOOP("Bible study tool for KDE 2"),
		KAboutData::License_GPL_V2,
		I18N_NOOP("(c)1999-2001, The BibleTime Team"),
		I18N_NOOP("BibleTime is an easy to use but powerful Bible study tool for KDE 2.\n\nWe are looking for translators and handbook authors.\nIf you'd like to join use send an eMail to info@bibletime.de."),
		"http://www.bibletime.de/",
		"info@bibletime.de"
	);
	
	//coders
	aboutData.addAuthor("Joachim Ansorg", I18N_NOOP("Project coordinator, frontend, backend"), "jansorg@gmx.de","");
	aboutData.addAuthor("Martin Gruner", 	I18N_NOOP("Frontend, backend"), "mg.pub@gmx.net", "");
//	aboutData.addAuthor("Chris Kujawa", 	I18N_NOOP("Frontend"),"christopher.kujawa@verizon.net", "");
//	aboutData.addAuthor("Luke Mauldin", 	I18N_NOOP("Frontend"),"lukeskyfly@txk.net", "");
//	aboutData.addAuthor("Tim Brodie", 	I18N_NOOP("Installation manager"),"tbrodie@displayworksinc.com", "");

	//documentation
	aboutData.addAuthor("Fred Saalbach", 	I18N_NOOP("Handbook"), "saalbach@sybercom.net", "");		
		
	//translators
	aboutData.addAuthor("Silvio Bacchetta",		I18N_NOOP("Translation into Italian"), "sorgilazzaro@tiscalinet.it", "");							
	aboutData.addAuthor("Nuno Bareto", 				I18N_NOOP("Translation into Portoguese"), "nbarr@clix.pt", "");	
	aboutData.addAuthor("Mario Bertrand", 	  I18N_NOOP("Translation into French"), "mbert@tbrq.org", "");			
	aboutData.addAuthor("Benedykt P. Barszcz",I18N_NOOP("Translation into Polish"), "kb2qzv@box43.gnet.pl", "");		
	aboutData.addAuthor("Birger Langkjer", 		I18N_NOOP("Translation into Danish"), "birger.langkjer@image.dk", "");				
	aboutData.addAuthor("Zdeno Podobny", 			I18N_NOOP("Translation into Slovak"), "zdpo@post.sk", "");	
	aboutData.addAuthor("Michal Rovnaník",		I18N_NOOP("Translation into Czech"), "Michal.Rovnanik@seznam.cz", "");			
	aboutData.addAuthor("Eduardo Sanchez", 		I18N_NOOP("Translation into Spanish"), "csanche2@calvin.edu", "");			
	aboutData.addAuthor("Beda Szukics",				I18N_NOOP("Translation into Italian"), "bszukics@bluewin.ch", "");								
	aboutData.addAuthor("Balint Sandor", 			I18N_NOOP("Translation into Hungarian"), "balintsa@freemail.hu", "");				
	aboutData.addAuthor("Espen Trydal", 			I18N_NOOP("Translation into Norwegian"), "etrydal@postkassa.no", "");		
//	aboutData.addAuthor("Jonathan Jones",			I18N_NOOP("Translation into Brazilian Portoguese"), "jones@brfree.com.br", "");					
//	aboutData.addAuthor("Walter Rodrigo de Sá Cruz",			I18N_NOOP("Translation into Portoguese"), "keytech@ig.com.br", "");		
	aboutData.addAuthor("Kees van Veen", 			I18N_NOOP("Translation into Dutch"), "cvn@interchain.nl", "");	
															
	//credits
	aboutData.addCredit("Bob Harman", 	I18N_NOOP("Bible study HowTo"), "N_Cov_Church@compuserve.com", "");		
//	aboutData.addCredit("Darwin Gregory", I18N_NOOP("Optionsdialog"), "darwin@ichristian.com", "");		
//	aboutData.addCredit("Jeffrey Hoyt", 	I18N_NOOP("handbook"), "jeffhoyt@earthlink.net", "http://jhoyt.faithweb.com");	
//	aboutData.addCredit("Juho Vahakangas", I18N_NOOP("Martin Luther startup logo"), "juhov@freenet.fi", "");
//	aboutData.addCredit("Thomas Hagedorn", I18N_NOOP("Sponsor of www.bibletime.de"), "tom@delix.de", "");		
//	aboutData.addCredit("Torsten Uhlmann", I18N_NOOP("backend"), "TUhlmann@gmx.de", "http://tuhlmann.purespace.de");
//	aboutData.addCredit("Troy A. Griffits", I18N_NOOP("Leader of the SWORD project.\nLots of help with the SWORD API!"), "scribe@crosswire.org", "");	
 		 	
 	KCmdLineArgs::init(argc, argv, &aboutData); 	
 	KCmdLineArgs::addCmdLineOptions ( options );
 		
#ifdef STATIC_BUILD
 	KApplication app(false); //disable styles
//	qWarning("path == %s", argv[0]);
//	qWarning("$PWD == %s", getenv("PWD"));
//	qWarning("$BT_PATH == %s", getenv("BT_PATH"));
	//get the path
	KGlobal::dirs()->addPrefix(".");
	KGlobal::dirs()->addPrefix(getenv("PWD"));
	KGlobal::dirs()->addPrefix(getenv("BT_PATH"));
	
	QStringList list = KGlobal::dirs()->allTypes();
	for (unsigned int i = 0; i < list.count(); ++i) {
		KGlobal::dirs()->addResourceType((*list.at(i)).latin1(), QString("%1/share/%2/").arg(getenv("BT_PATH")).arg( *list.at(i) ));
	}
#else
 	KApplication app;
#endif
	
 	KGlobal::dirs()->addResourceType("BT_pic", "share/apps/bibletime/pics/");

	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	// A binary option (on / off)
	if (args->isSet("debug"))
		showDebugMessages = true;
 		
 	if (app.isRestored()) {
		RESTORE(BibleTime)
	}
  else {
		bool showIt = false;	
	  {
			showIt = CBTConfig::get(CBTConfig::logo);	
	
			if(showIt) {
				KStartupLogo::createSplash();
				KStartupLogo::showSplash();				
				KStartupLogo::setStatusMessage( i18n("Starting BibleTime")+QString::fromLatin1("...") );
			};
	  }
		
	  bibletime = new BibleTime();
	
	  if (showIt) {
		  KStartupLogo::deleteSplash();	  		  	
	  	KStartupLogo::hideSplash();
		}

	  			
		//first startup of BibleTime?		
		{
#warning implement reaction to new BT version that was installed
			if (CBTConfig::get(CBTConfig::firstStartUp)) {
				CBTConfig::set(CBTConfig::firstStartUp, false);
				HTML_DIALOG(HELPDIALOG_FIRST_START);
			}			
		}			
		{
			if (!CBTConfig::get(CBTConfig::isConfigured)) {
				CBTConfig::set( CBTConfig::isConfigured, true);
				bibletime->slotSettingsOptions();
			}
		}		
		{ //The tip of the day
			if (CBTConfig::get(CBTConfig::tips))
				bibletime->slotHelpTipOfDay();
		}		
		bibletime->show();							
		{
		 	if (CBTConfig::get(CBTConfig::restoreWorkspace))
		 		bibletime->restoreWorkspace();
		}
	  setSignalHandler(signalHandler);		
  	return app.exec();
	}
}

