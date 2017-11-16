#include "solucao.h"
#include "busca.h"

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
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
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

	for(i = 0; i < n * n; i++){
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

int comparar_solucao(const void *s1, const void *s2){
	const Solucao *solucao1 = s1;
	const Solucao *solucao2 = s2;
	if(solucao1->fluxo_total < solucao2->fluxo_total){
		return 1;
	}else if(solucao1->fluxo_total > solucao2->fluxo_total){
		return -1;
	}else{
		return 0;
	}
}

void solucao_teste(Grafo *g, double **matriz_od){
	int melhor_caminho[g->n][g->n];
	double melhor_peso[g->n][g->n];

	int i, j;
	for(i = 0; i < g->n; i++){
		menor_caminho(g, i, melhor_caminho[i]);
		for(j = 0; j < g->n; j++){
			if(matriz_od[i][j] > 0){
				if(i == 9 && j == 15){
					melhor_peso[i][j] = calcular_tempo_caminho(g, melhor_caminho[i], i, j);
				}
				melhor_peso[i][j] = calcular_tempo_caminho(g, melhor_caminho[i], i, j);
			}else{
				melhor_peso[i][j] = 0;
			}
		}
	}

	Solucao *solucao = nova_Solucao_vazia(g->n, matriz_od);

	solucao_constroi_inicial(solucao, g);

	srand((unsigned) time(NULL));

	busca_local(g, solucao);

	double fora = 0;
	int v, w, a, b;
	int o, d;
	for(i = 0; i < g->n * g->n; i++){ //Para cada par OD
		o = solucao[i].origem;
		d = solucao[i].destino;
		if(solucao[i].caminhos != NULL){

			for(j = 0; j < solucao[i].caminhos->length; j++){ //Para cada caminho j

				Caminho *cj = arraylist_get_index(solucao[i].caminhos, j);

				/*
				 * Olhando se o caminho de d até o do caminho j é igual ao
				 * melhor caminho entre o e d.
				 */
				w = d;
				v = cj->pai[w];
				
				b = d;
				a = melhor_caminho[o][b];
				
				while(v != -1 && v == a && w == b){
					w = v;
					v = cj->pai[w];
					
					b = a;
					a = melhor_caminho[o][b];
				}

				//Se v == -1, então o caminho j é igual ao melhor caminho.
				if(v != -1){
					fora += cj->fluxo;
				}


				/*
				 * Se o tempo do caminho j for maior que o melho tempo do
				 * caminho OD, então todos os carros deste caminho estão fora do
				 * caminho ideal.
				 */
				/*if(calcular_tempo_caminho(g, cj->pai, o, d) != melhor_peso[o][d]){
					fora += cj->fluxo;
				}*/
			}
		}
	}
	printf("%f carros ficaram fora do seu caminho ideal\n", fora);
}
