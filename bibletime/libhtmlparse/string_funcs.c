/* libhtmlparse: replacements for non-ANSI string functions
   Mooneer Salem (mooneer@translator.cx) */

#include <string.h>

#ifdef NOSTRDUP

char *strdup (char *str) {
	char *tmp;

	tmp = calloc(strlen(str) + 1, 1);
	if (!tmp) return NULL;

	strcpy(tmp, str);
	return tmp;
}

#endif
