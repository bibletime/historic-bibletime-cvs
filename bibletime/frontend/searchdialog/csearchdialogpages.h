/********* Read the file LICENSE for license details. *********/

#ifndef CSEARCHDIALOGPAGES_H
#define CSEARCHDIALOGPAGES_H

//BibleTime includes
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbackend.h"
#include "backend/cswordmodulesearch.h"
#include "searchoptionsform.h"
#include "searchresultsform.h"

//Qt includes
#include <qwidget.h>
#include <qvgroupbox.h>

//KDE includes
#include <klistview.h>

//forward declarations
class QLabel;
class QCheckBox;
class QPushButton;
class QRadioButton;

class KComboBox;
class KActionMenu;
class KAction;
class KHistoryCombo;
class KProgress;
class KPopupMenu;

class CReadDisplay;


/** The page of the search dialog which contains the search result part.
  * @author The BibleTime team
  */
class CSearchResultPage : public SearchResultsForm {
	Q_OBJECT
public:
	CSearchResultPage(QWidget *parent=0, const char *name=0);
	~CSearchResultPage();
	/**
	* Sets the modules which contain the result of each.
	*/
	void setSearchResult(ListCSwordModuleInfo modules);
	
	QSize sizeHint() const { return baseSize(); }
	QSize minimumSizeHint() const { return minimumSize(); }

public slots: // Public slots
	/**
	* Resets the current l�st of modules and the displayed list of found entries.
	*/
	void reset();

protected: // Protected methods
	/**
	* Initializes the view of this widget.
	*/
	void initView();
	/**
	* Initializes the signal slot conections of the child widgets
	*/
	void initConnections();
	/**
	* This function highlights the searched text in the content using the search type given by search flags
	*/
	const QString highlightSearchedText(const QString& content, const QString& searchedText, const int searchFlags);

private:
	CReadDisplay* m_previewDisplay;
	ListCSwordModuleInfo m_modules;

protected slots: // Protected slots
	/**
	* Update the preview of the selected key.
	*/
	void updatePreview(const QString& key);
	/**
	* Shows a dialog with the search analysis of the current search.
	*/
	void showAnalysis();

};

class CSearchOptionsPage : public SearchOptionsForm  {
	Q_OBJECT
public:
	CSearchOptionsPage(QWidget *parent=0, const char *name=0);
	~CSearchOptionsPage();
	/**
	* Sets the search text used in the page.
	*/
	void setSearchText(const QString& text);
	/**
	* Returns the search text set in this page.
	*/
	const QString searchText();
	/**
	* Returns the list of used modules.
	*/
	const ListCSwordModuleInfo modules();
	/**
	* Return the selected search type,.
	*/
	const int searchFlags();
	/**
	* Sets all options back to the default.
	*/
	void reset();
	/**
	* Returns the selected search scope if a search scope was selected.
	*/
	sword::ListKey searchScope();
	/**
	* Returns the selected scope type.
	*/
	const CSwordModuleSearch::scopeType scopeType();

	QSize sizeHint() const { return baseSize(); }
	QSize minimumSizeHint() const { return minimumSize(); }

private:
	ListCSwordModuleInfo m_modules;
protected: // Protected methods
	/**
	* Initializes this page.
	*/
	void initView();
	/**
	* Reads the settings of the last searchdialog session.
	*/
	void readSettings();
	/**
	* Reads the settings for the searchdialog from disk.
	*/
	void saveSettings();

public slots: // Public slots
	/**
	* Sets the modules used by the search.
	*/
	void setModules( ListCSwordModuleInfo modules );
	/**
	* Reimplementation.
	*/
	void aboutToShow();
	/**
	* Refreshes the list of ranges and the range combobox. 
	*/
	void refreshRanges();
	/**
	 * Opens the modules chooser dialog.
	 */
	void chooseModules();

protected slots: // Protected slots
	void setupRanges();
	void syntaxHelp();

signals:
	void sigSetSearchButtonStatus(bool);
};

#endif
