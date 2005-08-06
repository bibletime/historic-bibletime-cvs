#ifndef BIBLETIME_INTERFACE_H
#define BIBLETIME_INTERFACE_H

//Qt includes
#include <qstring.h>
#include <qstringlist.h>

//KDE includes
#include <dcopobject.h>
#include <kmainwindowiface.h>

class BibleTimeInterface : virtual public DCOPObject
{
  K_DCOP

public:
 	BibleTimeInterface(QCString id) : DCOPObject(id) {}
// 	BibleTimeInterface(QObject* id) : KMainWindowInterface(id) {}
//  	BibleTimeInterface(QObject* id) : DCOPObject(id) {}
	
k_dcop:
	virtual ASYNC syncAllBibles(QString key) = 0;
	virtual ASYNC syncAllCommentaries(QString key) = 0;
	virtual ASYNC syncAllLexicons(QString key) = 0;
	virtual ASYNC syncAllVerseBasedModules(QString key) = 0;
	virtual ASYNC openWindow(QString moduleName, QString key) = 0;
	virtual ASYNC openDefaultBible(QString key) = 0;
	virtual ASYNC closeAllModuleWindows() = 0;
	virtual QStringList searchInOpenModules(QString searchText) = 0;
	virtual QStringList searchIndefaultBible(QString searchText) = 0;
  	
/*  	  -syncAllBibles("Gen.1.1")
>       -syncAllCommentaries("Gen.1.1")
>       -syncAllLexicons("Gen.1.1")
>       -syncAllVerseBasedModules("Gen.1.1")
>       -openWindow("KJV", "Gen.3.16")
>       -openDefaultBible("Gen.3.16") //same for commentaries and lexicons
>       -closeAllWindows()
>       -searchInOpenModules("Jesus Christ")
>       -searchInDefaultBible("Jesus Christ")*/
//     virtual ASYNC myAsynchronousMethod(QString someParameter) = 0;
//     virtual QRect mySynchronousMethod() = 0;
		
};

#endif
