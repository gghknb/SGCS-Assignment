all: 20141575

20141575: main.o lex.yy.o util.o cm.tab.o
	gcc main.o lex.yy.o  util.o cm.tab.o -o20141575 -lfl
main.o: main.c cm.tab.h
	gcc -c main.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
lex.yy.c: tiny.l globals.h cm.tab.h
	flex tiny.l
cm.tab.c cm.tab.h: cm.y
	bison -d -v cm.y
cm.tab.o : cm.tab.c
	gcc -c cm.tab.c -ly
util.o: util.c
	gcc -c util.c
clean:
	rm 20141575 main.o lex.yy.* util.o cm.tab.* cm.output
