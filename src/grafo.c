#include "grafo.h"

Grafo *novo_Grafo_vazio(int n){
    Grafo *g = malloc(sizeof(Grafo));
    
    g->n = n;
    g->m = 0;
    
    int i;
    g->adj = malloc(n * sizeof(double *));
    for(i=0; i<n; i++){
        g->adj[i] = calloc(n, sizeof(double));
    }
    
    return g;
}

Grafo *novo_Grafo_arquivo(const char *arquivo){
    /* Abrindo o arquivo */
    FILE *in = fopen(arquivo, "r");
    if(in == NULL){
        printf("Nao foi possivel abrir o arquivo '%s'.\n", arquivo);
        return NULL;
    }
    
    /* Lendo metadata */
    int n, z, ftn, m;
    fscanf(in, "<NUMBER OF ZONES> %d\t\t\t\t\t\t\t\t\t\t\t\n"
               "<NUMBER OF NODES> %d\t\t\t\t\t\t\t\t\t\t\t\n"
               "<FIRST THRU NODE> %d\t\t\t\t\t\t\t\t\t\t\t\n"
               "<NUMBER OF LINKS> %d\t\t\t\t\t\t\t\t\t\t\t\n"
               "<END OF METADATA>\t\t\t\t\t\t\t\t\t\t\t\n"
               "\n"
               "\n"
               "~ \tInit node \tTerm node \tCapacity \tLength \tFree Flow Time \tB\tPower\tSpeed limit \tToll \tType\t;\n",
           &z, &n, &ftn, &m);
    
    /* Preenchendo o grafo */
    Grafo *g = novo_Grafo_vazio(n);
    
    int init, term, length, fft, power, speed, toll, type;
    double capacity, b;
    int i;
    for(i=0; i<m; i++){
        fscanf(in, "\t%d\t%d\t%lf\t%d\t%d\t%lf\t%d\t%d\t%d\t%d\t;\n",
               &init, &term, &capacity, &length, &fft, &b, &power, &speed, &toll, &type);
        double peso = fft;
        grafo_add_aresta_d(g, --init, --term, peso);
    }
    fclose(in);
    
    return g;
}

void grafo_add_aresta_d(Grafo *g, int saida, int chegada, double peso){
    if(saida >= 0 && saida < g->n && chegada >= 0 && chegada < g->n){
        g->adj[saida][chegada] = peso;
        g->m++;
    }
}

int grafo_existe_aresta_d(Grafo *g, int saida, int chegada){
    if(saida >= 0 && saida < g->n && chegada >= 0 && chegada < g->n){
        return (g->adj[saida][chegada] != 0.0);
    }
    return 0;
}

void grafo_printa(Grafo *g){
    int i, j;
    printf("  | ");
    for(i=0; i<g->n; i++){
        printf("%2d ", i + 1);
    }
    printf("\n");
    for(i=0; i<g->n; i++){
        printf("%2d: ", i + 1);
        for(j=0; j<g->n; j++){
            printf("%2.0lf ", g->adj[i][j]);
        }
        printf("\n");
    }
}

int *menor_caminho(Grafo *g, int inicio){
    int i;
    int *pai = malloc(g->n * sizeof(*pai)), escolhido[g->n];
    double peso[g->n];
    
    double INF = 999999990.0;
    
    for(i=0; i<g->n; i++){
        escolhido[i] = 0;
        pai[i] = -1;
        peso[i] = INF;
    }
    peso[inicio] = 0;
    
    int v = inicio, w;
    while(v < g->n){
        v = g->n;
        for(i=0; i<g->n; i++){
            if(!escolhido[i]){
                if(v == g->n){
                    v = i;
                }else if(peso[i] < peso[v]){
                    v = i;
                }
            }
        }
        if(v == g->n){
            break;
        }
        escolhido[v] = 1;
        
        for(w=0; w<g->n; w++){
            if(!escolhido[w] && grafo_existe_aresta_d(g, v, w)){
                if(peso[w] > (peso[v] + g->adj[v][w])){
                    peso[w] = peso[v] + g->adj[v][w];
                    pai[w] = v;
                }
            }
        }
    }
    for(i=0; i<g->n; i++){
        /*printf("pai[%d]: %d\n", i+1, pai[i]+1);*/
    }
    return pai;
}

double **fluxos(const char *arquivo, int n){
    /* Abrindo o arquivo */
    FILE *in = fopen(arquivo, "r");
    if(in == NULL){
        printf("Nao foi possivel abrir o arquivo '%s'.\n", arquivo);
        return NULL;
    }
    
    /* Lendo metadata */
    int z;
    double tf;
    fscanf(in, "<NUMBER OF ZONES> %d\n"
               "<TOTAL OD FLOW> %lf\n"
               "<END OF METADATA>\n"
               "\n"
               "\n",
           &z, &tf);
    
    /* Preenchendo a matriz */
    double **fluxos = malloc(n * sizeof(*fluxos));
    int i, j, dump;
    for(i=0; i<n; i++){
        fluxos[i] = malloc(n * sizeof(*(fluxos[i])));
        fscanf(in, "Origin \t%d \n", &dump);
        for(j=0; j<n; j++){
            fscanf(in, "%d :", &dump);
            fscanf(in, "%lf; ", &fluxos[i][j]);
            if(j%4 == 0){
                fscanf(in, "\n");
            }
        }
        if(j%4 != 0){
            fscanf(in, "\n");
        }
        fscanf(in, "\n");
    }
    fclose(in);
    
    return fluxos;
}

void fluxo(Grafo *g, int inicio, int *caminho, double **fluxoTotal, double *fluxoI){
    int i, j, v, w;
    double *ft, fi = -1;
    
    for(i=0; i<g->n; i++){
        if(i != inicio && fluxoI[i] != 0.0){
            j = i;
            do{
                v = caminho[j];
                w = j;
                ft = &fluxoTotal[v][w];
                fi = fluxoI[i];
                fluxoTotal[v][w] += fluxoI[i];
                j = v;
            }while(j != inicio);
        }
    }
}
