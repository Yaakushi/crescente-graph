#include "save.h"
#include <stdio.h>
#include <stdlib.h>

int saveState(char *file, Monte *blocoPeriferico, Monte *blocoDecrescente, Monte *blocoCrescente, 
                int reembaralhadas, unsigned long time)
{
    int i, j;
    FILE *arq = fopen(file, "w");
    if(!arq) return 0;
    for(i = 0; i < 16; i++) {
        for(j = 0; blocoPeriferico[i][j] != NULL; j++) fwrite(blocoPeriferico[i][j], sizeof(char), 3, arq);
    }
}

int loadState(char *file, Monte **blocoPeriferico, Monte **blocoDecrescente, Monte **blocoCrescente, 
        int *reembaralhadas, unsigned long *time) 
{
    return 1;
}
