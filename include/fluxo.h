#ifndef FLUXO_H
#define FLUXO_H

#include "grafo.h"

double **origem_destino(const char *arquivo, int n, double *total_flow);

void fluxo(Grafo *g, int inicio, int *caminho, double *origemI, double porcentagem);

double fluxo_capacidade(Grafo *g, int origem, int destino, int *caminho, double *demanda);

void all_or_nothing(Grafo *g, double **matriz_od);

void incremental(Grafo *g, double **matriz_od);

void medias_sucessivas(Grafo *g, double **matriz_od);

#endif /* FLUXO_H */
