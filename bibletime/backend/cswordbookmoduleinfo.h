/***************************************************************************
                          cswordbookmoduleinfo.h  -  description
                             -------------------
    begin                : Thu Jan 24 2002
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
	CSwordBookModuleInfo( SWModule* module );
	CSwordBookModuleInfo( const CSwordBookModuleInfo& m );	
	~CSwordBookModuleInfo();
  /**
  * Returns the type of the module.
  */
  virtual const CSwordModuleInfo::ModuleType type() const;
  /**
  * Used for debugging
  */
  void printTree(TreeKeyIdx treeKey, TreeKeyIdx* target, int level = 1 );
  /**
  * Returns the maximal depth of sections and subsections.
  */
  const int depth();
  /**
  * @return A treekey filled with the structure of this module. Don't delete the returned key because it's casted from the module object.
  */
  TreeKeyIdx* tree();

private:
  void computeDepth(TreeKeyIdx treeKey, TreeKeyIdx* target, int level = 0 );	
	int m_depth;
};

inline const CSwordBookModuleInfo::ModuleType CSwordBookModuleInfo::type() const {
	return CSwordModuleInfo::GenericBook;
}


#endif
