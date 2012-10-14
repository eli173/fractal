# makefile for fractal generating program

cc = gcc
LDFLAGS = -Wall -framework OpenCL -framework SDL -framework Cocoa
CFLAGS = -Wall -c -I/usr/local/include/SDL
OBJECTS = main.o SDLMain.o

all:$(OBJECTS)
	$(CC) $(LDFLAGS) -o exec $(OBJECTS)
main.o:main.c
	$(CC) $(CFLAGS) main.c
SDLMain.o:SDLMain.m SDLMain.h
	$(CC) $(CFLAGS) SDLMain.m

clean:
	rm *.o