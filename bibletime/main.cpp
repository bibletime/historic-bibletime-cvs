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

//standard includes
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

//own includes
#include "resource.h"
#include "bibletime.h"
#include "config.h"

//frontend includes
#include "frontend/kstartuplogo.h"
#include "frontend/chtmldialog.h"
#include "frontend/cbtconfig.h"

//KDE includes
#include <kcmdlineargs.h>
#include <kcrash.h>
#include <kglobal.h>
#include <kapplication.h>
#include <kstandarddirs.h>
#include <kinstance.h>
#include <kaboutdata.h>
#include <klocale.h>

#include <kmessagebox.h>

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
    if (CBTConfig::get(CBTConfig::crashedLastTime)) {
      //crashed this time and the execution before this one, probably a bug which occurs every time
      CBTConfig::set(CBTConfig::crashedTwoTimes, true);
    }
    else {
      //try to restore next time.
      CBTConfig::set(CBTConfig::crashedLastTime, true);
    }
    if (bibletime) {
      bibletime->saveSettings();
			fprintf(stderr, "*** Saving seemed to be succesful. If restoring does not work on next startup \
please use the option --ignore-startprofile\n");
		}
		::exit(-1); //exit BibleTime
	}

	// Crash recovery signal handler
	static void crashHandler(int sigId) {
		setSignalHandler(SIG_DFL);
		fprintf(stderr, "*** BibleTime got signal %d (Crashing). Trying to save settings.\n", sigId);
    if (CBTConfig::get(CBTConfig::crashedLastTime)) {
      //crashed this time and the execution before this one, probably a bug which occurs every time
      CBTConfig::set(CBTConfig::crashedTwoTimes, true);
    }
    else {
      //try to restore next time.
      CBTConfig::set(CBTConfig::crashedLastTime, true);
    }
    if (bibletime) {
			bibletime->saveSettings();
			fprintf(stderr, "*** Saving seemed to be succesful. If restoring does not work on next startup \
please use the option --ignore-startprofile\n");		
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
		{"ignore-startprofile", I18N_NOOP("Ignore the startup profile saved after the last BibleTime execution."),0},
		{"open-default-bible <key>", I18N_NOOP("Opens the default bible with the given key. Use <random> to open ata random position."),0},
		{0,0,0}
	};	
		
	KAboutData aboutData(
		PACKAGE,
		"BibleTime",
		VERSION,
		I18N_NOOP("Bible study tool for KDE"),
		KAboutData::License_GPL_V2,
		I18N_NOOP("(c)1999-2002, The BibleTime Team"),
		I18N_NOOP("BibleTime is an easy to use but powerful Bible study tool for KDE.\n\nWe are looking for developers and translators.\nIf you'd like to join use send an eMail to info@bibletime.de."),
		"http://www.bibletime.de/",
		"info@bibletime.de"
	);
	
	//we could so change this to a file reading function (including commented out people
	//coders
	// active developers (sorted by name)                                                                                     	
	aboutData.addAuthor("Joachim Ansorg", I18N_NOOP("Project coordinator"), "joachim@ansorgs.de", 		"");
	aboutData.addAuthor("Martin Gruner", 	I18N_NOOP("Frontend, backend"), 	"mg.pub@gmx.net", 				"");
  aboutData.addAuthor("Nikolay Igotti",    I18N_NOOP("Frontend"),         "olonho@hotmail.com",     "");
//	aboutData.addAuthor("Mark Lybarger", 	I18N_NOOP("Searchdialog"), 				"mlybarge@insight.rr.com","");
//	aboutData.addAuthor("Chris Kujawa", 	I18N_NOOP("Frontend"),"christopher.kujawa@verizon.net", "");
//	aboutData.addAuthor("Luke Mauldin", 	I18N_NOOP("Frontend"),"lukeskyfly@txk.net", "");
//	aboutData.addAuthor("Tim Brodie", 	I18N_NOOP("Installation manager"),"tbrodie@displayworksinc.com", "");

	//artwork (sorted by name)
	aboutData.addAuthor("Timothy R. Butler", I18N_NOOP("Icons, startlogo, banners"), "tbutler@uninetsolutions.com", "www.uninetsolutions.com");
		
	//documentation (sorted by name)
	aboutData.addAuthor("Fred Saalbach", I18N_NOOP("Handbook"), "saalbach@sybercom.net", "");		
	
	//translators (sorted by language)
	aboutData.addAuthor("Michal Rovnaník",		I18N_NOOP("Translation into Czech"), 			"Michal.Rovnanik@seznam.cz", 	"");	
	aboutData.addAuthor("Birger Langkjer", 		I18N_NOOP("Translation into Danish"), 		"birger.langkjer@image.dk", 	"");
	aboutData.addAuthor("Kees van Veen", 			I18N_NOOP("Translation into Dutch"), 			"cvn@interchain.nl", 					"");
//	aboutData.addAuthor("Mario Bertrand", 	  I18N_NOOP("Translation into French"), 		"mbert@tbrq.org", 						"");
	aboutData.addAuthor("Mathias Bavay", 	  	I18N_NOOP("Translation into French"), 		"mathias.bavay@free.fr", 			"");
	aboutData.addAuthor("Balint Sandor", 			I18N_NOOP("Translation into Hungarian"), 	"balintsa@freemail.hu", 			"");
//	aboutData.addAuthor("Beda Szukics",				I18N_NOOP("Translation into Italian"), 		"bszukics@bluewin.ch", 				"");
	aboutData.addAuthor("Silvio Bacchetta",		I18N_NOOP("Translation into Italian"), 		"sorgilazzaro@tiscalinet.it", "");
	aboutData.addAuthor("Espen Trydal", 			I18N_NOOP("Translation into Norwegian"), 	"etrydal@postkassa.no", 			"");
	aboutData.addAuthor("Benedykt P. Barszcz",I18N_NOOP("Translation into Polish"), 		"kb2qzv@box43.gnet.pl", 			"");	
	aboutData.addAuthor("Nuno Bareto", 				I18N_NOOP("Translation into Portoguese"), "nbarr@oninet.pt", 						"");	
//	aboutData.addAuthor("Yuriy Salimovskyy", 	I18N_NOOP("Translation into Russian"), 		"y2000@pisem.net",						"");	
	aboutData.addAuthor("Zdeno Podobny", 			I18N_NOOP("Translation into Slovak"), 		"zdpo@post.sk", 							"");	
	aboutData.addAuthor("Eduardo Sanchez", 		I18N_NOOP("Translation into Spanish"), 		"csanche2@calvin.edu", 				"");

	//credits (sorted by name)
	aboutData.addCredit("Bob Harman", I18N_NOOP("Bible study HowTo"), "N_Cov_Church@compuserve.com", "");
	 	
	KCmdLineArgs::init(argc, argv, &aboutData); 	
	KCmdLineArgs::addCmdLineOptions ( options );
	
  KApplication::disableAutoDcopRegistration();

	KApplication app;
	KGlobal::dirs()->addResourceType("BT_pic", "share/apps/bibletime/pics/");

	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

	// A binary option (on / off)
	if (args->isSet("debug"))
		showDebugMessages = true;

	if (kapp->isRestored()){
		for(int n = 1; KMainWindow::canBeRestored(n); n++)
			(new BibleTime)->restore(n);
 	}
	else {
		const bool showIt = CBTConfig::get(CBTConfig::logo);	

		if(showIt) {
			KStartupLogo::createSplash();
			KStartupLogo::showSplash();				
			KStartupLogo::setStatusMessage( i18n("Starting BibleTime") + QString::fromLatin1("...") );
		}
		
		bibletime = new BibleTime();
		app.setMainWidget(bibletime);

		if (showIt) {
			KStartupLogo::hideSplash();
			KStartupLogo::deleteSplash();	  		  	
		}

		// a new BibleTime version was installed (maybe a completely new installation)
		if (CBTConfig::get(CBTConfig::bibletimeVersion) != VERSION) {
			CBTConfig::set(CBTConfig::bibletimeVersion, VERSION);
			CHTMLDialog dlg(HELPDIALOG_FIRST_START);
			dlg.exec();
			bibletime->slotSettingsOptions();
		}			

		setSignalHandler(signalHandler);
        
		//The tip of the day
		if (CBTConfig::get(CBTConfig::tips))
			bibletime->slotHelpTipOfDay();
		bibletime->show();		

    // restore the workspace
    bibletime->processCommandline();

    		
		const int ret = app.exec();
		delete bibletime;
		CPointers::deleteBackend();
    //we can set this safely now because we close now (hopyfully without crash)
    CBTConfig::set(CBTConfig::crashedLastTime, false);
    CBTConfig::set(CBTConfig::crashedTwoTimes, false);    
		return ret;
	}
}

