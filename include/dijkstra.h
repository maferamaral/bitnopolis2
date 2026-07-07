#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "grafo.h"

/* Resultado de um caminho minimo entre dois vertices do grafo. */
typedef void *ResultadoDijkstra;

/* Permite trocar o criterio: distancia, tempo ou outro peso derivado da aresta. */
typedef double (*FuncaoPesoAresta)(ArestaGrafo aresta);

ResultadoDijkstra calcular_dijkstra(Grafo grafo, const char *origem, const char *destino, FuncaoPesoAresta calcular_peso);
void destruir_resultado_dijkstra(ResultadoDijkstra resultado);

/* Pesos padrao usados pelos comandos de percurso. */
double peso_distancia_aresta(ArestaGrafo aresta);
double peso_tempo_aresta(ArestaGrafo aresta);

/* Acesso ao caminho calculado, na ordem origem -> destino. */
int caminho_dijkstra_existe(ResultadoDijkstra resultado);
double obter_custo_dijkstra(ResultadoDijkstra resultado);
int obter_quantidade_vertices_dijkstra(ResultadoDijkstra resultado);
VerticeGrafo obter_vertice_dijkstra(ResultadoDijkstra resultado, int indice);

#endif
