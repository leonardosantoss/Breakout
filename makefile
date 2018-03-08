CC=gcc

CFLAGS=-g -Wall -D_GNU_SOURCE=1 -D_REENTRANT
#CFLAGS=-g -Wall -D_REENTRANT
IMAGE_FLAGS=-lSDL2_image
SFLAGS=-lSDL2
TFLAGS =-lSDL2_ttf
SOURCES=game.c
BINARIES=game

#all: ex0 ex1 ex2 ex3 ex4 ex5 ex6 ex7 grafico texto
all: $(BINARIES)

game: game.c
	$(CC) -o game game.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(TFLAGS)

clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core a.out
