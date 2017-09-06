#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Grafo{
    int n; /** Número de vértices. */
    int m; /** Número de arestas. */
    
    double **adj; /** Matriz de adjacência. */
}Grafo;

Grafo *novo_Grafo_vazio(int n);

Grafo *novo_Grafo_arquivo(const char *arquivo);

void grafo_add_aresta_d(Grafo *g, int saida, int chegada, double peso);

void grafo_printa(Grafo *g);

int *menor_caminho(Grafo* g, int inicio);

double **fluxos(const char *arquivo, int n);

void fluxo(Grafo *g, int inicio, int *caminho, double **fluxoTotal, double *fluxoI);

#endif /* GRAFO_H */
