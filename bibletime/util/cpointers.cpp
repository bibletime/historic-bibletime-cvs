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
#include "backend/cdisplaytemplatemgr.h"

#include "frontend/printing/cprinter.h"

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

void CPointers::deleteLanguageMgr() {
	delete m_pointerCache.langMgr;
	m_pointerCache.langMgr = 0;
}

void CPointers::deleteDisplayTemplateMgr() {
	delete m_pointerCache.displayTemplateMgr;
	m_pointerCache.displayTemplateMgr = 0;
}

/** Returns a pointer to the printer object. */
CDisplayTemplateMgr* const CPointers::displayTemplateManager() {
  if (!m_pointerCache.displayTemplateMgr) {
    m_pointerCache.displayTemplateMgr = new CDisplayTemplateMgr();
  }
	return m_pointerCache.displayTemplateMgr;
}

