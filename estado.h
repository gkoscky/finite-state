/*
 *
 *
 *
 */

#ifndef ESTADO
#define ESTADO

//##### DECLARAÇÕES
typedef enum tipoEst{INICIAL, INTERMEDIARIO, FINAL} tipoEstado;

typedef struct Tran Transicao;
typedef struct Est Estado;
typedef struct setTran Transicoes;

struct Est{
	int numEstado;
	tipoEstado tipo;
	int numTransicoes;
	Transicao* transicoes;
};

struct Tran{
	char entrada;
	int numProxEstado;
	Estado* proxEstado;
};

struct setTran{
	char entrada;
	int numEstados;
	Estado* proxEstado;
};


//##### FUNÇÕES
int pushTransicao(Estado* estado, Transicao transicao);
int buscaTransicao(Estado* estado, char entrada);
Transicao buscaTransicoes(Estado* estado, char entrada);


#include "estado.c"

#endif
