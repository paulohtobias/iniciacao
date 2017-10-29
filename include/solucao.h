#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "array_list.h"
#include "fluxo.h"

typedef struct Caminho{
	double fluxo;
	int *pai;
}Caminho;

typedef struct Solucao{
	int origem;
	int destino;
	ArrayList *caminhos;
}Solucao;

Caminho *novo_Caminho_vazio(int n);

Solucao *nova_Solucao_vazia(int n, double **matriz_od);

void solucao_constroi_inicial(Solucao *solucao, Grafo *g, double **matriz_od);

#endif /* SOLUCAO_H */
