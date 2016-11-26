/*
 * low level cif i/o routines
 *
 * @(#) locif.c 1.4 10/4/94
 *
 * written by Dave Stampf
 */

#include <stdio.h>
#include <stdlib.h>

int column = 0;
long line = 0;
extern FILE *yyin;

int cifopen(char* fname)
{
	yyin = fopen(fname,"r");
	if (!yyin) {
		perror("Failed to open yyin");
		exit(1);
	}
	return 0;
}

int cifclose()
{
	if (yyin) fclose(yyin);
	yyin = NULL;
	return 0;
}

int cifungetc(int c)
{
	if (c == 0) c = -1;
	return ungetc(c,yyin);
}

int cifget()
{
	int c;
	
	c = getc(yyin);
	if (c == EOF) return 0;
	return c;
}	
