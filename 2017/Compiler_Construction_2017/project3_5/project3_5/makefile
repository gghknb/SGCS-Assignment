all: project3_5
project3_5: main.o lex.yy.o util.o cm.tab.o analyze.o symtab.o
	gcc main.o lex.yy.o util.o cm.tab.o analyze.o symtab.o -o project3_5
main.o: cm.tab.o main.c
	gcc -c main.c
lex.yy.c: cm.tab.o tiny.l
	flex tiny.l
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
util.o: util.c
	gcc -c util.c
cm.tab.c: cm.y
	bison -d cm.y
cm.tab.o: cm.tab.c
	gcc -c cm.tab.c
analyze.o: analyze.c
	gcc -c analyze.c
symtab.o: symtab.c
	gcc -c symtab.c
clean:
	rm *o project3_5 lex.yy.c cm.tab.h cm.tab.c
