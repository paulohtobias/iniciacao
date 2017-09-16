#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bheap.h"

//int init, term, length, fft, power, speed, toll, type;
	//double capacity, b;

typedef struct Aresta {
    int init;
    int term;
    double flow;
    double capacity;
    int length;
    int fft;
    double b;
    int power;
    int speed;
    int toll;
    int type;
    double travel_time;
} Aresta;

typedef struct Grafo {
    int n; /** Número de vértices. */
    int m; /** Número de arestas. */

    Aresta **arestas; /** Matriz de incidência. */
} Grafo;

Aresta nova_Aresta(int saida, int chegada, double peso);

double calcular_tempo(Aresta aresta);

Grafo *novo_Grafo_vazio(int n, int m);

Grafo *novo_Grafo_arquivo(const char *arquivo);

void grafo_add_aresta_d(Grafo *g, Aresta aresta);

void grafo_printa(Grafo *g);

void menor_caminho(Grafo* g, int inicio, int *pai);

double **origem_destino(const char *arquivo, int n);

void fluxo(Grafo *g, int inicio, int *caminho, double *origemI, double porcentagem);

#endif /* GRAFO_H */
