#include "busca.h"

ArrayList *busca_local_randomica_vizinhanca_N1(Grafo *g, Solucao *solucao, int indice_od, int indice_caminho){
	Caminho *caminho = arraylist_get_index(solucao[indice_od].caminhos, indice_caminho);

	ArrayList *vizinhos = new_ArrayList();
	
	/**
	printf("============================================================================\n");
	printf("busca_local_vizinho (%d, %d): Fluxo antes (retirando %f)\n", solucao[indice_od].origem, solucao[indice_od].destino, caminho->fluxo);
	grafo_printa(g);
	print_caminho(caminho->pai, solucao[indice_od].destino, g->n);
	getchar();/**/
	
	int i = 0;
	//Retirando o fluxo do grafo.
	fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, caminho->pai, -caminho->fluxo);
	
	//Bloqueando uma aresta aleatória do caminho.
	Aresta *aresta_bloqueada = NULL;
	
	int arestas = 0;
	int w = solucao[indice_od].destino;
	int v;
	//Conta a quantidade de arestas do caminho.
	do{
		v = caminho->pai[w];
		arestas++;
		w = v;
	}while(v != solucao[indice_od].origem);
	
	int aresta_aleatoria = rand() % arestas;
	
	//Encontrando os vértices da aresta.
	w = solucao[indice_od].destino;
	for(i = 0; i < aresta_aleatoria; i++){
		w = caminho->pai[w];
	}
	v = caminho->pai[w];
	
	for(i = 0; g->arestas[v][i].term != w; i++);
	aresta_bloqueada = &g->arestas[v][i];
	aresta_bloqueada->blocked = 1;
	
	/**
	printf("busca_local_vizinho: Fluxo retirado:\n");
	grafo_printa(g);
	print_caminho(caminho->pai, solucao[indice_od].destino, g->n);
	getchar();/**/
	
	double fluxo_restante = caminho->fluxo;

	i = 0;
	while(fluxo_restante > 0){
		Caminho *novo_caminho = novo_Caminho_vazio(g->n);

		menor_caminho(g, solucao[indice_od].origem, novo_caminho->pai);

		double _fluxo = 1;
		novo_caminho->fluxo = fluxo_capacidade(
			g, solucao[indice_od].origem, solucao[indice_od].destino,
			novo_caminho->pai, _fluxo
		);
		
		/* Se tiver estourado a capacidade. */
		if(novo_caminho->fluxo == 0){
			novo_caminho->fluxo = _fluxo;
			fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, novo_caminho->pai, novo_caminho->fluxo);
		}else if(novo_caminho->fluxo == -1){
			//A aresta bloqueada não deixou gerar novos caminhos.
			//Então a aresta será desbloqueada e o caminho será gerado.
			aresta_bloqueada->blocked = 0;
			menor_caminho(g, solucao[indice_od].origem, novo_caminho->pai);
			
			
			novo_caminho->fluxo = _fluxo;
			fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, novo_caminho->pai, novo_caminho->fluxo);
			
			aresta_bloqueada->blocked = 1;
		}
		
		/**
		printf("fluxo restante: %5.0f/%5.0f\n", fluxo_restante, caminho->fluxo);
		printf("%d: busca_local_vizinho: inseriu %f)\n", i++, novo_caminho->fluxo);
		grafo_printa(g);
		print_caminho(novo_caminho->pai, solucao[indice_od].destino, g->n);
		getchar();/**/

		arraylist_insert_last(vizinhos, novo_caminho);
		
		fluxo_restante -= novo_caminho->fluxo;
	}
	
	aresta_bloqueada->blocked = 0;
	
	return vizinhos;
}

ArrayList *busca_local_randomica_vizinhanca_N2(Grafo *g, Solucao *solucao, int indice_od, int indice_caminho){
	Caminho *caminho = arraylist_get_index(solucao[indice_od].caminhos, indice_caminho);

	ArrayList *vizinhos = new_ArrayList();
	
	/**
	printf("============================================================================\n");
	printf("busca_local_vizinho (%d, %d): Fluxo antes (retirando %f)\n", solucao[indice_od].origem, solucao[indice_od].destino, caminho->fluxo);
	grafo_printa(g);
	print_caminho(caminho->pai, solucao[indice_od].destino, g->n);
	getchar();/**/
	
	int i = 0;
	//Retirando o fluxo do grafo.
	fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, caminho->pai, -caminho->fluxo);
	
	//Bloqueando duas arestas aleatória do caminho.
	Aresta *aresta_bloqueada_1 = NULL;
	Aresta *aresta_bloqueada_2 = NULL;
	
	int arestas = 0;
	int w = solucao[indice_od].destino;
	int v;
	//Conta a quantidade de arestas do caminho.
	do{
		v = caminho->pai[w];
		arestas++;
		w = v;
	}while(v != solucao[indice_od].origem);
	
	//Sorteia a primeira aresta.
	int aresta_aleatoria_1 = rand() % arestas;
	
	//Sorteia uma segunda aresta, caso exista mais que uma no caminho.
	int aresta_aleatoria_2 = -1;
	if(arestas > 1){
		do{
			aresta_aleatoria_2 = rand() % arestas;
		}while(aresta_aleatoria_2 == aresta_aleatoria_1);
	}
	
	//Encontrando os vértices da aresta 1.
	w = solucao[indice_od].destino;
	for(i = 0; i < aresta_aleatoria_1; i++){
		w = caminho->pai[w];
	}
	v = caminho->pai[w];
	
	//Bloqueando a aresta 1.
	for(i = 0; g->arestas[v][i].term != w; i++);
	aresta_bloqueada_1 = &g->arestas[v][i];
	aresta_bloqueada_1->blocked = 1;
	
	if(aresta_aleatoria_2 != -1){
		//Encontrando os vértices da aresta 2.
		w = solucao[indice_od].destino;
		for(i = 0; i < aresta_aleatoria_2; i++){
			w = caminho->pai[w];
		}
		v = caminho->pai[w];
		
		//Bloqueando a aresta 2.
		for(i = 0; g->arestas[v][i].term != w; i++);
		aresta_bloqueada_2 = &g->arestas[v][i];
		aresta_bloqueada_2->blocked = 1;
	}
	//printf("Arestas: %d (%d,%d)\n", arestas, aresta_aleatoria_1, aresta_aleatoria_2);
	
	/**
	printf("busca_local_vizinho: Fluxo retirado:\n");
	grafo_printa(g);
	print_caminho(caminho->pai, solucao[indice_od].destino, g->n);
	getchar();/**/
	
	double fluxo_restante = caminho->fluxo;

	i = 0;
	while(fluxo_restante > 0){
		Caminho *novo_caminho = novo_Caminho_vazio(g->n);

		menor_caminho(g, solucao[indice_od].origem, novo_caminho->pai);

		double _fluxo = 1;
		novo_caminho->fluxo = fluxo_capacidade(
			g, solucao[indice_od].origem, solucao[indice_od].destino,
			novo_caminho->pai, _fluxo
		);
		
		/* Se tiver estourado a capacidade. */
		if(novo_caminho->fluxo == 0){
			novo_caminho->fluxo = _fluxo;
			fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, novo_caminho->pai, novo_caminho->fluxo);
		}else if(novo_caminho->fluxo == -1){
			//A(s) aresta(s) bloqueada(s) não deixou gerar novos caminhos.
			//Então a primeira aresta será desbloqueada e o caminho será gerado.
			aresta_bloqueada_1->blocked = 0;
			menor_caminho(g, solucao[indice_od].origem, novo_caminho->pai);
				
			novo_caminho->fluxo = fluxo_capacidade(
				g, solucao[indice_od].origem, solucao[indice_od].destino,
				novo_caminho->pai, _fluxo
			);
			
			//A liberação da aresta 1 conseguiu gerar um caminho, as todas as arestas estavam cheias.
			if(novo_caminho->fluxo == 0){
				novo_caminho->fluxo = _fluxo;
				fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, novo_caminho->pai, novo_caminho->fluxo);
			}else if(novo_caminho->fluxo == -1){
				//Mesmo com a liberação da primeira aresta, ainda não foi possível encontrar um caminho.
				if(aresta_aleatoria_2 != -1){
					aresta_bloqueada_2->blocked = 0;
					
					menor_caminho(g, solucao[indice_od].origem, novo_caminho->pai);
					
					novo_caminho->fluxo = _fluxo;
					fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, novo_caminho->pai, novo_caminho->fluxo);
					
					aresta_bloqueada_2->blocked = 1;
				}else{
					printf("NAO ERA PRA ENTRAR AQUI....\n");
					exit(1);
				}
			}
			
			/*novo_caminho->fluxo = _fluxo;
			fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, novo_caminho->pai, novo_caminho->fluxo);
			
			aresta_bloqueada_1->blocked = 1;*/
		}
		
		/**
		printf("fluxo restante: %5.0f/%5.0f\n", fluxo_restante, caminho->fluxo);
		printf("%d: busca_local_vizinho: inseriu %f)\n", i++, novo_caminho->fluxo);
		grafo_printa(g);
		print_caminho(novo_caminho->pai, solucao[indice_od].destino, g->n);
		getchar();/**/

		arraylist_insert_last(vizinhos, novo_caminho);
		
		fluxo_restante -= novo_caminho->fluxo;
	}
	
	aresta_bloqueada_1->blocked = 0;
	if(aresta_aleatoria_2 != -1){
		aresta_bloqueada_2->blocked = 0;
	}
	
	return vizinhos;
}

int debug = 0;
double busca_local_randomica(Grafo *g, Solucao *solucao, double fo_inicial, int iter_max, ArrayList *(*vizinhanca)(Grafo *, Solucao *, int, int)){
	int iter = 0;
	int indice_od;
	double fo, fo_novo;
	ArrayList *vizinhos;
	
	if(fo_inicial == -1){
		calcular_fo(g, &fo_inicial, NULL);
	}
	
	fo = fo_inicial;
	
	while(iter < iter_max){
		iter++;
		
		//Sorteando um par OD aleatoriamente.
		Solucao *ssss;
		do{
			ssss = NULL;
			indice_od = rand() % (g->n * g->n);
			ssss = &solucao[indice_od];
		}while(solucao[indice_od].caminhos == NULL);
		
		int indice_caminho = rand() % solucao[indice_od].caminhos->length;
		
		//Obtendo os novos caminhos.
		vizinhos = vizinhanca(g, solucao, indice_od, indice_caminho);
		
		calcular_fo(g, &fo_novo, NULL);
		
		if(debug){
			printf("BLR\n");
			printf("Antigo: %17f\n", fo);
			printf(" Novo : %17f\n\n", fo_novo);
			getchar();
		}
		
		if(fo_novo < fo){
			iter = 0;
			
			/**
			printf("Melhorou\n");
			printf("Antigo: %17f\n", fo);
			printf(" Novo : %17f\n\n", fo_novo);
			/**/
			
			fo = fo_novo;
			free_Caminho(arraylist_remove_index(solucao[indice_od].caminhos, indice_caminho));
			
			//Insere os caminhos gerados na lista de caminhos do par sorteado.
			while(!arraylist_is_empty(vizinhos)){
				arraylist_insert_last(solucao[indice_od].caminhos, arraylist_remove_last(vizinhos));
			}
		}else{
			//Desfaz as alterações.
			while(!arraylist_is_empty(vizinhos)){
				Caminho *temp = arraylist_remove_last(vizinhos);
				fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, temp->pai, -temp->fluxo);
				free_Caminho(temp);
			}
			
			Caminho *temp = arraylist_get_index(solucao[indice_od].caminhos, indice_caminho);
			fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, temp->pai, temp->fluxo);
		}	
	}
	
	calcular_fo(g, &fo, NULL);
	/**
	printf("========== BLR ===========\n");
	printf("Inicial: %17f\n", fo_inicial);
	printf("  Busca: %17f\n", fo);
	printf("Melhora: %17f\n", fo_inicial - fo);
	return fo;
	/**/
}

void pertubacao(Grafo *g, Solucao *solucao){
	int i, j;
	int n2 = g->n * g->n;
	
	//Escolhendo uma solução que possui caminhos com fluxo maior que 1.
	for(i = 0; i < n2; i++){
		if(solucao[i].caminhos != NULL && solucao[i].caminhos->length < solucao[i].fluxo_total){
			for(j = 0; j < solucao[i].caminhos->length; j++){
				Caminho *cj = arraylist_get_index(solucao[i].caminhos, j);
				
				if(cj->fluxo > 1){
					ArrayList *vizinhos = busca_local_randomica_vizinhanca_N2(g, solucao, i, j);
					
					free_Caminho(arraylist_remove_index(solucao[i].caminhos, j));
			
					//Insere os caminhos gerados na lista de caminhos do par escolhido.
					while(!arraylist_is_empty(vizinhos)){
						arraylist_insert_last(solucao[i].caminhos, arraylist_remove_last(vizinhos));
					}
					
					return;
				}
				
			}
		}
	}
	
}

void ILS(Grafo *g, double **matriz_od, int iter_max){
	srand((unsigned) time(NULL));

	int i, j;
	int iter = 0;
	int n2 = g->n * g->n;
	double fo = -22, fo_novo;
	Solucao *solucao = nova_Solucao_vazia(g->n, matriz_od);
	Solucao *s_linha = calloc(n2, sizeof(Solucao));

	solucao_constroi_inicial(solucao, g);

	//Busca local
	fo = busca_local_randomica(g, solucao, -1, 100, busca_local_randomica_vizinhanca_N1);

	//Copia
	copia_solucao(s_linha, solucao, g->n);

	while(iter < iter_max){
		iter++;
		
		//Pertubação
		pertubacao(g, s_linha);

		//Busca Local
		debug = 0;
		fo_novo = busca_local_randomica(g, s_linha, -1, 100, busca_local_randomica_vizinhanca_N2);
		
		//Verificação de melhora.
		if(fo_novo < fo){
			/**
			printf("========== ILS ===========\n");
			printf("Inicial: %17f\n", fo);
			printf("  Busca: %17f\n", fo_novo);
			printf("Melhora: %17f\n", fo - fo_novo);
			//getchar();
			/**/
			
			iter = 0;
			
			fo = fo_novo;
			
			//Copia
			copia_solucao(solucao, s_linha, g->n);
		}
	}
}
