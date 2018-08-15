project4_5: main.o analyze.o y.tab.o lex.yy.o util.o cgen.o code.o
	gcc -o project4_5 main.o analyze.o y.tab.o lex.yy.o util.o cgen.o code.o
main.o: globals.h scan.h y.tab.h analyze.h main.c
	gcc -c main.c
y.tab.o: globals.h util.h scan.h parse.h y.tab.h y.tab.c
	gcc -c y.tab.c
lex.yy.o: globals.h util.h scan.h y.tab.h lex.yy.c
	gcc -c lex.yy.c
util.o: globals.h util.h y.tab.h util.c
	gcc -c util.c
lex.yy.c: tiny.l
	flex tiny.l
y.tab.c: cm.y
	yacc -d cm.y
y.tab.h: cm.y
	yacc -d cm.y
analyze.o: util.h analyze.h analyze.c
	gcc -c analyze.c
code.o : code.h globals.h code.c
	gcc -c code.c
cgen.o : code.h cgen.h util.h globals.h cgen.c
	gcc -c cgen.c
clean:
	rm *.o lex.yy.c y.tab.h y.tab.c *.s *.tmp project4_5
