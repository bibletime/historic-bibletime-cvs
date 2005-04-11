/********* Read the file LICENSE for license details. *********/

#ifndef CSWORDGENBOOKMODULEINFO_H
#define CSWORDGENBOOKMODULEINFO_H

//BibleTime includes
#include "cswordmoduleinfo.h"

//Sword includes
#include <treekeyidx.h>


/** Class for generic book support
  * @author The BibleTime team
  */
class CSwordBookModuleInfo : public CSwordModuleInfo  {
public: 
	CSwordBookModuleInfo( sword::SWModule* module, CSwordBackend* const = CPointers::backend() );
	CSwordBookModuleInfo( const CSwordBookModuleInfo& m );	
	~CSwordBookModuleInfo();
  /**
  * Returns the type of the module.
  */
  virtual const CSwordModuleInfo::ModuleType type() const;
  /**
  * Returns the maximal depth of sections and subsections.
  */
  const int depth();
  /**
  * @return A treekey filled with the structure of this module. Don't delete the returned key because it's casted from the module object.
  */
  sword::TreeKeyIdx* const tree() const;

private:
  /**
  * A recursive helper function to help computng the module depth!
  */
  void computeDepth(sword::TreeKeyIdx* key, int level = 0 );	
	int m_depth;
};

inline const CSwordBookModuleInfo::ModuleType CSwordBookModuleInfo::type() const {
	return CSwordModuleInfo::GenericBook;
}


#endif
