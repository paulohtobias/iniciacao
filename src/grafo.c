#include "grafo.h"

Aresta nova_Aresta(int saida, int chegada, double peso) {
	Aresta aresta;
	aresta.init = saida;
	aresta.term = chegada;
	aresta.travel_time = peso;
	return aresta;
}

double calcular_tempo(Aresta aresta){
	double stall = 1 + aresta.b * pow(aresta.flow / aresta.capacity, aresta.power);
	return aresta.fft * stall;
}

Grafo *novo_Grafo_vazio(int n, int m) {
	Grafo *g = malloc(sizeof (Grafo));

	g->n = n;
	g->m = m;

	int i, j;
	g->arestas = malloc(n * sizeof (*g->arestas));
	for (i = 0; i < n; i++) {
		g->arestas[i] = malloc(m * sizeof (*g->arestas[i]));

		for (j = 0; j < m; j++) {
			g->arestas[i][j] = nova_Aresta(i, -1, INFINITY);
		}
	}

	return g;
}

Grafo *novo_Grafo_arquivo(const char *arquivo) {
	/* Abrindo o arquivo */
	FILE *in = fopen(arquivo, "r");
	if (in == NULL) {
		printf("Nao foi possivel abrir o arquivo '%s'.\n", arquivo);
		return NULL;
	}

	/* Lendo metadata */
	int n, z, ftn, m;
	fscanf(in, "<NUMBER OF ZONES> %d\t\t\t\t\t\t\t\t\t\t\t\n"
		"<NUMBER OF NODES> %d\t\t\t\t\t\t\t\t\t\t\t\n"
		"<FIRST THRU NODE> %d\t\t\t\t\t\t\t\t\t\t\t\n"
		"<NUMBER OF LINKS> %d\t\t\t\t\t\t\t\t\t\t\t\n"
		"<END OF METADATA>\t\t\t\t\t\t\t\t\t\t\t\n"
		"\n"
		"\n"
		"~ \tInit node \tTerm node \tCapacity \tLength \tFree Flow Time \tB\tPower\tSpeed limit \tToll \tType\t;\n",
		&z, &n, &ftn, &m);

	/* Preenchendo o grafo */
	Grafo *g = novo_Grafo_vazio(n, m);

	Aresta aresta;
	int i;
	for (i = 0; i < m; i++) {
		fscanf(in, "\t%d\t%d\t%lf\t%d\t%d\t%lf\t%d\t%d\t%d\t%d\t;\n",
			&aresta.init, &aresta.term, &aresta.capacity, &aresta.length,
			&aresta.fft, &aresta.b, &aresta.power, &aresta.speed, &aresta.toll,
			&aresta.type);
		
		--aresta.init;
		--aresta.term;
		aresta.flow = 0;
		aresta.travel_time = calcular_tempo(aresta);
		grafo_add_aresta_d(g, aresta);
	}
	fclose(in);

	return g;
}

void grafo_add_aresta_d(Grafo *g, Aresta aresta) {
	int saida = aresta.init;
	int chegada = aresta.term;
	if (saida >= 0 && saida < g->n && chegada >= 0 && chegada < g->n) {
		int i;
		for (i = 0; g->arestas[saida][i].term != -1; i++);
		g->arestas[saida][i] = aresta;
	}
}

Aresta grafo_get_aresta(Grafo *g, int saida, int chegada){
	int i;
	for(i=0; g->arestas[saida][i].term != chegada; i++);
	return g->arestas[saida][i];
}

void grafo_printa(Grafo *g) {
	int i, j;
	
	for (i = 0; i < g->n; i++) {
		printf("%2d: ", i + 1);
		for (j = 0; j < g->m && g->arestas[i][j].term != -1; j++) {
			double tempo = calcular_tempo(g->arestas[i][j]);
			printf("(%2d: %f) ", g->arestas[i][j].term + 1, tempo);
		}
		printf("\n");
	}
}

void menor_caminho(Grafo *g, int inicio, int *pai) {
	int i;
	int escolhido[g->n];
	double peso[g->n];
	bheap_t *heap = bh_alloc(g->n);

	for (i = 0; i < g->n; i++) {
		escolhido[i] = 0;
		pai[i] = -1;
		peso[i] = INFINITY;
		bh_insert(heap, i, INFINITY);
	}
	peso[inicio] = 0;
	bh_decrease_key(heap, inicio, 0);

	int v = inicio, w;
	while (1) {
		if(bh_empty(heap)){
			break;
		}
		v = bh_min(heap);
		escolhido[v] = 1;

		//Atualizando
		for (i = 0; i < g->m && g->arestas[v][i].term != -1; i++) {
			w = g->arestas[v][i].term;
			if (!escolhido[w]) {
				double travel_time = calcular_tempo(g->arestas[v][i]);
				if (peso[w] > (peso[v] + travel_time)) {
					peso[w] = peso[v] + travel_time;
					bh_decrease_key(heap, w, peso[w]);
					pai[w] = v;
				}
			}
		}
	}
	bh_free(heap);
}

double **origem_destino(const char *arquivo, int n) {
	/* Abrindo o arquivo */
	FILE *in = fopen(arquivo, "r");
	if (in == NULL) {
		printf("Nao foi possivel abrir o arquivo '%s'.\n", arquivo);
		return NULL;
	}

	/* Lendo metadata */
	int z;
	double tf;
	fscanf(in, "<NUMBER OF ZONES> %d\n"
		"<TOTAL OD FLOW> %lf\n"
		"<END OF METADATA>\n"
		"\n"
		"\n",
		&z, &tf);

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
			//Percorre o caminho de i até inicio
			//Atualizando o fluxo atual do grafo
			demanda = origemI[i] * porcentagem;
			w = i;
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
