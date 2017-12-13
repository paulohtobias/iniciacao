#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "array_list.h"
#include "fluxo.h"
#include <time.h>

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

void free_Caminho(Caminho *caminho);

void print_caminho(int *caminho, int destino, int n);

Solucao *nova_Solucao_vazia(int n, double **matriz_od);

void solucao_constroi_inicial(Solucao *solucao, Grafo *g);

int comparar_solucao(const void *s1, const void *s2);

#endif /* SOLUCAO_H */
