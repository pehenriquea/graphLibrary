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
    int node_adj[];
} node;

struct edge
{
    int conection[2];
    int weight;
    int isDirected;
};


int main(){

    int size;

    printf("Digite a quantidade de vertices: ");
    scanf("%d", &size);

    node nd[size];

    for (int i = 0; i < size; i++){
        nd[i].id = i;
    }

    printf("Tamanho do vetor: %d\n", sizeof(nd)/ sizeof (node));

    return 0;
}

void edge_insertion(){

    
}
