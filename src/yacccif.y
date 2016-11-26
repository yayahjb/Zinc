%token DATANAME DATAITEM BLOCKNAME LOOP LCOMMENT TCOMMENT

%{
/*
 * yacc description of a CIF 
 *
 * @(#) yacccif.y 1.12 10/4/94
 *
 * written by Dave Stampf
 */

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cif.h"
#include <alloca.h>
#include <ctype.h>

#define YYMAXLIMIT 100000

extern long line;
extern int column;

char* currentBlock = "";
long commentNum = 0;
extern int noComments;

%}
%union {
	char* str;
	struct tkn* tkn;
	struct clist* clist;
	struct namelist* namelist;
}

%type <str> LOOP DATANAME DATAITEM BLOCKNAME LCOMMENT TCOMMENT
%type <tkn> tLOOP tDATANAME tDATAITEM tBLOCKNAME 
%type <namelist> NAMES VALUES
%type <clist> PC
%%
CIF:	PC { pclist($1); } ZBLOCK BLOCKS	

PC:		{ $$ =  NULL; }
	| LCOMMENT PC	{
		clist* clp;
		clp = (clist*) malloc(sizeof(clist));
		clp->text = $1;
		clp->type = LCOMMENT;
		clp->next = $2;
		$$ = clp;
	}
	| TCOMMENT PC {
		clist* clp;
		clp = (clist*) malloc(sizeof(clist));
		clp->text = $1;
		clp->type = TCOMMENT;
		clp->next = $2;
		$$ = clp;
	}

ZBLOCK:	
	| PAIRS

BLOCKS:  
      | BLOCKS BLOCK

BLOCK: tBLOCKNAME { currentBlock = $1->name; pclist($1->comments); } PAIRS

tBLOCKNAME: BLOCKNAME  PC {
	tkn* tknp;
	tknp = (tkn*) malloc(sizeof(tkn));
	tknp->name = $1;
	tknp->comments = $2;
	$$ = tknp;
	}


PAIRS: PAIR			
     | PAIR PAIRS

tLOOP : LOOP PC {
			tkn* tknp;
			tknp = (tkn*) malloc(sizeof(tkn));
			tknp->name = NULL;
			tknp->comments = $2;
			$$ = tknp;
		}

tDATANAME : DATANAME PC {
			tkn* tknp;
			tknp = (tkn*) malloc(sizeof(tkn));
			tknp->name = $1;
			tknp->comments = $2;
			$$ = tknp;
		}

tDATAITEM : DATAITEM PC {
			tkn* tknp;
			tknp = (tkn*) malloc(sizeof(tkn));
			tknp->name = $1;
			tknp->comments = $2;
			$$ = tknp;
		}
PAIR:	tDATANAME tDATAITEM { 
			printf("%s\t%s\t\t%s\t\n",currentBlock,$1->name/*+1*/,$2->name);
			pclist($1->comments);
			pclist($2->comments);	
		}
	| tLOOP NAMES VALUES	{ 
           int numnames, numvalues;
            int index;
            namelist *names, *values;
			namelist *nlp;
			char* loopId;

            index = 0;
            values = $3;
            names = $2;
			/* find the first (alphabeticly) word in the names -> loop id */
			nlp = names;
			loopId = nlp->name->name/*+1*/;
			while (nlp->next) {
				nlp = nlp->next;
				if (strcmp(nlp->name->name/*+1*/,loopId) < 0) {
					loopId = nlp->name->name/*+1*/;
				}
			}	
            while (values) {
                printf("%s\t%s\t%d\t%s\t%s\n",currentBlock,
						names->name->name/*+1*/,index,values->name->name,loopId);
				if (index == 0) {
					pclist(names->name->comments);
				}
				pclist(values->name->comments);
                values = values->next;
                names = names->next;
                if (!names) {
                    names = $2;
                    index++;
                }
            }
			/*loopNum++; */
        }


NAMES: tDATANAME { namelist *nl;
				  nl = (namelist*) malloc(sizeof(namelist));
				  assert(nl);
				  nl->name = $1;
				  nl->next = NULL;
				  $$ = nl;
				}
	| tDATANAME NAMES	{ namelist *nl;
				  nl = (namelist*) malloc(sizeof(namelist));
				  assert(nl);
				  nl->name = $1;
				  nl->next = $2;
				  $$ = nl;
				}

VALUES: tDATAITEM 			{ namelist *nl;
				  nl = (namelist*) malloc(sizeof(namelist));
				  assert(nl);
				  nl->name = $1;
				  nl->next = NULL;
				  $$ = nl;
				}
	| tDATAITEM VALUES	{ namelist *nl;
				  nl = (namelist*) malloc(sizeof(namelist));
				  assert(nl);
				  nl->name = $1;
				  nl->next = $2;
				  $$ = nl;
				}
%%

int count(namelist* nl)
{
	int n = 0;

	while (nl) {
		n++;
		nl=nl->next;
	}
	return n;
}

int hash(char*s)
{
	int i = 0;
	while (*s) {
		i += *s++;
	}
	return i & 0x1f;
}

yyerror(char *s)
{
	printf("%s: Error at line %d column %d\n",s,line,column);
	return 1;
}

pclist(clist *clp)
{
	if (noComments) return;
	while (clp) {
		if (clp->type == TCOMMENT) {
				printf("%s\t)\t%d\t%s\t\n",currentBlock,commentNum++,clp->text);
		} else {
				printf("%s\t(\t%d\t%s\t\n",currentBlock,commentNum++,clp->text);
		}
		clp = clp->next;
	}
}

