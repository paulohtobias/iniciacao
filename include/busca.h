#ifndef BUSCA_H
#define BUSCA_H

#include "solucao.h"

void vizinhanca(Grafo *g, Solucao *solucao, SolucaoAresta *aresta);

void busca_local(Grafo *g, Solucao *solcao);

#endif /* BUSCA_H */
