/********* Read the file LICENSE for license details. *********/

#ifndef CPRINTER_H
#define CPRINTER_H

//BibleTime backend
#include "backend/cswordbackend.h"
#include "backend/cdisplayrendering.h"

//BibleTime frontend
#include "frontend/cbtconfig.h"

//Qt includes
#include <qobject.h>

class KHTMLPart;

namespace Printing {

	/**
	 * The CPrinter class manages the print item queue and the printing of them to the printer.
	 *
	 * @author The BibleTime team
	*/
class CPrinter : public QObject, public Rendering::CDisplayRendering {
		Q_OBJECT
public:
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

private:
		KHTMLPart* m_htmlPart;
	};

}

#endif
