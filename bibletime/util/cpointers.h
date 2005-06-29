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
	friend int main(int argc, char* argv[]); //main may set the printer

	//Empty virtuaual destructor
	virtual ~CPointers() {};
	
	/** Set the backend.
	* @param backend Pointer to the new application-wide Sword backend
	*/
	static void setBackend(CSwordBackend* const backend);
	/** Set the printer instance.
	* @param printer Pointer to the ne CPrinter instance.
	*/
	static void setPrinter(CPrinter* const printer);
	/** Set the info display.
	* @param iDisplay The pointer to the new info display.
	*/
	static void setInfoDisplay(CInfoDisplay* const iDisplay);

	/** Delete the backend. Should be called by BibleTimeApp,
	* because the backend should be deleted as late as possible.
	*/
	static void deleteBackend();
	/** Delete the printer. Should be called by BibleTimeApp,
	* because the printer should be deleted as late as possible.
	*/
	static void deletePrinter();
	/** Delete the language manager. Should be called by BibleTimeApp,
	* because the language manager should be deleted as late as possible.
	*/
	static void deleteLanguageMgr();
	/** Delete the display template manager. Should be called by BibleTimeApp,
	* because the template manager should be deleted as late as possible.
	*/
	static void deleteDisplayTemplateMgr();

public: // Public methods
  /**
  * Returns a pointer to the printer object.
  * @return The printer oobject.
  */
  inline static CPrinter* const printer();
  /** Returns a pointer to the backend
  * @return The backend pointer.
  */
  inline static CSwordBackend* const backend();
  /** Returns a pointer to the language manager
  * @return The language manager
  */
  inline static CLanguageMgr* const languageMgr();
  /** Returns a pointer to the info display.
  * @return The backend pointer.
  */
	inline static CInfoDisplay* const infoDisplay();  
  /** Returns a pointer to the application's display template manager
  * @return The backend pointer.
  */
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
