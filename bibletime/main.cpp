/********* Read the file LICENSE for license details. *********/

/* $Header$ */
/* $Revision$ */

//standard includes
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

//own includes
#include "bibletimeapp.h"
#include "bibletime.h"
#include "config.h"

//util includes
#include "util/scoped_resource.h"
#include "util/cresmgr.h"

//frontend includes
#include "frontend/kstartuplogo.h"
#include "frontend/cbtconfig.h"

//Qt includes
#include <qdir.h>

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
//more tests

bool showDebugMessages = false;
BibleTime* bibletime_ptr = 0;

void myMessageOutput( QtMsgType type, const char *msg ) {
	//we use this messagehandler to switch debugging off in final releases
	switch ( type ) {
		case QtDebugMsg:
		if (showDebugMessages) { //only show messages if they are enabled!
			fprintf( stderr,"(BibleTime %s) Debug: %s\n",VERSION, msg );
		}
		break;
		case QtWarningMsg:
		//if (showDebugMessages) //comment out for releases so users don't get our debug warnings
		fprintf( stderr,"(BibleTime %s) WARNING: %s\n",VERSION, msg );
		break;
		case QtFatalMsg:
		fprintf( stderr,"(BibleTime %s) _FATAL_: %s\nPlease contact info@bibletime.de and report this bug!",VERSION, msg );
		abort(); // dump core on purpose
	}
}

extern "C" {
	static void setSignalHandler(void (*handler)(int));

	// Crash recovery signal handler
	static void signalHandler(int sigId) {
		setSignalHandler(SIG_DFL);
		fprintf(stderr, "*** BibleTime got signal %d (Exiting)\n", sigId);
		// try to cleanup all windows
		if (CBTConfig::get
					(CBTConfig::crashedLastTime)) {
				//crashed this time and the execution before this one, probably a bug which occurs every time
				CBTConfig::set
					(CBTConfig::crashedTwoTimes, true);
			}
		else {
			//try to restore next time.
			CBTConfig::set
				(CBTConfig::crashedLastTime, true);
		}
		if (bibletime_ptr) {
			bibletime_ptr->saveSettings();
			fprintf(stderr, "*** Saving seemed to be successful. If restoring does not work on next startup please use the option --ignore-session\n");
		}
		::exit(-1); //exit BibleTime
	}

	// Crash recovery signal handler
	static void crashHandler(int sigId) {
		setSignalHandler(SIG_DFL);
		fprintf(stderr, "*** BibleTime got signal %d (Crashing). Trying to save settings.\n", sigId);
		if (CBTConfig::get
					(CBTConfig::crashedLastTime)) {
				//crashed this time and the execution before this one, probably a bug which occurs every time
				CBTConfig::set
					(CBTConfig::crashedTwoTimes, true);
			}
		else {
			//try to restore next time.
			CBTConfig::set
				(CBTConfig::crashedLastTime, true);
		}
		if (bibletime_ptr) {
			bibletime_ptr->saveSettings();
			fprintf(stderr, "*** Saving seemed to be successful. If restoring does not work on next startup please use the option --ignore-session\n");
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
			{"ignore-session", I18N_NOOP("Ignore the startup session that was saved when BibleTime was closed the last time."),0},
			{"open-default-bible <key>", I18N_NOOP("Open the standard Bible with the given key. Use <random> to open at a random position."),0},
			//{"install-local <path>", I18N_NOOP("Open the SwordSetup dialog to install works from <path>"),0},
			{0,0,0}
		};

	KAboutData aboutData(
		PACKAGE,
		"BibleTime",
		VERSION,
		I18N_NOOP("Bible study tool for KDE"),
		KAboutData::License_GPL_V2,
		I18N_NOOP("(c)1999-2006, The BibleTime Team"),
		I18N_NOOP("BibleTime is an easy to use but powerful Bible study tool for KDE.\n\nWe are looking for developers and translators.\nIf you'd like to join our team, please send an email to info@bibletime.info."),
		"http://www.bibletime.info/",
		"info@bibletime.info"
	);

	/***********************************************
	*    Active developers (sorted by last name)   *
	************************************************/
	// Project coordination
	aboutData.addAuthor("Joachim Ansorg", I18N_NOOP("Project coordinator"), "nospam@joachim-ansorg.de", "http://www.joachim-ansorg.de/");
	// Second main developer
	aboutData.addAuthor("Martin Gruner", I18N_NOOP("Frontend, backend"), "mg.pub@gmx.net", "");
	// artwork
	aboutData.addAuthor("James Ots", I18N_NOOP("Crystal icons, crystal startlogo, webpage"), "me@jamesots.com", "http://www.jamesots.com");

	//inactiv

	/***********************************************
	*        Credits (sorted by last name)         *
	************************************************/
	//highcolor icons, startlogo for BT <= 1.4
	aboutData.addCredit("David Blue", I18N_NOOP("High contrast template"), "davidslists@gmx.net");
	aboutData.addCredit("Tim Brodie",    I18N_NOOP("Installation manager"),"tbrodie@displayworksinc.com", "");
	aboutData.addCredit("Timothy R. Butler", I18N_NOOP("Icons, startlogo"), "tbutler@uninetsolutions.com", "http://www.uninetsolutions.com");
	//GUI improvements
	aboutData.addCredit("Jim Campbell",   I18N_NOOP("GUI"), "jdc.email@gmail.com", "http://www.crosswire.org/");   //Started the Sword project
	aboutData.addCredit("Troy A. Griffits",   I18N_NOOP("Founder of the Sword project"), "scribe@crosswire.org", "http://www.crosswire.org/");   //Started the Sword project
	//Sponsored many years the www.bibletime.de domain!
	aboutData.addCredit("Thomas Hagedorn",   I18N_NOOP("Sponsored our internet domain for many years"), "tom@theta-consulting.de", "");
	//He provided us with the Bible Study HowTo
	aboutData.addCredit("Bob Harman",        I18N_NOOP("Bible Study HowTo"), "ncc@ncchampton.org", "");
	// and / or search for multiple words, other fixes and improvements
	aboutData.addCredit("Nikolay Igotti", I18N_NOOP("Search dialog enhancements"), "olonho@hotmail.com", "");
	// Language codes iso639-1, iso639-2 and SIL language codes
	aboutData.addCredit("SIL International", I18N_NOOP("Language codes and names"), "", "http://www.ethnologue.com/iso639");
	//Indexed search, gui improvements
	aboutData.addCredit("Lee Carpenter",  I18N_NOOP("GUI, instant search"),"Lee Carpenter <elc@carpie.net>", "http://www.carpie.net");
	//Smaller frontend enhancements
	aboutData.addCredit("Chris Kujawa",  I18N_NOOP("Frontend"),"christopher.kujawa@verizon.net", "");
	//Smaller searchdialog enhancements
	aboutData.addCredit("Mark Lybarger",  I18N_NOOP("Searchdialog"), "mlybarge@insight.rr.com","");
	//Smaller frontend enhancements
	aboutData.addCredit("Luke Mauldin",  I18N_NOOP("Frontend"), "lukeskyfly@txk.net", "");
	// handbook documentation
	aboutData.addCredit("Fred Saalbach", I18N_NOOP("Documentation"), "saalbach@sybercom.net", "");
	// comitted search in default bible, opened modules, other smaller things
	aboutData.addCredit("Gary Sims", I18N_NOOP("Search dialog enhancements"), "gary@garysims.co.uk", "");
	// Very helpful testing
	aboutData.addCredit("Wolfgang Stradner", I18N_NOOP("Very helpful and detailed testing"), "wolfgang_stradner@wycliffe.org", "");
	//The first lead developer
	aboutData.addCredit("Torsten Uhlmann",   I18N_NOOP("The first lead developer"), "", "");
	//scoped_ptr and related classes
	aboutData.addCredit("David White",   I18N_NOOP("Helpful source code additions"), "", "http://www.wesnoth.org/");

	//special message so the translator get his credits in the about box, don't remove this!
	QString dummy = I18N_NOOP("_: NAME OF TRANSLATORS\nYour names"); //translator's name
	dummy = I18N_NOOP("_: EMAIL OF TRANSLATORS\nYour emails"); //translators eMail

	KCmdLineArgs::init(argc, argv, &aboutData);
	KCmdLineArgs::addCmdLineOptions( options );

	BibleTimeApp app;
	KGlobal::dirs()->addResourceType("BT_pic", "share/apps/bibletime/pics");
	KGlobal::dirs()->addResourceType("BT_DisplayTemplates", "share/apps/bibletime/display-templates");

	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

	// A binary option (on / off)
	if (args->isSet("debug")) {
		showDebugMessages = true;
	}

	if (kapp->isRestored()) {
		//    qWarning("Restoring BibleTime");
		RESTORE( BibleTime );
	}
	else {
		const bool showIt = CBTConfig::get
								(CBTConfig::logo);

		if(showIt) {
			KStartupLogo::createSplash();
			KStartupLogo::showSplash();
			KStartupLogo::setStatusMessage( i18n("Starting BibleTime") + QString("...") );
		}

		setSignalHandler(signalHandler);

		// compatibility stuff for 1.3, needs to be moved to better place later
		if (CBTConfig::get
					(CBTConfig::bibletimeVersion) != VERSION) {
				KStandardDirs stdDirs;
				QDir dir(stdDirs.saveLocation("data", "bibletime/"));
				if (!dir.exists("sessions/") && dir.exists("profiles/")) { //only old dir exists
					dir.rename("profiles", "sessions");
				}
			}

		bibletime_ptr = new BibleTime();

		// a new BibleTime version was installed (maybe a completely new installation)
		if (CBTConfig::get
					(CBTConfig::bibletimeVersion) != VERSION) {
				KStartupLogo::hideSplash();

				CBTConfig::set
					(CBTConfig::bibletimeVersion, VERSION);
				bibletime_ptr->slotSettingsOptions();
			}

		//The tip of the day
		if (CBTConfig::get
					(CBTConfig::tips)) {
				KStartupLogo::hideSplash();
				bibletime_ptr->slotHelpTipOfDay();
			}

		// restore the workspace and process command line options
		app.setMainWidget(bibletime_ptr);
		bibletime_ptr->show();
		bibletime_ptr->processCommandline(); //must be done after the bibletime window is visible

		if (showIt) {
			KStartupLogo::hideSplash();
			KStartupLogo::deleteSplash();
		}
	}

	const int ret = app.exec();

	delete bibletime_ptr;
	return ret;
}

