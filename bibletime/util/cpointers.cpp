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

#include "backend/cswordbackend.h"

#include "printing/cprinter.h"

static CSwordBackend* CPointers::m_backend = 0;
static CPrinter* CPointers::m_printer = 0;
static CLanguageMgr* CPointers::m_langMgr = 0;


void CPointers::setPrinter(CPrinter* const printer) {
	m_printer = printer;
}

void CPointers::setBackend(CSwordBackend* backend) {
	m_backend = backend;
}

void CPointers::deletePrinter() {
	delete m_printer;
	m_printer = 0;
}

void CPointers::deleteBackend() {
	delete m_backend;
	m_backend = 0;
}
