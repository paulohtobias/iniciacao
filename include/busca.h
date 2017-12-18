#ifndef BUSCA_H
#define BUSCA_H

#include "solucao.h"

ArrayList *busca_local_randomica(Grafo *g, Solucao *solucao, int indice_od, int indice_caminho);

ArrayList *busca_local_randomica_N2(Grafo *g, Solucao *solucao, int indice_od, int indice_caminho);

void busca_local(Grafo *g, Solucao *solcao);

#endif /* BUSCA_H */
