#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro_native_dialog.h>
#include "baralho.h"
#include "crescente.h"
#include "save.h"

int iniciaMontes(Baralho baralho, Monte bp[], Monte bd[], Monte bc[]) {
    Monte monteAux = geraMonte(baralho, 2);
    int i;
    for(i = 0; i < 4; i++) {
        bp[i] = geraMonteVazio();
        bd[i] = geraMonteVazio();
        bc[i] = geraMonteVazio();
    }
    for(; i < 16; i++) bp[i] = geraMonteVazio();
    monteAux = geraMonte(baralho, 2);
    for(i = 0; i < NAIPES; i++) {
        char *naipes = "PECO";
        if(!removeCarta(&monteAux, 'A', naipes[i]) || !removeCarta(&monteAux, 'K', naipes[i])) return 0;
        insereCarta(&(bd[i]), getCarta(naipes[i], 'K', baralho));
        insereCarta(&(bc[i]), getCarta(naipes[i], 'A', baralho));
    }
    embaralha(monteAux);
    i = 0;
    while(getMonteSize(monteAux)) {
        insereCarta(&(bp[i++]), tiraCarta(&monteAux, baralho));
        if(i >= 16) i = 0;
    }
    free(monteAux);
    for(i = 0; i < 4; i++) {
        embaralha(bc[i]);
        embaralha(bd[i]);
        embaralha(bp[i]);
    }
    for(; i < 16; i++) embaralha(bp[i]);
 
}

void printMesa(Monte bp[], Monte bd[], Monte bc[]) {
    int i, j;
    printf("%10c", ' ');
    for(i = 5; i < 13; i++) printf(" %2d   ", i);
    printf("\n%10c", ' ');
    for(i = 4; i < 12; i++) printf("[%c-%c] ", bp[i][0][0], bp[i][0][1]);
    printf("\n\n");
    for(i = 4; i > 0; i--) {
        printf("%2d [%c-%c]%14c", i, bp[i-1][0][0], bp[i-1][0][1], ' ');
        for(j = 0; j < 4; j++) {
            switch(i) {
                case 4:
                case 1:
                    printf(" %2d   ", j+1);
                    break;
                case 2: // Crescente
                    printf("[%c-%c] ", bc[j][0][0], bc[j][0][1]);
                    break;
                case 3: // Decrescente
                    printf("[%c-%c] ", bd[j][0][0], bd[j][0][1]);
                    break;
            }
        }
        printf("%14c[%c-%c] %2d", ' ', bp[16-i][0][0], bp[16-i][0][1], 17-i);
        printf("\n");
    }
}

int jogar(int load) {
    unsigned long sTime; 
    Baralho baralho;
    int i, running=1, reshuffles=5, status=WON;
    setBordas(0, 1, 0, 0); //Seta bordas da imagem na libbaralho
    if(!(baralho = init("../baralho.ppm", 71, 96, '2', "PECO"))) return 0;
    if(!(baralho = geraBaralho(baralho))) return 0;
    Monte blocoPeriferico[16], blocoDecrescente[4], blocoCrescente[4];
    sTime = time(NULL);
    if(!load) {
        if(!iniciaMontes(baralho, blocoPeriferico, blocoDecrescente, blocoCrescente)) return 0;
    } else {
        // TODO Dialogo gráfico
        char arquivo[100];
        printf("Insira o nome do jogo salvo: ");
        fgets(arquivo, 100, stdin);
        loadState(arquivo, &blocoPeriferico, &blocoDecrescente, &blocoCrescente, &reshuffles, &sTime);
    }
    do {
        char cmd;
        int arg[2];
        printMesa(blocoPeriferico, blocoDecrescente, blocoCrescente);
        printf("\n[M] Mover uma carta.\n[R] Reembaralhar os montes periféricos.\n[D] Desistir.\nOpção: ");
        cmd = getchar();
        if(cmd != '\n') while(getchar() != '\n'); // Limpar buffer de entrada.
        switch(cmd) {
            case 'M':
            case 'm':
                printf("* Insira o bloco de origem [1-16]: ");
                scanf("%d", &arg[0]);

                if(--arg[0] < 0 || arg[0] > 15) {
                    fputs("*** Erro: Entrada inválida - bloco inexistente.\n", stderr);
                    break;
                }
                printf("* Qual o destino da carta?\n[1] Bloco Periférico\n[2] Bloco Central Crescente\n[3] Bloco Central Decrescente.\nDestino: ");
                scanf("%d", &arg[1]);
                switch(arg[1]) {
                    case 1:
                        printf("* Insira o bloco de destino [1-16]: ");
                        scanf("%d", &arg[1]);
                        if(--arg[1] < 0 || arg[1] > 15) {
                            fputs("*** Erro: Entrada inválida - bloco inexistente.\n", stderr);
                            break;
                        }
                        // Talvez eu devesse ter implementado uma função na biblioteca para isso...
                        Carta origem = getCarta(blocoPeriferico[arg[0]][0][1], blocoPeriferico[arg[0]][0][0], 
                                baralho);
                        Carta destino = getCarta(blocoPeriferico[arg[1]][0][1], blocoPeriferico[arg[1]][0][0],
                                baralho);
                        if(!isNextCard(origem, destino) && !isNextCard(destino, origem)) {
                            fputs("*** Erro: Movimento inválido.\n", stderr);
                            break;
                        }
                        insereCarta(&blocoPeriferico[arg[1]], tiraCarta(&blocoPeriferico[arg[0]], baralho));
                        break;
                    case 2:
                        printf("* Insira o bloco de destino [1-4]: ");
                        scanf("%d", &arg[1]);
                        if(--arg[1] < 0 || arg[1] > 3) {
                            fputs("*** Erro: Entrada inválida - bloco inexistente.\n", stderr);
                            break;
                        }
                        origem = getCarta(blocoPeriferico[arg[0]][0][1], blocoPeriferico[arg[0]][0][0],
                                baralho);
                        destino = getCarta(blocoCrescente[arg[1]][0][1], blocoCrescente[arg[1]][0][0],
                                baralho);
                        if(!isNextCard(destino, origem)) {
                            fputs("*** Erro: Movimento inválido.\n", stderr);
                            break;
                        }
                        insereCarta(&blocoCrescente[arg[1]], tiraCarta(&blocoPeriferico[arg[0]], baralho));
                        break;
                    case 3:
                        printf("* Insira o bloco de destino [1-4]: ");
                        scanf("%d", &arg[1]);
                        if(--arg[1] < 0 || arg[1] > 3) {
                            fputs("*** Erro: Entrada inválida - bloco inexistente.\n", stderr);
                            break;
                        }
                        origem = getCarta(blocoPeriferico[arg[0]][0][1], blocoPeriferico[arg[0]][0][0],
                                baralho);
                        destino = getCarta(blocoDecrescente[arg[1]][0][1], blocoDecrescente[arg[1]][0][0],
                                baralho);
                        if(!isNextCard(origem, destino)) {
                            fputs("*** Erro: Movimento inválido.\n", stderr);
                            break;
                        }
                        insereCarta(&blocoDecrescente[arg[1]], tiraCarta(&blocoPeriferico[arg[0]], baralho));
                        break;
                }
                while(getchar() != '\n');
                break;
            case 'R':
            case 'r':
                
                break;
            case 'D':
            case 'd':
                status = LOST;
                running = 0;
                break;
            default:
                fputs("* Comando não encontrando.\n", stderr);
        }
        printf("\n");
    } while(running);
    return status;
}

int main() {
    jogar(1);
    return 0;
}
