//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersStack.h"
#include "instrumentation.h"

struct _GraphBellmanFordAlg {
  unsigned int* marked;  // To mark vertices when reached for the first time
  int* distance;  // The number of edges on the path from the start vertex
                  // distance[i]=-1, if no path found from the start vertex to i
  int* predecessor;  // The predecessor vertex in the shortest path
                     // predecessor[i]=-1, if no predecessor exists
  Graph* graph;
  unsigned int startVertex;  // The root of the shortest-paths tree
};

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g,
                                                unsigned int startVertex) {
  assert(g != NULL);  // Verifica se o grafo não é nulo
  assert(startVertex < GraphGetNumVertices(g));  // Verifica se o vértice inicial é válido
  assert(GraphIsWeighted(g) == 0);  // Verifica se o grafo não possui pesos nas arestas

  GraphBellmanFordAlg* result =
      (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);  // Verifica se a alocação de memória foi bem-sucedida

  // Grafo dado e vértice inicial para os caminhos mais curtos
  result->graph = g;
  result->startVertex = startVertex;

  unsigned int numVertices = GraphGetNumVertices(g);  // Obtém o número de vértices do grafo

  // Aloca e inicializa os arrays
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  result->distance = (int*)malloc(numVertices * sizeof(int));
  result->predecessor = (int*)malloc(numVertices * sizeof(int));

  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = -1;  // -1 representa infinito
    result->predecessor[i] = -1;  // Nenhum predecessor inicialmente
  }

  // Inicializa o vértice inicial
  result->distance[startVertex] = 0;
  result->marked[startVertex] = 1;

  // Algoritmo de Bellman-Ford
  for (unsigned int i = 1; i < numVertices; i++) { // V - 1 iterações
    for (unsigned int v = 0; v < numVertices; v++) {
      if (result->distance[v] != -1) { // Considera apenas vértices alcançáveis
        int* neighbors = GraphGetAdjacentsTo(g, v);
        int numNeighbors = neighbors[0]; // Primeiro elemento é a contagem
        for (int j = 1; j <= numNeighbors; j++) {
          int w = neighbors[j];
          if (result->distance[w] == -1 || result->distance[w] > result->distance[v] + 1) {
            result->distance[w] = result->distance[v] + 1;
            result->predecessor[w] = v;
            result->marked[w] = 1;
          }
        }
        free(neighbors); // Libera a lista de adjacência
      }
    }
  }

  // Verifica ciclos de peso negativo (não necessário para grafos sem peso, mas incluído para completude)
  for (unsigned int v = 0; v < numVertices; v++) {
    if (result->distance[v] != -1) {
      int* neighbors = GraphGetAdjacentsTo(g, v);
      int numNeighbors = neighbors[0];
      for (int j = 1; j <= numNeighbors; j++) {
        int w = neighbors[j];
        if (result->distance[w] > result->distance[v] + 1) {
          printf("O grafo contém um ciclo de peso negativo.\n");
          free(neighbors);
          GraphBellmanFordAlgDestroy(&result);
          return NULL;
        }
      }
      free(neighbors);
    }
  }

  return result;  // Retorna a estrutura contendo os resultados do algoritmo de Bellman-Ford
}

void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
  assert(*p != NULL);

  GraphBellmanFordAlg* aux = *p;

  free(aux->marked);
  free(aux->predecessor);
  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the paths information

int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->marked[v];
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->distance[v];
}
Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = StackCreate(GraphGetNumVertices(p->graph));

  if (p->marked[v] == 0) {
    return s;
  }

  // Store the path
  for (unsigned int current = v; current != p->startVertex;
       current = p->predecessor[current]) {
    StackPush(s, current);
  }

  StackPush(s, p->startVertex);

  return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = GraphBellmanFordAlgPathTo(p, v);

  while (StackIsEmpty(s) == 0) {
    printf("%d ", StackPop(s));
  }

  StackDestroy(&s);
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
  assert(p != NULL);

  Graph* original_graph = p->graph;
  unsigned int num_vertices = GraphGetNumVertices(original_graph);

  // The paths tree is a digraph, with no edge weights
  Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

  // Use the predecessors array to add the tree edges
  for (unsigned int w = 0; w < num_vertices; w++) {
    // Vertex w has a predecessor vertex v?
    int v = p->predecessor[w];
    if (v != -1) {
      GraphAddEdge(paths_tree, (unsigned int)v, w);
    }
  }

  // Display the tree in the DOT format
  GraphDisplayDOT(paths_tree);

  // Housekeeping
  GraphDestroy(&paths_tree);
}
