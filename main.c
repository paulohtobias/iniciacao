#include "grafo.h"

/*
 * 
 */
int main(int argc, char** argv) {
	int i, j;
	
	//Grafo inicial tem o free flow time como peso das arestas.
	Grafo *g = novo_Grafo_arquivo("testes/SiouxFalls/SiouxFalls_net.txt");

	//Inicializçao da matriz OD.
	double **od_matriz = origem_destino("testes/SiouxFalls/SiouxFalls_trips.txt", g->n);

	//Inicializaçao da matriz de caminhos.
	int **caminho = malloc(g->n * sizeof(int *));
	for(i=0; i<g->n; i++){
		caminho[i] = malloc(g->n * sizeof(int));
	}

	int n = 4;
	double porcentagem[4] = {0.4, 0.3, 0.2, 0.1};

	for (i = 0; i < n; i++) {
		//Calculando o menor caminho.
		for (j = 0; j < g->n; j++) {
			menor_caminho(g, j, caminho[j]);
		}
		
		//Calcula e distriui o fluxo parcial.
		for(j=0; j<g->n; j++){
			fluxo(g, j, caminho[j], od_matriz[j], 1);
		}

		double total = 0;
		for (i = 0; i < g->n; i++) {
			for (j = 0; j < g->m && g->arestas[i][j].term != -1; j++) {
				total += g->arestas[i][j].flow;
			}
		}
		printf("Total: %f\n", total);
		
		//grafo_printa(g);return 0;
		/* Mostrando os fluxos */
		/*printf("Fluxo parcial: %3.0f%%\n", porcentagem[i]);
		printf("      | ");
		for (i = 0; i < g->n; i++) {
			printf("%6d", i + 1);
		}
		printf("\n");*/
		/*for (i = 0; i < g->n; i++) {
			printf("%6d: ", i + 1);
			for (j = 0; j < g->arestas[i][j].term; j++) {
				printf("%6.0lf", fluxo_total[i][j]);
			}
			printf("\n");
		}*/
	}

	return 0;
}

