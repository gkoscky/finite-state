#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estado.h"
#include "automato.h"

const char pontuacao[6] = {',','.',';',':','!','?'};
//~ int popEstado(Automato* automato, Estado* estado);
//~ int adicionaEntrada(Estado* estado, char entrada);
//~ int adicionaSaida(Estado* estado, char entrada);

int pertence(const char lista[], char elemento);
int classeCar(char entrada);


int main(void){
    //~ char letrasM[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    //~ char letrasL[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    //~ char digitos[10] = {'0','1','2','3','4','5','6','7','8','9'};
    //~ char simbolos[5] = {'[',']','+','*','-'};

    FILE* arquivoEntrada;
    FILE* arquivoRegex;
    Automato automato;

    typedef int transicao[4];

    arquivoRegex = fopen("entradas/regex.txt", "r");
    char caractere, caractereAnt;
    transicao *transicoesNFA, *transicoesTmp;//, *transicoesDFA;


    int numTransicoes, numEstados, estadoAtual, classe, diferenca, flag;

    numTransicoes = 0;
    numEstados = 0;
    estadoAtual = 1;

    transicoesNFA = NULL;

//  MONTA TABELA DE TRANSIÇÕES NÃO-DETERMINÍSTICA
    while(estadoAtual > 0 && (caractere = fgetc(arquivoRegex)) != EOF){
        switch(estadoAtual){
            case 1:
                switch(classeCar(caractere)){
                    case 5:     // Leu '['
                        flag = numTransicoes;
                        estadoAtual = 2;
                    break;

                    case 6:     // Leu ']'
                        estadoAtual = -1;
                    break;

                    case 7:     // Leu '+'
                        estadoAtual = -1;
                    break;

                    case 8:     // Leu '*'
                        estadoAtual = -1;
                    break;

                    case 9:     // Leu '-'
                        estadoAtual = -1;
                    break;

                    case 10:    // Leu ' '
                        estadoAtual = -1;
                    break;

                    case 11:    // Leu '\n'
                        estadoAtual = 0;
                    break;

                    default:    // Leu literal
                        transicoesTmp = (transicao*) realloc(transicoesNFA, ((numTransicoes+1) * sizeof(transicao)));
                        if(transicoesTmp != NULL){
                            transicoesNFA = transicoesTmp;
                            transicoesNFA[numTransicoes][0] = ++numEstados;
                            transicoesNFA[numTransicoes][1] = caractere;
                            transicoesNFA[numTransicoes][2] = numEstados + 1;
                            transicoesNFA[numTransicoes][3] = (transicoesNFA[numTransicoes][0] > 1);
                            numTransicoes++;
                        }else estadoAtual = -2;
                    break;
                }
            break;

            case 2:
                switch(classeCar(caractere) < 4){
                    case 1:     // Leu dígito, letra ou outro literal
                        transicoesTmp = (transicao*) realloc(transicoesNFA, ((numTransicoes+1) * sizeof(transicao)));
                        if(transicoesTmp != NULL){
                            transicoesNFA = transicoesTmp;
                            transicoesNFA[numTransicoes][0] = ++numEstados;
                            transicoesNFA[numTransicoes][1] = caractere;
                            transicoesNFA[numTransicoes][2] = numEstados + 1;
                            transicoesNFA[numTransicoes][3] = (transicoesNFA[numTransicoes][0] > 1);
                            numTransicoes++;
                        }else estadoAtual = -2;

                        estadoAtual = 3;
                    break;

                    case 0:     // Leu símbolo especial
                        estadoAtual = -1;
                    break;
                }
            break;

            case 3:
                switch((classe = classeCar(caractere)) < 4){
                    case 1:     // Leu dígito, letra ou outro literal
                        transicoesTmp = (transicao*) realloc(transicoesNFA, ((numTransicoes+1) * sizeof(transicao)));
                        if(transicoesTmp != NULL){
                            transicoesNFA = transicoesTmp;
                            transicoesNFA[numTransicoes][0] = numEstados;
                            transicoesNFA[numTransicoes][1] = caractere;
                            transicoesNFA[numTransicoes][2] = numEstados + 1;
                            transicoesNFA[numTransicoes][3] = (transicoesNFA[numTransicoes][0] > 1);
                            numTransicoes++;
                        }else estadoAtual = -2;
                    break;

                    case 0: // Leu símbolo especial;
                        if(classe == 9) estadoAtual = 4;        // Leu '-'
                        else if(classe == 6) estadoAtual = 6;   // Leu ']'
                        else estadoAtual = -1;
                    break;
                }
            break;

            case 4:
                switch(classeCar(caractere) < 4){
                    case 1:     // Leu dígito, letra ou outro literal
                        caractereAnt = transicoesNFA[numTransicoes-1][1];
                        if(classeCar(caractere) == classeCar(caractereAnt) && caractere > caractereAnt){
                            diferenca = caractere - caractereAnt;
                            transicoesTmp = (transicao*) realloc(transicoesNFA, ((numTransicoes+diferenca) * sizeof(transicao)));
                            if(transicoesTmp != NULL){
                                transicoesNFA = transicoesTmp;
                                do{
                                    transicoesNFA[numTransicoes][0] = numEstados;
                                    transicoesNFA[numTransicoes][1] = ++caractereAnt;
                                    transicoesNFA[numTransicoes][2] = numEstados + 1;
                                    transicoesNFA[numTransicoes][3] = (transicoesNFA[numTransicoes][0] > 1);
                                    numTransicoes++;
                                }while(--diferenca > 0);
                                estadoAtual = 5;
                            }else estadoAtual = -2;
                        }else estadoAtual = -1;
                    break;

                    case 0:     // Leu símbolo especial
                        estadoAtual = -1;
                    break;
                }
            break;

            case 5:
                switch((classe = classeCar(caractere)) < 4){
                    case 1:     // Leu dígito, letra ou outro literal
                        transicoesTmp = (transicao*) realloc(transicoesNFA, ((numTransicoes+1) * sizeof(transicao)));
                        if(transicoesTmp != NULL){
                            transicoesNFA = transicoesTmp;
                            transicoesNFA[numTransicoes][0] = numEstados;
                            transicoesNFA[numTransicoes][1] = caractere;
                            transicoesNFA[numTransicoes][2] = numEstados + 1    ;
                            transicoesNFA[numTransicoes][3] = (transicoesNFA[numTransicoes][0] > 1);
                            numTransicoes++;
                        }else estadoAtual = -2;

                        estadoAtual = 3;
                    break;

                    case 0: // Leu símbolo especial;
                        if(classe == 6) estadoAtual = 6;    // Leu ']'
                        else estadoAtual = -1;
                    break;
                }
            break;

            case 6:
                switch((classe = classeCar(caractere)) < 4){
                    case 1:     // Leu dígito, letra ou outro literal
                        transicoesTmp = (transicao*) realloc(transicoesNFA, ((numTransicoes+1) * sizeof(transicao)));
                        if(transicoesTmp != NULL){
                            transicoesNFA = transicoesTmp;
                            transicoesNFA[numTransicoes][0] = ++numEstados;
                            transicoesNFA[numTransicoes][1] = caractere;
                            transicoesNFA[numTransicoes][2] = numEstados + 1;
                            transicoesNFA[numTransicoes][3] = (transicoesNFA[numTransicoes][0] > 1);
                            numTransicoes++;
                        }else estadoAtual = -2;

                        estadoAtual = 1;
                    break;

                    case 0: // Leu símbolo especial;
                        if(classe == 7 || classe == 8)          // Leu '+' ou '*'
                            estadoAtual = classe;
                        else if(classe == 5){                   // Leu '['
                            flag = numTransicoes;
                            estadoAtual = 2;
                        }else if(classe == 11) estadoAtual = 0; // Leu '\n'
                        else estadoAtual = -1;
                    break;
                }
            break;

            case 7:
                diferenca = numTransicoes - flag;
                flag = diferenca;
                transicoesTmp = (transicao*) realloc(transicoesNFA, ((numTransicoes+diferenca) * sizeof(transicao)));
                if(transicoesTmp != NULL){
                    transicoesNFA = transicoesTmp;
                    do{
                        transicoesNFA[numTransicoes][0] = numEstados + 1;
                        transicoesNFA[numTransicoes][1] = transicoesNFA[numTransicoes-flag][1];
                        transicoesNFA[numTransicoes][2] = numEstados + 1;
                        transicoesNFA[numTransicoes][3] = (transicoesNFA[numTransicoes][0] > 1);
                        numTransicoes++;
                    }while(--diferenca > 0);
                }else estadoAtual = -2;

                switch((classe = classeCar(caractere)) < 4){
                    case 1:     // Leu dígito, letra ou outro literal
                        transicoesTmp = (transicao*) realloc(transicoesNFA, ((numTransicoes+1) * sizeof(transicao)));
                        if(transicoesTmp != NULL){
                            transicoesNFA = transicoesTmp;
                            transicoesNFA[numTransicoes][0] = numEstados;
                            transicoesNFA[numTransicoes][1] = caractere;
                            transicoesNFA[numTransicoes][2] = numEstados + 1;
                            transicoesNFA[numTransicoes][3] = (transicoesNFA[numTransicoes][0] > 1);
                            numTransicoes++;
                        }else estadoAtual = -2;

                        estadoAtual = 1;
                    break;

                    case 0: // Leu símbolo especial;
                        if(classe == 5){                    // Leu '['
                            flag = numTransicoes;
                            estadoAtual = 2;
                        }else if(classe == 11) estadoAtual = 0;     // Leu '\n'
                        else estadoAtual = -1;
                    break;
                }
            break;

            case 8:
                diferenca = numTransicoes - flag;
                flag = diferenca;
                do{
                    transicoesNFA[numTransicoes-diferenca][2] = numEstados;
                }while(--diferenca > 0);
                numEstados--;
                switch((classe = classeCar(caractere)) < 4){
                    case 1:     // Leu dígito, letra ou outro literal
                        transicoesTmp = (transicao*) realloc(transicoesNFA, ((numTransicoes+1) * sizeof(transicao)));
                        if(transicoesTmp != NULL){
                            transicoesNFA = transicoesTmp;
                            transicoesNFA[numTransicoes][0] = numEstados;
                            transicoesNFA[numTransicoes][1] = caractere;
                            transicoesNFA[numTransicoes][2] = numEstados + 1;
                            transicoesNFA[numTransicoes][3] = (transicoesNFA[numTransicoes][0] > 1);
                            numTransicoes++;
                        }else estadoAtual = -2;

                        estadoAtual = 1;
                    break;

                    case 0: // Leu símbolo especial;
                        if(classe == 5){                    // Leu '['
                            flag = numTransicoes;
                            estadoAtual = 2;
                        }else if(classe == 11) estadoAtual = 0;     // Leu '\n'
                        else estadoAtual = -1;
                    break;
                }
            break;

            case -1:
                fputs("Expressão regular mal-formada",stderr);
                return EXIT_FAILURE;
            break;

            case -2:
                fputs("Ocorreu um erro",stderr);
                return EXIT_FAILURE;
            break;

            default:
                fputs("Ocorreu um erro",stderr);
                return EXIT_FAILURE;
            break;
        }

        if(estadoAtual == -1){
            fputs("Expressão regular mal-formada",stderr);
            return EXIT_FAILURE;
        }else if(estadoAtual == -2){
            fputs("Ocorreu um erro",stderr);
            return EXIT_FAILURE;
        }
    }

    int i;
    arquivoEntrada = fopen("entradas/saida.txt", "w");
    estadoAtual = transicoesNFA[0][0];

    for(i = 0; i < numTransicoes; i++){
        if(transicoesNFA[i][0] != estadoAtual){
            puts("--------------------------");
            estadoAtual = transicoesNFA[i][0];
        }
        if(transicoesNFA[i][2] == numEstados+1) transicoesNFA[i][2] = 0;
        fprintf(arquivoEntrada, "%d\t%c\t%d\t%d\n", transicoesNFA[i][0], transicoesNFA[i][1], transicoesNFA[i][2], transicoesNFA[i][3]);
        //~ printf("%d\t%c\t%d\t%d\n", transicoesNFA[i][0], transicoesNFA[i][1], transicoesNFA[i][2], transicoesNFA[i][3]);
    }
    fclose(arquivoEntrada);
//  CONVERTE TABELA NÃO-DETERMINÍSTICA EM DETERMINÍSTICA

    if(montaAutomato("entradas/saida.txt", &automato) == EXIT_SUCCESS){
        puts("==RESULTADO==");
        imprimeAutomato(&automato);
//~
        //~ arquivoEntrada = fopen("entradas/entrada.txt", "r");
        //~ int pos;
        //~ while((caractere = fgetc(arquivoEntrada)) != '\n'){
            //~ if((pos = buscaTransicao(automato.estadoAtual, caractere)) != -1){
                //~ automato.estadoAtual = automato.estadoAtual->transicoes[pos].proxEstado;
            //~ }else{
                //~ fputs("Entrada com símbolo inválido", stderr);
                //~ return EXIT_FAILURE;
            //~ }
        //~ }
        //~ fclose(arquivoEntrada);
        //~ if(automato.estadoAtual->tipo == FINAL) puts("Entrada válida");
        //~ else puts("Entrada não corresponde ao automato");
    }

    //~ printf("Pertence: %d\n", pertence(pontuacao, ';'));
    return EXIT_FAILURE;
}

int classeCar(char entrada){
    if(entrada >= 'a' && entrada <= 'z')        return 1;
    else if(entrada >= 'A' && entrada <= 'Z')   return 2;
    else if(entrada >= '0' && entrada <= '9')   return 3;
    else if(entrada == '[')                     return 5;
    else if(entrada == ']')                     return 6;
    else if(entrada == '+')                     return 7;
    else if(entrada == '*')                     return 8;
    else if(entrada == '-')                     return 9;
    else if(entrada == ' ')                     return 10;
    else if(entrada == '\n')                    return 11;
//  else if(pertence(pontuacao, entrada) == EXIT_SUCCESS)
//      return 5;


    return 0;
}

int pertence(const char lista[], char elemento){
    int i, tamanho = sizeof(lista)/sizeof(char);

    for(i = 0; i < tamanho; i++)
        if(elemento == lista[i]) return EXIT_SUCCESS;

    return EXIT_FAILURE;
}
