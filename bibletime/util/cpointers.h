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

//BibleTime includes
#include "backend/clanguagemgr.h"

class CSwordBackend;
class CPrinter;
class CLanguageMgr;
class CInfoDisplay;

/** Holds the pointers to important classes like modules, backend etc.
  * @author The BibleTime team
  */
class CPointers {
protected:
	friend class BibleTime; //BibleTime may initialize this object
 	friend class BibleTimeApp; //BibleTimeApp may initialize this object	
	friend int main(int argc, char* argv[]);
	
	static void setBackend(CSwordBackend* const);
	static void setPrinter(CPrinter* const);
	static void setInfoDisplay(CInfoDisplay* const);
	static void deleteBackend();
	static void deletePrinter();

public: // Public methods
  /**
  * Returns a pointer to the printer object.
  */
  inline static CPrinter* const printer();
  /**
  * Returns a pointer to the backend ...
  */
  inline static CSwordBackend* const backend();
  inline static CLanguageMgr* const languageMgr();
	inline static CInfoDisplay* const infoDisplay();  

	struct PointerCache {
		PointerCache() {
			backend = 0;
			printer = 0;
			langMgr = 0;
			infoDisplay = 0;
		};
		
		CSwordBackend* backend;
		CPrinter*      printer;
		CLanguageMgr*  langMgr;
		CInfoDisplay*  infoDisplay;
	};

};

extern CPointers::PointerCache m_pointerCache;

/** Returns a pointer to the backend ... */
inline CSwordBackend* const CPointers::backend() {
	return m_pointerCache.backend;
}

/** Returns a pointer to the backend ... */
inline CLanguageMgr* const CPointers::languageMgr() {
  if (!m_pointerCache.langMgr) {
    m_pointerCache.langMgr = new CLanguageMgr();
  }
	return m_pointerCache.langMgr;
}

/** Returns a pointer to the printer object. */
inline CPrinter* const CPointers::printer() {
	return m_pointerCache.printer;
}

/** Returns a pointer to the printer object. */
inline CInfoDisplay* const CPointers::infoDisplay() {
	return m_pointerCache.infoDisplay;
}


#endif
