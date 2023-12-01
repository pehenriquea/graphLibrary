/*
Trabalho de Grafos - Outubro/2023
Professor: Walisson Ferreira de Carvalho
Integrantes:
Isabela Ferreira Scarabelli
Pedro Henrique de Almeida Santos
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INFINITO 99999999
#define ROWS 800
#define COLS 800

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

// Define a estrutura para representar um ponto no grid
typedef struct {
    int x, y;
} Point;

// Define a estrutura para representar um nó no grid
typedef struct {
    int f, g, h;
    Point parent;
} Node;

Node grid[ROWS][COLS];

//Indo do início até o fim do percurso
Point start; //início
Point goal; //objetivo

//Métricas tempo de execução
time_t start_time_d, end_time_d; //Dijkstra
time_t start_time_b, end_time_b; //Bellman-Ford
time_t start_time_f, end_time_f; //Floyd-Warshall
time_t start_time_a, end_time_a; //A*

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
int is_graph_regular(node *nd, Grafo *g);
int is_graph_complete(node *nd, Grafo *g);
void print_matrix(Grafo *g, int **matrix);
void generate_file(Grafo *g, int **matrix);
void depth_first_search(node *nd, Grafo *g, int vDfs);
int dfs(node *nd, int v, int time);
int breadth_first_search(node *nd, Grafo *g, int **matrix, int source,
                         int node);
void is_connected(int **matrix);
void dfs_connected(int **matrix, int v, bool *visited);
void menu(node *nd, Grafo *g, int **matrix);

Grafo g; // Graph g global variable
int *dist, *pred, **distFw, *distDj;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// create the graph for the adjacent list
void create_graph_list(Grafo *g, int n) {
  g->lista = calloc(n, sizeof(No *));
  g->n = n;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Delete the graph for the adjacent list
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
// If it is a directed graph, this function will be called twice just swapping
// v1 and v2
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

// Another way to verify if exists connections between two nodes
int is_nodes_connected(node *nd, Grafo *g) {

  int n1, n2;

  printf("Digite o primeiro vertice a ser comparado: ");
  scanf("%d", &n1);

  printf("Digite o segundo vertice a ser comparado: ");
  scanf("%d", &n2);

  for (int i = 0; i < g->n; i++) {
    for (int j = 0; j < g->n; j++) {
      if (nd[n1].node_adj[i] == nd[n2].node_adj[j]) {
        return 1;
      }
    }
  }

  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Removing edge from the adge list of v1
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
  }

  nd[n1].degree++;
  nd[n2].degree++;

  matrix[n1][n2] = weight;
  matrix[n2][n1] = weight;

  add_edge_list(&g, n1, n2);
  add_edge_list(&g, n2, n1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Remove an edge from the non directed graph
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

// Add an edge to the directed graph
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

// Consult node degree
void consult_node_degree(node *nd) {

  int n;

  printf("Digite o vértice que deseja consultar: \n");
  scanf("%d", &n);

  printf("Grau do vertice %d: %d\n", n, nd[n].degree);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Consult graph degree
void consult_graph_degree(node *nd, Grafo *g) {

  int g_degree = 0;

  for (int i = 0; i < g->n; i++) {

    g_degree += nd[i].degree;
  }

  printf("Grau do grafo: %d\n", g_degree);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// consult node neighbourhood
void consult_node_neighbourhood(node *nd) {

  int n = 0;

  printf("Digite o vertice para exibir seus vizinhos: ");
  scanf("%d", &n);

  if (nd[n].degree == 0) {
    printf("%d não possui viziinhos.\n", n);
  } else {
    printf("Adjacentes de %d: ", n);
    for (int i = 0; i < nd[n].degree; i++)
      printf("\t%d", nd[n].node_adj[i]);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Verify if this graph is regular
int is_graph_regular(node *nd, Grafo *g) {

  int comp;

  comp = nd[0].degree;

  for (int i = 1; i < g->n; i++) {

    if (nd[i].degree == comp) {
      continue;
    } else
      return 0;
  }

  return 1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Verify if this graph is complete
int is_graph_complete(node *nd, Grafo *g) {

  for (int i = 0; i < g->n; i++) {

    if (nd[i].degree == (g->n - 1)) {
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

// Generate CSV file for Gephi representation
void generate_file(Grafo *g, int **matrix) {

  int **m_aux, r;
  FILE *arq = fopen("grafo.csv", "w");

  m_aux = malloc(g->n * sizeof(int *));
  for (int i = 0; i < g->n; i++) {
    m_aux[i] = malloc(g->n * sizeof(int));
  }

  for (int i = 0; i < g->n; i++) {
    for (int j = 0; j < g->n; j++) {
      m_aux[i][j] = matrix[i][j];
    }
  }

  r = fputs("Source,Target,Weight\n", arq);

  if (r == EOF) {
    printf("Erro ao tentar gravar os dados! \n");
  }

  for (int i = 0; i < g->n; i++) {
    for (int j = 0; j < g->n; j++) {
      if (m_aux[i][j] != 0 || m_aux[j][i] != 0) {
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
  for (int i = 0; i < g->n; i++) {
    free(m_aux[i]);
  }
  fclose(arq);

  printf("Arquivo grafo.csv foi criado com sucesso!\n");
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

// Depth First Search Function
// Depending on wich node you begin you will have a different result
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

// Breadth First Search Function
// Depending on wich node you begin you will have a different result
int breadth_first_search(node *nd, Grafo *g, int **matrix, int source,
                         int node) {

  int queue[g->n];
  int start = 0;
  int len_queue = 1;
  int flag = 0;

  for (int i = 0; i < g->n; i++) {
    queue[i] = NULL;
    nd[i].isVisited = 0;
  }

  queue[start] = source;

  printf("\nCaminho que passa por %d ate %d: ", source, node);
  while (start < len_queue) {

    for (int i = 0; i < g->n; i++) {
      for (int j = 0; j < g->n; j++) {
        if (matrix[i][j] != 0 && i == queue[start]) {
          flag = 1;
        }
      }
    }

    if (queue[start] == node) {
      return 1;
      break;
    } else {
      for (int i = 0; i < g->n; i++) {
        if (nd[start].isVisited == 0 && flag == 1) {
          nd[start].isVisited = 1;
          queue[len_queue] = nd[start].id;
          printf("--> %d ", nd[start].id);
          len_queue++;
        }
      }
    }

    start++;
  }

  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void is_graph_connected(int **matrix) {
  bool visitados[g.n];
  int initialVertice = 0;

  for (int i = 0; i < g.n; i++) {
    visitados[i] = false;
  }

  dfs_connected(matrix, initialVertice, &visitados);

  for (int i = 0; i < g.n; i++) {
    if (visitados[i] == false) {
      printf("O grafo não é conexo");
      return;
    }
  }

  printf("O grafo é conexo");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void dfs_connected(int **matrix, int v, bool *visited) {
  visited[v] = true;

  for (int i = 0; i < g.n; i++) {
    if (matrix[v][i] != 0 && !visited[i]) {
      dfs_connected(matrix, i, visited);
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void random_node_conected(node *nd, int **matrix) {

  int a, b, w, flag = 0;

  for (int i = 0; i < g.n * 3; i++) {

    a = rand() % g.n;
    b = rand() % g.n;
    w = 1 + (rand() % 10);

    while (a == b)
      b = rand() % g.n;

    for (int i = 0; i < nd[a].degree; i++) {
      if (nd[a].node_adj[i] == -1) {
        nd[a].node_adj[i] = b;
        flag = 1;
        break;
      }
    }

    if (flag == 0) {
      nd[a].node_adj[nd[a].degree] = b;
    }

    flag = 0;

    nd[a].degree++;
    matrix[a][b] = w;

    add_edge_list(&g, a, b);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void random_node_not_conected(node *nd, int **matrix) {

  int a, b, w, flag = 0;

  for (int i = 0; i < g.n * 3; i++) {

    a = rand() % g.n;
    b = rand() % g.n;
    w = 1 + (rand() % 10);

    while (a == b) {
      b = rand() % g.n;
    }

    for (int i = 0; i < nd[a].degree; i++) {

      if (nd[a].node_adj[i] == -1) {
        nd[a].node_adj[i] = b;
        flag = 1;
        break;
      }
    }

    if (flag == 0) {
      nd[a].node_adj[nd[a].degree] = b;
    }

    flag = 0;

    for (int i = 0; i < nd[b].degree; i++) {

      if (nd[b].node_adj[i] == -1) {
        nd[b].node_adj[i] = a;
        flag = 1;
        break;
      }
    }

    if (flag == 0) {
      nd[b].node_adj[nd[b].degree] = a;
    }

    matrix[a][b] = w;
    matrix[b][a] = w;

    add_edge_list(&g, a, b);
    add_edge_list(&g, b, a);
  }

  for (int i = 0; i < g.n; i++)
    for (int j = 0; j < g.n; j++){
      if (matrix[i][j] != 0)
        nd[i].degree++;
    }

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void inicializarBF(int v, node *nd) {
  for (int i = 0; i < g.n; i++) {
    dist[i] = INFINITO;
    pred[i] = -1;
  }
  dist[v] = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void bellmand_Ford(int v, node *nd, int **matrix) {
  int u;
  inicializarBF(v, nd);
  for (int i = 1; i < g.n; i++) {
    for (int j = 0; j < g.n; j++) {
      for (int k = 0; k < nd[j].degree; k++) {
        u = nd[j].node_adj[k];
        if (dist[u] > dist[j] + matrix[j][u]) {
          dist[u] = dist[j] + matrix[j][u];
          pred[u] = j;
        }
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void PrintCaminho(int s, int v) {
  if (v == s)
    printf("  %d  ", s);
  else if (pred[v] == -1)
    printf("Não existe caminho de %d a %d ", s, v);
  else {
    PrintCaminho(s, pred[v]);
    printf("  %d  ", v);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void printMatrixBF(int v) {
  for (int i = 0; i < g.n; i++) {
    printf("Distância de %d para %d = %d \n", v, i, dist[i]);
    printf("Predecessor de %d = %d \n\n", i, pred[i]);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void inicializarFW(node *nd, int **matrix) {
  int u;
  for (int i = 0; i < g.n; i++) {
    for (int j = 0; j < g.n; j++) {
      distFw[i][j] = INFINITO; // Infinito
    }
    distFw[i][i] = 0;
  }

  for (int j = 0; j < g.n; j++) {
    for (int k = 0; k < nd[j].degree; k++) {
      u = nd[j].node_adj[k];
      distFw[j][u] = matrix[j][u];
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void floyd_warshall(node *nd, int **matrix) {
  int u;
  inicializarFW(nd, matrix);
  for (int k = 0; k < g.n; k++) {
    for (int i = 0; i < g.n; i++) {
      for (int j = 0; j < g.n; j++) {
        if (distFw[i][j] > distFw[i][k] + distFw[k][j]) {
          distFw[i][j] = distFw[i][k] + distFw[k][j];
        }
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void printMatrixFW() {
  for (int i = 0; i < g.n; i++) {
    for (int j = 0; j < g.n; j++) {
      printf("Distância de %d para %d = %d \n", i, j, distFw[i][j]);
    }
    printf(" - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int minDistance(int dist[], int menorCaminho[], Grafo *g) {
  int min = INFINITO, min_index;

  for (int v = 0; v < g->n; v++) {
    if (menorCaminho[v] == 0 && dist[v] <= min) {
      min = dist[v];
      min_index = v;
    }
  }

  return min_index;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void dijkstra(node *nd, Grafo *g, int **matrix) {

  for (int src = 0; src < g->n; src++) {

    int distDj[g->n];         // Vetor de distâncias mais curtas
    int menorCaminho[g->n]; // Conjunto de vértices incluídos no caminho mais
                            // curto

    for (int i = 0; i < g->n; i++) {
      distDj[i] = INFINITO;
      menorCaminho[i] = 0;
    }

    distDj[src] = 0;

    for (int j = 0; j < g->n - 1; j++) {
      int u = minDistance(distDj, menorCaminho, g);

      menorCaminho[u] = 1;

      for (int v = 0; v < g->n; v++) {
        if (!menorCaminho[v] && matrix[u][v] && distDj[u] != INFINITO &&
            distDj[u] + matrix[u][v] < distDj[v]) {
          distDj[v] = distDj[u] + matrix[u][v];
        }
      }
    }

    /*printf("Distancia da origem (Vertice %d)\nVertice\t\tDistancia\n", src);
    for (int i = 0; i < g->n; i++) {
      if (distDj[i] != INFINITO)
        printf("%d \t\t %d\n", i, distDj[i]);
      else
        printf("%d \t\t INFINITO\n", i);
    }*/
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int is_valid(int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int is_obstacle(int x, int y) {
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int calculate_h_value(int x, int y) {
    return abs(x - goal.x) + abs(y - goal.y);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void initialize_grid() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            grid[i][j].f = grid[i][j].g = grid[i][j].h = INFINITO;
            grid[i][j].parent.x = -1;
            grid[i][j].parent.y = -1;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void a_star(){
  printf("X Início: ");
  scanf("%d", &start.x);
  printf("\nY Início: ");
  scanf("%d", &start.y);
  printf("\nX Objetivo: ");
  scanf("%d", &goal.x);
  printf("\nY Objetivo: ");
  scanf("%d", &goal.y);

  time(&start_time_a);
  // Inicializa a grade
  initialize_grid();
  // Inicializa o nó inicial
  grid[start.x][start.y].f = grid[start.x][start.y].g = grid[start.x][start.y].h = 0;

  // Inicializa a lista aberta (open list) e a lista fechada (closed list)
  int open_list[ROWS * COLS][2]; // Representa a lista aberta como um conjunto de coordenadas (x, y)
  int closed_list[ROWS][COLS] = {0}; // Representa a lista fechada como uma matriz de valores booleanos

  int open_list_size = 1;
  open_list[0][0] = start.x;
  open_list[0][1] = start.y;

  while (open_list_size > 0) {
      // Encontrar o nó com menor custo na lista aberta
      int current_index = 0;
      for (int i = 1; i < open_list_size; ++i) {
          if (grid[open_list[i][0]][open_list[i][1]].f < grid[open_list[current_index][0]][open_list[current_index][1]].f) {
              current_index = i;
          }
      }

      // Obter as coordenadas do nó atual
      int x = open_list[current_index][0];
      int y = open_list[current_index][1];

      // Remover o nó atual da lista aberta
      open_list[current_index][0] = open_list[open_list_size - 1][0];
      open_list[current_index][1] = open_list[open_list_size - 1][1];
      open_list_size--;

      // Adicionar o nó atual à lista fechada
      closed_list[x][y] = 1;

      // Verificar os nós vizinhos
      for (int i = -1; i <= 1; ++i) {
          for (int j = -1; j <= 1; ++j) {
              int new_x = x + i;
              int new_y = y + j;

              // Verificar se o vizinho é válido
              if (is_valid(new_x, new_y) && !closed_list[new_x][new_y] && !is_obstacle(new_x, new_y)) {
                  // Calcular os novos valores de g, h e f
                  int new_g = grid[x][y].g + 1;
                  int new_h = calculate_h_value(new_x, new_y);
                  int new_f = new_g + new_h;

                  // Verificar se o vizinho já está na lista aberta
                  int in_open_list = 0;
                  for (int k = 0; k < open_list_size; ++k) {
                      if (open_list[k][0] == new_x && open_list[k][1] == new_y) {
                          in_open_list = 1;
                          break;
                      }
                  }

                  if (!in_open_list || new_f < grid[new_x][new_y].f) {
                      // Atualizar os valores do vizinho
                      grid[new_x][new_y].f = new_f;
                      grid[new_x][new_y].g = new_g;
                      grid[new_x][new_y].h = new_h;
                      grid[new_x][new_y].parent.x = x;
                      grid[new_x][new_y].parent.y = y;

                      // Adicionar o vizinho à lista aberta, se ainda não estiver lá
                      if (!in_open_list) {
                          open_list[open_list_size][0] = new_x;
                          open_list[open_list_size][1] = new_y;
                          open_list_size++;
                      }
                  }
              }
          }
      }
  }

  time(&end_time_a);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void aEstrela(){
  a_star();

  // Imprimir o caminho a seguir
  Point current = goal;
  while (current.x != -1 && current.y != -1) {
      printf("(%d, %d) <- ", current.x, current.y);
      current = grid[current.x][current.y].parent;
  }
  printf("\n");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// Menu Options
void menu(node *nd, Grafo *g, int **matrix) {
  int op, vDfs, source, search, ret_bfs;

  while (op != 14) {
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
    printf("10 - Caminho Mínimo: Bellman-Ford\n");
    printf("11 - Caminho Mínimo: Dijkstra\n");
    printf("12 - Caminho Mínimo: Floyd-Warshall\n");
    printf("13 - Caminho Mínimo: A Estrela\n");
    printf("14 - Sair\n");
    scanf("%d", &op);

    switch (op) {
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
      is_graph_connected(matrix);
      break;

    case 5:
      if (is_graph_regular(nd, g)) {
        printf("Grafo é regular!\n");
      } else {
        printf("Grafo não é regular!\n");
      }
      break;

    case 6:
      if (is_nodes_connected(nd, g)) {
        printf("Os nós estão conectados!\n");
      } else {
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
      // Breadth First Search
      printf("\n * * * * * * * * * * * * * * * * * * * * * \n");
      printf("* * * * * * Busca em Largura * * * * * * \n\n");
      printf("Escolha um vértice para começar a busca em largura: \n");
      scanf("%d", &source);
      printf("Escolha o vértice que deseja encontrar: \n");
      scanf("%d", &search);
      ret_bfs = breadth_first_search(nd, g, matrix, source, search);

      if (ret_bfs == 0)
        printf("\nVertice nao encontrado no grafo.\n");
      else
        printf("\nVertice encontrado no grafo.\n");

      break;

    case 10:
      // Bellman-Ford
      printf("\nBellman-Ford * * * * * * * * * * * * * \n\n");

      time(&start_time_b);
      for (int i = 0; i < g->n; i++) {
        bellmand_Ford(i, nd, matrix);
        //printMatrixBF(i);
        //printf(" - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
      }
      time(&end_time_b);
      printf("Tempo de execução: %d segundos\n", (int)(end_time_b - start_time_b));
      break;

    case 11:
      time(&start_time_d);
      dijkstra(nd, g, matrix);
      time(&end_time_d);
      printf("Tempo de execução: %d segundos\n", (int)(end_time_d - start_time_d));
      break;

    case 12:
      // Floyd-Warshall
      printf("\nFloyd-Warshall * * * * * * * * * * * * * \n");
      time(&start_time_f);
      floyd_warshall(nd, matrix);
      //printMatrixFW();
      time(&end_time_f);
      printf("Tempo de execução: %d segundos\n", (int)(end_time_f - start_time_f));
      break;
    case 13:
      printf("\nA Estrela * * * * * * * * * * * * * \n");
      aEstrela();
      printf("Tempo de execução: %d segundos\n", (int)(end_time_a - start_time_a));
      break;
    }
  }
}

/* Main * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int main() {
  int size;
  int type;
  int **matrix;
  int rFlag;

  printf("\n * * * * * * * * * * * * * * * * * * * * * \n\n");
  printf("\n * * * * * * Trabalho de Grafos * * * * * *\n");
  printf("\n * * Isabela Scarabelli e Pedro Almeida * * \n\n");
  printf("Digite 0 para grafo direcionado e 1 para grafo nao direcionado: ");
  scanf("%d", &type);

  printf("Digite a quantidade de vertices: ");
  scanf("%d", &size);

  dist = malloc(size * sizeof(int *));
  pred = malloc(size * sizeof(int *));

  distDj = malloc(size * sizeof(int *));

  distFw = malloc(size * sizeof(int *));
  for (int i = 0; i < size; i++) {
    distFw[i] = malloc(size * sizeof(int));
  }

  printf("Deseja criar um grafo aleatorio? (1)Sim   (0)Nao: ");
  scanf("%d", &rFlag);

  create_graph_list(&g, size); // Creating graph for adjacency list

  node nd[size];
  // Alocating memory for list of adjacencies
  for (int i = 0; i < size; i++) {
    nd[i].node_adj = (int *)malloc((size - 1) * sizeof(int));
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

    if (rFlag != 1) {
      printf("Nesse grafo, as arestas estao representadas pelos ids: ");
      for (int i = 0; i < size; i++) {
        nd[i].id = i;
        nd[i].degree = 0;
        printf("%d   ", nd[i].id);
      }
    } else {
      for (int i = 0; i < size; i++) {
        nd[i].id = i;
        nd[i].degree = 0;
      }
      random_node_conected(nd, matrix);
    }

    printf("\n");

    // Menu
    while (op != 2 && rFlag == 0) {
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

    if (rFlag != 1) {
      printf("Nesse grafo, as arestas estao representadas pelos ids: ");
      for (int i = 0; i < size; i++) {
        nd[i].id = i;
        nd[i].degree = 0;
        printf("%d   ", nd[i].id);
      }
    } else {
      for (int i = 0; i < size; i++) {
        nd[i].id = i;
        nd[i].degree = 0;
      }
      random_node_not_conected(nd, matrix);
    }

    // Menu
    while (op != 2 && rFlag == 0) {
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

   if (rFlag == 0) {
  printf("\n  * * * * * * * * * * * * * * * * * * *  \n");
  printf("* * * * * * Graph as a matrix * * * * * * \n");
  printf("\n");
  print_matrix(&g, matrix);
  printf("\n");
  printf("\n");
  }

  menu(nd, &g, matrix);

  // Free pointers
  for (int i = 0; i < size; i++) {
    free(matrix[i]);
  }
  free(matrix);

  return 0;
}
