/* libhtmlparse: Quick and simple HTML parser
   Mooneer Salem (mooneer@translator.cx) */

#ifndef LIBHTML_H
#define LIBHTML_H

#ifdef __cplusplus
extern "C" {
#endif

struct ArgvTable {
	char *arg, *val;
};

struct ErrorTree {
	unsigned int line;
	unsigned char type;
	char *problem;
};

int (*commentCallBack) (char *comment);
int (*commentStartCallBack) ();
int (*commentEndCallBack) ();
int (*declCallBack) (char *tag, struct ArgvTable *args, int numargs);
int (*startCallBack) (char *tag, struct ArgvTable *args, int numargs);
int (*endCallBack) (char *tag);
int (*textCallBack) (char *text);
int (*textStartCallBack) ();
int (*textEndCallBack) ();

struct ErrorTree *parse (const char *html);
struct ArgvTable *addArgToTable (struct ArgvTable *args, char *arg, char *val);
void freeArgs (struct ArgvTable *args);

#ifdef __cplusplus
}
#endif

#endif

