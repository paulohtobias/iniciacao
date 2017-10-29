#include "solucao.h"

Caminho *novo_Caminho_vazio(int n){
	Caminho *caminho = malloc(sizeof(Caminho));
	
	caminho->fluxo = 0.0;
	caminho->pai = malloc(n * sizeof(int));
	
	return caminho;
}

Solucao *nova_Solucao_vazia(int n, double **matriz_od){
	int n2 = n*n;
	Solucao *solucao = malloc(n2 * sizeof(Solucao));
	
	int i, j, k = 0;
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			if(matriz_od[i][j] > 0.0){
				solucao[k].origem = i;
				solucao[k].destino = j;
				solucao[k].caminhos = new_ArrayList();
			}else{
				solucao[k].origem = solucao[k].destino = -1;
				solucao[k].caminhos = NULL;
			}
			k++;
		}
	}
	
	return solucao;
}

void solucao_constroi_inicial(Solucao *solucao, Grafo *g, double **matriz_od){
	int i, j, k = 0;
	int n = g->n;
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			while(matriz_od[i][j] > 0.0){
				Caminho *caminho = novo_Caminho_vazio(n);
				menor_caminho(g, k, caminho->pai);
				caminho->fluxo = fluxo_capacidade(g, i, j, caminho->pai, &matriz_od[i][j]);
				arraylist_insert_last(solucao[k].caminhos, caminho);
			}
			k++;
		}
	}
}
