#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

void add_non_directed_edge(node *nd, int **matrix);
void remove_non_directed_edge(node *nd, int **matrix);
void add_directed_edge(node *nd, int **matrix);
void remove_directed_edge(node *nd, int **matrix);

int main(){

    int size;
    int type;
    int **matrix;
    node *nd;

    printf("Digite 0 para grafo direcionado e 1 para grafo nao direcionado: ");
    scanf("%d", &type);

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

    for (int i = 0; i < size; i++){
        free(matrix[i]);
    }
    free(matrix);
    free(nd);
    return 0;
}

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

}

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

}

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

}

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

    } else printf("Vertice nao alcancavel.");

}
