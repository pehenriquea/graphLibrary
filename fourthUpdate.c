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
void create_graph_list(Grafo *g, int n);
void delete_graph_list(Grafo *g);
void add_edge_list(Grafo *g, int v1, int v2);
int has_edge_list(Grafo *g, int v1, int v2);
void remove_edge_list(Grafo *g, int v1, int v2);
void print_graph_list(Grafo *gr);
void add_non_directed_edge(node *nd, int **matrix);
void remove_non_directed_edge(node *nd, int **matrix);
void add_directed_edge(node *nd, int **matrix);
void remove_directed_edge(node *nd, int **matrix);
void consult_node_degree(node *nd);
void consult_graph_degree(node *nd, Grafo *g);
void consult_node_neighbourhood(node *nd);
int is_graph_connected();
int is_graph_regular(node *nd, Grafo *g);
int is_graph_complete(node *nd, Grafo *g);
int is_nodes_connected(node *nd, Grafo *g);
void print_matrix(Grafo *g, int **matrix);
void generate_file(Grafo *g, int **matrix);
void breadth_first_search(node *nd, Grafo *g);

Grafo g; // Graph g global variable
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//create the adjacent list
void create_graph_list(Grafo *g, int n) {
  g->lista = calloc(n, sizeof(No *));
  g->n = n;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Delete the entire adjacent list
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
//Add an edge from the non directed graph
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
//Remove an edge from the non directed graph
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
//Add an edge to the directed graph
void add_directed_edge(node *nd, int **matrix){

  int n1, n2, weight, flag;

  printf("Digite o vertice de saida: ");
  scanf("%d", &n1);

  printf("Digite o vertice de chegada: ");
  scanf("%d", &n2);

  printf("Digite o peso da aresta adicionada (Digite 1 caso ela nao seja ponderada): ");
  scanf("%d", &weight);

  printf("%d", nd[1].degree);

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
//Remove an edge from the directed graph
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Consult node degree
void consult_node_degree(node *nd){

    int n;

    printf("Digite o vertice que deseja consultar: ");
    scanf("%d", &n);

    printf("Grau do vertice %d: %d", n, nd[n].degree);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Consult graph degree
void consult_graph_degree(node *nd, Grafo *g){

    int g_degree;

    for (int i = 0; i < g->n; i++){

        g_degree += nd[i].degree;

    }

    printf("Grau do grafo: %d", g_degree);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//consult node neighbourhood
void consult_node_neighbourhood(node *nd){

    int n = 0;

    printf("Digite o vertice para exibir seus vizinhos: ");
    scanf("%d", &n);

    printf("Adjacentes de %d: ", n);

    for (int i = 0; i < nd[n].degree; i++)
        printf("\t%d", nd[n].node_adj[i]);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Verify if this graph is connected
int is_graph_connected(){
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Verify if this graph is regular
int is_graph_regular(node *nd, Grafo *g){

    int comp;

    comp = nd[0].degree;

    for (int i = 1; i < g->n; i++){

        if (nd[i].degree == comp){
            continue;
        } else return 0;

    }

    return 1;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Verify if this graph is complete
int is_graph_complete(node *nd, Grafo *g){

  for (int i = 0; i < g->n; i++){

    if (nd[i].degree == (g->n - 1)){
      continue;
    } else 
        return 0;

  }

  return 1;
    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Verify if exists connections between two nodes
int is_nodes_connected(node *nd, Grafo *g){

  int n1, n2;

  printf("Digite o primeiro vertice a ser comparado: ");
  scanf("%d", &n1);

  printf("Digite o segundo vertice a ser comparado: ");
  scanf("%d", &n2);

  for (int i = 0; i < g->n; i++){
    for (int j = 0; j < g->n; j++){
      if (nd[n1].node_adj[i] == nd[n2].node_adj[j]){
        return 1;
      }
    }
  }

  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Show on screen the resulting matrix
void print_matrix(Grafo *g, int **matrix){

  printf(" \t");

  for (int i = 0; i < g->n; i++){
    printf("%d\t", i);
  }

  for (int i = 0; i < g->n; i++){
    printf("\n%d", i);
    for (int j = 0; j < g->n; j++){
      printf("\t%d", matrix[i][j]);
    }
  }

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Generate CSV file for Gephi representation
void generate_file(Grafo *g, int **matrix){

  int **m_aux, r;
  FILE *arq = fopen("grafo.csv", "w");

  m_aux = malloc(g->n * sizeof(int*));
  for (int i = 0; i < g->n; i++){
      m_aux[i] = malloc (g->n * sizeof(int));
  }

  for (int i = 0; i < g->n; i++){
    for (int j = 0; j < g->n; j++){
      m_aux[i][j] = matrix[i][j];
    }
  }

  r = fputs("Source,Target,Weight\n", arq);
   
  if( r == EOF)
  {
      printf("Erro ao tentar gravar os dados! \n");
  }

  for (int i = 0; i < g->n; i++){
    for (int j = 0; j < g->n; j++){
      if (m_aux[i][j] != 0 || m_aux[j][i] != 0){
        fprintf(arq, "%d", i);
        fputc(',', arq);
        fprintf(arq, "%d", j);
        fputc(',', arq);
        fprintf(arq, "%d", m_aux[i][j]);
        fputc('\n', arq);

        m_aux[i][j] = 0;
        m_aux[j][i] = 0;
      }
    }
  } 

  free(m_aux);
  for (int i = 0; i < g->n; i++){
      free(m_aux[i]);
  }
  fclose(arq);
}

/* Main * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int main(){
  int size;
  int type;
  int **matrix;

  printf("Digite 0 para grafo direcionado e 1 para grafo nao direcionado: ");
  scanf("%d", &type);

  printf("Digite a quantidade de vertices: ");
  scanf("%d", &size);
  
  create_graph_list(&g, size); //Creating graph fo adjacency list

  node nd[size];

  //Directed graph
  if (type == 0){

      int op;

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

        printf("%d", nd[0].degree);

      //Menu
      //while(1){
          printf("\nDigite 0 para adicionar arestas e 1 para remover: ");
          scanf("%d", &op);

          if (op == 0){
              add_directed_edge(nd, matrix);
          } else if (op == 1){
              remove_directed_edge(nd, matrix);
          }
      //}

  //Not directed graph
  } else if (type == 1){

      int op;

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
      while(1){
          printf("\nDigite 0 para adicionar arestas e 1 para remover: ");
          scanf("%d", &op);

          if (op == 0){
              add_non_directed_edge(nd, matrix);
          } else if (op == 1){
              remove_non_directed_edge(nd, matrix);
          }

          generate_file(&g, matrix);
      }

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


  free(matrix);
  for (int i = 0; i < size; i++){
      free(matrix[i]);
  }
  free(nd);
  return 0;
}
