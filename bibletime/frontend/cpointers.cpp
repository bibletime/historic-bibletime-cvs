/***************************************************************************
                          cpointers.cpp  -  description
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

#include "cpointers.h"
#include "../backend/cswordbackend.h"
#include "../printing/cprinter.h"

//the static pointers
static CSwordBackend* m_backend = 0;
static CPrinter* m_printer = 0;

CPointers::CPointers(CSwordBackend* backend, CPrinter* printer) {
	m_backend = backend;
	m_printer = printer;
}

CPointers::~CPointers(){

}

CPointers::CPointers(){
	ASSERT(m_backend);
	ASSERT(m_printer);
}

/** Returns a pointer to the backend ... */
CSwordBackend* CPointers::backend() const{
	ASSERT(m_backend);
	return m_backend;
}

/** Returns a pointer to the printer object. */
CPrinter* CPointers::printer() const{
	ASSERT(m_printer);	
	return m_printer;
}
