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
			solucao[k].origem = i;
			solucao[k].destino = j;
			solucao[k].fluxo_total = matriz_od[i][j];
			if(matriz_od[i][j] > 0.0){
				solucao[k].caminhos = new_ArrayList();
			}else{
				//solucao[k].origem = solucao[k].destino = -1;
				solucao[k].caminhos = NULL;
			}
			k++;
		}
	}

	return solucao;
}

void solucao_constroi_inicial(Solucao *solucao, Grafo *g){
	int i;
	int n = g->n;
	getchar();
	
	qsort(solucao, n * n, sizeof(Solucao), comparar_solucao);
	
	for(i=0; i<n*n; i++){
		double fluxo_restante = solucao[i].fluxo_total;
		while(fluxo_restante > 0.0){
			Caminho *caminho = novo_Caminho_vazio(n);
			menor_caminho(g, solucao[i].origem, caminho->pai);
			caminho->fluxo = fluxo_capacidade(
				g, solucao[i].origem, solucao[i].destino, caminho->pai,
				&fluxo_restante
			);
			/**printf("%d: OD (%d-%d)\n", i, solucao[i].origem, solucao[i].destino);
			grafo_printa(g);/**/
			//getchar();
			arraylist_insert_last(solucao[i].caminhos, caminho);
		}
	}
}

int comparar_solucao(Solucao *s1, Solucao *s2){
	if(s1->fluxo_total < s2->fluxo_total){
		return 1;
	}else if(s1->fluxo_total > s2->fluxo_total){
		return -1;
	}else{
		return 0;
	}
}

void solucao_teste(Grafo *g, double **matriz_od){
	Solucao *solucao = nova_Solucao_vazia(g->n, matriz_od);
	
	solucao_constroi_inicial(solucao, g);
}
