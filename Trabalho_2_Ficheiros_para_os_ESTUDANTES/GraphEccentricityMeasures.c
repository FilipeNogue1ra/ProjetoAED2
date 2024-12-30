//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
  assert(g != NULL);  // Verifica se o grafo não é nulo

  unsigned int numVertices = GraphGetNumVertices(g);  // Obtém o número de vértices do grafo
  GraphEccentricityMeasures* result = (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
  result->graph = g;
  result->eccentricity = (int*)malloc(numVertices * sizeof(int));
  result->graphRadius = -1;
  result->graphDiameter = -1;

  // Executa o algoritmo de Bellman-Ford para calcular as distâncias entre todos os pares de vértices
  GraphAllPairsShortestDistances* distancesMatrix = GraphAllPairsShortestDistancesExecute(g);

  // Calcula a excentricidade de cada vértice
  for (unsigned int v = 0; v < numVertices; v++) {
    int maxDistance = -1;
    for (unsigned int w = 0; w < numVertices; w++) {
      int distance = GraphGetDistanceVW(distancesMatrix, v, w);
      if (distance > maxDistance) {
        maxDistance = distance;
      }
    }
    result->eccentricity[v] = maxDistance;
    if (result->graphRadius == -1 || maxDistance < result->graphRadius) {
      result->graphRadius = maxDistance;
    }
    if (result->graphDiameter == -1 || maxDistance > result->graphDiameter) {
      result->graphDiameter = maxDistance;
    }
  }

  // Determina os vértices centrais
  unsigned int numCentralVertices = 0;
  for (unsigned int v = 0; v < numVertices; v++) {
    if (result->eccentricity[v] == result->graphRadius) {
      numCentralVertices++;
    }
  }

  result->centralVertices = (unsigned int*)malloc((numCentralVertices + 1) * sizeof(unsigned int));
  result->centralVertices[0] = numCentralVertices;
  unsigned int index = 1;
  for (unsigned int v = 0; v < numVertices; v++) {
    if (result->eccentricity[v] == result->graphRadius) {
      result->centralVertices[index++] = v;
    }
  }

  // Libera a memória alocada para a matriz de distâncias
  GraphAllPairsShortestDistancesDestroy(&distancesMatrix);

  return result;  // Retorna a estrutura contendo as medidas de excentricidade
}

void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
  assert(p != NULL);  // Verifica se a estrutura não é nula
  assert(p->centralVertices != NULL);  // Verifica se o array de vértices centrais não é nulo

  unsigned int numCentralVertices = p->centralVertices[0];
  unsigned int* centralVerticesCopy = (unsigned int*)malloc((numCentralVertices + 1) * sizeof(unsigned int));
  for (unsigned int i = 0; i <= numCentralVertices; i++) {
    centralVerticesCopy[i] = p->centralVertices[i];
  }

  return centralVerticesCopy;  // Retorna uma cópia do array de vértices centrais
}

// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
  assert(p != NULL);  // Verifica se a estrutura não é nula

  printf("Graph Radius: %d\n", p->graphRadius);  // Imprime o raio do grafo
  printf("Graph Diameter: %d\n", p->graphDiameter);  // Imprime o diâmetro do grafo

  printf("Vertex Eccentricities:\n");
  for (unsigned int v = 0; v < GraphGetNumVertices(p->graph); v++) {
    printf("Vertex %u: %d\n", v, p->eccentricity[v]);  // Imprime a excentricidade de cada vértice
  }

  printf("Central Vertices:\n");
  unsigned int numCentralVertices = p->centralVertices[0];
  for (unsigned int i = 1; i <= numCentralVertices; i++) {
    printf("Vertex %u\n", p->centralVertices[i]);  // Imprime cada vértice central
  }
}
