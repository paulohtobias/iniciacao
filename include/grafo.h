#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bheap.h"

typedef struct Aresta {
    int init;
    int term;
    int flow;
    double capacity;
    int length;
    int fft;
    double b;
    int power;
    int speed;
    int toll;
    int type;
    double travel_time; ///Não usado ainda.
} Aresta;

typedef struct Grafo {
    int n; ///Número de vértices.
    int m; ///Número de arestas.
	
	int total_flow; ///Número total de veículos.

    Aresta **arestas; ///Matriz de incidência.
} Grafo;

Aresta nova_Aresta(int saida, int chegada, double peso);

double calcular_tempo(Aresta aresta);

double calcular_tempo_caminho(Grafo *g, int *pai, int origem, int destino);

Grafo *novo_Grafo_vazio(int n, int m);

Grafo *novo_Grafo_arquivo(const char *arquivo);

void grafo_add_aresta_d(Grafo *g, Aresta aresta);

Aresta grafo_get_aresta(Grafo *g, int saida, int chegada);

void grafo_printa(Grafo *g);

void menor_caminho(Grafo* g, int inicio, int *pai);

#endif /* GRAFO_H */
