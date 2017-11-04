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
	double fluxo_total;
	ArrayList *caminhos;
}Solucao;

Caminho *novo_Caminho_vazio(int n);

Solucao *nova_Solucao_vazia(int n, double **matriz_od);

void solucao_constroi_inicial(Solucao *solucao, Grafo *g);

int comparar_solucao(Solucao *s1, Solucao *s2);

#endif /* SOLUCAO_H */
