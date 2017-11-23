#ifndef FLUXO_H
#define FLUXO_H

#include "grafo.h"

int **origem_destino(const char *arquivo, int n, int *total_flow);

void fluxo(Grafo *g, int origem, int destino, int *pai, int demanda);

void fluxo_origem(Grafo *g, int inicio, int *caminho, int *origemI, double porcentagem);

int fluxo_capacidade(Grafo *g, int origem, int destino, int *caminho, int *demanda);

void calcular_fo(Grafo *g, double *tmedio, int *arestas_estouradas);

void all_or_nothing(Grafo *g, int **matriz_od);

void incremental(Grafo *g, int **matriz_od);

void medias_sucessivas(Grafo *g, int **matriz_od);

#endif /* FLUXO_H */
