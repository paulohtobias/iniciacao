#ifndef BUSCA_H
#define BUSCA_H

#include "solucao.h"

ArrayList *busca_local_vizinho_od(Grafo *g, Solucao *solucao, int indice_od, int indice_caminho);

void busca_local(Grafo *g, Solucao *solcao);

#endif /* BUSCA_H */
