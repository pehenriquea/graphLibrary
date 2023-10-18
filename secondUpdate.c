#include <stdio.h>
#include <stdlib.h>

typedef struct no {
  int v; /* Vertice (id)*/
  struct no *prox;
} No;

typedef struct grafo {
  int n; /* Number of nodes */
  No **lista;
} Grafo;

typedef struct
{
    int id;
    int degree;
    int father;
    int discoveryTime;
    int finishedTime;
    int isExplored;
    int isVisited;
    int node_adj[]; //For directed graphs, it keeps exit nodes. Which means the reachable nodes.
} node;

// Function prototypes
void add_non_directed_edge(node *nd, int **matrix);
void remove_non_directed_edge(node *nd, int **matrix);
void add_directed_edge(node *nd, int **matrix);
void remove_directed_edge(node *nd, int **matrix);
void create_graph_list(Grafo *g, int n);
void delete_graph_list(Grafo *g);
void add_edge_list(Grafo *g, int v1, int v2);
int has_edge_list(Grafo *g, int v1, int v2);
void remove_edge_list(Grafo *g, int v1, int v2);
void print_graph_list(Grafo *gr);

Grafo g; // Graph g global variable
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void create_graph_list(Grafo *g, int n) {
  g->lista = calloc(n, sizeof(No *));
  g->n = n;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Retorna True (1) ou False (0) caso exista uma aresta entre v1 e v2
int has_edge_list(Grafo *g, int v1, int v2) {
  No *l = g->lista[v1];
  while (l != NULL && l->v < v2)
    l = l->prox;
  if (l != NULL && l->v == v2)
    return 1;
  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Removendo aresta da lista de adjacência de v1
void remove_edge_list(Grafo *g, int v1, int v2) {
  No **a = &g->lista[v1];
  while (*a != NULL && (*a)->v < v2)
    a = &(*a)->prox;
  if (*a != NULL && (*a)->v == v2) {
    No *r = *a;
    *a = (*a)->prox;
    free(r);
  }else{
    printf("Aresta inexistente!");
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Print graph that uses adjacency list
void print_graph_list(Grafo *gr) {
  for (int i = 0; i < gr->n; i++) {
    printf("v%d", i);     // Print first node
    No *a = gr->lista[i]; // point to the first node of the adjacency list
    while (a) {           // while there is a adjacency list to print
      printf(" --> v%d", a->v); // print the node
      a = a->prox;              // next neighobour
    }
    printf("\n");
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
void add_non_directed_edge (node *nd, int **matrix){

    int n1, n2, weight, flag = 0;

    printf("Digite um vertice a ser ligado: ");
    scanf("%d", &n1);

    printf("Digite outro vertice a ser ligado com o anterior: ");
    scanf("%d", &n2);

    printf("Digite o peso da aresta adicionada (Digite 1 caso ela nao seja ponderada): ");
    scanf("%d", &weight);


    for (int i = 0; i < nd[n1].degree; i++){

        if (nd[n1].node_adj[i] == -1){
            nd[n1].node_adj[i] = n2;
            flag = 1;
            break;
        } 
    }

    if (flag == 0){
        nd[n1].node_adj[nd[n1].degree] = n2;
    }

    flag = 0;

    for (int i = 0; i < nd[n2].degree; i++){

        if (nd[n2].node_adj[i] == -1){
            nd[n2].node_adj[i] = n1;
            flag = 1;
            break;
        } 
    }

    if (flag == 0){
        nd[n2].node_adj[nd[n2].degree] = n1;
    }

    nd[n1].degree++;
    nd[n2].degree++;

    matrix[n1][n2] = weight;
    matrix[n2][n1] = weight;

    add_edge_list(&g, n1, n2);
    add_edge_list(&g, n2, n1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
void remove_non_directed_edge (node *nd, int **matrix){
  int n1, n2;

  printf("Digite um vertice a ser removido: ");
  scanf("%d", &n1);

  printf("Digite outro vertice a ser removido junto com o anterior: ");
  scanf("%d", &n2);

  for (int i = 0; i < nd[n1].degree; i++){

      if (nd[n1].node_adj[i] == n2){
          nd[n1].node_adj[i] = -1;
          break;
      }
  }

  nd[n1].degree--;

  for (int i = 0; i < nd[n2].degree; i++){

      if (nd[n2].node_adj[i] == n1){
          nd[n2].node_adj[i] = -1;
          break;
      }
  }

  nd[n2].degree--;

  matrix[n1][n2] = 0;
  matrix[n2][n1] = 0;

  remove_edge_list(&g, n1, n2);
  remove_edge_list(&g, n2, n1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
void add_directed_edge(node *nd, int **matrix){

  int n1, n2, weight, flag;

  printf("Digite o vertice de saida: ");
  scanf("%d", &n1);

  printf("Digite o vertice de chegada: ");
  scanf("%d", &n2);

  printf("Digite o peso da aresta adicionada (Digite 1 caso ela nao seja ponderada): ");
  scanf("%d", &weight);

  for (int i = 0; i < nd[n1].degree; i++){

      if (nd[n1].node_adj[i] == -1){
          nd[n1].node_adj[i] = n2;
          flag = 1;
          break;
      } 
  }

  if (flag == 0){
      nd[n1].node_adj[nd[n1].degree] = n2;
  }

  flag = 0;

  nd[n1].degree++;
  matrix[n1][n2] = weight;

  add_edge_list(&g, n1, n2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
void remove_directed_edge (node *nd, int **matrix){

  int n1, n2;

  printf("Digite o vertice de saida: ");
  scanf("%d", &n1);

  printf("Digite um vértice de chegada alcancavel pelo vertice anteriormente digitado: ");
  scanf("%d", &n2);

  if (matrix[n1][n2] != 0){

      for (int i = 0; i < nd[n1].degree; i++){

          if (nd[n1].node_adj[i] == n2){
              nd[n1].node_adj[i] = -1;
              break;
          }
      }

      nd[n1].degree--;

      matrix[n1][n2] = 0;

      remove_edge_list(&g, n1, n2);
  } else printf("Vertice nao diretamente alcançavel (não são vizinhos).");

}

/* Main * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int main(){
  int size;
  int type;
  int **matrix;
  node *nd;

  printf("Digite 0 para grafo direcionado e 1 para grafo nao direcionado: ");
  scanf("%d", &type);
  create_graph_list(&g, 6); //Creating graph fo adjacency list

  //Directed graph
  if (type == 0){

      int op;

      printf("Digite a quantidade de vertices: ");
      scanf("%d", &size);

      nd = malloc(size * sizeof(node));

      matrix = malloc(size * sizeof(int*));

      for (int i = 0; i < size; i++){
          matrix[i] = malloc (size * sizeof(int));
      }

      //Initializing matrix with 0
      for (int i = 0; i < size; i++){
          for (int j = 0; j < size; j++){
              matrix[i][j] = 0;
          }
      }

      printf("Nesse grafo, as arestas estao representadas pelos ids: ");
      for (int i = 0; i < size; i++){
          nd[i].id = i;
          nd[i].degree = 0;
          printf("%d   ", nd[i].id);
      }

      //Menu
      //while(1){
          printf("\nDigite 0 para adicionar arestas e 1 para remover: ");
          scanf("%d", &op);

          if (op == 0){
              add_directed_edge(&nd, matrix);
          } else if (op == 1){
              remove_directed_edge(&nd, matrix);
          }
      //}

  //Not directed graph
  } else if (type == 1){

      int op;

      printf("Digite a quantidade de vertices: ");
      scanf("%d", &size);

      nd = malloc(size * sizeof(node));

      matrix = malloc(size * sizeof(int*));

      for (int i = 0; i < size; i++){
          matrix[i] = malloc (size * sizeof(int));
      }

      //Initializing matrix with 0
      for (int i = 0; i < size; i++){
          for (int j = 0; j < size; j++){
              matrix[i][j] = 0;
          }
      }

      printf("Nesse grafo, as arestas estao representadas pelos ids: ");
      for (int i = 0; i < size; i++){
          nd[i].id = i;
          printf("%d   ", nd[i].id);
      }

      //Menu
      //while(1){
          printf("\nDigite 0 para adicionar arestas e 1 para remover: ");
          scanf("%d", &op);

          if (op == 0){
              add_non_directed_edge(&nd, matrix);
          } else if (op == 1){
              remove_non_directed_edge(&nd, matrix);
          }
      //}

  }

  /*add_edge_list(&g, 0, 1);
  add_edge_list(&g, 1, 2);
  add_edge_list(&g, 3, 1);
  add_edge_list(&g, 4, 0);
  add_edge_list(&g, 5, 3);
  add_edge_list(&g, 5, 5);
  print_graph_list(&g);
  printf("\n");
  remove_edge_list(&g, 5, 3);
  print_graph_list(&g);
  remove_edge_list(&g, 3, 5);*/

  printf("\n");
  print_graph_list(&g);
  printf("\n");

  for (int i = 0; i < size; i++){
      free(matrix[i]);
  }
  free(matrix);
  free(nd);
  return 0;
}
