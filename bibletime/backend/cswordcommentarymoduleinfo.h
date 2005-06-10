/********* Read the file LICENSE for license details. *********/

#ifndef CSWORDCOMMENTARYMODULEINFO_H
#define CSWORDCOMMENTARYMODULEINFO_H

//own includes
#include "cswordbiblemoduleinfo.h"

/** Commentary module implementation.
 * This CSwordModule implementation provides access to Sword's commentary modules.
 * @author The BibleTime team
 * @version $Id$
 */
class CSwordCommentaryModuleInfo : public CSwordBibleModuleInfo  {
public:
	CSwordCommentaryModuleInfo( sword::SWModule* module, CSwordBackend* const = CPointers::backend() );
	~CSwordCommentaryModuleInfo();
	/** Reimplementation to return the commentary type.
	*/
  virtual const CSwordModuleInfo::ModuleType type() const;
  /** Reimplementation to clone the current object.
  */
  virtual CSwordModuleInfo* clone();
  /**
  * Returns true if this module may be written by the write display windows.
  */
  virtual const bool isWritable() const;
};

inline const CSwordModuleInfo::ModuleType CSwordCommentaryModuleInfo::type() const{
	return CSwordModuleInfo::Commentary;
}

#endif
