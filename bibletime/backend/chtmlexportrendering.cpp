//
// C++ Implementation: chtmlexportrendering
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

//Backend
#include "chtmlexportrendering.h"

#include "cdisplaytemplatemgr.h"
#include "clanguagemgr.h"
#include "cswordkey.h"
#include "cswordmoduleinfo.h"

//Util
#include "util/cpointers.h"
#include "util/scoped_resource.h"

//KDE includes
#include <klocale.h>

namespace Rendering {

CHTMLExportRendering::CHTMLExportRendering(const CHTMLExportRendering::Settings& settings, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions) 
	: m_displayOptions(displayOptions),
		m_filterOptions(filterOptions),
		m_settings(settings)
{
}

CHTMLExportRendering::~CHTMLExportRendering() {
}

const QString CHTMLExportRendering::renderEntry( const KeyTreeItem& i, CSwordKey* k) {
	if (i.hasAlternativeContent()) {
		QString ret = QString::fromLatin1("<div class=\"entry\"><div class=\"rangeheading\">%1</div>").arg(i.getAlternativeContent());
		
		if (i.hasChildItems()) {
			KeyTree const * tree = i.childList();
			
			for ( KeyTreeItem* c = tree->first(); c; c = tree->next() ) {
				ret.append( renderEntry( *c ) );
			}
		}

		ret.append("</div>");
		return ret; //WARNING: Return already here!
	}
		
		
	ListCSwordModuleInfo modules = i.modules();	
	util::scoped_ptr<CSwordKey> scoped_key( !k ? CSwordKey::createInstance(modules.first()) : 0 );
	
	CSwordKey* key = k ? k : scoped_key;
  
	QString renderedText = (modules.count() > 1) ? QString::fromLatin1("<tr>") : QString::null;

	// Only insert the table stuff if we are displaying parallel.
  // Otherwise, strip out he table stuff -> the whole chapter will be rendered in one cell!

  //declarations out of the loop for optimization
  QString entry;
  QString keyText;
  bool isRTL;
	
	//taken out of the loop for optimization
	QString preverseHeading;
	QString langAttr;
	
	ListCSwordModuleInfo::const_iterator end_modItr = modules.end();
	for (ListCSwordModuleInfo::const_iterator mod_Itr(modules.begin()); mod_Itr != end_modItr; ++mod_Itr) {
    key->module(*mod_Itr);
    key->key( i.key() );
    keyText = key->key();
    isRTL = ((*mod_Itr)->textDirection() == CSwordModuleInfo::RightToLeft);
		entry = QString::null;

		langAttr = ((*mod_Itr)->language()->isValid())
			? QString::fromLatin1("xml:lang=\"%1\" lang=\"%2\"")
				.arg((*mod_Itr)->language()->abbrev())//twice the same
				.arg((*mod_Itr)->language()->abbrev()) 
			: QString::fromLatin1("xml:lang=\"%1\" lang=\"%2\"")
				.arg((*mod_Itr)->module()->Lang()) //again twice times the same
				.arg((*mod_Itr)->module()->Lang());
		
		const QString key_renderedText = key->renderedText();
		
		if (m_filterOptions.headings) {
			AttributeValue::const_iterator it =
				(*mod_Itr)->module()->getEntryAttributes()["Heading"]["Preverse"].begin();
			const AttributeValue::const_iterator end =
				(*mod_Itr)->module()->getEntryAttributes()["Heading"]["Preverse"].end();
			
			for (; it != end; ++it) {
				preverseHeading = QString::fromUtf8(it->second.c_str());
				
				if (!preverseHeading.isEmpty()) {
					entry.append(
						QString::fromLatin1("<div %1 class=\"sectiontitle\">%1</div>")
							.arg(langAttr)
							.arg(preverseHeading)
					);
				}		
			}
		}
		
// 		entry += QString::fromLatin1("<%1 %2 %3 dir=\"%4\">") //linebreaks = div, without = span
		entry.append( 
			QString::fromLatin1("<%1 %2 %3 %4>") //linebreaks = div, without = span
				.arg(m_displayOptions.lineBreaks 
						? QString::fromLatin1("div") 
						: QString::fromLatin1("span")
				)
				.arg((modules.count() == 1) //insert only the class if we're not in a td
						? (	i.settings().highlight 
							? QString::fromLatin1("class=\"currententry\"") 
							: QString::fromLatin1("class=\"entry\"")
						)
						: QString::null
				)
				.arg(langAttr)
	// 			.arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"));
				.arg(isRTL ? QString::fromLatin1("dir=\"rtl\"") : QString::null) 
		);

 		//keys should normally be left-to-right, but this doesn't apply in all cases
// 		entry.append( QString::fromLatin1("<span dir=\"ltr\" class=\"entryname\">%1</span>").arg(entryLink(i, *mod_Itr)) );
		entry.append( QString::fromLatin1("<span class=\"entryname\">%1</span>").arg(entryLink(i, *mod_Itr)) );
		
		if (m_settings.addText) {
			//entry.append( QString::fromLatin1("<span %1>%2</span>").arg(langAttr).arg(key_renderedText) );
			entry.append( key_renderedText );
		}
		
		if (i.hasChildItems()) {
			KeyTree const * tree = i.childList();
			
			for (KeyTreeItem* c = tree->first(); c; c = tree->next()) {
				entry.append( renderEntry(*c) );
			}
		}
		
		entry.append(m_displayOptions.lineBreaks
			? QString::fromLatin1("</div>\n") 
			: QString::fromLatin1("</span>\n")
		);
		
  	if (modules.count() == 1) {
			renderedText.append( entry );
		}
  	else {
	    renderedText.append( 
				QString::fromLatin1("<td class=\"%1\" %2 dir=\"%3\">%4</td>\n")
					.arg( i.settings().highlight ? QString::fromLatin1("currententry") : QString::fromLatin1("entry"))
					.arg( langAttr )
					.arg( isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr") )
					.arg( entry ) 
			);
		}
	}
 	
	if (modules.count() > 1) {
		renderedText.append( QString::fromLatin1("</tr>\n") );
	}
	
  return renderedText;	
}

void CHTMLExportRendering::initRendering() {
  CPointers::backend()->setDisplayOptions( m_displayOptions );
  CPointers::backend()->setFilterOptions( m_filterOptions );
}

const QString CHTMLExportRendering::finishText( const QString& text, KeyTree& tree ) {
	ListCSwordModuleInfo modules = tree.collectModules();

	const CLanguageMgr::Language* const lang = modules.first()->language();

	CDisplayTemplateMgr* tMgr = CPointers::displayTemplateManager();
	CDisplayTemplateMgr::Settings settings;
	settings.modules = modules;
	settings.langAbbrev = ((modules.count() == 1) && lang->isValid())
		?	lang->abbrev() 
		: "unknown";

	return tMgr->fillTemplate(i18n("Export"), text, settings);
}

/*!
    \fn CHTMLExportRendering::entryLink( KeyTreeItem& item )
 */
const QString CHTMLExportRendering::entryLink( const KeyTreeItem& item, CSwordModuleInfo* ) {
	return item.key();
}

}; //end of namespace "Rendering"
