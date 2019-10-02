CC=gcc
CFLAGS=-g -Wall `sdl2-config --cflags`
LDLIBS=`sdl2-config --libs` -lSDL2_image -ljpeg -lSDL2_gfx -lreadline `pkg-config --cflags --libs check`
ALL=cimp

all: $(ALL)
cimp: main.o structure.o clavier.o fenetre.o transformation.o
	$(CC) $(CFLAGS) main.o structure.o clavier.o fenetre.o transformation.o -o $(ALL) $(LDLIBS)

main.o: main.c structure.h main.h clavier.h fenetre.h transformation.h 
structure.o: structure.c structure.h
clavier.o: clavier.c structure.h clavier.h fenetre.h transformation.h
fenetre.o: fenetre.c structure.h fenetre.h clavier.h
transformation.o: transformation.c structure.h transformation.h clavier.h fenetre.h

test: test.o structure.o clavier.o fenetre.o transformation.o
	$(CC) $(CFLAGS) test.o structure.o clavier.o fenetre.o transformation.o -o test $(LDLIBS)

test.o: test.c test.h structure.h clavier.h fenetre.h transformation.h 

clean:
	rm -rf *.o $(ALL) test
