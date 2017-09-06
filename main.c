#include "grafo.h"

/*
 * 
 */
int main(int argc, char** argv) {
    Grafo *g = novo_Grafo_arquivo("testes/SiouxFalls/SiouxFalls_net.txt");
    
    int i;
    int **caminho = malloc(g->n * sizeof(*caminho));
    double **fluxo_total = malloc(g->n * sizeof(*fluxo_total));
    for(i=0; i<g->n; i++){
        fluxo_total[i] = calloc(g->n, sizeof(*(fluxo_total[i])));
        caminho[i] = menor_caminho(g, i);
    }
    
    double **f = fluxos("testes/SiouxFalls/SiouxFalls_trips.txt", g->n);
    
    /* Calculando o fluxo de cada vértice */
    for(i=0; i<g->n; i++){
        fluxo(g, i, caminho[i], fluxo_total, f[i]);
    }
    
    /* Mostrando os fluxos */
    int j;
    printf("      | ");
    for(i=0; i<g->n; i++){
        printf("%6d", i + 1);
    }
    printf("\n");
    for(i=0; i<g->n; i++){
        printf("%6d: ", i + 1);
        for(j=0; j<g->n; j++){
            printf("%6.0lf", fluxo_total[i][j]);
        }
        printf("\n");
    }

    return 0;
}

