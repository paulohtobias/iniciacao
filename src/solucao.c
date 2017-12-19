#include "solucao.h"

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

Solucao *nova_Solucao_vazia(int n, double **matriz_od){
	int n2 = n * n;
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
		double fluxo_restante = solucao[i].fluxo_total;
		while(fluxo_restante > 0){
			//printf("%d: OD (%d-%d) | ", i, solucao[i].origem, solucao[i].destino);

			//Calcula o menor caminho.
			Caminho *caminho = novo_Caminho_vazio(n);
			menor_caminho(g, solucao[i].origem, caminho->pai);
			
			//Tenta passar o fluxo pelo caminho
			caminho->fluxo = fluxo_capacidade(
				g, solucao[i].origem, solucao[i].destino, caminho->pai,
				fluxo_restante
			);
			
			//Se tiver estourado a capacidade.
			if(caminho->fluxo == 0){
				caminho->fluxo = fluxo_restante;
				fluxo(g, solucao[i].origem, solucao[i].destino, caminho->pai, caminho->fluxo);
			}
			
			fluxo_restante -= caminho->fluxo;
			
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

Solucao *copia_solucao(Solucao *dst, Solucao *src, int n){
	int i, j;
	int n2 = n * n;
	for(i = 0; i < n2; i++){
		dst[i].origem = src[i].origem;
		dst[i].destino = src[i].destino;
		dst[i].fluxo_total = src[i].fluxo_total;
		
		//Zerando uma possível lista de caminhos da lista src.
		if(dst[i].caminhos != NULL){
			free_ArrayList(dst[i].caminhos, free_Caminho);
			dst[i].caminhos = NULL;
		}
		
		//Copiando o caminho.
		if(src[i].caminhos != NULL){
			dst[i].caminhos = new_ArrayList_max_size(src[i].caminhos->length);
			
			for(j = 0; j < src[i].caminhos->length; j++){
				Caminho *src_caminho = arraylist_get_index(src[i].caminhos, j);
				Caminho *dst_caminho = novo_Caminho_vazio(n);
				
				dst_caminho->fluxo = src_caminho->fluxo;
				memcpy(dst_caminho->pai, src_caminho->pai, n * sizeof(*src_caminho->pai));
				
				arraylist_insert_last(dst[i].caminhos, dst_caminho);
			}
		}
	}
}
