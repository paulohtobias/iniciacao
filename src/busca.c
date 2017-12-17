#include "busca.h"

ArrayList *busca_local_vizinho_od(Grafo *g, Solucao *solucao, int indice_od, int indice_caminho){
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

void busca_local(Grafo *g, Solucao *solucao){
	int melhora = 0;
	int indice_od;
	double fo, fo_novo;
	ArrayList *vizinhos;
	
	calcular_fo(g, &fo, NULL);
	
	double fo_inicial = fo;
	
	do{
		
		//Sorteando um par OD aleatoriamente.
		Solucao *ssss;
		do{
			ssss = NULL;
			indice_od = rand() % (g->n * g->n);
			ssss = &solucao[indice_od];
		}while(solucao[indice_od].caminhos == NULL);
		
		int indice_caminho = rand() % solucao[indice_od].caminhos->length;
		
		//Obtendo os novos caminhos.
		vizinhos = busca_local_vizinho_od(g, solucao, indice_od, indice_caminho);
		
		calcular_fo(g, &fo_novo, NULL);
		
		if(fo_novo < fo){
			/**
			printf("Melhorou\n");
			printf("Antigo: %17f\n", fo);
			printf(" Novo : %17f\n\n", fo_novo);
			/**/
			
			melhora = 0;
			
			fo = fo_novo;
			free_Caminho(arraylist_remove_index(solucao[indice_od].caminhos, indice_caminho));
			
			//Insere os caminhos gerados na lista de caminhos do par sorteado.
			while(!arraylist_is_empty(vizinhos)){
				arraylist_insert_last(solucao[indice_od].caminhos, arraylist_remove_last(vizinhos));
			}
		}else{
			melhora++;
			
			//Desfaz as alterações.
			while(!arraylist_is_empty(vizinhos)){
				Caminho *temp = arraylist_remove_last(vizinhos);
				fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, temp->pai, -temp->fluxo);
				free_Caminho(temp);
			}
			
			Caminho *temp = arraylist_get_index(solucao[indice_od].caminhos, indice_caminho);
			fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, temp->pai, temp->fluxo);
		}
		
	}while(melhora < 100);
	
	/*printf("=========================\n");
	printf("Inicial: %17f\n", fo_inicial);
	printf("  Busca: %17f\n", fo);
	printf("Melhora: %17f\n", fo_inicial - fo);*/
}
