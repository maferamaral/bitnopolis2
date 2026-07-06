#ifndef ARVORE_MINIMA_H
#define ARVORE_MINIMA_H

#include "grafo.h"

int expandir_infraestrutura_arvore_minima(Grafo grafo, double limite_velocidade);
int expandir_infraestrutura_arvore_minima_com_arestas(Grafo grafo, double limite_velocidade, ArestaGrafo *arestas_expandidas, int capacidade);

#endif
