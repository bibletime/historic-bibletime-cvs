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

//KDE includes
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <kapp.h>
#include <kstddirs.h>
#include <kconfig.h>
#include <kinstance.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kimageio.h>
#include <kiconloader.h>

//Qt includes
#include <qfont.h>

//own includes
#include "ressource.h"
#include "bibletime.h"
#include "frontend/kstartuplogo.h"
#include "frontend/ctipwindow.h"
#include "frontend/ctoolclass.h"
#include "frontend/chtmldialog.h"
#include "config.h"

bool showDebugMessages = false;

void myMessageOutput( QtMsgType type, const char *msg ) {	
//we use this messagehandler to switch debugging off in final releases
	switch ( type ) {
		case QtDebugMsg:
		 	if (showDebugMessages)	//only show messages if they are enabled!
		 		fprintf( stderr,"(BibleTime 1.0) Debug: %s\n", msg );
			break;
		case QtWarningMsg:
			fprintf( stderr,"(BibleTime 1.0) WARNING: %s\n", msg );
			break;
		case QtFatalMsg:
			fprintf( stderr,"(BibleTime 1.0) _FATAL_: %s\nPlease contact info@bibletime.de and report this bug!", msg );
			abort();	// dump core on purpose
	}
}

	int main(int argc, char* argv[]) {
	//create about data for this application
	static KCmdLineOptions options[] = {
		{ "debug", I18N_NOOP("Enable debug messages."), 0 }
	};
	
	qInstallMsgHandler( myMessageOutput );
	KAboutData aboutData(
		PACKAGE,
		"BibleTime",
		VERSION,
		I18N_NOOP("An easy to use but powerful Bible study tool"),
		KAboutData::License_GPL_V2,
		I18N_NOOP("(c)1999-2001, The BibleTime Team"),
		I18N_NOOP(""),
		"http://www.bibletime.de/",
		"info@bibletime.de"
	);
	
	//coders
	aboutData.addAuthor("Torsten Uhlmann (not active)", I18N_NOOP("backend"), "TUhlmann@gmx.de", "http://tuhlmann.purespace.de");
	aboutData.addAuthor("Joachim Ansorg", I18N_NOOP("Project coordinator, frontend, backend"), "jansorg@gmx.de","");	
//	aboutData.addAuthor("Darwin Gregory (not active)", I18N_NOOP("optionsdialog"), "darwin@ichristian.com", "");	
	aboutData.addAuthor("Martin Gruner", I18N_NOOP("frontend and misc things"), "mg.pub@gmx.net", "");
//	aboutData.addAuthor("Keith Robertson (not active)", "", "kjrobert@uwaterloo.ca", "");
	
	//documentation
	aboutData.addAuthor("Fred Saalbach", 	I18N_NOOP("Handbook"), "saalbach@sybercom.net", "");		
//	aboutData.addAuthor("Jeffrey Hoyt", 	I18N_NOOP("handbook"), "jeffhoyt@earthlink.net", "http://jhoyt.faithweb.com");
		
	//translators
	aboutData.addAuthor("Benedykt P. Barszcz", I18N_NOOP("Translation to Polish"), "kb2qzv@box43.gnet.pl", "");		
	aboutData.addAuthor("Mario Bertrand", 	I18N_NOOP("Translation to French"), "mbert@tbrq.org", "");		
//	aboutData.addAuthor("Simon Bockius", 	I18N_NOOP("Partial translation of the handbook to German"), "", "");
	aboutData.addAuthor("Birger Langkjer", I18N_NOOP("Translation to Danish"), "birger.langkjer@image.dk", "");		
	aboutData.addAuthor("Michal Rovnaník", I18N_NOOP("Translation to Czech"), "Michal.Rovnanik@seznam.cz", "");		
	aboutData.addAuthor("Balint Sandor", I18N_NOOP("Translation to Hungarian"), "balintsa@freemail.hu", "");
	aboutData.addAuthor("Zdenko Podobny", I18N_NOOP("Translation to Slovak"), "zdpo@post.sk", "");
	aboutData.addAuthor("Espen Trydal", I18N_NOOP("Translation to Norwegian"), "etrydal@postkassa.no", "");	
	aboutData.addAuthor("Kees van Veen", I18N_NOOP("Translation to Dutch"), "cvn@interchain.nl", "");
							
	//credits
	aboutData.addCredit("Troy A. Griffits", I18N_NOOP("Leader of the SWORD project.\nLots of help with the SWORD API!"), "scribe@crosswire.org", "");		
	aboutData.addCredit("Thomas Hagedorn", I18N_NOOP("Sponsor of www.bibletime.de!"), "tom@delix.de", "");	
	aboutData.addCredit("Juho Vahakangas", I18N_NOOP("The Martin Luther startup logo"), "juhov@freenet.fi", "");
 	
 	KCmdLineArgs::init(argc, argv, &aboutData); 	
 	KCmdLineArgs::addCmdLineOptions ( options );

	
 	KApplication app;
 	QFont f = app.font();
 	f.setCharSet(QFont::AnyCharSet);
 	app.setFont(f); 	
 	
 	KGlobal::dirs()->addResourceType("BT_pic", KStandardDirs::kde_default("data") + kapp->name() + "/pics/");


	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	// A binary option (on / off)
	if (args->isSet("debug")) {
		showDebugMessages = true;
	}
 		
 	if (app.isRestored()) {
		RESTORE(BibleTime);
	}
  else {
		KConfig *config = KGlobal::config();
		KStartupLogo *start_logo = 0;		
		bool showIt = false;	
	  {
  		KConfigGroupSaver groupSaver(config,"General");
			showIt = config->readBoolEntry("Logo",true);	
	
			if(showIt) {
		 		start_logo = new KStartupLogo();
		 		start_logo->show();
			};
	  }
	  BibleTime* bibletime = new BibleTime();			
		
		//first startup of BibleTime?
		{
			KConfigGroupSaver groupSaver(config, "General");
			if (config->readBoolEntry(QString("firstStartup %1").arg(VERSION), true)) {
				config->writeEntry( QString("firstStartup %1").arg(VERSION), false);
				HTML_DIALOG(HELPDIALOG_FIRST_START);
			}
			if (!config->readBoolEntry(QString("isConfigured %1").arg(VERSION), false)) {
				config->writeEntry( QString("isConfigured %1").arg(VERSION), true);
				bibletime->slotSettingsOptions();
			}
		}
		
	  if (showIt) {
			start_logo->close();
			delete start_logo;
		}
		
		{ //The tip of the day
			KConfigGroupSaver groupSaver(config, "Daily tip");
			if (config->readBoolEntry("TipsOnStart", true)) {
				bibletime->slotHelpTipOfDay();
			}			
			bibletime->show();					
		}		
  	return app.exec();
	}
}
    	