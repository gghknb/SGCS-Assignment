test: main.o Output.o String_Manipulation.o
	g++ -o test main.o Output.o String_Manipulation.o

main.o: main.cpp Header.h
	gcc -c -g main.cpp
output.o: Output.cpp Header.h
	gcc -c -g Output.cpp
String_Manipulation.o: String_Manipulation.cpp Header.h
	gcc -c -g String_Manipulation.cpp
clean:
	rm *.o test 


