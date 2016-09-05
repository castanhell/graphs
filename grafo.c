#ifndef _GRAFOC
#define _GRAFOC

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>

/* Parte1: Estruturas, Variaveis Prototipos de funcoes usadas nesse arquivo */

/*Prototipos*/

/*Grafo*/
grafo inicia_grafo(Agraph_t *g);
int destroi_vertices(grafo g);

/*Hash*/
typedef struct hash_t hash_t;
hash_t * novahash (int size);
int destroi_hash(hash_t * hash);
unsigned long djb2(unsigned char *str);
int index (hash_t *h, char *key);
void insert (hash_t *h, char *key, void *value);
void *lookup (hash_t *h, char *key);

/* Hash de busca de vertices e grafo - adaptado de https://rosettacode.org/wiki/Associative_arrays/Creation/C */
struct hash_t {
    int size;
    char **keys;
    void **values;
};
 
hash_t * novahash (int size) {
    hash_t *h = calloc(1, sizeof (hash_t));
    if(h == 0) { printf("Sem memoria suficiente para alocar a hash"); return 0;}
    h->keys = calloc(size, sizeof (char *));
    if(h->keys == 0) { printf("Sem memoria suficiente para alocar as chaves da hash"); return 0;}
    h->values = calloc(size, sizeof (void *));
    if(h->values == 0) { printf("Sem memoria suficiente para alocar os valores da hash"); return 0;}
    h->size = size;
    return h;
}

int destroi_hash(hash_t *hash){
  free(hash->keys);
  free(hash->values);  
  free(hash);
  return 1;
}

unsigned long djb2(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    
    while (c = *str++)
	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return (int)hash;
}
 
int index (hash_t *h, char *key) {
    int i = djb2(key) % h->size;
    while (h->keys[i] && strcmp(h->keys[i],key)!=0)
        i = (i + 1) % h->size;
    return i;
}
 
void insert (hash_t *h, char *key, void *value) {
    int i = index(h, key);
    h->keys[i] = key;
    h->values[i] = value;
}
 
void *lookup (hash_t *h, char *key) {
    int i = index(h, key);
    return h->values[i];
}

/* Parte 2 - Grafo */

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
    hash_t *hashvertices;
    /* Lista de vertices - Para acesso sequencial O(|V(G)|)*/
    vertice vertices;
};

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

/* Parte 3: Vertice e aresta */


/* Define uma aresta ou arco (depende do atributo ponderado do grafo) */
typedef struct aresta{
    long int peso;
    /* origem da aresta */
    char* origem;
    /* destino da aresta */
    char* destino;
} aresta;

/* Define o vertice, tal como detalhado no header */
struct vertice{
    /* nome do vertice */
    char * nome;
    /* Grau para o grafo, se nao ordenado ou grau de entrada, se ordenado */
    unsigned int grau;
    unsigned int grauSaida;
    /* aresta ou arco */
    aresta* arestas;
};

char *nome_vertice(vertice v){
    return v->nome;
}

vertice vertice_nome(char *s, grafo g){
    return lookup(g->hashvertices,s);
}

/* Parte 4 - Leitura e escrita de grafos */


/* Le grafo */

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

grafo inicia_grafo(Agraph_t *g){
    if ( !g ) return NULL;

    grafo graph = malloc(sizeof(struct grafo));
    if(graph == 0 ){ printf("Erro ao alocar grafo\n"); return 0; }

    graph = (grafo) malloc(sizeof (struct grafo));
    graph->nVertices = 0;
    graph->nArestas = 0;
    graph->direcionadoPonderado = agisdirected(g) ? 1 : 0;
    graph->nome = agnameof(g);

    /* Determina o numero de vertices */
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v)){
        graph->nVertices++;
    }

    /* Aloca memoria para o vetor de vertices e a hash */
    vertice ver = (vertice) calloc(graph->nVertices,(sizeof (struct vertice)));
    if(ver == 0){ printf("Sem memoria suficiente para alocar todos os vertices"); return 0; }
    /* Associa os vertices */
    graph->vertices = ver;
    /* Instancia nova hash, respeitando marca experimental de 80%, segundo https://en.wikipedia.org/wiki/Hash_table */
    /* Tambem aproveito para deixar uma sobra (25% de nVertices), para insercoes de vertices */
    hash_t *hash = novahash((int) (1.5*graph->nVertices));
    if(hash == 0){ printf("Sem memoria suficiente para alocar a hash"); return 0; }
    graph->hashvertices=hash;

    /* Indica se o primeiro vertice foi checado */
    int chkPonderado = 0;
    int vi = 0;
    /* Determina o numero de arestas vertice */
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v)){
        /* Preenche os dados do vertice */
        ver[vi].nome=agnameof(v);
        for (Agedge_t *a=agfstedge(g,v); a; a=agnxtedge(g,a,v)) {
	    char *peso = agget(a, (char *)"peso");
            /* Checa se todos os nos possuem peso caso seja ponderado */
            if(!checa_ponderado(graph,&chkPonderado,peso)){
                return 0;
            }
            /* A primeira aresta determina se o grafo possui peso ou nao. Se ela tiver aresta, todas as demais devem possuir. Idem se ela nao possuir */
	    if (v == agtail(a)) {
		graph->nArestas++; 
                ver[vi].grau += 1;
	    }
	    if (v == aghead(a)) {
                if(ponderado(graph)){
     	            ver[vi].grauSaida += 1;
                }
                else{
                   ver[vi].grau += 1;
                }
	    }
        }
        
        /* Para cada vertice: Insere as arestas */
        aresta *ar = calloc(ver[vi].grau,sizeof(aresta));
        if(ar == 0) { printf("Sem memoria para alocar a aresta"); return 0; }
        /* Indice da aresta atual */
        int ai = 0;

        for (Agedge_t *a=agfstedge(g,v); a; a=agnxtedge(g,a,v)) {
	    if (v == agtail(a)) {
                ar[ai].destino = agnameof(aghead(a));
                ar[ai].origem = agnameof(agtail(a));
                if(ponderado(graph)){
                    ar[ai].peso = (long) atoi(agget(a, (char *)"peso"));
                }
            }
            ai++;
        }
        ver[vi].arestas = ar;
        /* Insere o vertice na hash */
        insert(hash, ver[vi].nome, (void *) (ver+vi));
        vi++;
    }

    return graph;
}

grafo le_grafo(FILE *input){
    if (input == 0){
	printf("le_grafo: Entrada invalida\n");
        return 0;
    }

    Agraph_t *g = agread(stdin, NULL);
    if ( !g )
        return 0;

    grafo graph = inicia_grafo(g);
    agclose(g);

    return graph;
}  

/* destroi */

int destroi_vertices(grafo g){
    int i = 0;
    for(i = 0; i < g->nVertices; g++){
    }
    free(g->vertices);
    
    return 1;
}

int destroi_grafo(grafo g){
    if(g == 0){
        printf("destroi_grafo: Grafo invalido\n");
        return 0;
    }
    /* Destroi todos os verties */
    destroi_vertices(g);
    /* destroi hash */
    destroi_hash(g->hashvertices);
    /* destroi o proprio grafo */
    free(g);
    
    return 1;
}

/* escreve grafo */

void imprime_vertices(grafo g){
    int i;
    for(i = 0; i < g->nVertices; i++){
       printf("    \"%s\"\n", nome_vertice(g->vertices+i));
    }
}

void imprime_arestas(grafo g){
    int i,j;
    char * rep_aresta = direcionado(g) ? '>' : '-';
    for(i = 0; i < g->nVertices; i++){
       vertice v = g->vertices+i;
       for( j = 0; j < v->grau;j++){
           printf("    \"%s\" -%c \"%s\"",
               (v->arestas+j)->origem,
               rep_aresta,
               (v->arestas+j)->destino
               );

           if(ponderado(g)){
              printf(" [peso=%d]",(v->arestas+j)->peso );
           }
          printf("\n");
       }
    }
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
    printf("strict %sgraph %s {\n\n", direcionado(g) ? "di" : "", nome_grafo(g));
    imprime_vertices(g);
    printf("\n");
    imprime_arestas(g);
    printf("}\n");
    
    return 0;
}

#endif
