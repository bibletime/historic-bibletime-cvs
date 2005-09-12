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
class BibleTimeInterface : virtual public DCOPObject {
	//This K_DCOP line is required so that the k_dcop: and void are processed.
	K_DCOP

public:
BibleTimeInterface(QCString id) : DCOPObject(id) {}

k_dcop:
	/** Sync all open Bible windows to the key.
	*/
	virtual void syncAllBibles(QString key) = 0;
	/** Sync all open commentary windows to the key.
	*/
	virtual void syncAllCommentaries(QString key) = 0;
	/** Sync all open lexicon windows to the key.
	*/
	virtual void syncAllLexicons(QString key) = 0;
	/** Sync all open verse based (i.e. Bibles and commentaries) windows to the key.
	*/
	virtual void syncAllVerseBasedModules(QString key) = 0;
	/** Open a new read window for the module moduleName using the given key
	*/
	virtual void openWindow(QString moduleName, QString key) = 0;
	/** Open a new read window for the default Bible module using the given key
	*/
	virtual void openDefaultBible(QString key) = 0;
	/** Close all open windows.
	*/
	virtual void closeAllModuleWindows() = 0;
	/** Seach the searchText in the specified module. The search result will be in the form
	*    [modulename] osis_ref_of_the_found_key
	* For example [KJV] Gen.1.1
	*/
	virtual QStringList searchInModule(const QString& moduleName, const QString& searchText) = 0;
	/** Search in all open modules and return the search result.
	* The result is in the same format as searchInModule
	*/
	virtual QStringList searchInOpenModules(const QString& searchText) = 0;
	/** Search in the default Bible module and return the search result.
	* The result is in the same format as searchInModule
	*/
	virtual QStringList searchInDefaultBible(const QString& searchText) = 0;
};

#endif
