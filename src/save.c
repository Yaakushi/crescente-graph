#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int saveState(char *file, Monte *blocoPeriferico, Monte *blocoDecrescente, Monte *blocoCrescente, 
                int reembaralhadas, unsigned long spentTime, unsigned long limitTime, int mode)
{
    int i, j;
    int bSize;
    FILE *arq = fopen(file, "w");
    if(!arq) return 0;
    spentTime = time(NULL) - spentTime;
    for(i = 0; i < 16; i++) {
        bSize = getMonteSize(blocoPeriferico[i]);
        fwrite(&bSize, sizeof(int), 1, arq);
        for(j = 0; j < getMonteSize(blocoPeriferico[i]); j++)
            fwrite(blocoPeriferico[i][j], sizeof(char), 3, arq);
    }
    for(i = 0; i < 4; i++) {
        bSize = getMonteSize(blocoCrescente[i]);
        fwrite(&bSize, sizeof(int), 1, arq);
        for(j = 0; j < getMonteSize(blocoCrescente[i]); j++)
            fwrite(blocoCrescente[i][j], sizeof(char), 3, arq);
        bSize = getMonteSize(blocoDecrescente[i]);
        fwrite(&bSize, sizeof(int), 1, arq);
        for(j = 0; j < getMonteSize(blocoDecrescente[i]); j++)
            fwrite(blocoDecrescente[i][j], sizeof(char), 3, arq);
    }
    fwrite(&reembaralhadas, sizeof(int), 1, arq);
    fwrite(&spentTime, sizeof(unsigned long), 1, arq);
    fwrite(&mode, sizeof(int), 1, arq);
    if(mode == 1) fwrite(&limitTime, sizeof(unsigned long), 1, arq);
    fclose(arq);
    return 1;
}

int loadState(char *file, Monte *blocoPeriferico, Monte *blocoDecrescente, Monte *blocoCrescente, 
                int *reembaralhadas, unsigned long *spentTime, unsigned long *limitTime, int *mode)
{
    FILE *arq = fopen(file, "r");
    if(!arq) {
       return 0;
    }
    int i, j;
    int bSize;
    unsigned long time;
    for(i = 0; i < 16; i++) {
        fread(&bSize, sizeof(int), 1, arq);
        blocoPeriferico[i] = malloc(sizeof(char *) * (bSize + 1));
        if(!blocoPeriferico[i]) return 0;
        blocoPeriferico[i][bSize] = NULL;
        for(j = 0; j < bSize; j++) {
            blocoPeriferico[i][j] = malloc(sizeof(char) * 3);
            if(!blocoPeriferico[i][j]) return 0;
            fread(blocoPeriferico[i][j], sizeof(char), 3, arq);
        }
    }
    for(i = 0; i < 4; i++) {
        fread(&bSize, sizeof(int), 1, arq);
        blocoCrescente[i] = malloc(sizeof(char *) * (bSize + 1));
        if(!blocoCrescente[i]) return 0;
        blocoCrescente[i][bSize] = NULL;
        for(j = 0; j < bSize; j++) {
            blocoCrescente[i][j] = malloc(sizeof(char) * 3);
            if(!blocoCrescente[i][j]) return 0;
            fread(blocoCrescente[i][j], sizeof(char), 3, arq);
        }
        fread(&bSize, sizeof(int), 1, arq);
        blocoDecrescente[i] = malloc(sizeof(char *) * (bSize + 1));
        if(!blocoDecrescente[i]) return 0;
        blocoDecrescente[i][bSize] = NULL;
        for(j = 0; j < bSize; j++) {
            blocoDecrescente[i][j] = malloc(sizeof(char) * 3);
            if(!blocoDecrescente[i][j]) return 0;
            fread(blocoDecrescente[i][j], sizeof(char), 3, arq);
        }
    }
    fread(reembaralhadas, sizeof(int), 1, arq);
    fread(&time, sizeof(unsigned long), 1, arq);
    spentTime -= time;
    fread(mode, sizeof(int), 1, arq);
    if(*mode == 1) fread(limitTime, sizeof(unsigned long), 1, arq);
    return 1;
}
