#ifndef CRESCENTE_H_
#define CRESCENTE_H_

#define WON 1
#define LOST 2

#define CRESCENTE 1
#define DECRESCENTE 0

#define NBARALHOS 2

#define CONFIG_FILE "crescente_config"
#define LOG_FILE "crescente_log"

#define SIZEFILENAME 255

#include "baralho.h"

int iniciaMontes(Baralho baralho, Monte bp[], Monte bd[], Monte bc[]);

void printMesa(Monte bp[], Monte bd[], Monte bc[]);

unsigned long jogar(int load, int mode, Baralho baralho);

void parseConfig(FILE *config, char *filename, char *naipes, char *first, int *cw, int *ch, int *bordas, unsigned long *bestTime);

void writeConfig(FILE *config, char *filename, char *naipes, char first, int cw, int ch, int *bordas, unsigned long bestTime);

#endif
