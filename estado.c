int pushTransicao(Estado* estado, Transicao transicao){
	int tamanho;
	tamanho = estado->numTransicoes + 1;

	Transicao* tmp = (Transicao*) realloc(estado->transicoes, (tamanho * sizeof(Transicao)));
	if(tmp != NULL){
		estado->transicoes = tmp;
		estado->transicoes[estado->numTransicoes] = transicao;
		estado->numTransicoes++;
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int buscaTransicao(Estado* estado, char entrada){
	int i;

	for(i = 0; i < estado->numTransicoes; i++)
		if(estado->transicoes[i].entrada == entrada) return i;

	return -1;
}
