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

//BibleTime includes
#include "btsetupwizard.h"

#include "frontend/kstartuplogo.h"
#include "frontend/cbtconfig.h"
	
//standard includes
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

//KDE includes
#include <kcmdlineargs.h>
#include <kcrash.h>
#include <kglobal.h>
#include <kapplication.h>
#include <kstandarddirs.h>
#include <kinstance.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kmessagebox.h>

#define PACKAGE "setupwizard"
#define VERSION "0.1"


bool showDebugMessages = false;
BTSetupWizard* wizard = 0;

void myMessageOutput( QtMsgType type, const char *msg ) {	
	//we use this messagehandler to switch debugging off in final releases
	switch ( type ) {
		case QtDebugMsg:
			if (showDebugMessages) //only show messages if they are enabled!
				fprintf( stderr,"(SetupWizard %s) Debug: %s\n",VERSION, msg );
			break;
		case QtWarningMsg:
				fprintf( stderr,"(SetupWizard %s) WARNING: %s\n",VERSION, msg );
			break;
		case QtFatalMsg:
			fprintf( stderr,"(SetupWizard %s) _FATAL_: %s\nPlease contact info@bibletime.de and report this bug!",VERSION, msg );
			abort();	// dump core on purpose
	}
}

extern "C" {
	static void setSignalHandler(void (*handler)(int));

	// Crash recovery signal handler
	static void signalHandler(int sigId) {
		setSignalHandler(SIG_DFL);
		fprintf(stderr, "*** BTSetupWizard got signal %d (Exiting)\n", sigId);
		// try to cleanup all windows
		if (wizard) {
//			wizard->saveSettings();
			fprintf(stderr, "*** Saving seemed to be succesful.\n");
		}
		::exit(-1); //exit BibleTime
	}

	// Crash recovery signal handler
	static void crashHandler(int sigId) {
		setSignalHandler(SIG_DFL);
		fprintf(stderr, "*** BTSetupWizard got signal %d (Crashing). Trying to save settings.\n", sigId);
		if (wizard) {
//			bibletime->saveSettings();
			fprintf(stderr, "*** Saving seemed to be succesful.\n");		
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
		{"start-bibletime", I18N_NOOP("Start BibleTime after SetupWizard exits"),0},
		{0,0,0}
	};	
		
	KAboutData aboutData (
		"setupwizard",
		"BibleTime SetupWizard",
		VERSION,
		I18N_NOOP("Setup wizard for BibleTime and Sword"),
		KAboutData::License_GPL_V2,
		I18N_NOOP("(c)2002, The BibleTime Team"),
		I18N_NOOP("BibleTime Setup Wizard is a tool to quickly setup/download/install the modules of Sword."),
		"http://www.bibletime.de/",
		"info@bibletime.de"
	);
	
	// active developers (sorted by name)                                                                                     	
	aboutData.addAuthor("Joachim Ansorg", I18N_NOOP("Project coordinator"), "jansorg@gmx.de",					"");
	aboutData.addAuthor("Martin Gruner", I18N_NOOP("Developer"), "mg.pub@gmx.net",					"");

	KCmdLineArgs::init(argc, argv, &aboutData); 	
	KCmdLineArgs::addCmdLineOptions ( options );	
  KApplication::disableAutoDcopRegistration();

	KApplication app;
	KGlobal::dirs()->addResourceType("BT_pic", "share/apps/bibletime/pics/");

	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

	// A binary option (on / off)
	if (args->isSet("debug"))
		showDebugMessages = true;

	if (app.isRestored()) {
		for(int n = 1; KMainWindow::canBeRestored(n); n++)
			(new BTSetupWizard)->restore(n);
 	}
	else {
		const bool showIt = CBTConfig::get(CBTConfig::logo);	
		if (showIt) {
			KStartupLogo::createSplash();
			KStartupLogo::showSplash();				
			KStartupLogo::setStatusMessage( i18n("Starting BibleTime Setup Wizard") + QString::fromLatin1("...") );
		}
		
		wizard = new BTSetupWizard();
//		app.setMainWidget(wizard);
		if (showIt) {
			KStartupLogo::hideSplash();
			KStartupLogo::deleteSplash();	  		  	
		}
		wizard->show();
		
		setSignalHandler(signalHandler);		
		
		return app.exec();
	}
}
