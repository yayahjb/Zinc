/*
 *
 * @(#) cif.h 1.2 9/12/94
 */

typedef struct clist {
	char* text;
	int type;
	struct clist *next;
} clist;

typedef struct tkn {
	char* name;
	clist* comments;
} tkn;

typedef struct namelist {
        tkn* name;
        struct namelist* next;
} namelist;


