#
# %Z% %M% %I% %G%
#

CFLAGS = -g -Wall -O3

PREFIX ?= $(HOME)
INSTDIR = $(PREFIX)/bin
PERL = /usr/local/bin/perl
WISH = /usr/local/bin/wish

all:	cifZinc zincCif zb zincNl cifdiff zincSubset zincGrep

cifZinc : cifCat.o lex.o y.tab.o locif.o ciflib1.o
	$(CC) $(CFLAGS) cifCat.o lex.o y.tab.o locif.o ciflib1.o -o cifZinc

cifCat.o : cifCat.c cif.h
	$(CC) -c cifCat.c

zincCif : zincCif.dist
	sed s,PERL,$(PERL), zincCif.dist > zincCif
	chmod +x zincCif

zb : zb.dist
	sed s,WISH,$(WISH), zb.dist > zb
	chmod +x zb

zincNl : zincNl.dist
	sed s,PERL,$(PERL), zincNl.dist > zincNl
	chmod +x zincNl

cifdiff :
	chmod +x cifdiff

zincSubset :
	chmod +x zincSubset

locif.o : locif.c
	cc -c $(CFLAGS) locif.c

y.tab.o : y.tab.c
	$(CC) -c $(CFLAGS) y.tab.c

y.tab.c: yacccif.y
	bison -y -d yacccif.y

y.tab.h: yacccif.y
	bison -y -d yacccif.y

cifread.o: cifread.c cif.h
	$(CC) -c $(CFLAGS) cifread.c

ciflib1.o : ciflib1.c cif.h
	$(CC) -c $(CFLAGS) ciflib1.c

libcif.a : ciflib1.o y.tab.o lex.yy.o locif.o
	ar r libcif.a ciflib1.o y.tab.o lex.yy.o locif.o

lex.o : lex.c y.tab.h
	$(CC) -c $(CFLAGS) lex.c

testnl : testnl.f
	$(FC) testnl.f -o testnl

dist :	Makefile cif.h cifCat.c cifdiff ciflib1.c lex.c yacccif.y zb.dist zincCif.dist zincGrep zincNl.dist zincSubset locif.c zincGrep coords testnl.f simple1.cif simple2.cif testnl
	tar -cvf dist Makefile cif.h cifCat.c cifdiff ciflib1.c lex.c yacccif.y zb.dist zincCif.dist zincGrep zincNl.dist zincSubset locif.c zincGrep testnl.f simple1.cif simple2.cif coords testnl

install:	cifZinc zincCif zb zincNl cifdiff zincSubset zincGrep
	install -f $(INSTDIR) cifZinc zincCif zb zincNl cifdiff zincSubset zincGrep

clean:
	rm -rf *.o core lex.yy.c y.tab.c y.tab.h 

