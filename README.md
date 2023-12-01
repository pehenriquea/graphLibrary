# Algortimo em Grafos - Biblioteca de Grafos
💻 Integrantes: Isabela Ferreira Scarabelli e Pedro Henrique de Almeida Santos.

📖 Disciplina: Algoritmos em Grafos

🏫 Pontifícia Universidade Católica de Minas Gerais

Os grafos podem ser representados através de matriz de adjacência, lista de adjacência e uma estrutura criada pelo grupo chamada 'node'.

A biblioteca desenvolvida deve disponibilizar as seguintes funcionalidades:
- Criar o grafo com N vértices (N a ser informado pelo usuário).
- Inserir e remover arestas (para grafos direcionados e não direcionados, com arestas ponderadas ou não).
- Consultar o grau de um vértice.
- Consultar o grau do grafo.
- Consultar os vizinhos de um vértice.
- Verificar se o grafo é conexo.
- Verificar se o grafo é regular.
- Verificar se o grafo é completo.
- Fazer busca em profundidade.
- Fazer busca em largura.
- Verificar a existência de caminho entre dois vértices e caso existir, exibir o caminho.
- Permitir a visualização do grafo na ferramenta Gephi gerando um arquivo de formato CSV chamado grafo.csv.

As estruturas (structs) utilizadas no projeto são:
- No --> utilizada para a lista de adjacência, possui o identificador do vértice e um ponteiro para o próximo.
- Grafo --> também utilizado pela lista de adjacência, possui a quantidade de vértices do grafo e uma lista contendo cada vértice e sua respectiva lista de adjacência (seus vértices adjacentes). 
- node --> estrutura criada para auxiliar na gravação dos dados importantes dos nós. Ela possui as informações: id, grau do nó, pai / tempo de descoberta e tempo de término / já explorado / já visitado (utilizado para as buscas), e uma lista com seus nós adjacentes.
  
![image](https://github.com/pehenriquea/graphLibrary/assets/73960096/87fb48e3-f45c-45a8-8cc7-83065bbede59)

No projeto também estão desenvolvidos os algoritmos de caminho mínimo:
- Dijkstra
- Bellman-Ford
- Ford-Warshall
- A*


Resultado das métricas:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 500 vértices &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.000 vértices &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;10.000 vértices

Bellman-Ford:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 3 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  76 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; x

Dijkstra:                      1 segundo                                    8 segundos                                                     x segundos

Ford-Warshall:                  1 segundo                                    3 segundos                                                     x segundos

Para olhar os resultados dos algoritmos de caminho mínimo basta descomentar as chamadas de funções de print.
