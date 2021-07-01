int montaAutomato(char* arquivo, Automato* automato){
	FILE* arquivoTabela;

	arquivoTabela = fopen(arquivo, "r");

	automato->qtdEstados	= 0;
	automato->listaEstados	= NULL;
	automato->estadoAtual	= NULL;
	automato->estadoInicial	= NULL;
	automato->estadoFinal	= NULL;

	Estado estado;
	Estado* pEstado;
	Transicao transicao;

	int numero, proximo, qtdArg;
	char entrada;
	if((qtdArg = fscanf(arquivoTabela,"%d %c %d %*c", &numero, &entrada, &proximo)) != EOF && qtdArg == 3){
		estado.numEstado		= numero;
		estado.tipo				= INICIAL;
		estado.numTransicoes	= 0;
		estado.transicoes		= NULL;

		pushEstado(automato, estado);

		pEstado = automato->estadoAtual;
		transicao.entrada = entrada;
		transicao.numProxEstado = proximo;
		transicao.proxEstado = NULL;
		//~ if(proximo != pEstado->numEstado)
			//~ if(buscaEstado(automato, proximo) == EXIT_SUCCESS)
				//~ transicao.proxEstado = automato->estadoAtual;
			//~ else transicao.proxEstado = NULL;
		//~ else transicao.proxEstado = pEstado;

		pushTransicao(pEstado, transicao);

		while((qtdArg = fscanf(arquivoTabela,"%d %c %d %*c", &numero, &entrada, &proximo)) != EOF && qtdArg == 3){
			if(buscaEstado(automato, numero) == EXIT_FAILURE){
				estado.numEstado = numero;
				estado.tipo = INTERMEDIARIO;
				estado.numTransicoes = 0;
				estado.transicoes = NULL;

				pushEstado(automato, estado);
			}

			pEstado = automato->estadoAtual;
			transicao.entrada = entrada;
			transicao.numProxEstado = proximo;
			transicao.proxEstado = NULL;

			//~ if(proximo != pEstado->numEstado)
				//~ if(buscaEstado(automato, proximo) == EXIT_SUCCESS)
					//~ transicao.proxEstado = automato->estadoAtual;
				//~ else transicao.proxEstado = NULL;
			//~ else transicao.proxEstado = pEstado;

			pushTransicao(pEstado, transicao);
		}

		estado.numEstado = 0;
		estado.tipo = FINAL;
		estado.numTransicoes = 0;
		estado.transicoes = NULL;

		pushEstado(automato, estado);

		if(emendaTransicoes(automato) == EXIT_FAILURE){
			fprintf(stderr, "Tabela de transições inconsistente");
			return EXIT_FAILURE;
		}

		automato->estadoFinal	= automato->estadoAtual;
		automato->estadoInicial	= automato->listaEstados;
		automato->estadoAtual	= automato->listaEstados;

		fclose(arquivoTabela);

		return EXIT_SUCCESS;
	}

	fprintf(stderr, "Arquivo de entrada vazio");
	return EXIT_FAILURE;
}

void imprimeAutomato(Automato* automato){
	printf("Qtd Estados: %d\n", automato->qtdEstados);
	printf("Estado inicial: %d\n\n", automato->estadoInicial->numEstado);

	int i, j;
	for(i = 0; i < automato->qtdEstados; i++){
		printf("\nEstado %d (%d transições):\n", automato->listaEstados[i].numEstado, automato->listaEstados[i].numTransicoes);
		for(j = 0; j < automato->listaEstados[i].numTransicoes; j++){
			printf("\t%c -> %d", automato->listaEstados[i].transicoes[j].entrada, automato->listaEstados[i].transicoes[j].proxEstado->numEstado);
			(automato->listaEstados[i].transicoes[j].proxEstado == NULL)? puts(" não ligado") : putc('\n', stdout);
		}
	}

}

int emendaTransicoes(Automato* automato){
	int i, j;
	Estado* pAtualAnt;

	pAtualAnt = automato->estadoAtual;

	for(i = 0; i < automato->qtdEstados; i++){
		for(j = 0; j < automato->listaEstados[i].numTransicoes; j++){
			if(automato->listaEstados[i].transicoes[j].proxEstado == NULL){
				if(buscaEstado(automato, automato->listaEstados[i].transicoes[j].numProxEstado) == EXIT_SUCCESS)
					automato->listaEstados[i].transicoes[j].proxEstado = automato->estadoAtual;
				else{
					automato->estadoAtual = pAtualAnt;
					return EXIT_FAILURE;
				}
			}
		}
	}
	automato->estadoAtual = pAtualAnt;
	return EXIT_SUCCESS;
}

int validaAutomato(Automato* automato){
	int i, j;
	Estado* pAtualAnt;

	pAtualAnt = automato->estadoAtual;

	for(i = 0; i < automato->qtdEstados; i++){
		for(j = 0; j < automato->listaEstados[i].numTransicoes; j++){
			if(automato->listaEstados[i].transicoes[j].proxEstado == NULL){
				//~ printf("\tTransição nula: (%d) %c -> %d\n", automato->listaEstados[i].numEstado, automato->listaEstados[i].transicoes[j].entrada, automato->listaEstados[i].transicoes[j].numProxEstado);
				if(buscaEstado(automato, automato->listaEstados[i].transicoes[j].numProxEstado) == EXIT_SUCCESS)
					automato->listaEstados[i].transicoes[j].proxEstado = automato->estadoAtual;
				else{
					automato->estadoAtual = pAtualAnt;
					return EXIT_FAILURE;
				}
			}
		}
	}
	automato->estadoAtual = pAtualAnt;
	return EXIT_SUCCESS;
}

int buscaEstado(Automato* automato, int numEstado){
	int i;
	for(i = 0; i < automato->qtdEstados; i++){
		if(automato->listaEstados[i].numEstado == numEstado){
			automato->estadoAtual = &automato->listaEstados[i];
			return EXIT_SUCCESS;
		}
	}
	return EXIT_FAILURE;
}

int pushEstado(Automato* automato, Estado estado){
	int tamanho;
	tamanho = automato->qtdEstados + 1;

	Estado* tmp = (Estado*) realloc(automato->listaEstados, (tamanho * sizeof(Estado)));

	if(tmp != NULL){
		automato->listaEstados = tmp;
		automato->listaEstados[automato->qtdEstados] = estado;
		automato->estadoAtual = &automato->listaEstados[automato->qtdEstados];
		automato->qtdEstados++;

		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
