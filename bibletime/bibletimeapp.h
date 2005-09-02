/********* Read the file LICENSE for license details. *********/

#ifndef BIBLETIMEAPP_H
#define BIBLETIMEAPP_H

//BibleTime
#include "util/cpointers.h"

//KDE
#include <kapplication.h>

/** The BibleTimeApp class is used to clean up all instances of the backend and to delete all created module objects.
  * @author The BibleTime team
  */
class BibleTimeApp : public KApplication, public CPointers {
public: 
	BibleTimeApp();
	virtual ~BibleTimeApp();

protected:
	void initDCOP();
};

#endif
