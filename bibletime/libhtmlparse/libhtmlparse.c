/* libhtmlparse: Quick and simple HTML parser
   Mooneer Salem (mooneer@translator.cx) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libhtmlparse.h"

static unsigned short numArgs;

struct ArgvTable *addArgToTable (struct ArgvTable *args, char *arg, char *val) {
        numArgs++;
	if (!args) {
		args = malloc(sizeof(struct ArgvTable)*numArgs); memset(args, '\0', sizeof(args));
	} else args = realloc(args, sizeof(struct ArgvTable)*numArgs);

	if (!args) {
		printf("memory error\n");
		exit(EXIT_FAILURE);
	}

        args[numArgs-1].arg = arg; 
        args[numArgs-1].val = val; 
	return args;
}

void freeArgs (struct ArgvTable *args) {   
	if(args) {
		free(args);
		args=NULL;
		numArgs=0; 
	}
}

#ifdef NOSTRDUP
extern char *strdup (char *string);
#endif

typedef enum {FALSE=0, TRUE} Boolean;

struct ErrorTree *parse (const char *html) {
	/* Will use below variable later when error detection is introduced. */
	/*struct ErrorTree *errors;*/
	char *tag=NULL, *text=NULL, *comment=NULL;
	struct ArgvTable *args;
	char *tmp, *arg=NULL, *val=NULL, *html2;
	/*unsigned char istag, istrue, tagended;*/
	Boolean istag, istrue, tagended;

	html2 = strdup(html);
	tmp = html2;

	while(*tmp) {
		if (*tmp == '<' || istag) {
			istag = 0;
			if (*++tmp == '!') {
				comment:
				if (*++tmp == '-' || *(tmp+1) == '-') {
					tmp += 2;
					while(isspace(*tmp++));
					tmp--;
					comment = tmp;
					(*commentStartCallBack)();
					while(*tmp && !(*tmp == '-' && *(tmp+1) == '-' && *(tmp+2) == '>')) tmp++;
					if (*tmp) {
						while(isspace(*--tmp));
						*++tmp = '\0';
						while(isspace(*++tmp));
						tmp += 2;
					}
					(*commentCallBack)(comment); 
					(*commentEndCallBack)();
					tmp++;
					continue;
				} else {
					tmp--;
					goto starttag;
				}
			} else if (*tmp == '/' || *(tmp-1) == '/') {
				if (*tmp == '/') tmp++;
				tag = tmp;
				while(*tmp && *tmp != '>') tmp++;
				if (*tmp) *tmp = '\0';
				(*endCallBack)(tag);
				tmp++;
				continue;
			} else {
				starttag:
				if (*(tmp-1) != '<') tmp--;
				if (*tmp == '!' && *(tmp+1) == '-') goto comment;
				tag = tmp;
				while(*tmp && *tmp != '>' && !isspace(*tmp)) tmp++;
				if (*tmp == '>') {
					*tmp = '\0';
					if (*tag == '!') {
						tag++;
						(*declCallBack)(tag, NULL, 0);
					} else (*startCallBack)(tag, NULL, 0);
					tmp++;
					continue;
				} else if (isspace(*tmp)) {
					*tmp = '\0';
					while(isspace(*++tmp));
				} else {
					if (*tag == '!') {
						tag++;
						(*declCallBack)(tag, NULL, 0);
					} else (*startCallBack)(tag, NULL, 0);
					break;
				}

				args = NULL;
				numArgs=0;
				while(*tmp) {
					istrue=0; tagended=0;
					while(isspace(*tmp++));
					if (!isspace(*(tmp-1))) tmp--;
					arg = tmp;
					if (*arg == '"' || *arg == '\'') {
						tmp++; arg = tmp;
						while(*tmp && !(*tmp == '"' && *(tmp-1) != '\\' && *tmp != '\'')) tmp++;
						*tmp++ = '\0';
						args = addArgToTable(args, arg, val);
						if (*tmp != '>') continue;
						if (*(tmp+1) == '>') {
							*tmp++ = '\0';
						}
						arg=NULL;
						break;
					}

					val = NULL;
					while(*tmp != '=' && !isspace(*tmp) && *tmp != '>') tmp++;

					if (!isspace(*tmp) && *tmp != '>') istrue=1;
					if (*tmp == '>') tagended=1;

					*tmp = '\0';
					tmp++;

					if (istrue) {
						if (*tmp != '\'' && *tmp != '"') {
							val = tmp;
							while(!isspace(*tmp) && *tmp != '>') tmp++;
							if (*tmp == '>') {
								*tmp = '\0';
								args = addArgToTable(args, arg, val);
								/*tmp++;*/
								break;
							} else {
								*tmp++ = '\0';
								args = addArgToTable(args, arg, val);
								/*while(isspace(*tmp++));*/
								continue;
							}
						} else {
							tmp++;
							val = tmp;
							while(*tmp && (*tmp != '\'' || (*tmp == '\'' && *(tmp-1) == '\\')) && (*tmp != '"' || (*tmp == '"' && *(tmp-1) == '\\'))) tmp++;
							if (*tmp == '>') {
								*tmp = '\0';
								tmp++;
								args = addArgToTable(args, arg, val);
								break;
							} else if (*(tmp+1) == '>') {
                                                                *tmp = '\0';
                                                                tmp++;
								args = addArgToTable(args, arg, val);
								break;
							} else {
								*tmp = '\0';
								tmp += 2;
								args = addArgToTable(args, arg, val);
							}
						}
					} else {
						args = addArgToTable(args, arg, val);
						/*tmp--;*/
						if (!tagended) continue;
						tagended = 0; tmp--; break;
					}
				}

				if (*tag == '!') {
					tag++;
					(*declCallBack)(tag, args, numArgs);
				} else (*startCallBack)(tag, args, numArgs);

				freeArgs(args);
				tmp++;
				continue;
			}				
		} else {
			text = tmp;
			if (*(text-1) == '!') {
				tmp--;
				if (*(text-1) == '<') {
					tmp--;
					continue;
				}
			}
			while(isspace(*tmp) && *tmp != '<' && *tmp) tmp++;
			if (*tmp == '<' && !isspace(*(tmp+1))) {
				continue;
			} else if (!*tmp) break;

			(*textStartCallBack)();
			for(;;) {
				while(*tmp && *tmp != '<') tmp++;
				if (*tmp == '<') {
					if (isspace(*(tmp + 1))) {
						tmp++;
						continue;
					} else istag = 1;
				}
				break;
			}
			*tmp = '\0';
			(*textCallBack)(text);
			(*textEndCallBack)();
			tmp++;
			continue;
		}
	}

	free(html2);

	return NULL;
}


