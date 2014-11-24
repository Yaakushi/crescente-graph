OUTPUTFILE=../bin/crescente
INCFILES=jogo.h save.h
CC=gcc -g -Wall
INCDIR=../include
LIBDIR=../lib
ALLEGRO=`pkg-config --cflags --libs allegro_dialog-5.0`

%.o: %.c $(INCFILES)
	$(CC) -c $< -I$(INCDIR)

.PHONY: all clean distclean

all: $(OUTPUTFILE)

$(OUTPUTFILE): crescente.o save.o jogo.o
	$(CC) -o $(OUTPUTFILE) $^ -L$(LIBDIR) -lbaralho -I$(INCDIR) $(ALLEGRO)

clean:
	rm -f *.swp *~ *.tmp

distclean: clean
	rm -f *.o  core ../bin/*
