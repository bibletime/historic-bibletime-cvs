/***************************************************************************
                          gbfhtml.cpp  -  description
                             -------------------
    begin                : Thu Jun 24 1999
    copyright            : (C) 1999 by Torsten Uhlmann
    email                : TUhlmann@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//BibleTime includes
#include "cswordmoduleinfo.h"
#include "cswordbackend.h"
#include "util/cpointers.h"

#include "bt_gbfhtml.h"

//sytsme includes
#include <stdlib.h>
#include <stdio.h>

//Sword includes
#include <utilxml.h>

//Qt includes
#include <qregexp.h>
#include <qstring.h>


BT_GBFHTML::BT_GBFHTML() : sword::GBFHTML() {

  setTokenStart("<");
	setTokenEnd(">");
	setTokenCaseSensitive(true);
	
	setEscapeStringCaseSensitive(true);
	setPassThruUnknownEscapeString(true); //the HTML widget will render the HTML escape codes	

  
//	replaceTokenSubstitute("Rf", ")</span>");// end of footnote
  if (tokenSubMap.find("Rf") != tokenSubMap.end()) { //remove note tag
	  tokenSubMap.erase( tokenSubMap.find("Rf") );
  }

	replaceTokenSubstitute("FI", "<span class=\"italic\">"); // italics begin
	replaceTokenSubstitute("Fi", "</span>");

	replaceTokenSubstitute("FB", "<span class=\"bold\">"); // bold begin
	replaceTokenSubstitute("Fb", "</span>");

	replaceTokenSubstitute("FR", "<span class=\"jesuswords\">");
	replaceTokenSubstitute("Fr", "</span>");

	replaceTokenSubstitute("FU", "<u>"); // underline begin
	replaceTokenSubstitute("Fu", "</u>");

//	replaceTokenSubstitute("FO", "<cite>"); //  Old Testament quote begin
//	replaceTokenSubstitute("Fo", "</cite>");
	replaceTokenSubstitute("FO", "<span class=\"quotation\">"); //  Old Testament quote begin
	replaceTokenSubstitute("Fo", "</span>");


  replaceTokenSubstitute("FS", "<span class-\"sup\">"); // Superscript begin// Subscript begin
	replaceTokenSubstitute("Fs", "</span>");

  replaceTokenSubstitute("FV", "<span class=\"sub\">"); // Subscript begin
	replaceTokenSubstitute("Fv", "</span>");

//	replaceTokenSubstitute("TT", QString::fromLatin1(" <h1><font color=\"%1\">").arg(text_color).local8Bit());
//	replaceTokenSubstitute("Tt", "</font></h1>");
	replaceTokenSubstitute("TT", "<div class=\"booktitle\">");
	replaceTokenSubstitute("Tt", "</div>");

	replaceTokenSubstitute("TS", "<div class=\"sectiontitle\">");
	replaceTokenSubstitute("Ts", "</div>");

	replaceTokenSubstitute("PP", "<span class=\"poetry\">"); //  poetry  begin
	replaceTokenSubstitute("Pp", "</span>");

	replaceTokenSubstitute("Fn", "</font>"); //  font  end
	replaceTokenSubstitute("CL", "<br/>"); //  new line
	replaceTokenSubstitute("CM", "<!p><br/>"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired

  replaceTokenSubstitute("CG", "&gt;"); // literal greater-than sign
	replaceTokenSubstitute("CT", "&lt;"); // literal less-than sign

  replaceTokenSubstitute("JR", "<span class=\"right\">"); // right align begin
	replaceTokenSubstitute("JC", "<span class=\"center\">"); // center align begin
	replaceTokenSubstitute("JL", "</span>"); // align end
}

/** No descriptions */
char BT_GBFHTML::processText(sword::SWBuf& buf, const sword::SWKey * key, const sword::SWModule * module){
	GBFHTML::processText(buf, key, module);

	//we have to go through the text and put all text which belongs to strongs into span tags
	//the text might look like 
	//	Am Anfang<WH07225> schuf<WH01254><WTH8804> Gott<WH0430> Himmel<WH08064> und Erde<WH0776>.
	
	int pos = -1;
	int lastPos = -1;
	int posRev = 0;
	QString t = QString::fromUtf8(buf.c_str());
	QRegExp startRE("<W(T?)([GH]\\d+)>");
	QRegExp endRE("(^)|>");
	
	CSwordModuleInfo* m = CPointers::backend()->findModuleByName( module->Name() ); 
	if (m && (m->has(CSwordBackend::lemmas) || m->has(CSwordBackend::strongNumbers))) { //only parse if the module has strongs or lemmas
		do {
			pos = startRE.search(t, pos+1);
			posRev = endRE.searchRev(t,pos);
			if (pos == posRev+2) { //two morph/strongs which belong together
				posRev = endRE.searchRev(t,posRev-1);
			}
			
			if ((pos >= 0) && (posRev>= 0) && (posRev < pos) && (posRev > lastPos)) {//we found a valid text span
				bool isMorph = !startRE.cap(1).isEmpty();
				const QString value = startRE.cap(2);
	// 			qWarning("found %s", value.latin1());
				
				posRev = !posRev ? 0 : posRev+1;
				
				QString part = t.mid(posRev, pos-posRev);
				int end = t.find(">", pos+1);
				
				if (end > pos) {
					if (!part.isEmpty()) {					
						QString rep = 
							isMorph 
							? QString("<span morph=\"%1\">%2</span>").arg(value).arg(part)
							: QString("<span lemma=\"%1\">%2</span>").arg(value).arg(part);
						
						t.replace(posRev, end-posRev+1, rep);
						pos = posRev + rep.length() - (end-pos) - part.length()+1;
					}
					else {//found text is empty, this morph/lemma belongs to the one before (i.e. two following belonging to the same text)
						//remove the GBF tag
						t.remove(posRev, end-posRev+1);
						pos -= end-posRev+1;
						
						int attrPos = t.findRev(QRegExp("morph=|lemma="),posRev);
						if ((attrPos > 0) && (attrPos < posRev)) {
							//insert the new attribute here
							QString attr = QString::fromLatin1("%1=\"%2\" ").arg(isMorph ? "morph" : "lemma").arg(value);
							t.insert(attrPos, attr);
							pos+=attr.length();
						}
						
					}
				}			
			}
			
			lastPos = pos;
		}
		while (pos >= 0);
	 	
// 		qWarning("replaced: %s", t.latin1());
	}	

	buf = (const char*)t.utf8();
  return 1;
}

bool BT_GBFHTML::handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData) {
	if (!substituteToken(buf, token)) {  //more than a simple replace
  	const unsigned int tokenLength = strlen(token);
		unsigned long i;
    sword::SWBuf value;
    BT_UserData* myUserData = dynamic_cast<BT_UserData*>(userData);
    sword::SWModule* myModule = const_cast<sword::SWModule*>(myUserData->module); //hack to be able to call stuff like Lang()

    if (	 !strncmp(token, "WG", 2) 
				|| !strncmp(token, "WH", 2) 
				|| !strncmp(token, "WTG", 3) 
				|| !strncmp(token, "WTH", 3) )
		{
			buf.append('<');
			buf.append(token);
			buf.append('>');
		}
		else if (!strncmp(token, "RB", 2)) {
//			buf += "<span class=\"footnotepre\">";
			myUserData->hasFootnotePreTag = true;
		}

		else if (!strncmp(token, "RF", 2)) {
			/*if (myUserData->hasFootnotePreTag) {
				myUserData->hasFootnotePreTag = false;
				buf += "</span> ";
			}
			buf += "<span class=\"footnote\"> (";*/
			
			buf.appendFormatted(" <span class=\"footnote\" note=\"%s/%s/%s\">.</span> ", 
				myModule->Name(),
				myUserData->key->getShortText(),
				QString::number(myUserData->swordFootnote++).latin1()
			);
			userData->suspendTextPassThru = true;
		}
		else if (!strncmp(token, "Rf", 2)) { //end of footnote
			userData->suspendTextPassThru = false;
		}
		
		else if (!strncmp(token, "FN", 2)) {
			buf += "<font face=\"";
			for (i = 2; i < tokenLength; i++) {
				if(token[i] != '\"') {
					buf += token[i];
        }
      }
			buf += "\">";
		}
		else if (!strncmp(token, "CA", 2)) {	// ASCII value
			buf += (char)atoi(&token[2]);
		}		
		else {
			return GBFHTML::handleToken(buf, token, userData);
		}
	}
	return true;
}

