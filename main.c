#include "fluxo.h"

int main(int argc, char** argv) {
	//Grafo inicial tem o free flow time como peso das arestas.
	Grafo *g = novo_Grafo_arquivo("testes/SiouxFalls/SiouxFalls_net.txt");

	//Inicializçao da matriz OD.
	double **matriz_od = origem_destino("testes/SiouxFalls/SiouxFalls_trips.txt", g->n);

	//Menu
	int op;
	printf("1 - All or Nothing\n"
		   "2 - Incremental\n"
		   "3 - Medias Sucessivas\n");
	
	scanf("%d", &op);
	
	switch(op){
		case 1:
			all_or_nothing(g, matriz_od);
			break;
		case 2:
			incremental(g, matriz_od);
			break;
		case 3:
			medias_sucessivas(g, matriz_od);
			break;
		default:
			printf("Opção inválida\n");
	}
	
	grafo_printa(g);
	
	//Calculando tempo medio.
	int maior_i = 0, maior_j = 0;
	int menor_i = 0, menor_j = 0;
	double tempo;
	double tmedio = 0;
	int veiculos = 360600;
	int estouro = 0;
	
	int i, j;
	for(i=0; i<g->n; i++){
		for(j=0; g->arestas[i][j].term != -1; j++){
			
			printf("Aresta {%d, %d} %f/%f: %f%%\n", i + 1, g->arestas[i][j].term + 1, g->arestas[i][j].flow, g->arestas[i][j].capacity, 100 * g->arestas[i][j].flow / g->arestas[i][j].capacity);
			
			tempo = calcular_tempo(g->arestas[i][j]);
			tmedio += (tempo * g->arestas[i][j].flow);
			if(g->arestas[i][j].flow > g->arestas[i][j].capacity){
				estouro++;
			}
			//Achando o maior tempo.
			if(tempo > calcular_tempo(g->arestas[maior_i][maior_j])){
				maior_i = i;
				maior_j = j;
			}
			//Achando o menor tempo.
			if(tempo < calcular_tempo(g->arestas[menor_i][menor_j])){
				menor_i = i;
				menor_j = j;
			}
		}
	}
	
	printf("Maior tempo {%d, %d}: %f\n", maior_i + 1, g->arestas[maior_i][maior_j].term + 1, calcular_tempo(g->arestas[maior_i][maior_j]));
	printf("Menor tempo {%d, %d}: %f\n", menor_i + 1, g->arestas[menor_i][menor_j].term + 1, calcular_tempo(g->arestas[menor_i][menor_j]));
	printf("Tempo medio: %f\n", tmedio / veiculos);
	printf("Estouro: %d/%d\n", estouro, g->m);

	return 0;
}
