/********* Read the file LICENSE for license details. *********/

//BibleTime includes
#include <stdlib.h>
#include "bt_osishtml.h"
#include "versekey.h"

#include "clanguagemgr.h"
#include "creferencemanager.h"
#include "cswordmoduleinfo.h"
#include "frontend/cbtconfig.h"
#include "util/cpointers.h"

#include <iostream>

//Sword includes
#include <swmodule.h>
#include <swbuf.h>
#include <utilxml.h>

//KDE includes
#include <klocale.h>

//Qt includes
#include <qstring.h>

using sword::SWBuf;
using sword::XMLTag;

using namespace Filters;

BT_OSISHTML::BT_OSISHTML() : sword::OSISHTMLHREF() {
	setPassThruUnknownEscapeString(true); //the HTML widget will render the HTML escape codes

	addTokenSubstitute("inscription", "<span class=\"inscription\">");
	addTokenSubstitute("/inscription","</span>");

	addTokenSubstitute("mentioned", "<span class=\"mentioned\">");
	addTokenSubstitute("/mentioned", "</span>");

	addTokenSubstitute("divineName", "<span class=\"name\"><span class=\"divine\">");
	addTokenSubstitute("/divineName", "</span></span>");

	addTokenSubstitute("seg type=\"morph\"", "<span class=\"morphSegmentation\">");
	addTokenSubstitute("/seg", "</span>");
}

bool BT_OSISHTML::handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData) {
	// manually process if it wasn't a simple substitution

	if (!substituteToken(buf, token)) {
		BT_UserData* myUserData = dynamic_cast<BT_UserData*>(userData);
		sword::SWModule* myModule = const_cast<sword::SWModule*>(myUserData->module); //hack

		XMLTag tag(token);
		//     qWarning("found %s", token);
		const bool osisQToTick = ((!userData->module->getConfigEntry("OSISqToTick")) || (strcmp(userData->module->getConfigEntry("OSISqToTick"), "false")));

		if (!tag.getName()) {
			return false;
		}

		// <div> tag
		if (!strcmp(tag.getName(), "div")) {
			//handle intro

			if ((!tag.isEmpty()) && (!tag.isEndTag())) { //start tag
				SWBuf type( tag.getAttribute("type") );

				if (type == "introduction") {
					buf.append("<div class=\"introduction\">");
				}
				else if (type == "chapter") {
					buf.append("<div class=\"chapter\" />"); //don't open a div here, that would lead to a broken XML structure
				}
				else {
					buf.append("<div>");
				}
			}
			else if (tag.isEndTag()) { //end tag
				buf.append("</div>");
			}
		}
		else if (!strcmp(tag.getName(), "w")) {
			if ((!tag.isEmpty()) && (!tag.isEndTag())) { //start tag
				const char *attrib;
				const char *val;

				XMLTag outTag("span");
				SWBuf attrValue;

				if ((attrib = tag.getAttribute("xlit"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outTag.setAttribute("xlit", val);
				}

				if ((attrib = tag.getAttribute("gloss"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outTag.setAttribute("gloss", val);
				}

				if ((attrib = tag.getAttribute("lemma"))) {
					const int count = tag.getAttributePartCount("lemma");
					int i = (count > 1) ? 0 : -1;  // -1 for whole value cuz it's faster, but does the same thing as 0
					attrValue = "";

					do {
						if (attrValue.length()) {
							attrValue.append( "|" );
						}

						attrib = tag.getAttribute("lemma", i);

						if (i < 0) { // to handle our -1 condition
							i = 0;
						}

						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;

						if ((*val == 'H') || (*val == 'G')) {
							attrValue.append(val);
						}
					}
					while (++i < count);

					if (attrValue.length()) {
						outTag.setAttribute("lemma", attrValue.c_str());
					}
				}

				if ((attrib = tag.getAttribute("morph"))) {
					const int count = tag.getAttributePartCount("morph");
					int i = (count > 1) ? 0 : -1;  // -1 for whole value cuz it's faster, but does the same thing as 0

					attrValue = "";

					do {
						if (attrValue.length()) {
							attrValue.append('|');
						}

						attrib = tag.getAttribute("morph", i);

						if (i < 0) {
							i = 0; // to handle our -1 condition
						}

						val = strchr(attrib, ':');

						if (val) { //the prefix gives the modulename
							attrValue.append( !strncmp(attrib, "x-", 2) ? attrib+2 : attrib );
						}
						else { //no prefix given
							const bool skipFirst = ((val[0] == 'T') && ((val[1] == 'H') || (val[1] == 'H')));
							attrValue.append( skipFirst ? val+1 : val );
						}
					}
					while (++i < count);

					if (attrValue.length()) {
						outTag.setAttribute("morph", attrValue.c_str());
					}
				}

				if ((attrib = tag.getAttribute("POS"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outTag.setAttribute("pos", val);
				}

				buf.append( outTag.toString() );
			}
			else if (tag.isEndTag()) { // end or empty <w> tag
				buf.append("</span>");
			}
		}

		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) { //start tag
				const SWBuf type( tag.getAttribute("type") );

				if (type == "crossReference") { //note containing cross references
					myUserData->inCrossrefNote = true;
					myUserData->noteType = BT_UserData::CrossReference;

					/*     //get the refList value of the right entry attribute
					     AttributeList notes = myModule->getEntryAttributes()["Footnote"];
					     bool foundNote = false;

					     SWBuf id( tag.getAttribute("osisID") );
					     SWBuf refList;

					     for (AttributeList::iterator list_it = notes.begin(); (list_it != notes.end()) && !foundNote; ++list_it ) {
					      for (AttributeValue::iterator val_it = list_it->second.begin(); (val_it != list_it->second.end()) && !foundNote; ++val_it ) {
					       if ((val_it->first == "osisID") && (val_it->second == id)) {
					        foundNote = true; //this break the loop
					        refList = list_it->second["refList"];
					       }
					      }
					     }

					     if (refList.length()) {
					      buf.append(" <span class=\"crossreference\" crossrefs=\"");
					      buf.append(refList.c_str());
					      buf.append("\"> ");

					           myUserData->noteType = BT_UserData::CrossReference;
					     }
					     else {
					      myUserData->noteType = BT_UserData::Unknown;
					     }*/

					buf.append("<span class=\"crossreference\">");
				}

				/* else if (type == "explanation") {
				     }
				     */
				else if (type == "strongsMarkup") {
					/**
					* leave strong's markup notes out, in the future we'll probably have
					* different option filters to turn different note types on or off
					*/

					myUserData->suspendTextPassThru = true;
					myUserData->noteType = BT_UserData::StrongsMarkup;
				}
				else if (type == "alternative") {
					//      qWarning("found alternative");
					//       buf.append(" <span class=\"alternative\">");
					myUserData->noteType = BT_UserData::Alternative;
					myUserData->suspendTextPassThru = true;
				}
				else {
					//           qWarning("found note in %s", myUserData->key->getShortText());
					buf.append(" <span class=\"footnote\" note=\"");
					buf.append(myModule->Name());
					buf.append('/');
					buf.append(myUserData->key->getShortText());
					buf.append('/');
					buf.append( QString::number(myUserData->swordFootnote++).latin1() ); //inefficient
					buf.append("\">*</span> ");

					myUserData->noteType = BT_UserData::Footnote;

					myUserData->suspendTextPassThru = true;
				}
			}
			else { //if (tag.isEndTag()) {
				Q_ASSERT(myUserData->noteType != BT_UserData::Unknown);

				if (myUserData->noteType == BT_UserData::CrossReference) {
					buf.append("</span> ");
					myUserData->suspendTextPassThru = false;
					myUserData->inCrossrefNote = false;
				}
				else if (myUserData->noteType == BT_UserData::Alternative) {
					buf.append(" <span class=\"alternative\" alternative=\"");
					buf.append(myUserData->lastTextNode);
					buf.append("\" title=\"");
					buf.append((const char*)i18n("Alternative text").utf8());
					buf.append("\" />");
				}

				myUserData->noteType = BT_UserData::Unknown;
				myUserData->suspendTextPassThru = false;
			}
		}

		// The <p> paragraph tag is handled by OSISHTMLHref
		else if (!strcmp(tag.getName(), "reference")) { // <reference> tag

			if (!tag.isEndTag() && !tag.isEmpty()) {
				QString ref( tag.getAttribute("osisRef") );
				QString hrefRef( ref );
				Q_ASSERT(!ref.isEmpty());

				if (!ref.isEmpty()) {
					//find out the mod, using the current module makes sense if it's a bible or commentary because the refs link into a bible by default.
					//If the osisRef is something like "ModuleID:key comes here" then the
					// modulename is given, so we'll use that one
					CSwordModuleInfo* mod = CPointers::backend()->findSwordModuleByPointer(myModule);
					Q_ASSERT(mod);
					if (!mod || (mod->type() != CSwordModuleInfo::Bible
							&& mod->type() != CSwordModuleInfo::Commentary)) {

						mod = CBTConfig::get
								  ( CBTConfig::standardBible );

						//       qWarning("setting standard bible module");
					}

					Q_ASSERT(mod);

					//if the osisRef like "GerLut:key" contains a module, use that
					int pos = ref.find(":");

					if ((pos >= 0) && ref.at(pos-1).isLetter() && ref.at(pos+1).isLetter()) {
						QString newModuleName = ref.left(pos);
						hrefRef = ref.mid(pos+1);

						/*      qWarning("found mod-key with mod=%s and key=%s",
						       newModuleName.latin1(), ref.latin1()
						      );
						*/

						if (CPointers::backend()->findModuleByName(newModuleName)) {
							mod = CPointers::backend()->findModuleByName(newModuleName);
						}
					}

					buf.append("<a href=\"");
					buf.append( //create the hyperlink with key and mod
						CReferenceManager::encodeHyperlink(
							mod->name(), hrefRef, CReferenceManager::typeFromModule(mod->type())
						).utf8()
					);
					buf.append("\" crossrefs=\"");
					buf.append((const char*)ref.utf8()); //ref must contains the osisRef module marker if there was any
					buf.append("\">");
				}
			}
			else if (tag.isEndTag()) {
				buf.append("</a>");
			}
			else { // empty reference marker
				// -- what should we do?  nothing for now.
			}

			/*   if (!myUserData->inCrossrefNote && !tag.isEndTag() && !tag.isEmpty() && tag.getAttribute("osisRef")) {
			        const char* ref = tag.getAttribute("osisRef");

			    buf.append("<span class=\"crossreference\" crossrefs=\"");
			    buf.append(ref);
			    buf.append("\">");
			   }
			   else if (!myUserData->inCrossrefNote && tag.isEndTag()) {
			     buf.append("</span>");
			   }
			   else { // empty reference marker
			    // -- what should we do?  nothing for now.
			   }*/
		}

		// <l> is handled by OSISHTMLHref
		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if (!tag.isEndTag() && !tag.isEmpty()) {
				buf.append("<div class=\"sectiontitle\">");
			}
			else if (tag.isEndTag()) {
				buf.append("</div>");
			}
			else { // empty title marker
				// what to do?  is this even valid?
				buf.append("<br/>");
			}
		}

		// <hi> highlighted text
		else if (!strcmp(tag.getName(), "hi")) {
			const SWBuf type = tag.getAttribute("type");

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "bold") {
					buf.append("<span class=\"bold\">");
				}
				else if (type == "illuminated") {
					buf.append("<span class=\"illuminated\">");
				}
				else if (type == "italic") {
					buf.append("<span class=\"italic\">");
				}
				else if (type == "line-through") {
					buf.append("<span class=\"line-through\">");
				}
				else if (type == "normal") {
					buf.append("<span class=\"normal\">");
				}
				else if (type == "small-caps") {
					buf.append("<span class=\"small-caps\">");
				}
				else if (type == "underline") {
					buf.append("<span class=\"underline\">");
				}
			}
			else if (tag.isEndTag()) { //all hi replacements are html spans
				buf.append("</span>");
			}
		}

		//name
		else if (!strcmp(tag.getName(), "name")) {
			const SWBuf type = tag.getAttribute("type");

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "geographic") {
					buf.append("<span class=\"name\"><span class=\"geographic\">");
				}
				else if (type == "holiday") {
					buf.append("<span class=\"name\"><span class=\"holiday\">");
				}
				else if (type == "nonhuman") {
					buf.append("<span class=\"name\"><span class=\"nonhuman\">");
				}
				else if (type == "person") {
					buf.append("<span class=\"name\"><span class=\"person\">");
				}
				else if (type == "ritual") {
					buf.append("<span class=\"name\"><span class=\"ritual\">");
				}
				else {
					buf.append("<span class=\"name\"><span>");
				}
			}
			else if (tag.isEndTag()) { //all hi replacements are html spans
				buf.append("</span></span> ");
			}
		}
		else if (!strcmp(tag.getName(), "transChange")) {
			SWBuf type( tag.getAttribute("type") );

			if ( !type.length() ) {
				type = tag.getAttribute("changeType");
			}

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "added") {
					buf.append("<span class=\"transchange\" title=\"");
					buf.append((const char*)i18n("Added text").utf8());
					buf.append("\"><span class=\"added\">");
				}
				else if (type == "amplified") {
					buf.append("<span class=\"transchange\"><span class=\"amplified\">");
				}
				else if (type == "changed") {
					buf.append("<span class=\"transchange\"><span class=\"changed\">");
				}
				else if (type == "deleted") {
					buf.append("<span class=\"transchange\"><span class=\"deleted\">");
				}
				else if (type == "moved") {
					buf.append("<span class=\"transchange\"><span class=\"moved\">");
				}
				else if (type == "tenseChange") {
					buf.append("<span class=\"transchange\"><span class=\"tenseChange\">");
				}
				else {
					buf.append("<span class=\"transchange\"><span>");
				}
			}
			else if (tag.isEndTag()) { //all hi replacements are html spans
				buf.append("</span></span>");
			}
		}
		else if (!strcmp(tag.getName(), "p")) {
			if (tag.isEmpty()) {
				buf.append("<p/>");
			}
		}

		// <q> quote
		else if (!strcmp(tag.getName(), "q")) {
			SWBuf type = tag.getAttribute("type");
			SWBuf who = tag.getAttribute("who");
			const char *lev = tag.getAttribute("level");
			int level = (lev) ? atoi(lev) : 1;

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				myUserData->quote.who = who;

				if(osisQToTick) //alternate " and '
					buf.append((level % 2) ? '\"' : '\'');

				if (who == "Jesus") {
					buf.append("<span class=\"jesuswords\">");
				}
			}
			else if (tag.isEndTag()) {
				if (myUserData->quote.who == "Jesus") {
					buf.append("</span>");
				}

				if (osisQToTick) { //alternate " and '
					buf.append((level % 2) ? '\"' : '\'');
				}

				myUserData->quote.who = "";
			}
		}

		// abbr tag
		else if (!strcmp(tag.getName(), "abbr")) {
			if (!tag.isEndTag() && !tag.isEmpty()) {
				const SWBuf expansion = tag.getAttribute("expansion");

				buf.append("<span class=\"abbreviation\" expansion=\">");
				buf.append(expansion);
				buf.append("\">");
			}
			else if (tag.isEndTag()) {
				buf.append("</span>");
			}
		}

		// <milestone> tag
		else if (!strcmp(tag.getName(), "milestone")) {
			const SWBuf type = tag.getAttribute("type");

			if ((type == "screen") || (type == "line")) {//line break
				buf.append("<br/>");
				userData->supressAdjacentWhitespace = true;
			}
		}
		else { //all tokens handled by OSISHTMLHref will run through the filter now
			return sword::OSISHTMLHREF::handleToken(buf, token, userData);
		}
	}

	return false;
}
