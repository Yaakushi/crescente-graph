OUTPUTFILE=../bin/crescente
INCFILES=crescente.h
CC=gcc -g
INCDIR=../include/
LIBDIR=../lib/
ALLEGRO=`pkg-config --cflags --libs allegro_dialog-5.0`

%.o: %.c $(INCFILES)
	$(CC) -c $< -I$(HOME)/ufpr/oficina/libbaralho/include

.PHONY: all crescente clean distclean

all: crescente

crescente: crescente.o save.o
	$(CC) -o $(OUTPUTFILE) $^ -L$(HOME)/ufpr/oficina/libbaralho/lib -lbaralho -I$(HOME)/ufpr/oficina/libbaralho/include $(ALLEGRO)

clean:
	rm -f *.swp *~ *.tmp

distclean: clean
	rm -f *.o  core ../bin/*
