/*
 * A simple cif access library. Provide basic interface - no knowledge of
 * dictionaries.
 *
 * @(#) ciflib1.c 1.4 10/4/94
 *
 * written by Dave Stampf
 */

/*
 * CifRead - parse a cif
 */

#include <stdio.h>
#include "cif.h"

extern FILE *yyin;

int CifRead1(char* fname)
{
	int res;

	if (fname) {
			res = cifopen(fname);
			if (res == 0) {
				return yyparse();
			}
			else return res;
	} else {
		yyin = stdin;
		return yyparse();
	}
}

