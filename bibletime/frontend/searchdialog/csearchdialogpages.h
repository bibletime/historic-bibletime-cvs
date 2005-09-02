/********* Read the file LICENSE for license details. *********/

#ifndef CSEARCHDIALOGPAGES_H
#define CSEARCHDIALOGPAGES_H

//BibleTime includes
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbackend.h"
#include "backend/cswordmodulesearch.h"

//Qt includes
#include <qwidget.h>

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

class CModuleResultView : public KListView {
	Q_OBJECT
public:
	CModuleResultView(QWidget* parent);
	~CModuleResultView();
	/**
	* Setups the tree using the given list of modules.
	*/
	void setupTree( ListCSwordModuleInfo modules );
	/**
	* Returns the currently active module.
	*/
	CSwordModuleInfo* const activeModule();

protected: // Protected methods
	/**
	* Initializes this widget.
	*/
	void initView();
	/**
	* Initializes the connections of this widget
	*/
	void initConnections();

protected slots: // Protected slots
	/**
	* Is executed when an item was selected in the list.
	*/
	void executed( QListViewItem* );
	/**
	* Copies the whole search result with the text into the clipboard.
	*/
	void copyResultWithText();
	/**
	* Copies the whole search result into the clipboard.
	*/
	void copyResult();
	/**
	* This slot opens the popup menu at the given position
	*/
	void showPopup(KListView*, QListViewItem*, const QPoint&);
	/**
	* Appends the whole search result to the printer queue.
	*/
	void printResult();
	/**
	* Saves the search result with it's text.
	*/
	void saveResultWithText();
	/**
	* Saves the search result keys.
	*/
	void saveResult();

signals:
	void moduleSelected(CSwordModuleInfo*);
	void moduleChanged();

private:
	struct {
		KActionMenu* saveMenu;
		struct {
			KAction* result;
			KAction* resultWithText;
		}
		save;

		KActionMenu* printMenu;
		struct {
			KAction* result;
		}
		print;

		KActionMenu* copyMenu;
		struct {
			KAction* result;
			KAction* resultWithText;
		}
		copy;

	}
	m_actions;
	KPopupMenu* m_popup;
};

class CSearchResultView  : public KListView {
	Q_OBJECT
public:
	CSearchResultView(QWidget* parent);
	virtual ~CSearchResultView();
	/** Returns the module which is currently used. */
	CSwordModuleInfo* const module();

protected: // Protected methods
	/**
	* Initializes the view of this widget.
	*/
	void initView();
	void initConnections();
	virtual QDragObject* dragObject();

public slots: // Public slots
	void saveItems();
	/**
	* Setups the list with the given module.
	*/
	void setupTree(CSwordModuleInfo*);
	void copyItemsWithText();
	void copyItems();
	void saveItemsWithText();
	/**
	* Reimplementation to show the popup menu.
	*/
	virtual void showPopup(KListView*, QListViewItem* i, const QPoint& point);

protected slots: // Protected slots
	void printItems();
	/**
	* Is connected to the signal executed, which is emitted when a mew item was chosen.
	*/
	void executed(QListViewItem*);

private:
	struct {
		KActionMenu* saveMenu;
		struct {
			KAction* result;
			KAction* resultWithText;
		}
		save;

		KActionMenu* printMenu;
		struct {
			KAction* result;
		}
		print;

		KActionMenu* copyMenu;
		struct {
			KAction* result;
			KAction* resultWithText;
		}
		copy;
	}
	m_actions;
	KPopupMenu* m_popup;
	CSwordModuleInfo* m_module;

signals: // Signals
	void keySelected(const QString&);
};


/** The page of the search dialog which contains the search result part.
  * @author The BibleTime team
  */
class CSearchResultPage : public QWidget  {
	Q_OBJECT
public:
	CSearchResultPage(QWidget *parent=0, const char *name=0);
	~CSearchResultPage();
	/**
	* Sets the modules which contain the result of each.
	*/
	void setSearchResult(ListCSwordModuleInfo modules);

public slots: // Public slots
	/**
	* Resets the current löist of modules and the displayed list of found entries.
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
	CModuleResultView* m_moduleListBox;
	CSearchResultView* m_resultListBox;
	CReadDisplay* m_previewDisplay;
	ListCSwordModuleInfo m_modules;
	QPushButton* m_analyseButton;

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

class CSearchOptionsPage : public QWidget  {
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
	* Prepares the stuff which is required for a search, e.g. setting back the percentage bars.
	*/
	void prepareSearch();
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

private:
	QLabel* m_modulesLabel;
	KHistoryCombo* m_searchTextCombo;
	QPushButton* m_chooseModulesButton;
	ListCSwordModuleInfo m_modules;

	KComboBox* m_rangeChooserCombo;
	QPushButton* m_chooseRangeButton;
	QRadioButton* m_multipleWordsRadio;
	QRadioButton* m_multipleWordsORRadio;
	QRadioButton* m_exactTextRadio;
	QRadioButton* m_regexpRadio;

	int m_multipleWordsRadioID;
	int m_exactTextRadioID;
	int m_regexpRadioID;

	KComboBox* m_textTypeCombo;

	QCheckBox* m_caseSensitiveBox;
	KProgress* m_currentProgressBar;
	KProgress* m_overallProgressBar;

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
	* Sets the progress for the "all modules" bar.
	*/
	void setOverallProgress( const int );
	/**
	* Sets the value for the "curret module" progress bar.
	*/
	void setCurrentModuleProgress( const int );
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
	/**
	* Launches RegExpEditor
	*/
	void searchTypeSelected(int buttonID);

	void textTypeSelected();

signals:
	void sigSetSearchButtonStatus(bool);
};

#endif
