#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Assuming a maximum number of nodes in the grid
#define MAX_NODES 100

// Function to find the path using A*
int *A_Star(int start, int goal, int (*heuristicFunc)(int, int, int),
            int gridWidth, int (*distanceFunc)(int, int, int)) {
  int *openSet = NULL;
  int openSetSize = 1;
  openSet = (int *)malloc(sizeof(int) * openSetSize);
  openSet[0] = start;

  int cameFrom[MAX_NODES];
  for (int i = 0; i < MAX_NODES; i++) {
    cameFrom[i] = -1; // Initialize to -1 indicating no path
  }

  int gScore[MAX_NODES];
  for (int i = 0; i < MAX_NODES; i++) {
    gScore[i] = __INT_MAX__; // Initialize to infinity
  }
  gScore[start] = 0;

  int fScore[MAX_NODES];
  for (int i = 0; i < MAX_NODES; i++) {
    fScore[i] = __INT_MAX__; // Initialize to infinity
  }
  fScore[start] = heuristicFunc(start, goal, gridWidth);

  while (openSetSize > 0) {
    // Find the node in openSet with the lowest fScore value
    int current = openSet[0];
    for (int i = 1; i < openSetSize; i++) {
      if (fScore[openSet[i]] < fScore[current]) {
        current = openSet[i];
      }
    }

    if (current == goal) {
      // Reconstruct and return the path
      int *path = (int *)malloc(sizeof(int) * MAX_NODES);
      int pathLength = 0;
      int pathNode = current;
      while (pathNode != -1) {
        path[pathLength++] = pathNode;
        pathNode = cameFrom[pathNode];
      }

      // Reverse the path
      int *reversedPath = (int *)malloc(sizeof(int) * pathLength);
      for (int i = 0; i < pathLength; i++) {
        reversedPath[i] = path[pathLength - 1 - i];
      }

      // Free memory
      free(openSet);
      free(path);

      return reversedPath;
    }

    // Remove current from openSet
    for (int i = 0; i < openSetSize; i++) {
      if (openSet[i] == current) {
        openSet[i] = openSet[openSetSize - 1];
        openSetSize--;
        break;
      }
    }

    // For each neighbor of current
    for (int neighbor = 0; neighbor < MAX_NODES; neighbor++) {
      if (neighbor == current ||
          distanceFunc(current, neighbor, gridWidth) == __INT_MAX__) {
        continue; // Skip if not a neighbor or no direct connection
      }

      int tentative_gScore =
          gScore[current] + distanceFunc(current, neighbor, gridWidth);

      if (tentative_gScore < gScore[neighbor]) {
        // This path to neighbor is better than any previous one. Record it!
        cameFrom[neighbor] = current;
        gScore[neighbor] = tentative_gScore;
        fScore[neighbor] =
            tentative_gScore + heuristicFunc(neighbor, goal, gridWidth);

        // Add neighbor to openSet if not already in it
        int inOpenSet = 0;
        for (int i = 0; i < openSetSize; i++) {
          if (openSet[i] == neighbor) {
            inOpenSet = 1;
            break;
          }
        }

        if (!inOpenSet) {
          openSetSize++;
          openSet = (int *)realloc(openSet, sizeof(int) * openSetSize);
          openSet[openSetSize - 1] = neighbor;
        }
      }
    }
  }

  // Open set is empty but goal was never reached
  free(openSet);
  return NULL;
}

// Function to calculate the distance (Manhattan) between two nodes in a grid
int distance(int current, int neighbor, int gridWidth) {
  // Assuming nodes are numbered from 0 to gridWidth*gridWidth - 1
  // and each node has coordinates (x, y) in a grid of width gridWidth.
  int currentX = current % gridWidth;
  int currentY = current / gridWidth;
  int neighborX = neighbor % gridWidth;
  int neighborY = neighbor / gridWidth;

  // Calculate the Manhattan distance between the two nodes
  int manhattanDistance = abs(currentX - neighborX) + abs(currentY - neighborY);

  return manhattanDistance;
}

// Function to calculate the heuristic (Manhattan) for A*
int heuristic(int current, int goal, int gridWidth) {
  // Assuming nodes are numbered from 0 to gridWidth*gridWidth - 1
  // and each node has coordinates (x, y) in a grid of width gridWidth.
  int currentX = current % gridWidth;
  int currentY = current / gridWidth;
  int goalX = goal % gridWidth;
  int goalY = goal / gridWidth;

  // Calculate the Manhattan distance between the current node and the goal
  int manhattanDistance = abs(currentX - goalX) + abs(currentY - goalY);

  return manhattanDistance;
}

// Example usage
int main() {
  // Example grid with a width of 10 (10x10 grid)
  int gridWidth = 10;

  // Example nodes
  int startNode = 0;
  int goalNode = 5;

  // Find the path using A*
  int *path = A_Star(startNode, goalNode, heuristic, gridWidth, distance);

  // Print the path
  if (path != NULL) {
    printf("Path found: ");
    for (int i = 0; i < gridWidth * gridWidth; i++) {
      if (path[i] == -1) {
        break;
      }
      printf("%d ", path[i]);
    }
    free(path);
  } else {
    printf("No path found.\n");
  }

  return 0;
}
