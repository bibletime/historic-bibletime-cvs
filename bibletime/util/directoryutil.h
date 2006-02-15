//
// C++ Interface: directoryutil
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef UTIL_FILESDIRECTORYUTIL_H
#define UTIL_FILESDIRECTORYUTIL_H

#include <qstring.h>

namespace util {

namespace filesystem {

/**
Tools for working with directories.

	@author The BibleTime team <info@bibletime.info>
*/
class DirectoryUtil{
public:
    DirectoryUtil();

    ~DirectoryUtil();
	
    static void removeRecursive(const QString& dir);
};

}

}

#endif
