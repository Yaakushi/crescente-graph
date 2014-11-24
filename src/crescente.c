#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "baralho.h"
#include "jogo.h"

int main() {
    Baralho baralho;
    srand(time(NULL));
    if(chdir("share")) {
        // Não está na raiz do projeto.
        chdir(".."); //Sobe um nível, na esperança que o programa tenha sido executado a partir da bin e/ou src.
        if(chdir("share")) {
            fputs("*** Erro: Não foi possível localizar a pasta 'share'. Você está na raiz do projeto?\n", stderr);
            return -1; //Não encotrou a pasta share.
        }
    }
    FILE *fconfig;
    if(!(fconfig = fopen(CONFIG_FILE, "r"))) {
        printf("Diretório atual: %s.\n", getcwd(NULL, 0));
        fputs("*** Erro: Não foi possível abrir o arquivo de configuração.\n", stderr);
        return -2;
    }
    char filename[SIZEFILENAME], naipes[5], firstCarta;
    int cw, ch, bordas[4];
    unsigned long bestTime;
    parseConfig(fconfig, filename, naipes, &firstCarta, &cw, &ch, bordas, &bestTime);
    setBordas(bordas[0], bordas[1], bordas[2], bordas[3]); //Seta bordas da imagem na libbaralho
    if(!(baralho = init(filename, cw, ch, firstCarta, naipes))) return 0;
    char mode;
    do {
        unsigned long time = 0;
        int load = 0, gamemode = -1;
        printf("[ Paciência crescente v1.0 ]\n[M] Melhor tempo\n[C] Contra o relógio\n[L] Carregar jogo\n"
                "[P] Placar: Mostrar melhor tempo.\n[S] Sair\nEscolha: ");
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
            case 'P':
            case 'p':
                if(bestTime) 
                    printf("* O melhor tempo é de: %ld minutos e %ld segundos.\n\n", bestTime/60, bestTime%60);
                else
                    printf("* Não há nenhum registro. Que tal jogar uma partida?\n\n");
                break;
            case 'S':
                if(!(fconfig = fopen(CONFIG_FILE, "w"))) {
                    printf("Diretório atual: %s.\n", getcwd(NULL, 0));
                    fputs("*** Erro: Não foi possível abrir o arquivo de configuração.\n", stderr);
                    return -2;
                }
                writeConfig(fconfig, filename, naipes, firstCarta, cw, ch, bordas, bestTime);
                fclose(fconfig);
                return 0;
        }
        if(load != 0 || gamemode != -1) time = jogar(load, gamemode, baralho);
        if(time != 0 && (bestTime == 0 || time < bestTime)) {
            printf("* Parabéns, o seu tempo de %ld minutos e %ld segundos foi o melhor dessa sessão.\n",
                    time/60, time%60);
            bestTime = time;
        } else if(time != 0) printf("* O seu tempo de %ld minutos e %ld segundos não foi o melhor dessa sessão.\n",
                   time/60, time %60);
    } while(1);
    return 0;
}
