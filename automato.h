/*
 *
 *
 *
 */

#ifndef AUTOMATO
#define AUTOMATO

//##### DECLARAÇÕES
typedef struct Aut Automato;

struct Aut{
	int qtdEstados;
	Estado* listaEstados;
	Estado* estadoAtual;
	Estado* estadoInicial;
	Estado* estadoFinal;
};


//##### FUNÇÕES
int montaAutomato(char* arquivo, Automato* automato);
int buscaEstado(Automato* automato, int numEstado);
int validaAutomato(Automato* automato);
int emendaTransicoes(Automato* automato);
int pushEstado(Automato* automato, Estado estado);
void imprimeAutomato(Automato* automato);

#include "automato.c"
#endif
