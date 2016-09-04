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

/* Hash de busca de vertices e grafo - adaptado de https://rosettacode.org/wiki/Associative_arrays/Creation/C */
typedef struct {
    int size;
    void **keys;
    void **values;
} hash_t;
 
hash_t *novahash (int size) {
    hash_t *h = calloc(1, sizeof (hash_t));
    h->keys = calloc(size, sizeof (void *));
    h->values = calloc(size, sizeof (void *));
    h->size = size;
    return h;
}
 
int index (hash_t *h, void *key) {
    int i = (int) key % h->size;
    while (h->keys[i] && h->keys[i] != key)
        i = (i + 1) % h->size;
    return i;
}
 
void insert (hash_t *h, void *key, void *value) {
    int i = index(h, key);
    h->keys[i] = key;
    h->values[i] = value;
}
 
void *lookup (hash_t *h, void *key) {
    int i = index(h, key);
    return h->values[i];
}

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

int checa_ponderado(grafo graph, int* chkPonderado, char *peso){
    if ( peso && *peso ){
	if(!*chkPonderado){
	    graph->direcionadoPonderado += 2;
	    *chkPonderado = 1;
	}
	else if (chkPonderado){
	    if (!ponderado(graph)){
		/* Todas as arestas devem possuir peso */
		printf("Todas as arestas devem possuir peso\n");
		return 0;
	    }
	}
    }
    else{
	if(!*chkPonderado){
	    *chkPonderado = 1;
	}
	else if (chkPonderado){
	    if (ponderado(graph)){
		/* Todas as arestas devem possuir peso */
		printf("Todas as arestas devem possuir peso\n");
		return 0;
	    }
	}
    }
    return 1;
}

Agraph_t * inicia_grafo(Agraph_t *g){
    if ( !g )
	return NULL;

    graph = (grafo) malloc(sizeof (struct grafo));
    graph->nVertices = 0;
    graph->nArestas = 0;
    graph->direcionadoPonderado = agisdirected(g) ? 1 : 0;
    graph->nome = agnameof(g);

    /* Determina o numero de vertices */
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v)){
        graph->nVertices++;
    }

    /* Aloca memoria */
    vertice v = (vertice) calloc(graph->nVertices,(sizeof (struct vertice)));

    /* Indica se o primeiro vertice foi checado */
    int chkPonderado = 0;
    /* Determina o numero de arestas vertice */
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v)){
        int edgeNumber=0;
        for (Agedge_t *a=agfstedge(g,v); a; a=agnxtedge(g,a,v)) {
	    char *peso = agget(a, (char *)"peso");
            /* Checa se todos os nos possuem peso caso seja ponderado */
            if(!checa_ponderado(graph,&chkPonderado,peso)){
                return 0;
            }
            /* A primeira aresta determina se o grafo possui peso ou nao. Se ela tiver aresta, todas as demais devem possuir. Idem se ela nao possuir */
	    if (v == agtail(a)) {
		graph->nArestas++; 
		edgeNumber++;
	    }
        }
        /* Para cada vertice: Insere as arestas */
    }

    return g;
}

grafo le_grafo(FILE *input){
    if (input == 0){
	printf("le_grafo: Entrada invalida\n");
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
        printf("destroi_grafo: Grafo invalido\n");
        return 0;
    }
    /* Para cada vertice : destroi arestas */

    /* Destroi todos os verties */
    
    return 0;
}

grafo escreve_grafo(FILE *output, grafo g){
    if (output == 0){
	printf("escreve_grafo: saida invalida\n");
        return 0;
    }
    if(g == 0){
        printf("escreve_grafo: Grafo invalido\n");
        return 0;
    }
    return 0;
}

#endif
