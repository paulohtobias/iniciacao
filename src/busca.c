#include "busca.h"

void busca_local_vizinho(Grafo *g, Solucao *solucao, int indice){
	int caminho_indice = rand() % solucao[indice].caminhos->length;

	Caminho *caminho = arraylist_remove_index(solucao[indice].caminhos, caminho_indice);

	int i;
	//Retirando o fluxo do grafo.
	int w = solucao[indice].destino;
	int v = caminho->pai[w];
	while(v != solucao[indice].origem){

		for(i = 0; g->arestas[v][i].term != w; i++);

		g->arestas[v][i].flow -= caminho->fluxo;

		w = v;
		v = caminho->pai[w];
	}

	while(caminho->fluxo > 0.0){
		Caminho *novo_caminho = novo_Caminho_vazio(g->n);

		menor_caminho(g, solucao[indice].origem, novo_caminho->pai);

		novo_caminho->fluxo = fluxo_capacidade(
			g, solucao[indice].origem, solucao[indice].destino,
			novo_caminho->pai, &caminho->fluxo
		);

		arraylist_insert_last(solucao[indice].caminhos, novo_caminho);
	}
}
