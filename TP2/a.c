#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 5
#define COLS 5
#define INFINITO 9999999

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

Point start = {0, 0};
Point goal = {4, 4};

int is_valid(int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS);
}

int is_obstacle(int x, int y) {
    return 0;
}

int calculate_h_value(int x, int y) {
    return abs(x - goal.x) + abs(y - goal.y);
}

void initialize_grid() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            grid[i][j].f = grid[i][j].g = grid[i][j].h = INFINITO;
            grid[i][j].parent.x = -1;
            grid[i][j].parent.y = -1;
        }
    }
}

// Implementa a lógica do algoritmo A*
void a_star() {
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
}

int main() {
    a_star();

    // Imprimir o caminho a seguir
    Point current = goal;
    while (current.x != -1 && current.y != -1) {
        printf("(%d, %d) <- ", current.x, current.y);
        current = grid[current.x][current.y].parent;
    }
    printf("\n");

    return 0;
}
