#include "fluxo.h"

double **origem_destino(const char *arquivo, int n, double *total_flow) {
	/* Abrindo o arquivo */
	FILE *in = fopen(arquivo, "r");
	if (in == NULL) {
		printf("Nao foi possivel abrir o arquivo '%s'.\n", arquivo);
		return NULL;
	}

	/* Lendo metadata */
	int z;
	fscanf(in, "<NUMBER OF ZONES> %d\n"
		"<TOTAL OD FLOW> %lf\n"
		"<END OF METADATA>\n"
		"\n"
		"\n",
		&z, total_flow);

	/* Preenchendo a matriz */
	double **fluxos = malloc(n * sizeof (*fluxos));
	int i, j, dump;
	for (i = 0; i < n; i++) {
		fluxos[i] = malloc(n * sizeof (*(fluxos[i])));
		fscanf(in, "Origin \t%d \n", &dump);
		for (j = 0; j < n; j++) {
			fscanf(in, "%d :", &dump);
			fscanf(in, "%lf; ", &fluxos[i][j]);
			if (j % 4 == 0) {
				fscanf(in, "\n");
			}
		}
		if (j % 4 != 0) {
			fscanf(in, "\n");
		}
		fscanf(in, "\n");
	}
	fclose(in);

	return fluxos;
}

void fluxo(Grafo *g, int inicio, int *caminho, double *origemI, double porcentagem){
	int i, j, v, w;
	double demanda;
	
	for(i=0; i<g->n; i++){
		if(origemI[i] > 0.0){
			//Atualizando o fluxo atual do grafo
			demanda = origemI[i] * porcentagem;
			
			//Percorre o caminho de i até inicio
			w = i;
			if(caminho[w] == -1){
				continue;
			}
			do{
				v = caminho[w];
				
				//Encontrando a aresta {v, w}
				for(j=0; g->arestas[v][j].term != w; j++);
				
				g->arestas[v][j].flow += demanda;
				
				w = v;
			}while(v != inicio);
		}
	}
}

void all_or_nothing(Grafo *g, double **matriz_od){
	int i;
	
	int caminho[g->n][g->n];
	
	for(i=0; i<g->n; i++){
		menor_caminho(g, i, caminho[i]);
	}
	
	for(i=0; i<g->n; i++){
		fluxo(g, i, caminho[i], matriz_od[i], 1);
	}
}

void incremental(Grafo *g, double **matriz_od){
	int i, j;

	int caminho[g->n][g->n];

	double porcentagem[4] = {0.4, 0.3, 0.2, 0.1};

	for(i=0; i<4; i++){
		for(j=0; j<g->n; j++){
			menor_caminho(g, j, caminho[j]);
		}

		for(j=0; j<g->n; j++){
			fluxo(g, j, caminho[j], matriz_od[j], porcentagem[i]);
		}
	}
}

void medias_sucessivas(Grafo *g, double **matriz_od){
	int i, j, k, n = 100;
	
	int caminho[g->n][g->n];

	for (i = 0; i < n; i++) {
		//Retirando 1/n do fluxo
		for(j=0; j<g->n; j++){
			for(k=0; k<g->m && g->arestas[j][k].term != -1; k++){
				g->arestas[j][k].flow -= (g->arestas[j][k].flow * (double)1 / (i+1));
			}
		}

		//Calculando o menor caminho.
		for (j = 0; j < g->n; j++) {
			menor_caminho(g, j, caminho[j]);
		}

		//Calcula e distriui o fluxo parcial.
		for(j=0; j<g->n; j++){
			fluxo(g, j, caminho[j], matriz_od[j], (double)1 / (i+1));
		}
	}
}
