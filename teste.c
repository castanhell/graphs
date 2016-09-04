#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {

  grafo g = le_grafo(stdin);

  if ( !g )
    return 1;

  printf("nome: %s\n", nome_grafo(g));
  printf("%sdirecionado\n", direcionado(g) ? "" : "não ");
  printf("%sponderado\n", ponderado(g) ? "" : "não ");
  printf("%d vértices\n", numero_vertices(g));
  printf("%d arestas\n", numero_arestas(g));
  printf("%s\n" , nome_vertice(vertice_nome("a",g)));
  printf("%s\n" , nome_vertice(vertice_nome("b",g)));
  printf("%s\n" , nome_vertice(vertice_nome("d",g)));
  printf("%s\n" , nome_vertice(vertice_nome("c",g)));

  escreve_grafo(stdout, g);

  return ! destroi_grafo(g);
}
