//
// C++ Implementation: cplaintextexportrendering
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

//Backend includes
#include "cplaintextexportrendering.h"
#include "cswordkey.h"

//Util
#include "util/scoped_resource.h"



namespace Rendering {

CPlainTextExportRendering::CPlainTextExportRendering(const CPlainTextExportRendering::Settings& settings, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions)
	: CHTMLExportRendering(settings, displayOptions, filterOptions)
{
}

CPlainTextExportRendering::~CPlainTextExportRendering() {
}

const QString CPlainTextExportRendering::renderEntry( const KeyTreeItem& i, CSwordKey*  ) {
	if (!m_settings.addText) {
		return QString::fromLatin1("%1\n").arg(i.key());
	}
		
	ListCSwordModuleInfo modules = i.modules();	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
	QString renderedText = QString::fromLatin1("%1:\n").arg(i.key());
		
  QString entry;
  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    key->key( i.key() );
		
		entry += key->strippedText() + "\n";
		renderedText += entry;
	}
	
	return renderedText;	
}

const QString CPlainTextExportRendering::finishText( const QString& oldText, KeyTree& ) {
	return oldText;
}

};
