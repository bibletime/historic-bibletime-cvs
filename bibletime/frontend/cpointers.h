/***************************************************************************
                          cpointers.h  -  description
                             -------------------
    begin                : Sun Feb 17 2002
    copyright            : (C) 2002 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CPOINTERS_H
#define CPOINTERS_H


class CSwordBackend;
class CPrinter;
/**Holds the pointers to important classes like modules, backend etc.
  *@author The BibleTime team
  */

class CPointers {
protected:
	friend class BibleTime; //BibleTime may initialize this object	
	static void setBackend(CSwordBackend*);
	static void setPrinter(CPrinter*);

public: // Public methods
  /**
  * Returns a pointer to the printer object.
  */
  CPrinter* printer() const;
  /**
  * Returns a pointer to the backend ...
  */
  CSwordBackend* backend() const;
};

#endif
