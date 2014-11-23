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
    return 1;
}

void printMesa(Monte bp[], Monte bd[], Monte bc[]) {
    int i, j;
    printf("%10c", ' ');
    for(i = 5; i < 13; i++) printf(" %2d   ", i);
    printf("\n%10c", ' ');
    for(i = 4; i < 12; i++) {
        if(getMonteSize(bp[i])) {
            printf("[%c-%c] ", bp[i][0][0], bp[i][0][1]);
        } else {
            printf("[   ] ");
        }
    }
    printf("\n\n");
    for(i = 4; i > 0; i--) {
        if(getMonteSize(bp[i-1])) {
            printf("%2d [%c-%c]%14c", i, bp[i-1][0][0], bp[i-1][0][1], ' ');
        } else {
            printf("%2d [   ]%14c", i, ' ');
        }
        for(j = 0; j < 4; j++) {
            switch(i) {
                case 4:
                case 1:
                    printf(" %2d   ", j+1);
                    break;
                case 2: // Crescente
                    if(getMonteSize(bc[j])) {
                        printf("[%c-%c] ", bc[j][0][0], bc[j][0][1]);
                    } else {
                        printf("[   ] ");
                    }
                    break;
                case 3: // Decrescente
                    if(getMonteSize(bd[j])) {
                        printf("[%c-%c] ", bd[j][0][0], bd[j][0][1]);
                    } else {
                        printf("[   ] ");
                    }
                    break;
            }
        }
        if(getMonteSize(bp[j])) {
            printf("%14c[%c-%c] %2d", ' ', bp[16-i][0][0], bp[16-i][0][1], 17-i);
        } else {
            printf("%14c[   ] %2d", ' ', 17-i);
        }
        printf("\n");
    }
}

unsigned long jogar(int load, int mode) {
    unsigned long sTime, lTime;
    if(mode == 1) { //Contra o tempo
        int tmp = 0;
        printf("* Insira o limite de tempo, em minutos: ");
        do {
            scanf("%d", &tmp);
        } while (tmp <= 0);
        lTime = tmp * 60;
        while(getchar() != '\n');
    }
    printf("\n\n");
    Baralho baralho;
    int i, running=1, reshuffles=5;
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
        arquivo[strlen(arquivo)-1]='\0';
        for(i = 0; i < 4; i++) {
            blocoPeriferico[i] = geraMonteVazio();
            blocoCrescente[i] = geraMonteVazio();
            blocoDecrescente[i] = geraMonteVazio();
        }
        for(i = 4; i < 16; i++) blocoPeriferico[i] = geraMonteVazio();
        if(!loadState(arquivo, blocoPeriferico, blocoDecrescente, blocoCrescente, &reshuffles, &sTime, &lTime, 
                    &mode)) 
        {
            fputs("*** Erro: Não foi possível carregar o jogo.\n", stderr);
            return 0;
        }
        printf("* Jogo carregado com sucesso.\n");
    }
    do {
        char cmd;
        int arg[2];
        printMesa(blocoPeriferico, blocoDecrescente, blocoCrescente);
        printf("\n[M] Mover uma carta.\n[R] Reembaralhar os montes periféricos (%d restantes).\n"
                "[D] Desistir.\n[S] Salvar jogo\nOpção: ", reshuffles);
        cmd = getchar();
        if(cmd != '\n') while(getchar() != '\n'); // Limpar buffer de entrada.
        switch(cmd) {
            case 'S':
            case 's':
                printf("* Escolha o nome do save: ");
                char file[30];
                scanf("%s", file);
                if(!saveState(file, blocoPeriferico, blocoDecrescente, blocoCrescente, reshuffles, sTime, lTime, 
                            mode)) fputs("*** Erro: Não foi possível salvar.\n", stderr);
                else printf("* Salvo em %s com sucesso!\n", file);
                while(getchar() != '\n');
                break;
            case 'M':
            case 'm':
                printf("* Insira o bloco de origem [1-16]: ");
                scanf("%d", &arg[0]);

                if(--arg[0] < 0 || arg[0] > 15) {
                    fputs("*** Erro: Entrada inválida - bloco inexistente.\n", stderr);
                    break;
                }
                if(!getMonteSize(blocoPeriferico[arg[0]])) {
                    fputs("*** Erro: Esse bloco não possui cartas.\n", stderr);
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
                        Carta origem, destino;
                        // Talvez eu devesse ter implementado uma função na biblioteca para isso...
                        if(getMonteSize(blocoPeriferico[arg[1]])) {
                            origem = getCarta(blocoPeriferico[arg[0]][0][1], blocoPeriferico[arg[0]][0][0], 
                                                baralho);
                            destino = getCarta(blocoPeriferico[arg[1]][0][1], blocoPeriferico[arg[1]][0][0],
                                                baralho);
                            if(!isNextCard(origem, destino) && !isNextCard(destino, origem)) 
                            {
                                fputs("*** Erro: Movimento inválido.\n", stderr);
                                break;
                            }
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
                        if(getMonteSize(blocoCrescente[arg[1]])) {
                            origem = getCarta(blocoPeriferico[arg[0]][0][1], blocoPeriferico[arg[0]][0][0],
                                                baralho);
                            destino = getCarta(blocoCrescente[arg[1]][0][1], blocoCrescente[arg[1]][0][0],
                                                baralho);
                            if(!isNextCard(destino, origem)) {
                                fputs("*** Erro: Movimento inválido.\n", stderr);
                                break;
                            }
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
                        if(getMonteSize(blocoDecrescente[arg[1]])) {
                            origem = getCarta(blocoPeriferico[arg[0]][0][1], blocoPeriferico[arg[0]][0][0],
                                                baralho);
                            destino = getCarta(blocoDecrescente[arg[1]][0][1], blocoDecrescente[arg[1]][0][0],
                                                baralho);
                            if(!isNextCard(origem, destino)) {
                                fputs("*** Erro: Movimento inválido.\n", stderr);
                                break;
                            }
                        }
                        insereCarta(&blocoDecrescente[arg[1]], tiraCarta(&blocoPeriferico[arg[0]], baralho));
                        break;
                }
                while(getchar() != '\n');
                break;
            case 'R':
            case 'r':
                if(reshuffles > 0) {
                    for(i = 0; i < 16; i++) embaralha(blocoPeriferico[i]);
                    reshuffles--;
                } else {
                    fputs("*** Erro: Você não possui embaralhamentos restantes.\n", stderr);
                }   
                break;
            case 'D':
            case 'd':
                return 0;
                break;
            default:
                fputs("* Comando não encontrando.\n", stderr);
        }
        printf("\n");
        if(running) {
            running = 0;
            for(i = 0; i < 16; i++) {
                if(getMonteSize(blocoPeriferico[i])) running = 1;
            }
        }
    } while(running);
    sTime = time(NULL) - sTime;
    if(mode == 1 && sTime > lTime) return 0;
    return sTime;
}

int main() {
    srand(time(NULL));
    char mode;
    unsigned long bestTime = 0;
    do {
        int load = 0, gamemode = 0;
        printf("[ Paciência crescente v1.0 ]\n[M] Melhor tempo\n[C] Contra o relógio\n[L] Carregar jogo\n"
                "[S] Sair\nEscolha: ");
        mode = getchar();
        if(mode != '\n') while(getchar() != '\n');
        switch(mode) {
            case 'm':
            case 'M':
                gamemode = 0; break;
            case 'c':
            case 'C':
                gamemode = 1; break;
            case 'l':
            case 'L':
                load = 1;
                break;
            case 'S':
                mode = 's'; break;
        }
        if(gamemode != -1) {
            unsigned long time = jogar(load, gamemode);
            if((bestTime == 0 && time != 0) || (time != 0 && time < bestTime)) {
                printf("* Parabéns, o seu tempo de %ld minutos e %ld segundos foi o melhor dessa sessão.\n",
                        time/60, time%60);
                bestTime = time;
            } else printf("* O seu tempo de %ld minutos e %ld segundos não foi o melhor dessa sessão.\n",
                        time/60, time %60);
        }
    } while(mode != 's');
    return 0;
}
