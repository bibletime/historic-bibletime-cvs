//
// C++ Interface: cprinter
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CPRINTER_H
#define CPRINTER_H

//BibleTime backend
#include "backend/cswordbackend.h"
#include "backend/ctextrendering.h"

//BibleTime frontend
#include "frontend/cbtconfig.h"

//Qt includes
#include <qobject.h>

//KDE includes
#include <khtml_part.h>

class KHTMLPart;

/**
 * The CPrinter class manages the print item queue and the printing of them to the printer.
 *
 * @author The BibleTime team
*/
class CPrinter : public QObject, public CDisplayRendering
{
	Q_OBJECT
public:
	class Item : public CDisplayRendering::KeyTreeItem {
	public:
		Item(const QString& key, CSwordModuleInfo* module, const Settings settings);
		Item(const QString& startKey, const QString& stopKey, CSwordModuleInfo* module, const Settings settings);
		Item(const Item& i);
    const QString& getAlternativeContent() const;
		const bool hasAlternativeContent() const {
			return !m_alternativeContent.isEmpty();
		};
		
	private:
		QString m_stopKey;
		QString m_alternativeContent;
	};

	CPrinter(
		QObject *parent,
		CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(), 
		CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults()
	);

	virtual ~CPrinter();
	
	void printKeyTree( KeyTree& );

protected:
	virtual const QString entryLink(const KeyTreeItem& item, CSwordModuleInfo* const module);
	virtual const QString renderEntry( const KeyTreeItem&, CSwordKey* = 0 );
	virtual const QString finishText(const QString& arg1, KeyTree& tree);

// 	virtual const QString renderKeyTree( KeyTree& );

private:
	KHTMLPart* m_htmlPart;
};

#endif
