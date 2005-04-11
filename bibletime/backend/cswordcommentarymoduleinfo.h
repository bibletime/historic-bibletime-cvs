/********* Read the file LICENSE for license details. *********/

#ifndef CSWORDCOMMENTARYMODULEINFO_H
#define CSWORDCOMMENTARYMODULEINFO_H

//own includes
#include "cswordbiblemoduleinfo.h"

/**
  * @author The BibleTime team
  * @version $Id$
  */
class CSwordCommentaryModuleInfo : public CSwordBibleModuleInfo  {
public:
	CSwordCommentaryModuleInfo( sword::SWModule* module, CSwordBackend* const = CPointers::backend() );
	~CSwordCommentaryModuleInfo();
  virtual const CSwordModuleInfo::ModuleType type() const;
  virtual CSwordModuleInfo* clone();
  /**
  * Returns true if this module may be written by the write display windows.
  */
  virtual const bool isWritable();
};

/** No descriptions */
inline const CSwordModuleInfo::ModuleType CSwordCommentaryModuleInfo::type() const{
	return CSwordModuleInfo::Commentary;
}

#endif
