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

CPointers::PointerCache m_pointerCache;

void CPointers::setPrinter(CPrinter* const printer) {
	m_pointerCache.printer = printer;
}

void CPointers::setBackend(CSwordBackend* const backend) {
	m_pointerCache.backend = backend;
}

void CPointers::setInfoDisplay(CInfoDisplay* const infoDisplay) {
	m_pointerCache.infoDisplay = infoDisplay;
}

void CPointers::deletePrinter() {
	delete m_pointerCache.printer;
	m_pointerCache.printer = 0;
}

void CPointers::deleteBackend() {
	delete m_pointerCache.backend;
	m_pointerCache.backend = 0;
}
