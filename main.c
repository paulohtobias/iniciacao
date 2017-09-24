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

	return 0;
}
