/********* Read the file LICENSE for license details. *********/

#ifndef BIBLETIME_INTERFACE_H
#define BIBLETIME_INTERFACE_H

//Qt includes
#include <qstring.h>
#include <qstringlist.h>

//KDE includes
#include <dcopobject.h>
#include <kmainwindowiface.h>

/** DCOP interface definition for BibleTime.
* This is the interface definition for BibleTime's DCOP client interface. The main window class "BibleTime" 
* inherits from this class and re-implements these pure virtual functions.
* It uses virtual inheritance so that a widget which inherits twice from DCOPObject does not get mixed up.
* This file is turned into bibletimeinterface_skel.cpp by the KDE automake system.
*
* @author Joachim Ansorg
*/
class BibleTimeInterface : virtual public DCOPObject
{
	//This K_DCOP line is required so that the k_dcop: and ASYNC are processed.
  K_DCOP

public:
 	BibleTimeInterface(QCString id) : DCOPObject(id) {}
	
k_dcop:
	/** Sync all open Bible windows to the key.
	*/
	virtual ASYNC syncAllBibles(QString key) = 0;
	/** Sync all open commentary windows to the key.
	*/
	virtual ASYNC syncAllCommentaries(QString key) = 0;
	/** Sync all open lexicon windows to the key.
	*/
	virtual ASYNC syncAllLexicons(QString key) = 0;
	/** Sync all open verse based (i.e. Bibles and commentaries) windows to the key.
	*/
	virtual ASYNC syncAllVerseBasedModules(QString key) = 0;
	/** Open a new read window for the module moduleName using the given key
	*/
	virtual ASYNC openWindow(QString moduleName, QString key) = 0;
	/** Open a new read window for the default Bible module using the given key
	*/
	virtual ASYNC openDefaultBible(QString key) = 0;
	/** Close all open windows.
	*/
	virtual ASYNC closeAllModuleWindows() = 0;
	/** Search in all open modules and return the search result.
	*/
	virtual QStringList searchInOpenModules(QString searchText) = 0;
	/** Search in the default Bible module and return the search result.
	*/
	virtual QStringList searchIndefaultBible(QString searchText) = 0;
};

#endif
