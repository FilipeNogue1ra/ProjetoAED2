//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
  assert(g != NULL);  // Verifica se o grafo não é nulo
  assert(GraphIsDigraph(g));  // Verifica se o grafo é orientado
  assert(GraphIsWeighted(g) == 0);  // Verifica se o grafo não possui pesos nas arestas

  unsigned int numVertices = GraphGetNumVertices(g);  // Obtém o número de vértices do grafo
  Graph* transitiveClosure = GraphCreate(numVertices, 1, 0);  // Cria um novo grafo para armazenar o fecho transitivo

  for (unsigned int v = 0; v < numVertices; v++) {
    GraphBellmanFordAlg* bfResult = GraphBellmanFordAlgExecute(g, v);  // Executa o algoritmo de Bellman-Ford para o vértice v

    for (unsigned int w = 0; w < numVertices; w++) {
      if (v != w && GraphBellmanFordAlgReached(bfResult, w)) {  // Verifica se w é alcançável a partir de v e se v != w
        GraphAddEdge(transitiveClosure, v, w);  // Adiciona uma aresta de v para w no fecho transitivo
      }
    }

    GraphBellmanFordAlgDestroy(&bfResult);  // Libera a memória alocada para o resultado do Bellman-Ford
  }

  return transitiveClosure;  // Retorna o grafo contendo o fecho transitivo
}