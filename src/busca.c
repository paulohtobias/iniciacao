#include "busca.h"

ArrayList *busca_local_vizinho(Grafo *g, Solucao *solucao, int indice_od, int indice_caminho){
	Caminho *caminho = arraylist_get_index(solucao[indice_od].caminhos, indice_caminho);

	ArrayList *vizinhos = new_ArrayList();
	
	printf("busca_local_vizinho (%d, %d): Fluxo antes (retirando %d)\n", solucao[indice_od].origem, solucao[indice_od].destino, caminho->fluxo);
	grafo_printa(g);
	print_caminho(caminho->pai, solucao[indice_od].destino, g->n);
	getchar();
	
	//Retirando o fluxo do grafo.
	fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, caminho->pai, -caminho->fluxo);
	
	/*printf("busca_local_vizinho: Fluxo retirado:\n");
	grafo_printa(g);
	print_caminho(caminho->pai, solucao[indice_od].destino, g->n);
	getchar();*/
	
	int fluxo_restante = caminho->fluxo;

	int i = 0;
	while(fluxo_restante > 0.0){
		Caminho *novo_caminho = novo_Caminho_vazio(g->n);

		menor_caminho(g, solucao[indice_od].origem, novo_caminho->pai);

		novo_caminho->fluxo = fluxo_capacidade(
			g, solucao[indice_od].origem, solucao[indice_od].destino,
			novo_caminho->pai, &fluxo_restante
		);
		
		/*printf("%d: busca_local_vizinho: inseriu %d)\n", i++, novo_caminho->fluxo);
		grafo_printa(g);
		print_caminho(novo_caminho->pai, solucao[indice_od].destino, g->n);
		getchar();*/

		arraylist_insert_last(vizinhos, novo_caminho);
	}
	
	return vizinhos;
}

void busca_local(Grafo *g, Solucao *solucao){
	int melhora = 0;
	int indice_od;
	double fo, fo_novo;
	ArrayList *vizinhos;
	
	calcular_fo(g, &fo, NULL);
	
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
		vizinhos = busca_local_vizinho(g, solucao, indice_od, indice_caminho);
		
		calcular_fo(g, &fo_novo, NULL);
		
		if(fo_novo < fo){
			printf("Melhorou\n");
			printf("Antigo: %17f\n", fo);
			printf(" Novo : %17f\n\n", fo_novo);
			
			melhora = 0;
			
			fo = fo_novo;
			arraylist_remove_index(solucao[indice_od].caminhos, indice_caminho);
			
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
				free(temp->pai);
				free(temp);
			}
			
			Caminho *temp = arraylist_get_index(solucao[indice_od].caminhos, indice_caminho);
			fluxo(g, solucao[indice_od].origem, solucao[indice_od].destino, temp->pai, temp->fluxo);
		}
		
	}while(melhora < 100);
}
