#ifndef ARVORE_MINIMA_H
#define ARVORE_MINIMA_H

#include "grafo.h"

/* Expande arestas lentas usando uma arvore geradora minima da regiao afetada. */
int expandir_infraestrutura_arvore_minima(Grafo grafo, double limite_velocidade);

/* Guarda em arestas_expandidas, se houver espaco, as arestas alteradas. */
int expandir_infraestrutura_arvore_minima_com_arestas(Grafo grafo, double limite_velocidade, ArestaGrafo *arestas_expandidas, int capacidade);

#endif
