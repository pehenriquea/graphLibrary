#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct node
{
    int degree;
    int nodeadj[];
    int father;
    int discoveryTime;
    int finishedTime;
    bool isExplored;
    bool isVisited;
};

struct edge
{
    int conection[2];
    int weight;
    bool isDirected;
};


int main (){

    int size;
    int *n_nodes;

    printf("Digite a quantidade de vértices: ");
    scanf("%d", &size);

    n_nodes = malloc(size * sizeof(node));

    node nd[n_nodes];

    free(n_nodes);
    return 0;
}
