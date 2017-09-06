#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>

#define INF 999999999.0

typedef struct Aresta{
    int saida;
    int chegada;
    double custo;
}Aresta;

typedef struct Grafo{
    int n; /** Número de vértices. */
    int m; /** Número de arestas. */
    
    Aresta **arestas; /** Matriz de incidência. */
}Grafo;

Aresta nova_Aresta(int saida, int chegada, double peso);

Grafo *novo_Grafo_vazio(int n, int m);

Grafo *novo_Grafo_arquivo(const char *arquivo);

void grafo_add_aresta_d(Grafo *g, int saida, int chegada, double peso);

void grafo_printa(Grafo *g);

int *menor_caminho(Grafo* g, int inicio);

double **fluxos(const char *arquivo, int n);

void fluxo(Grafo *g, int inicio, int *caminho, double **fluxoTotal, double *fluxoI);

#endif /* GRAFO_H */
