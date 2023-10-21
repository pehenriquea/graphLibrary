/*
Trabalho de Grafos - Outubro/2023
Professor: Walisson Ferreira de Carvalho
Integrantes: 
Isabela Ferreira Scarabelli
Pedro Henrique de Almeida Santos
*/

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

typedef struct {
  int id;
  int degree;
  int father;
  int discoveryTime;
  int finishedTime;
  int isExplored;
  int isVisited;
  int *node_adj; // For directed graphs, it keeps exit nodes. Which means the
                 // reachable nodes.
} node;

// Function prototypes
void create_graph_list(Grafo *g, int n);
void delete_graph_list(Grafo *g);
void add_edge_list(Grafo *g, int v1, int v2);
int has_edge_list(Grafo *g, int v1, int v2);
int is_nodes_connected(node *nd, Grafo *g);
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
void print_matrix(Grafo *g, int **matrix);
void generate_file(Grafo *g, int **matrix);
void depth_first_search(node *nd, Grafo *g, int vDfs);
int dfs(node *nd, int v, int time);
void breadth_first_search(node *nd, Grafo *g, int source, int node);
void menu(node *nd, Grafo *g, int **matrix);

Grafo g; // Graph g global variable

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//create the graph for the adjacent list
void create_graph_list(Grafo *g, int n) {
  g->lista = calloc(n, sizeof(No *));
  g->n = n;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Delete the graph for the adjacent list
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

// Adding edges to the adjacent list of v1
//If it is a directed graph, this function will be called twice just swapping v1 and v2
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

// Retorn True (1) or False (0) in case there is an edge between v1 and v2
int has_edge_list(Grafo *g, int v1, int v2) {
  No *l = g->lista[v1];
  while (l != NULL && l->v < v2)
    l = l->prox;
  if (l != NULL && l->v == v2)
    return 1;
  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Another way to verify if exists connections between two nodes
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

//Removing edge from the adge list of v1
void remove_edge_list(Grafo *g, int v1, int v2) {
  No **a = &g->lista[v1];
  while (*a != NULL && (*a)->v < v2)
    a = &(*a)->prox;
  if (*a != NULL && (*a)->v == v2) {
    No *r = *a;
    *a = (*a)->prox;
    free(r);
  } else {
    printf("Aresta inexistente!");
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Print graph that uses adjacency list
void print_graph_list(Grafo *gr) {
  for (int i = 0; i < gr->n; i++) {
    printf("v%d", i);           // Print first node
    No *a = gr->lista[i];       // point to the first node of the adjacency list
    while (a) {                 // while there is a adjacency list to print
      printf(" --> v%d", a->v); // print the node
      a = a->prox;              // next neighobour
    }
    printf("\n");
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////Add an edge to the non directed graph
void add_non_directed_edge(node *nd, int **matrix) {

  int n1, n2, weight, flag = 0;

  printf("Digite um vertice a ser ligado: ");
  scanf("%d", &n1);

  printf("Digite outro vertice a ser ligado com o anterior: ");
  scanf("%d", &n2);

  printf("Digite o peso da aresta adicionada (Digite 1 caso ela nao seja "
         "ponderada): ");
  scanf("%d", &weight);

  for (int i = 0; i < nd[n1].degree; i++) {

    if (nd[n1].node_adj[i] == -1) {
      nd[n1].node_adj[i] = n2;
      flag = 1;
      break;
    }
  }

  if (flag == 0) {
    nd[n1].node_adj[nd[n1].degree] = n2;
    printf("%d: %d\n", n1, nd[n1].node_adj[nd[n1].degree]);
  }

  flag = 0;

  for (int i = 0; i < nd[n2].degree; i++) {

    if (nd[n2].node_adj[i] == -1) {
      nd[n2].node_adj[i] = n1;
      flag = 1;
      break;
    }
  }

  if (flag == 0) {
    nd[n2].node_adj[nd[n2].degree] = n1;
    printf("%d: %d\n", n2, nd[n2].node_adj[nd[n2].degree]);
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
void remove_non_directed_edge(node *nd, int **matrix) {
  int n1, n2;

  printf("Digite um vertice a ser removido: ");
  scanf("%d", &n1);

  printf("Digite outro vertice a ser removido junto com o anterior: ");
  scanf("%d", &n2);

  for (int i = 0; i < nd[n1].degree; i++) {

    if (nd[n1].node_adj[i] == n2) {
      nd[n1].node_adj[i] = -1;
      break;
    }
  }

  nd[n1].degree--;

  for (int i = 0; i < nd[n2].degree; i++) {

    if (nd[n2].node_adj[i] == n1) {
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
void add_directed_edge(node *nd, int **matrix) {

  int n1, n2, weight, flag = 0;

  printf("Digite o vertice de saida: ");
  scanf("%d", &n1);

  printf("Digite o vertice de chegada: ");
  scanf("%d", &n2);

  printf("Digite o peso da aresta adicionada (Digite 1 caso ela nao seja "
         "ponderada): ");
  scanf("%d", &weight);

  for (int i = 0; i < nd[n1].degree; i++) {

    if (nd[n1].node_adj[i] == -1) {
      nd[n1].node_adj[i] = n2;
      flag = 1;
      break;
    }
  }

  if (flag == 0) {
    nd[n1].node_adj[nd[n1].degree] = n2;
  }

  flag = 0;

  nd[n1].degree++;
  matrix[n1][n2] = weight;

  add_edge_list(&g, n1, n2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////Remove an edge from the directed graph
void remove_directed_edge(node *nd, int **matrix) {

  int n1, n2;

  printf("Digite o vertice de saida: ");
  scanf("%d", &n1);

  printf("Digite um vértice de chegada alcancavel pelo vertice anteriormente "
         "digitado: ");
  scanf("%d", &n2);

  if (matrix[n1][n2] != 0) {

    for (int i = 0; i < nd[n1].degree; i++) {

      if (nd[n1].node_adj[i] == n2) {
        nd[n1].node_adj[i] = -1;
        break;
      }
    }

    nd[n1].degree--;

    matrix[n1][n2] = 0;

    remove_edge_list(&g, n1, n2);
  } else
    printf("Vertice nao diretamente alcançavel (não são vizinhos).");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Consult node degree
void consult_node_degree(node *nd){

    int n;

    printf("Digite o vértice que deseja consultar: \n");
    scanf("%d", &n);

    printf("Grau do vertice %d: %d\n", n, nd[n].degree);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Consult graph degree
void consult_graph_degree(node *nd, Grafo *g){

    int g_degree = 0;

    for (int i = 0; i < g->n; i++){

        g_degree += nd[i].degree;

    }

    printf("Grau do grafo: %d\n", g_degree);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//consult node neighbourhood
void consult_node_neighbourhood(node *nd){

    int n = 0;

    printf("Digite o vertice para exibir seus vizinhos: ");
    scanf("%d", &n);

    if(nd[n].degree == 0){
      printf("%d não possui viziinhos.\n", n);
    }else{
      printf("Adjacentes de %d: ", n);
      for (int i = 0; i < nd[n].degree; i++)
         printf("\t%d", nd[n].node_adj[i]);
    }
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

// Shows on screen the resulting matrix
void print_matrix(Grafo *g, int **matrix) {

  printf(" \t");

  for (int i = 0; i < g->n; i++) {
    printf("%d\t", i);
  }

  for (int i = 0; i < g->n; i++) {
    printf("\n%d", i);
    for (int j = 0; j < g->n; j++) {
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Depth First Search Inicialization
// i = the vertice choosed by the user
void depth_first_search(node *nd, Grafo *g, int vDfs) {
  int time = 0;
  int counter = 0;

  for (int i = 0; i < g->n; i++) {
    nd[i].isExplored = 0;
    nd[i].isVisited = 0;
    nd[i].father = -1;
    nd[i].discoveryTime = 0;
    nd[i].finishedTime = 0;
  }

  // Executing the DFS untill all discovery time is differen from 0
  for (int i = vDfs; counter < g->n; i = i % g->n) {
    if (nd[i].isVisited == 0) {
      printf("Tree: %d ", i);
      time = dfs(nd, i, time);
      printf("\n");
    }
    i++;
    counter++;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Depth First Search Function
//Depending on wich node you begin you will have a different result
int dfs(node *nd, int v, int time) {
  int w = 0; // neighboors
  int i = 0; // auxiliar

  time++;

  nd[v].discoveryTime = time;
  nd[v].isVisited = 1;

  while (w != -1) {
    if (i < nd[v].degree) {
      w = nd[v].node_adj[i];
      if (nd[w].discoveryTime == 0) {
        nd[w].father = v;
        printf("-- %d ", w);
        time = dfs(nd, w, time);
      }

    } else {
      w = -1;
    }

    i++;
  }

  time++;
  nd[v].finishedTime = time;
  nd[i].isExplored = 1;

  return time;
}

//Breadth First Search Function
//Depending on wich node you begin you will have a different result
void breadth_first_search(node *nd, Grafo *g, int source, int node){

  int queue[g->n];
  int start = 0;
  int len_queue = 1;
  int result = 0;

  for (int i = 0; i < g->n; i++){
    queue[i] = NULL;
    nd[i].isVisited = 0;
  }

  queue[start] = source;

  while (start < len_queue){

    if (queue[start] == node){
      result = 1;
      break;
    } else {
      for (int i = 0; i < nd[start].degree; i++){
        if (nd[start].isVisited == 0){
          nd[start].isVisited = 1;
          queue[len_queue] = nd[start].id;
          len_queue++;
        }
      }
    }

    start++;
    
  }

  if (result == 0)
    printf("Vertice nao encontrado no grafo.");
  else
    printf("Vertice encontrado no grafo.");

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Menu Options
void menu(node *nd, Grafo *g, int **matrix){
  int op, vDfs, source, search;

  while(op != 10){
    printf("\nEscolha uma opção:\n");
    printf("1 - Consultar grau de um nó\n");
    printf("2 - Consultar grau do grafo\n");
    printf("3 - Consultar vizinhos de um nó\n");
    printf("4 - Verficar se o grafo é conexo\n");
    printf("5 - Verificar se o grafo é regular\n");
    printf("6 - Verificar se um par de nós está conectado\n");
    printf("7 - Gerar aquivo para aplicativo Gephi\n");
    printf("8 - Busca em Profundidade\n");
    printf("9 - Busca em Largura\n");
    printf("10 - Sair\n");
    scanf("%d", &op);
  
    switch(op){
      case 1: 
        consult_node_degree(nd);
      break;
      
      case 2:
        consult_graph_degree(nd, g);
      break;
  
      case 3:
        consult_node_neighbourhood(nd);
      break;
  
      case 4:
        if(is_graph_connected()){
          printf("Grafo é conexo!\n");
        }else{
          printf("Grafo não é conexo!\n");
        }
      break;
  
      case 5:
        if(is_graph_regular(nd, g)){
          printf("Grafo é regular!\n");
        }else{
          printf("Grafo não é regular!\n");
        }
  
      case 6:
        if(is_nodes_connected(nd, g)){
          printf("Os nós estão conectados!\n");
        }else{
          printf("Os nós não estão conectados!\n");
        }
      break;
  
      case 7:
        generate_file(g, matrix);
      break;
  
      case 8:
        // Depth First Search
        printf("\n * * * * * * * * * * * * * * * * * * * * * \n");
        printf("* * * * * * Busca em Profundidade * * * * * * \n\n");
        printf("Escolha um vértice para começar a busca em profundidade: \n");
        scanf("%d", &vDfs);
        depth_first_search(nd, g, vDfs);
      break;
  
      case 9:
        //Breadth First Search
        printf("\n * * * * * * * * * * * * * * * * * * * * * \n");
        printf("* * * * * * Busca em Largura * * * * * * \n\n");
        printf("Escolha um vértice para começar a busca em largura: \n");
        scanf("%d", &source);
        printf("Escolha o vértice que deseja encontrar: \n");
        scanf("%d", &search);
        breadth_first_search(nd, g, source, search);
        
      break;
    }
  }
}

/* Main * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int main() {
  int size;
  int type;
  int **matrix;

  printf("\n * * * * * * * * * * * * * * * * * * * * * \n\n");
  printf("\n * * * * * * Trabalho de Grafos * * * * * *\n");
  printf("\n * * Isabela Scarabelli e Pedro Almeida * * \n\n");
  printf("Digite 0 para grafo direcionado e 1 para grafo nao direcionado: ");
  scanf("%d", &type);

  printf("Digite a quantidade de vertices: ");
  scanf("%d", &size);

  create_graph_list(&g, size); // Creating graph for adjacency list

  node nd[size];
  // Alocating memory for list of adjacencies
  for (int i = 0; i < size; i++) {
    nd[i].node_adj = (int *)malloc((size-1) * sizeof(int));
  }

  // Directed graph
  if (type == 0) {
    int op;

    // Alocating memory for matrix
    matrix = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
      matrix[i] = malloc(size * sizeof(int));
    }

    // Initializing matrix with 0
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        matrix[i][j] = 0;
      }
    }

    printf("Nesse grafo, as arestas estao representadas pelos ids: ");
    for (int i = 0; i < size; i++) {
      nd[i].id = i;
      nd[i].degree = 0;
      printf("%d   ", nd[i].id);
    }

    printf("\n");

    // Menu
    while (op != 2) {
      printf("\n\n(0) Adiciona arestas\n(1) Remover \n(2) Sair\n\n");
      scanf("%d", &op);

      if (op == 0) {
        add_directed_edge(nd, matrix);
      } else if (op == 1) {
        remove_directed_edge(nd, matrix);
      }
    }

    // Not directed graph
  } else if (type == 1) {
    int op;

    // Alocating memory for matrix
    matrix = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
      matrix[i] = malloc(size * sizeof(int));
    }

    // Initializing matrix with 0
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        matrix[i][j] = 0;
      }
    }

    printf("Nesse grafo, as arestas estao representadas pelos ids: ");
    for (int i = 0; i < size; i++) {
      nd[i].id = i;
      nd[i].degree = 0;
      printf("%d   ", nd[i].id);
    }

    // Menu
    while (op != 2) {
      printf("\n(0)Adicionar arestas\n(1)Remover\n(2)Sair\n\n");
      scanf("%d", &op);

      if (op == 0) {
        add_non_directed_edge(nd, matrix);
      } else if (op == 1) {
        remove_non_directed_edge(nd, matrix);
      }
    }
  }

  // menu();
  //  Prints
  printf("\n  * * * * * * * * * * * * * * * * * * * * * * * \n");
  printf("* * * * * * Graph as a adjacency list * * * * * * \n");
  printf("\n");
  print_graph_list(&g);
  printf("\n");
  printf("\n");

  printf("\n  * * * * * * * * * * * * * * * * * * *  \n");
  printf("* * * * * * Graph as a matrix * * * * * * \n");
  printf("\n");
  print_matrix(&g, matrix);
  printf("\n");
  printf("\n");

  menu(nd, &g, matrix);
  
  // Free pointers
  for (int i = 0; i < size; i++) {
    free(matrix[i]);
  }
  free(matrix);

  return 0;
}
