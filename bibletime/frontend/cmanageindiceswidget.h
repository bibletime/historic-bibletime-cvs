/********* Read the file LICENSE for license details. *********/

#ifndef CMANAGEINDICESWIDGET_H
#define CMANAGEINDICESWIDGET_H

#include "manageindicesform.h"

#include <qwidget.h>

class QCheckListItem;

namespace BookshelfManager {
	
/**
* This class encapsulates the "Manage search indices" page of the Bookshelf
* Manager.  It allows for creation and deletion of search indicies for each
* installed module.  It also allows for deletion of orphaned indices.
*/
class CManageIndicesWidget : public ManageIndicesForm
{
	Q_OBJECT
	
public:
	/**
	* Constructor
	*/
	CManageIndicesWidget(QWidget* parent, const char* name = 0);

protected:
	/**
	* Initializes the look and feel of this page
	*/
	void initView();
	/**
	* Populates the module list with installed modules and orphaned indices
	*/
	void populateModuleList();

	// member variables
	QCheckListItem* m_modsWithIndices;
	QCheckListItem* m_modsWithoutIndices;
	QCheckListItem* m_orphanedIndices;
	
public slots:
	/**
	* Creates indices for selected modules if no index currently exists
	*/
	void createIndices();
	/**
	* Deletes indices for selected modules and selected orphans
	*/
	void deleteIndices();
};

}

#endif
