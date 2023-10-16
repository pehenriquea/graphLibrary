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
    return 0;
}
