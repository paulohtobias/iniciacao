#include "fluxo.h"

int **origem_destino(const char *arquivo, int n, int *total_flow) {
	/* Abrindo o arquivo */
	FILE *in = fopen(arquivo, "r");
	if (in == NULL) {
		printf("Nao foi possivel abrir o arquivo '%s'.\n", arquivo);
		return NULL;
	}

	/* Lendo metadata */
	int z;
	double dtotal_flow;
	fscanf(in, "<NUMBER OF ZONES> %d\n"
		"<TOTAL OD FLOW> %lf\n"
		"<END OF METADATA>\n"
		"\n"
		"\n",
		&z, &dtotal_flow);

	(*total_flow) = dtotal_flow;
	/* Preenchendo a matriz */
	//(*total_flow) *= 0.1;
	int **fluxos = malloc(n * sizeof (*fluxos));
	int i, j, dump;
	double dfluxo;
	for (i = 0; i < n; i++) {
		fluxos[i] = malloc(n * sizeof (*(fluxos[i])));
		fscanf(in, "Origin \t%d \n", &dump);
		for (j = 0; j < n; j++) {
			fscanf(in, "%d :", &dump);
			fscanf(in, "%lf; ", &dfluxo);
			fluxos[i][j] = dfluxo;
			//fluxos[i][j]*=0.1;
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

void fluxo(Grafo *g, int origem, int destino, int *pai, int demanda){
	int i, v, w;
	
	w = destino;
	do{
		v = pai[w];

		//Encontrando a aresta {v, w}
		for(i=0; g->arestas[v][i].term != w; i++);

		g->arestas[v][i].flow += demanda;

		w = v;
	}while(v != origem);
}

void fluxo_origem(Grafo *g, int inicio, int *caminho, int *origemI, double porcentagem){
	int i;
	int demanda;
	
	for(i=0; i<g->n; i++){
		if(origemI[i] > 0){
			//Atualizando o fluxo atual do grafo
			demanda = (int)(origemI[i] * porcentagem);
			
			//Aloca o fluxo.
			fluxo(g, inicio, i, caminho, demanda);
		}
	}
}

int fluxo_capacidade(Grafo *g, int origem, int destino, int *caminho, int *demanda){
	int i, v, w;
	
	int fluxo_maximo;

	//Encontrando o gargalo.
	fluxo_maximo = *demanda;
	//Percorre o caminho de i até inicio
	w = destino;
	if(caminho[w] == -1){
		printf("Nao existe caminho de %d ate %d\n", origem, destino);
		//getchar();
	}else{
		do{
			v = caminho[w];

			//Encontrando a aresta {v, w}
			for(i=0; g->arestas[v][i].term != w; i++);

			int capacidade_residual = (int)g->arestas[v][i].capacity - g->arestas[v][i].flow;
			if(capacidade_residual <= 0){
				//printf("OD: (%2d, %2d) | Aresta cheia: {%2d, %2d}\n", origem, destino, g->arestas[v][i].init, g->arestas[v][i].term);
				return 0;
			}

			if(capacidade_residual < fluxo_maximo){
				fluxo_maximo = capacidade_residual;
			}

			w = v;
		}while(v != origem);

		//Alocando o fluxo.
		fluxo(g, origem, destino, caminho, *demanda);
	}

	//Atualiza a demanda.
	(*demanda) -= fluxo_maximo;
	
	return fluxo_maximo;
}

void calcular_fo(Grafo *g, double *tmedio, int *arestas_estouradas){
	double tempo;
	
	int i, j;
	for(i=0; i<g->n; i++){
		for(j=0; g->arestas[i][j].term != -1; j++){
			
			//printf("Aresta {%d, %d} %f/%f: %.2f%%\n", i + 1, g->arestas[i][j].term + 1, g->arestas[i][j].flow, g->arestas[i][j].capacity, 100 * g->arestas[i][j].flow / g->arestas[i][j].capacity);
			
			//Calculando o tempo.
			if(tmedio != NULL){
				tempo = calcular_tempo(g->arestas[i][j]);
				(*tmedio) += (tempo * g->arestas[i][j].flow);
			}
			
			//Calculando o número de arestas que estouraram a capcidade máxima.
			if(arestas_estouradas != NULL && g->arestas[i][j].flow > g->arestas[i][j].capacity){
				(*arestas_estouradas)++;
			}
		}
	}
	(*tmedio) /= g->total_flow;
}

void all_or_nothing(Grafo *g, int **matriz_od){
	int i;
	
	int caminho[g->n][g->n];
	
	for(i=0; i<g->n; i++){
		menor_caminho(g, i, caminho[i]);
	}
	
	for(i=0; i<g->n; i++){
		fluxo_origem(g, i, caminho[i], matriz_od[i], 1);
	}
}

void incremental(Grafo *g, int **matriz_od){
	int i, j;

	int caminho[g->n][g->n];

	double porcentagem[4] = {0.4, 0.3, 0.2, 0.1};

	for(i=0; i<4; i++){
		for(j=0; j<g->n; j++){
			menor_caminho(g, j, caminho[j]);
		}

		for(j=0; j<g->n; j++){
			fluxo_origem(g, j, caminho[j], matriz_od[j], porcentagem[i]);
		}
	}
}

void medias_sucessivas(Grafo *g, int **matriz_od){
	int i, j, k, n = 100;
	
	int caminho[g->n][g->n];

	for (i = 0; i < n; i++) {
		//Retirando 1/n do fluxo
		for(j=0; j<g->n; j++){
			for(k=0; k<g->m && g->arestas[j][k].term != -1; k++){
				g->arestas[j][k].flow -= (int)(g->arestas[j][k].flow * (double)1 / (i+1));
			}
		}

		//Calculando o menor caminho.
		for (j = 0; j < g->n; j++) {
			menor_caminho(g, j, caminho[j]);
		}

		//Calcula e distriui o fluxo parcial.
		for(j=0; j<g->n; j++){
			fluxo_origem(g, j, caminho[j], matriz_od[j], (double)1 / (i+1));
		}
	}
}
