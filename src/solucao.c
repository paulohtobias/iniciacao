#include "solucao.h"
#include "busca.h"

Caminho *novo_Caminho_vazio(int n){
	Caminho *caminho = malloc(sizeof(Caminho));

	caminho->fluxo = 0;
	caminho->pai = malloc(n * sizeof(int));

	return caminho;
}

void free_Caminho(Caminho *caminho){
	free(caminho->pai);
	free(caminho);
}

void print_caminho(int *caminho, int destino, int n){
	int c[n];
	int i = 0;
	int v = destino;
	do{
		c[i++] = v;
		v = caminho[v];
	}while(v != -1);
	
	for(i = i - 1; i >= 0; i--){
		printf("%2d - ", c[i]);
	}
}

Solucao *nova_Solucao_vazia(int n, int **matriz_od){
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

	qsort(solucao, n * n, sizeof(Solucao), comparar_solucao);

	for(i = 0; i < n * n; i++){
		int fluxo_restante = solucao[i].fluxo_total;
		while(fluxo_restante > 0){
			//printf("%d: OD (%d-%d) | ", i, solucao[i].origem, solucao[i].destino);

			//Calcula o menor caminho.
			Caminho *caminho = novo_Caminho_vazio(n);
			menor_caminho(g, solucao[i].origem, caminho->pai);
			
			//Tenta passar o fluxo pelo caminho
			caminho->fluxo = fluxo_capacidade(
				g, solucao[i].origem, solucao[i].destino, caminho->pai,
				&fluxo_restante
			);
			
			//Se tiver estourado a capacidade.
			if(caminho->fluxo == 0){
				caminho->fluxo = fluxo_restante;
				fluxo_restante = 0;
				fluxo(g, solucao[i].origem, solucao[i].destino, caminho->pai, caminho->fluxo);
			}
			
			/*printf("Fluxo (C,R,T): (%d,%d,%d)\n", caminho->fluxo, fluxo_restante, solucao[i].fluxo_total);
			print_caminho(caminho->pai, solucao[i].destino, n);
			printf("\n");
			grafo_printa(g);*/
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

void solucao_teste(Grafo *g, int **matriz_od){
	int melhor_caminho[g->n][g->n];
	double melhor_custo[g->n][g->n];

	int i, j;
	/*for(i = 0; i < g->n; i++){
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
	}*/

	Solucao *solucao = nova_Solucao_vazia(g->n, matriz_od);

	solucao_constroi_inicial(solucao, g);

	srand((unsigned) time(NULL));

	busca_local(g, solucao);
	
	//Achando o melhor caminho.
	for(i=0; i<g->n; i++){
		menor_caminho(g, i, melhor_caminho[i]);
		for(j=0; j<g->n; j++){
			if(matriz_od[i][j] > 0){
				melhor_custo[i][j] = calcular_tempo_caminho(g, melhor_caminho[i], i, j);
			}else{
				melhor_custo[i][j] = INFINITY;
			}
		}
	}

	int fora = 0;
	double aec = 0;
	int o, d;
	for(i = 0; i < g->n * g->n; i++){ //Para cada par OD
		o = solucao[i].origem;
		d = solucao[i].destino;
		if(solucao[i].caminhos != NULL){
			
			for(j = 0; j < solucao[i].caminhos->length; j++){ //Para cada caminho j

				Caminho *cj = arraylist_get_index(solucao[i].caminhos, j);

				/*
				 * Se o tempo do caminho j for maior que o melhor tempo do
				 * caminho OD, então todos os carros deste caminho estão fora do
				 * caminho ideal.
				 */
				double custo_r = calcular_tempo_caminho(g, cj->pai, o, d);
				/*printf("Melhor tempo entre %d - %d: %f\n", o, d, melhor_peso[o][d]);
				printf("Tempo calculado: %f # Fluxo: %.0f\n", tempo, cj->fluxo);
				getchar();*/
				
				aec += (cj->fluxo * (custo_r - melhor_custo[o][d]));
				
				if(custo_r != melhor_custo[o][d]){
					fora += cj->fluxo;
				}
			}
		}
	}
	aec /= g->total_flow;
	printf("%d/%d (%.2f%%) carros ficaram fora do seu caminho ideal\n", fora, g->total_flow, (double)fora / g->total_flow * 100);
	printf("AEC: %f\n\n", aec);
}
