/***************************************************************************
                     thmlhtml.cpp  -  ThML to HTML filter
                             -------------------
    begin                : 1999-10-27
    copyright            : 2001 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "bt_thmlhtml.h"

BT_ThMLHTML::BT_ThMLHTML() {
	setTokenStart("<");
	setTokenEnd(">");

/* THESE ARE NOT NEEDED IN BT

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);

	addEscapeStringSubstitute("nbsp", " ");
	addEscapeStringSubstitute("quot", "\"");
	addEscapeStringSubstitute("amp", "&");
	addEscapeStringSubstitute("lt", "<");
	addEscapeStringSubstitute("gt", ">");
	addEscapeStringSubstitute("brvbar", "|");
	addEscapeStringSubstitute("sect", "§");
	addEscapeStringSubstitute("copy", "©");
	addEscapeStringSubstitute("laquo", "«");
	addEscapeStringSubstitute("reg", "®");
	addEscapeStringSubstitute("acute", "´");
	addEscapeStringSubstitute("para", "¶");
	addEscapeStringSubstitute("raquo", "»");

	addEscapeStringSubstitute("Aacute", "Á");
	addEscapeStringSubstitute("Agrave", "À");
	addEscapeStringSubstitute("Acirc", "Â");
	addEscapeStringSubstitute("Auml", "Ä");
	addEscapeStringSubstitute("Atilde", "Ã");
	addEscapeStringSubstitute("Aring", "Å");
	addEscapeStringSubstitute("aacute", "á");
	addEscapeStringSubstitute("agrave", "à");
	addEscapeStringSubstitute("acirc", "â");
	addEscapeStringSubstitute("auml", "ä");
	addEscapeStringSubstitute("atilde", "ã");
	addEscapeStringSubstitute("aring", "å");
	addEscapeStringSubstitute("Eacute", "É");
	addEscapeStringSubstitute("Egrave", "È");
	addEscapeStringSubstitute("Ecirc", "Ê");
	addEscapeStringSubstitute("Euml", "Ë");
	addEscapeStringSubstitute("eacute", "é");
	addEscapeStringSubstitute("egrave", "è");
	addEscapeStringSubstitute("ecirc", "ê");
	addEscapeStringSubstitute("euml", "ë");
	addEscapeStringSubstitute("Iacute", "Í");
	addEscapeStringSubstitute("Igrave", "Ì");
	addEscapeStringSubstitute("Icirc", "Î");
	addEscapeStringSubstitute("Iuml", "Ï");
	addEscapeStringSubstitute("iacute", "í");
	addEscapeStringSubstitute("igrave", "ì");
	addEscapeStringSubstitute("icirc", "î");
	addEscapeStringSubstitute("iuml", "ï");
	addEscapeStringSubstitute("Oacute", "Ó");
	addEscapeStringSubstitute("Ograve", "Ò");
	addEscapeStringSubstitute("Ocirc", "Ô");
	addEscapeStringSubstitute("Ouml", "Ö");
	addEscapeStringSubstitute("Otilde", "Õ");
	addEscapeStringSubstitute("oacute", "ó");
	addEscapeStringSubstitute("ograve", "ò");
	addEscapeStringSubstitute("ocirc", "ô");
	addEscapeStringSubstitute("ouml", "ö");
	addEscapeStringSubstitute("otilde", "õ");
	addEscapeStringSubstitute("Uacute", "Ú");
	addEscapeStringSubstitute("Ugrave", "Ù");
	addEscapeStringSubstitute("Ucirc", "Û");
	addEscapeStringSubstitute("Uuml", "Ü");
	addEscapeStringSubstitute("uacute", "ú");
	addEscapeStringSubstitute("ugrave", "ù");
	addEscapeStringSubstitute("ucirc", "û");
	addEscapeStringSubstitute("uuml", "ü");
	addEscapeStringSubstitute("Yacute", "Ý");
	addEscapeStringSubstitute("yacute", "ý");
	addEscapeStringSubstitute("yuml", "ÿ");

	addEscapeStringSubstitute("deg", "°");
	addEscapeStringSubstitute("plusmn", "±");
	addEscapeStringSubstitute("sup2", "²");
	addEscapeStringSubstitute("sup3", "³");
	addEscapeStringSubstitute("sup1", "¹");
	addEscapeStringSubstitute("nbsp", "º");
	addEscapeStringSubstitute("pound", "£");
	addEscapeStringSubstitute("cent", "¢");
	addEscapeStringSubstitute("frac14", "¼");
	addEscapeStringSubstitute("frac12", "½");
	addEscapeStringSubstitute("frac34", "¾");
	addEscapeStringSubstitute("iquest", "¿");
	addEscapeStringSubstitute("iexcl", "¡");
	addEscapeStringSubstitute("ETH", "Ð");
	addEscapeStringSubstitute("eth", "ð");
	addEscapeStringSubstitute("THORN", "Þ");
	addEscapeStringSubstitute("thorn", "þ");
	addEscapeStringSubstitute("AElig", "Æ");
	addEscapeStringSubstitute("aelig", "æ");
	addEscapeStringSubstitute("Oslash", "Ø");
	addEscapeStringSubstitute("curren", "¤");
	addEscapeStringSubstitute("Ccedil", "Ç");
	addEscapeStringSubstitute("ccedil", "ç");
	addEscapeStringSubstitute("szlig", "ß");
	addEscapeStringSubstitute("Ntilde", "Ñ");
	addEscapeStringSubstitute("ntilde", "ñ");
	addEscapeStringSubstitute("yen", "¥");
	addEscapeStringSubstitute("not", "¬");
	addEscapeStringSubstitute("ordf", "ª");
	addEscapeStringSubstitute("uml", "¨");
	addEscapeStringSubstitute("shy", "­");
	addEscapeStringSubstitute("macr", "¯");
*/
	setTokenCaseSensitive(true);

	addTokenSubstitute("note place=\"foot\"", " <small>(");
	addTokenSubstitute("/note", ")</small> ");
	addTokenSubstitute("foreign lang=\"el\"", "<font face=\"SIL Galatia\">");
	addTokenSubstitute("foreign lang=\"he\"", "<font face=\"SIL Ezra\">");
	addTokenSubstitute("/foreign", "</font>");

}

bool BT_ThMLHTML::handleToken(char **buf, const char *token, DualStringMap &userData) {
	unsigned long i;
	if (!substituteToken(buf, token)) {
	// manually process if it wasn't a simple substitution
		if (!strncmp(token, "sync type=\"Strongs\" value=\"", 27) && (token[27] == 'H')) {
			pushString(buf, QString(" <font color=\"%1\"><small><em><a href=\"sword://strongs_hebrew/").arg(strongs_color).utf8());
//			pushString(buf, "<a href=\"");
			for (i = 5; i < strlen(token)-1; i++)
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
			for (i = 28; i < strlen(token)-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "&gt;</a></em></small></font> ");
//			pushString(buf, "</a>");
		}
#warning not handled: token[27] == 'A')
		if (!strncmp(token, "sync type=\"Strongs\" value=\"", 27) && (token[27] == 'G')) {
			pushString(buf, QString(" <font color=\"%1\"><small><em><a href=\"sword://strongs_greek/").arg(strongs_color).utf8());
//			pushString(buf, "<a href=\"");
			for (i = 5; i < strlen(token)-1; i++)
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
			for (i = 28; i < strlen(token)-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "&gt;</a></em></small></font> ");
//			pushString(buf, "</a>");
		}
		else if (!strncmp(token, "sync type=\"Morph\" value=\"", 25)) {
			pushString(buf, QString(" <font color=\"%1\"><small><em><a href=\"sword://morph/").arg(morph_color).utf8());
//			pushString(buf, "<a href=\"");
			for (i = 5; i < strlen(token)-1; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf, "\">(");
			for (i = 28; i < strlen(token)-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf, ")</a></em></small></font>");
//			pushString(buf, "</a>");
		}

		else if (!strncmp(token, "scripRef p", 10) || !strncmp(token, "scripRef v", 10)) {
			userData["inscriptRef"] = "true";
#warning make color costomizable
			pushString(buf, "<a href=\"sword://bible/");
			for (i = 9; i < strlen(token)-1; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf,"\">");
		}

		// we're starting a scripRef like "<scripRef>John 3:16</scripRef>"
		else if (!strcmp(token, "scripRef")) {
			userData["inscriptRef"] = "false";
			// let's stop text from going to output
			userData["suspendTextPassThru"] = "true";
		}

		// we've ended a scripRef
		else if (!strcmp(token, "/scripRef")) {
			if (userData["inscriptRef"] == "true") { // like  "<scripRef passage="John 3:16">John 3:16</scripRef>"
				userData["inscriptRef"] = "false";
				pushString(buf, "</a>");
			}
			
			else { // like "<scripRef>John 3:16</scripRef>"
				pushString(buf, "<a href=\"sword://bible/");
				pushString(buf, userData["lastTextNode"].c_str());
				pushString(buf, "\">");
				pushString(buf, userData["lastTextNode"].c_str());
				// let's let text resume to output again
				userData["suspendTextPassThru"] = "false";	
				pushString(buf, "</a>");
			}
		}
			
		else if (!strncmp(token, "div class=\"sechead\"", 19)) {
			userData["SecHead"] = "true";
			pushString(buf, "<br /><b><i>");
		}
		else if (!strncmp(token, "/div", 4)) {
			if (userData["SecHead"] == "true") {
				pushString(buf, "</i></b><br />");
				userData["SecHead"] = "false";
			}
		}

#warning what is that?
/*	else if (!strncmp(token, "sync type=\"Strongs\" value=\"T", 28)) {
			pushString(buf, "<a href=\"");
			for (i = 5; i < strlen(token)-1; i++)
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
			for (i = 29; i < strlen(token)-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "</a>");
		}

		else {
			*(*buf)++ = '<';
			for (i = 0; i < strlen(token); i++)
				*(*buf)++ = token[i];
				*(*buf)++ = '>';
			//return false;  // we still didn't handle token
		}*/
	}
	return true;
}


/*

char BT_ThMLHTML::ProcessText(char *text, int maxlen)
{
  char *to, *from, token[2048];
  int tokpos = 0;
  bool intoken 	= false;
  int len;
  bool ampersand = false;

  len = strlen(text) + 1;						// shift string to right of buffer
  if (len < maxlen) {
    memmove(&text[maxlen - len], text, len);
    from = &text[maxlen - len];
  }
  else	from = text;							// -------------------------------
  for (to = text; *from; from++)
    {
      if (*from == '<') {
	intoken = true;
	tokpos = 0;
	memset(token, 0, 2048);
	ampersand = false;
	continue;
      }
      else if (*from == '&') {
	intoken = true;
	tokpos = 0;
	memset(token, 0, 2048);
	ampersand = true;
	continue;
      }
	// process desired tokens
	if (!strncmp(token, "sync type=\"Strongs\" value=\"", 27) && (token[27] == 'H' || token[27] == 'G' || token[27] == 'A')) {
	  *to++ = '<';
	  *to++ = 'S';
	  *to++ = 'M';
	  *to++ = 'A';
	  *to++ = 'L';
	  *to++ = 'L';
	  *to++ = '>';
	  *to++ = '<';
	  *to++ = 'E';
	  *to++ = 'M';
	  *to++ = '>';
	  for (unsigned int i = 28; token[i] != '\"'; i++)
	    *to++ = token[i];
	  *to++ = '<';
	  *to++ = '/';
	  *to++ = 'E';
	  *to++ = 'M';
	  *to++ = '>';
	  *to++ = '<';
	  *to++ = '/';
	  *to++ = 'S';
	  *to++ = 'M';
	  *to++ = 'A';
	  *to++ = 'L';
	  *to++ = 'L';
	  *to++ = '>';
	}
	else if (!strncmp(token, "sync type=\"Morph\" value=\"", 25)) {
	  *to++ = '<';
	  *to++ = 'S';
	  *to++ = 'M';
	  *to++ = 'A';
	  *to++ = 'L';
	  *to++ = 'L';
	  *to++ = '>';
	  *to++ = '<';
	  *to++ = 'E';
	  *to++ = 'M';
	  *to++ = '>';
	  for (unsigned int i = 25; token[i] != '\"'; i++)
	    *to++ = token[i];
	  *to++ = '<';
	  *to++ = '/';
	  *to++ = 'E';
	  *to++ = 'M';
	  *to++ = '>';
	  *to++ = '<';
	  *to++ = '/';
	  *to++ = 'S';
	  *to++ = 'M';
	  *to++ = 'A';
	  *to++ = 'L';
	  *to++ = 'L';
	  *to++ = '>';
	}
	else if (!strncmp(token, "scripRef version", 16)) {
	  *to++ = '<';
	  *to++ = 'A';
	  *to++ = ' ';
	  *to++ = 'H';
	  *to++ = 'R';
	  *to++ = 'E';	
	  *to++ = 'F';	
	  *to++ = '=';
	  *to++ = '\"';	
 	  *to++ = 's';	
	  *to++ = 'w';	 	
	  *to++ = 'o';		
	  *to++ = 'r';
	  *to++ = 'd';
	  *to++ = ':';
	  *to++ = '/';
	  *to++ = '/';	
	  *to++ = '/'; //no specific module	
	  for (unsigned int i = 32; token[i] != '\"'; i++)
	    *to++ = token[i];
	  *to++ = '\"';
	  *to++ = '>';
  	} 
	else if (!strncmp(token, "/scripRef", 9)) { 
	  *to++ = ' ';
	  *to++ = '<';
	  *to++ = '/';
	  *to++ = 'A';
	  *to++ = '>';	  
	}	
	else if (!strncmp(token, "sync type=\"Strongs\" value=\"T", 28)) {
	  *to++ = '<';
	  *to++ = 'S';
	  *to++ = 'M';
	  *to++ = 'A';
	  *to++ = 'L';
	  *to++ = 'L';
	  *to++ = '>';
	  *to++ = '<';
	  *to++ = 'I';
	  *to++ = '>';
	  for (unsigned int i = 29; token[i] != '\"'; i++)
	    *to++ = token[i];
	  *to++ = '<';
	  *to++ = '/';
	  *to++ = 'I';
	  *to++ = '>';
	  *to++ = '<';
	  *to++ = '/';
	  *to++ = 'S';
	  *to++ = 'M';
	  *to++ = 'A';
	  *to++ = 'L';
	  *to++ = 'L';
	  *to++ = '>';
	}
	else if (!strncmp(token, "note place=\"foot\"", 17)) {
	  *to++ = ' ';
	  *to++ = '<';
	  *to++ = 'S';
	  *to++ = 'M';
	  *to++ = 'A';
	  *to++ = 'L';
	  *to++ = 'L';
	  *to++ = '>';
	  *to++ = '(';
	}
	else if (!strncmp(token, "/note", 5)) {
	  *to++ = ')';
	  *to++ = '<';
	  *to++ = '/';
	  *to++ = 'S';
	  *to++ = 'M';
	  *to++ = 'A';
	  *to++ = 'L';
	  *to++ = 'L';
	  *to++ = '>';
	  *to++ = ' ';
	}
	else if (!strncmp(token, "foreign lang=\"el\"", 17)) {
	  *to++ = '<';
	  *to++ = 'F';
	  *to++ = 'O';
	  *to++ = 'N';
	  *to++ = 'T';
	  *to++ = ' ';
	  *to++ = 'F';
	  *to++ = 'A';
	  *to++ = 'C';
	  *to++ = 'E';
	  *to++ = '=';
	  *to++ = '\"';
	  *to++ = 'S';
	  *to++ = 'I';
	  *to++ = 'L';
	  *to++ = ' ';
	  *to++ = 'G';
	  *to++ = 'a';
	  *to++ = 'l';
	  *to++ = 'a';
	  *to++ = 't';
	  *to++ = 'i';
	  *to++ = 'a';
	  *to++ = '\"';
	  *to++ = '>';
	}
	else if (!strncmp(token, "foreign lang=\"he\"", 17)) {
	  *to++ = 'F';
	  *to++ = 'O';
	  *to++ = 'N';
	  *to++ = 'T';
	  *to++ = ' ';
	  *to++ = 'F';
	  *to++ = 'A';
	  *to++ = 'C';
	  *to++ = 'E';
	  *to++ = '=';
	  *to++ = '\"';
	  *to++ = 'S';
	  *to++ = 'I';
	  *to++ = 'L';
	  *to++ = ' ';
	  *to++ = 'E';
	  *to++ = 'z';
	  *to++ = 'r';
	  *to++ = 'a';
	  *to++ = '\"';
	  *to++ = '>';
	}
	else if (!strncmp(token, "/foreign", 8)) {
	  *to++ = '<';
	  *to++ = '/';
	  *to++ = 'F';
	  *to++ = 'O';
	  *to++ = 'N';
	  *to++ = 'T';
	  *to++ = '>';
	}
	else {
	  *to++ = '<';
	  for (unsigned int i = 0; i < strlen(token); i++)
	    *to++ = token[i];
	  *to++ = '>';
	}
        continue;
      }
	if (intoken) {
		if (tokpos < 2047)
			token[tokpos++] = *from;
     }
	else	*to++ = *from;
  }
  *to++ = 0;
  *to = 0;
  return 0;
}

*/

