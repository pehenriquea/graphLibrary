#include <stdio.h>
#include <stdlib.h>

typedef struct no {
  int v; /* Vértice */
  struct no *prox;
} No;

typedef struct grafo {
  int n; /* Número de nós */
  No **lista;
} Grafo;

void create_graph(Grafo *g, int n) {
  g->lista = calloc(n, sizeof(No *));
  g->n = n;
}

void delete_graph_list(Grafo *g) {
  for (int i = 0; i < g->n; i++) {
    No *l = g->lista[i];
    while (l) {
      No *r = l;
      l = l->prox;
      free(r);
    }
  }
  free(g->lista);
}

// Adicionando aresta na lista de adjacência de v1
void add_edge_list(Grafo *g, int v1, int v2) {
  No **a = &g->lista[v1];
  while (*a != NULL && (*a)->v < v2) { // salvando os vértices em ordem
                                       // crescente
    a = &(*a)->prox;
  }

  if (*a == NULL || (*a)->v != v2) { // evita salvar vértices já salvos
    No *n = malloc(sizeof(No));
    n->v = v2;
    n->prox = *a;
    *a = n;
  }
}

// Retorna True (1) ou False (0) caso exista uma aresta entre v1 e v2
int has_edge_list(Grafo *g, int v1, int v2) {
  No *l = g->lista[v1];
  while (l != NULL && l->v < v2)
    l = l->prox;
  if (l != NULL && l->v == v2)
    return 1;
  return 0;
}

// Removendo aresta da lista de adjacência de v1
void remove_edge_list(Grafo *g, int v1, int v2) {
  No **a = &g->lista[v1];
  while (*a != NULL && (*a)->v < v2)
    a = &(*a)->prox;
  if (*a != NULL && (*a)->v == v2) {
    No *r = *a;
    *a = (*a)->prox;
    free(r);
  }
}

void print_graph(Grafo *gr) {
  for (int i = 0; i < gr->n; i++) {
    printf("v%d", i);     // Imprimo em qual aresta estou
    No *a = gr->lista[i]; // chamo a cabeça da lista de adjacencia desta aresta
    while (a) {           // enquanto as adjacencias não forem nula
      printf(" --> v%d", a->v); // imprimo o vértice
      a = a->prox;              // passo para proxima adjacencia
    }
    printf("\n");
  }
}

int main() {
  Grafo g;
  create_graph(&g, 6);
  add_edge_list(&g, 0, 1);
  add_edge_list(&g, 1, 2);
  add_edge_list(&g, 3, 1);
  add_edge_list(&g, 4, 0);
  add_edge_list(&g, 5, 3);
  add_edge_list(&g, 5, 5);
  print_graph(&g);
  return 0;
}
