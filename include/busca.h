#ifndef BUSCA_H
#define BUSCA_H

#include "solucao.h"

ArrayList *busca_local_randomica_vizinhanca_N1(Grafo *g, Solucao *solucao, int indice_od, int indice_caminho);

ArrayList *busca_local_randomica_vizinhanca_N2(Grafo *g, Solucao *solucao, int indice_od, int indice_caminho);

double busca_local_randomica(Grafo *g, Solucao *solucao, double fo_inicial, int iter_max, ArrayList *(*vizinhanca)(Grafo *, Solucao *, int, int));

void ILS(Grafo *g, double **matriz_od, int iter_max);

#endif /* BUSCA_H */
