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

/** Holds the pointers to important classes like modules, backend etc.
  * @author The BibleTime team
  */
class CPointers {
private:
//Our static pointers to store the backend, printer and langMgr pointers
  static CSwordBackend* m_backend;
  static CPrinter* m_printer;
  static CLanguageMgr* m_langMgr;
  
protected:
	friend class BibleTime; //BibleTime may initialize this object	
	friend int main(int argc, char* argv[]);
	static void setBackend(CSwordBackend* const);
	static void setPrinter(CPrinter* const);
	static void deleteBackend();
	static void deletePrinter();

public: // Public methods
  /**
  * Returns a pointer to the printer object.
  */
  static CPrinter* const printer();
  /**
  * Returns a pointer to the backend ...
  */
  static CSwordBackend* const backend();
  static CLanguageMgr* const languageMgr();  
};


/** Returns a pointer to the backend ... */
inline CSwordBackend* const CPointers::backend() {
	return m_backend;
}

/** Returns a pointer to the backend ... */
inline CLanguageMgr* const CPointers::languageMgr() {
  if (!m_langMgr)
    m_langMgr = new CLanguageMgr();
	return m_langMgr;
}

/** Returns a pointer to the printer object. */
inline CPrinter* const CPointers::printer() {
	return m_printer;
}

#endif
