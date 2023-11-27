#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 999999

// Estrutura para representar um nó no grafo
typedef struct {
    int id;
    int heuristic;  // Valor heurístico para o nó (distância estimada até o destino)
} Node;

// Estrutura para representar um nó na lista de adjacência
typedef struct No {
    int v;
    struct No* prox;
} No;

// Estrutura para representar o grafo
typedef struct {
    int n;      // Número de nós
    No** lista; // Lista de adjacência
} Grafo;

// Função para encontrar o índice do nó com a menor função de custo no array
int findMinCostNode(int* cost, bool* visited, int size) {
    int minCost = INF;
    int minCostNode = -1;

    for (int i = 0; i < size; i++) {
        if (!visited[i] && cost[i] < minCost) {
            minCost = cost[i];
            minCostNode = i;
        }
    }

    return minCostNode;
}

// Função para adicionar uma aresta à lista de adjacência
void addEdge(Grafo* g, int src, int dest) {
    No* newNode = (No*)malloc(sizeof(No));
    newNode->v = dest;
    newNode->prox = g->lista[src];
    g->lista[src] = newNode;
}

// Função para inicializar o grafo (substitua com sua própria lógica)
Grafo* initializeGraph() {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->n = 4;  // Número de nós no exemplo

    g->lista = (No**)malloc(g->n * sizeof(No*));
    for (int i = 0; i < g->n; i++) {
        g->lista[i] = NULL;
    }

    // Adicione arestas ao grafo (substitua com sua própria lógica)
    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 1, 3);
    addEdge(g, 2, 3);

    return g;
}

// Algoritmo A*
void AStar(Grafo* g, Node* nodes, int start, int goal) {
    // ... (o restante do seu código A*)
}

int main() {
    // Exemplo de uso
    Grafo* g = initializeGraph();

    // Exemplo de inicialização de nós
    Node nodes[] = {
        {0, 5},  // Exemplo: nó 0 com heurística 5
        {1, 3},  // Exemplo: nó 1 com heurística 3
        {2, 2},  // Exemplo: nó 2 com heurística 2
        {3, 0}   // Exemplo: nó 3 com heurística 0
    };

    AStar(g, nodes, 0, 3);  // Exemplo: encontrar caminho do nó 0 para o nó 3

    // Libere a memória (substitua com sua própria lógica)
    // ...

    return 0;
}
