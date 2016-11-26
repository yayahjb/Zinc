/*
 * cifCat - display the contents of the cif file in a different way.
 *
 * This program is the basis of making CIF unix usable.
 *
 * @(#) cifCat.c 1.4 10/4/94
 *
 * written by Dave Stampf
 */

#include <stdio.h>

#include "cif.h"

int noComments = 0;

main(int argc, char** argv)
{
	/* handle args */

	argv++;
	argc--;

	while (--argc > 0) {
		if (**argv == '-') {
			if (*(*argv+1) == 'c') {
				noComments = 1;
			}
			argv++;
		} else break;

	}	
	if (argc == -1) {
		CifRead1(NULL);
	} else {
		CifRead1(*argv);
	}
}
