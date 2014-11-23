#ifndef SAVE_H_
#define SAVE_H_

#include "baralho.h"

// saveState
// Escreve no arquivo o estado dos montes, os re-embaralhamentos restantes e o tempo já gasto no jogo.
int saveState(char *file, Monte *blocoPeriferico, Monte *blocoDecrescente, Monte *blocoCrescente, int reembaralhadas, unsigned long spentTime, unsigned long limitTime, int mode);

// loadState
// Lê do arquivo o estado dos montes, os re-embaralhamentos restantes e o tempo já gasto no jogo.
int loadState(char *file, Monte *blocoPeriferico, Monte *blocoDecrescente, Monte *blocoCrescente, int *reembaralhadas, unsigned long *spentTime, unsigned long *limitTime, int *mode);
#endif
