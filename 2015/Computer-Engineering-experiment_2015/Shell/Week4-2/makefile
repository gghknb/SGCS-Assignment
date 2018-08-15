OBJECTS = main.o menu.o zero.o

run : $(OBJECTS)
	gcc -o a.out $(OBJECTS)
main.o : myheader.h main.c
	gcc -c main.c
menu.o : myheader.h menu.c
	gcc -c menu.c
zero.o : myheader.h zero.c
	gcc -c zero.c
clean :
	rm $(OBJECTS)
