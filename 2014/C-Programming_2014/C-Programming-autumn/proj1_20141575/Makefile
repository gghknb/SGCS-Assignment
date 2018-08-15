CC = gcc
CFLAGS = -W -Wall
TARGET = 20140000.out
SRCS = 20140000.h 20140000.c
OBJECTS = 20140000.o

$(TARGET):$(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CFLAGS)

$(OBJECTS): 20140000.c
	$(CC) -c 20140000.c $(CFLAGS)

clean:
	rm -rf *.o *.out
