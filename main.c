#include "busca.h"

int i, j;
int main(int argc, char** argv) {
	if(argc < 3){
		printf("Faltam os nomes dos arquivos de testes.\n");
		printf("./main.out testes/SiouxFalls/SiouxFalls_net.txt testes/SiouxFalls/SiouxFalls_trips.txt\n"
			   "./main.out testes/RedeArtigo/redeArtigo.txt testes/RedeArtigo/tripsArtigo.txt\n");
		return 1;
	}
	//Grafo inicial tem o free flow time como peso das arestas.
	Grafo *g = novo_Grafo_arquivo(argv[1]);
	
	//grafo_printa(g); //return 0;

	//Inicializçao da matriz OD.
	double **matriz_od = origem_destino(argv[2], g->n, &g->total_flow);
	
	/*for(i=0; i<g->n; i++){
		for(j=0; j<g->n; j++){
			printf("%4d ", matriz_od[i][j]);
		}
		printf("\n");
	}
	return 0;*/

	//Menu
	int op = atoi(argv[3]);
	/*printf("1 - All or Nothing\n"
		   "2 - Incremental\n"
		   "3 - Medias Sucessivas\n"
		   "4 - Solucao\n");
	
	scanf("%d", &op);*/
	
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
		case 4:
			solucao_teste(g, matriz_od);
			break;
		default:
			printf("Opção inválida\n");
	}
	
	//grafo_printa(g);
	
	//Calculando tempo medio.
	double tmedio = 0;
	int estouro = 0;
	
	calcular_fo(g, &tmedio, &estouro);
	
	printf("%f\n", tmedio); return 0;
	printf("Tempo medio: %f\n", tmedio);
	printf("Estouro: %d/%d\n", estouro, g->m);

	return 0;
}
