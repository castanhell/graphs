#ifndef _GRAFOC
#define _GRAFOC

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>

/* Variaveis Prototipos de funcoes usadas nesse arquivo */

/*Variaveis*/

grafo graph;

/*Prototipos*/

Agraph_t * inicia_grafo(Agraph_t *agrapht);

/* Parte 1 - Estruturas */

/* Hash de busca de vertices e grafo */



/* Define o grafo, tal como detalhado no header */
struct grafo{
/* Nome do grafo */
char* nome; 
/* Propriedades do grafo : Direcionado e ponderado
   ÃSalva um pouco de memoria, o elemento eh convertido na funcao de retorno
   O primeiro bit 1 indica se eh direcionado, o segundo bit indica se eh ponderado
   Ou seja, possui os seguintes valores:
   0 - Nao eh direcionado
   1 - Direcionado
   2 - Ponderado
   3 - Direcionado e ponderado
 */
unsigned int direcionadoPonderado;
/* Numero de vertices e arestas, sempre recalculados quando o grafo eh criado */
unsigned int nVertices;
unsigned int nArestas;
/* Estrutura de vertices - Eh acessada como hash */
vertice vertices;
};

/* Define uma aresta, a qual pode ser com ou sem peso */
typedef struct aresta{
unsigned int peso;
/* destino da aresta */
vertice *destino;
/* Proximo elemento na lista, se for nulo eh o fim da lista*/
struct aresta* prox;
} aresta;

/* Define o vertice, tal como detalhado no header */
struct vertice{
/* Grau para o grafo, se nao ordenado ou grau de entrada, se ordenado */
unsigned int grau;
unsigned int grauSaida;
/* Numero do vertice, criado juntamente com a estrutura */
unsigned int numero;
/* aresta */
};


/* Parte 2 - Funcoes relativas a grafos */

char *nome_grafo(grafo g){
    return g->nome;   
}

int direcionado(grafo g){
    return (g->direcionadoPonderado & 0x01) > 0;
}

int ponderado(grafo g){
    return (g->direcionadoPonderado & 0x02) > 0;
}

unsigned int numero_vertices(grafo g){
    return g->nVertices;
}

unsigned int numero_arestas(grafo g){
    return g->nArestas;
}

/* Parte 2 - Leitura e escrita de grafos */

Agraph_t * inicia_grafo(Agraph_t *g){
    if ( !g )
	return NULL;

    graph = (grafo) malloc(sizeof (struct grafo));
    graph->nVertices = 0;
    graph->nArestas = 0;
    graph->direcionadoPonderado = agisdirected(g) ? 1 : 0;
    /* TODO  : ponderado */
    graph->nome = agnameof(g);

    /* Determina o numero de vertices */
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v)){
        graph->nVertices++;
    }

    /* Aloca memoria */
    vertice v = (vertice) calloc(graph->nVertices,(sizeof (struct vertice)));

    /* Determina o numero de arestas vertice */
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v)){
        graph->nVertices++;
        for (Agedge_t *a=agfstedge(g,v); a; a=agnxtedge(g,a,v)) {
          if (v == agtail(a)) {
             graph->nArestas++; 
          }
        }
        /* Para cada vertice: Insere as arestas */
    }


    return g;
}

grafo le_grafo(FILE *input){
    if (input == 0){
	printf("le_grafo: Entrada invalida");
        return 0;
    }
    Agraph_t *g = agread(stdin, NULL);
    if ( !g )
        return 0;
    agclose(inicia_grafo(g));
    return graph;
}  

int destroi_grafo(grafo g){
    if(g == 0){
        printf("destroi_grafo: Grafo invalido");
        return 0;
    }
    /* Para cada vertice : destroi arestas */

    /* Destroi todos os verties */
    
    return 0;
}

grafo escreve_grafo(FILE *output, grafo g){
    if (output == 0){
	printf("escreve_grafo: saida invalida");
        return 0;
    }
    if(g == 0){
        printf("escreve_grafo: Grafo invalido");
        return 0;
    }
    return 0;
}

#endif
