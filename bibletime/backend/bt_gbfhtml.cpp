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

 	CSwordModuleInfo* m = CPointers::backend()->findModuleByName( module->Name() ); 
	if (m && !(m->has(CSwordBackend::lemmas) || m->has(CSwordBackend::strongNumbers))) { //only parse if the module has strongs or lemmas
		return 1;
	}

	//Am Anfang<WH07225> schuf<WH01254><WTH8804> Gott<WH0430> Himmel<WH08064> und Erde<WH0776>.
	QString result;
	
	QString t = QString::fromUtf8(buf.c_str());
	QRegExp tag("(<W[T]?[HG]\\d+>)+");
	
	QStringList list;
	int lastMatchEnd = 0;
	int pos = tag.search(t,0);
	
	while (pos != -1) {
		list.append(t.mid(lastMatchEnd, pos+tag.matchedLength()-lastMatchEnd));
	
		lastMatchEnd = pos+tag.matchedLength();
		pos = tag.search(t,pos+tag.matchedLength());
	}
	
	if (!t.right(t.length() - lastMatchEnd).isEmpty()) {
		list.append(t.right(t.length() - lastMatchEnd));
	}

	tag = QRegExp("<W(T?)([HG]\\d+)>");

	for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
		QString e = *it;
		
		const bool textPresent = (e.stripWhiteSpace().remove(QRegExp("[.,;:]")).left(1) != "<");
		if (!textPresent) {
			continue;
		}
		
		int pos = tag.search(e, 0);
		bool insertedTag = false;
		while (pos != -1) {
			const bool isMorph = !tag.cap(1).isEmpty();
			const QString value = tag.cap(2);
			
			if (value.isEmpty()) {
				break;
			}
			
			//insert the span
			if (!insertedTag) {
				e.replace(pos, tag.matchedLength(), "</span>");
 				pos += 7;
				
				QString rep = QString("<span lemma=\"%1\">").arg(value);
				e.prepend( rep );
				pos += rep.length();
			}
			else { //add the attribute to the existing tag
				e.remove(pos, tag.matchedLength());
				
				const int attrPos = e.find(QRegExp("morph=|lemma="),0);
				if (attrPos >= 0) {
					QString attr = QString::fromLatin1("%1=\"%2\" ").arg(isMorph ? "morph" : "lemma").arg(value);
					e.insert(attrPos, attr);
					
					pos += attr.length();
				}
			}
				
			insertedTag = true;
			pos = tag.search(e, pos);
		}
		
		result += e;
	}
	
	if (list.count()) {
		buf = (const char*)result.utf8();
	}
  
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
