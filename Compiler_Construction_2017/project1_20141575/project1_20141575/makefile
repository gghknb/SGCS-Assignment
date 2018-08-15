all: 20141575

20141575: main.o lex.yy.o util.o
	gcc main.o lex.yy.o  util.o -o20141575 -lfl
main.o: main.c
	gcc -c main.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
lex.yy.c: tiny.l
	flex tiny.l
util.o: util.c
	gcc -c util.c
clean:
	rm 20141575 main.o lex.yy.o util.o lex.yy.c
