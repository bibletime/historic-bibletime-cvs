/********* Read the file LICENSE for license details. *********/

#ifndef CPOINTERS_H
#define CPOINTERS_H

//BibleTime includes
#include "backend/clanguagemgr.h"

class CSwordBackend;
class CLanguageMgr;
class CDisplayTemplateMgr;

namespace Printing { class CPrinter; }
using namespace Printing;

namespace InfoDisplay { class CInfoDisplay; };
using namespace InfoDisplay;

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
	static void deleteLanguageMgr();
	static void deleteDisplayTemplateMgr();

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
	static CDisplayTemplateMgr* const displayTemplateManager();

	struct PointerCache {
		PointerCache() {
			backend = 0;
			printer = 0;
			langMgr = 0;
			infoDisplay = 0;
			displayTemplateMgr = 0;
		};
		
		CSwordBackend* backend;
		CPrinter*      printer;
		CLanguageMgr*  langMgr;
		CInfoDisplay*  infoDisplay;
		CDisplayTemplateMgr* displayTemplateMgr;
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
