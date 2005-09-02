/********* Read the file LICENSE for license details. *********/

#include "cpointers.h"

//BibleTime's backend
#include "backend/cswordbackend.h"
#include "backend/cdisplaytemplatemgr.h"

//BibleTime's frontend
#include "frontend/cprinter.h"


CPointers::PointerCache m_pointerCache;

void CPointers::setPrinter(Printing::CPrinter* const printer) {
    Q_ASSERT( m_pointerCache.printer == 0);
    CPointers::deletePrinter();
    m_pointerCache.printer = printer;
}

void CPointers::setBackend(CSwordBackend* const backend) {
    Q_ASSERT( m_pointerCache.backend == 0);
    CPointers::deleteBackend();
    m_pointerCache.backend = backend;
}

void CPointers::setInfoDisplay(InfoDisplay::CInfoDisplay* const infoDisplay) {
    Q_ASSERT( m_pointerCache.infoDisplay == 0);
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

