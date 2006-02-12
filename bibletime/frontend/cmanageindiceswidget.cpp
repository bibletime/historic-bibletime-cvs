/********* Read the file LICENSE for license details. *********/

#include "cmanageindiceswidget.h"

#include "cmoduleindexdialog.h"

#include "util/ctoolclass.h"
#include "util/cresmgr.h"
#include "util/cpointers.h"

#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbackend.h"

//Qt includes
#include <qlabel.h>
#include <qlayout.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qdir.h>
#include <qlistview.h>
#include <qfileinfo.h>

//KDE includes
#include <klocale.h>
#include <klistview.h>
#include <kiconloader.h>

namespace BookshelfManager {

/** Constructor */
CManageIndicesWidget::CManageIndicesWidget(QWidget* parent, const char* name) :
	ManageIndicesForm(parent, name) {
	
	initView();
	populateModuleList();
};

/** Initializes the look and feel of this page */
void CManageIndicesWidget::initView()
{
	// Set description label
	QVBoxLayout* box = new QVBoxLayout(m_labelFrame, 0, 0);
	QLabel* mainLabel = CToolClass::explanationLabel(m_labelFrame,
		i18n("Manage module search indices"),
		i18n("You can use the list below to create and/or delete search indices for your"
		"Sword modules."));
	box->addWidget(mainLabel);

	// configure the list view
	m_moduleList->addColumn(i18n("Module"));
	m_moduleList->addColumn(i18n("Index size"));
	m_moduleList->setRootIsDecorated(true);
	m_moduleList->setColumnWidth(0, 150);
	m_moduleList->setColumnAlignment(1, Qt::AlignRight);

	// icons for our buttons
	m_createIndicesButton->setIconSet(SmallIcon("folder_new", 16));
	m_deleteIndicesButton->setIconSet(SmallIcon("remove", 16));

	// connect our signals/slots
	connect(m_createIndicesButton, SIGNAL(clicked()), this, SLOT(createIndices()));
	connect(m_deleteIndicesButton, SIGNAL(clicked()), this, SLOT(deleteIndices()));
}

/** Populates the module list with installed modules and orphaned indices */
void CManageIndicesWidget::populateModuleList()
{
	m_moduleList->clear();
		
	// populate installed modules
	m_modsWithIndices = new QCheckListItem(m_moduleList, i18n("Modules with indices"),
		QCheckListItem::CheckBoxController);
	m_modsWithIndices->setOpen(true);

	m_modsWithoutIndices = new QCheckListItem(m_moduleList, i18n("Modules without indices"),
		QCheckListItem::CheckBoxController);
	m_modsWithoutIndices->setOpen(true);

	ListCSwordModuleInfo& modules = CPointers::backend()->moduleList();
	ListCSwordModuleInfo::iterator end_it = modules.end();
	for (ListCSwordModuleInfo::iterator it = modules.begin(); it != end_it; ++it) {
		QCheckListItem* item = NULL;
		if ((*it)->hasIndex()) {
			item = new QCheckListItem(m_modsWithIndices, (*it)->name(),
				QCheckListItem::CheckBox);
			item->setText(1, QString("%1 K").arg((*it)->indexSize() / 1024));
		}
		else {
			item = new QCheckListItem(m_modsWithoutIndices, (*it)->name(),
				QCheckListItem::CheckBox);
			item->setText(1, i18n("0 K"));
		}
	}
	
	// populate orphaned modules
	m_orphanedIndices = new QCheckListItem(m_moduleList, i18n("Orphaned Indices"),
		QCheckListItem::CheckBoxController);
	m_orphanedIndices->setOpen(true);

	QDir dir(CSwordModuleInfo::getBaseIndexLocation());
	dir.setFilter(QDir::Dirs);
	for (unsigned int i = 0; i < dir.count(); i++) {
		if (dir[i] != "." && dir[i] != ".." &&
			CPointers::backend()->findModuleByName(dir[i]) == NULL) {
			QCheckListItem* oitem = new QCheckListItem(m_orphanedIndices, dir[i],
				QCheckListItem::CheckBox);
			// get size
			QDir index(dir.path() + "/" + dir[i]);
			index.setFilter(QDir::Files);
			unsigned long size = 0;
			const QFileInfoList* infoList = index.entryInfoList();
			if (infoList) {
				QFileInfoListIterator it(*infoList);
				QFileInfo* info;
				while ((info = it.current())!= NULL) {
					++it;
					size += info->size();
				}
			}
			oitem->setText(1, QString("%1 K").arg(size / 1024));
		}
	}
}

/** Creates indices for selected modules if no index currently exists */
void CManageIndicesWidget::createIndices()
{
	QCheckListItem* top = m_modsWithoutIndices;
	//bool indicesCreated = false;
	QCheckListItem* item = (QCheckListItem*)top->firstChild();

	ListCSwordModuleInfo moduleList;
	while (item) {
		if (item->isOn()) {
			CSwordModuleInfo* module =
				CPointers::backend()->findModuleByName(item->text().utf8());

			
			if (module /*&& !module->hasIndex()*/) {
				moduleList.append( module );
				//module->buildIndex();
				//indicesCreated = true;
			}
		}
		item = (QCheckListItem*)item->nextSibling();
	}

	//Shows the progress dialog
	CModuleIndexDialog::getInstance()->indexAllModules( moduleList );
	
	//if (indicesCreated) {
		populateModuleList();
	//}
}

/** Deletes indices for selected modules and selected orphans */
void CManageIndicesWidget::deleteIndices()
{
	// delete installed module indices
	QCheckListItem* top = m_modsWithIndices;
	bool indicesDeleted = false;
	QCheckListItem* item = (QCheckListItem*)top->firstChild();
	while (item) {
		if (item->isOn()) {
			CSwordModuleInfo* module =
				CPointers::backend()->findModuleByName(item->text().utf8());
			if (module) {
				module->deleteIndex();
				indicesDeleted = true;
			}
		}
		item = (QCheckListItem*)item->nextSibling();
	}

	// delete orphaned indices
	top = m_orphanedIndices;
	if (top) {
		QCheckListItem* item = (QCheckListItem*)top->firstChild();
		while (item) {
			if (item->isOn()) {
				QDir dir(CSwordModuleInfo::getBaseIndexLocation());
				dir.setFilter(QDir::All);
				for (unsigned int i = 0; i < dir.count(); i++) {
					if (dir[i] == item->text()) {
						// delete all files in directory
						dir.cd(item->text());
						for (unsigned int j = 0; j < dir.count(); j++) {
							dir.remove(dir[j]);
						}
						// delete the directory
						dir.cdUp();
						dir.rmdir(item->text());
						indicesDeleted = true;
					}
				}
			}
			item = (QCheckListItem*)item->nextSibling();
		}
	}

	// repopulate the list if an action was taken
	if (indicesDeleted) {
		populateModuleList();
	}
}

}
