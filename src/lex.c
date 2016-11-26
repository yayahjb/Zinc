/*
 * lex.c - a program to read in a valid cif and break it into standard tokens.
 *
 * @(#) lex.c 1.3 10/4/94
 *
 * written by Dave Sampf
 */

#include "y.tab.h"
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define cifget()	(getc(yyin))
#define cifungetc( xc )	(ungetc((xc),yyin))
void reset (void);
void appendc(char c);

char* s = 0;
int slength = 0;
int ns = 0;
char* strdup1(char* s, int n);
FILE *yyin;
extern long line;

int yylex()
{
	static int startOfLine = 1;
	static int firstNonWhite = 1;
	
	int c;

	/* The basic flow - skip blanks, get token. */

	while (((c = cifget()) != EOF) && isspace(c)) { 
		if (c == '\n') {
				line++;
				startOfLine = 1;
				firstNonWhite = 1;
		}
	}

	if (c == EOF) return 0;

	/* See if this is a block, or not */

	reset();
	if (startOfLine && c == ';') {	/* handle blocks. */
		startOfLine = 0;
		appendc(c);
		while (1) {
			c = cifget();
			if (startOfLine && (c == ';')) {
				/* RemoveMultipleBlanks(s); */
				/* RemoveFirstLineBlanks(s); */
				appendc(c);
				yylval.str = strdup1(s,slength);
				return DATAITEM;
			} else {
				startOfLine = (c == '\n');
				if (c != '\n') {
					appendc(c);
				} else {
					appendc('\\');
					appendc('n');
					line++;
				}
			}
		}
	} else {
		char cnext;
		if (c == '"') {
			/* capture to the next " */
			firstNonWhite = 0;
			appendc(c);
			while ((c = cifget()) != EOF) {
				if ( c == '\n' ) {
					appendc('"');
					cifungetc(c);
					break;
				}
				if ( c != '"' ) {
					appendc(c);
					continue;
				}
				cnext = cifget();
				cifungetc(cnext);
				if (cnext == '\n' || cnext == EOF || cnext == ' ' || cnext == '\t') {
					break;
				}
				appendc(c);
                        }
			appendc(c);
			yylval.str = strdup1(s,slength);
			return DATAITEM;
		} else if (c == '\'') {
			firstNonWhite = 0;
			appendc(c);
			while ((c = cifget()) != EOF) {
				if ( c == '\n' ) {
					appendc('\'');
					cifungetc(c);
					break;
				}
				if ( c != '\'' ) {
					appendc(c);
					continue;
				}
				cnext = cifget();
				cifungetc(cnext);
				if (cnext == '\n' || cnext == EOF || cnext == ' ' || cnext == '\t') {
					break;
				}
				appendc(c);
                        }
			appendc(c);
			yylval.str = strdup1(s,slength);
			return DATAITEM;
		} else {
				if (c == '#') {
					appendc(c);
					while ((c = cifget()) != '\n') {
						appendc(c);
					}
					startOfLine = 1;
					line++;
					yylval.str = strdup1(s,slength);
					if (firstNonWhite) {
						return LCOMMENT;
					}
					firstNonWhite = 1;
					return TCOMMENT;
				}
				appendc(c);
				while (1) {
					c = cifget();
					startOfLine = c == '\n';
					if (c == '\n') line++;
					if ((c == EOF) || (isspace(c))) {
						break;
					} else {
						appendc(c);
					}	
				}
				firstNonWhite = startOfLine;
				if (strncmp(s,"loop_",5) == 0) {
					/* yylval.str = 0; */
					return LOOP;
				} else if (strncmp(s,"data_",5) == 0) {
					slength -= 5;
					yylval.str = strdup1(s+5,slength);
					return BLOCKNAME;
				} else if (strncmp(s,"global_",7) == 0) {
					yylval.str = strdup1(s,slength);
					return BLOCKNAME;
				} else if (*s == '_') {
					yylval.str = strdup1(s,slength);
					return DATANAME;
				} else {
					yylval.str = strdup1(s,slength);
					return DATAITEM;
				}
		}
	}
}


void appendc(char c)
{
    int n = slength;

    slength++;
    while (ns < slength + 1) {
        char *tmp;
        ns = 2*ns;
        tmp = (char*) malloc(ns);
        assert(tmp);
        strcpy(tmp,s);
        free(s);
        s = tmp;
    }
	*(s+n) = c;
}
	
void reset()
{
    if (ns == 0) {
        s = (char*) malloc(64);
        assert(s);
        ns = 64;
    }
    s[0] = 0;
    slength = 0;
}
	
int yywrap()
{
    return 1;
}

void RemoveFirstLineBlanks(char* s)
{
        char *p = s;

        /* remove all leading blanks up to and including first newline */

		s[slength] = 0;
        while (*s && (*s == ' ')) {
                s++;
                slength--;
        }

		if ((*s == '\\') && (s[1] == 'n')) {
			s++; s++;
			slength--; slength--;
		}

        while (*s) {
			*p++ = *s++;
		}
		*p = 0;
}

void RemoveMultipleBlanks(char* s)
{
        int flag = 0;
        char *p = s;

        /* first, remove all leading blanks */

		s[slength] = 0;
        while (*s && (*s == ' ')) {
                s++;
                slength--;
        }

        while (*s) {
                if (*s == ' ') {
                        if (flag) {
                                s++;
                                slength--;
                        } else {
                                *p++ = *s++;
                                flag = 1;
                        }
                } else {
                        *p++ = *s++;
                        flag = 0;
                }
        }
        *p = 0;
}

char* strdup1(char* s, int n)
{
        char *t;

		s[slength] = 0;
		assert(n >= slength);
        t = (char*) malloc(n + 1);
        strcpy(t,s);
        return t;
}

