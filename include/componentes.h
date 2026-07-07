#ifndef COMPONENTES_H
#define COMPONENTES_H

#include "grafo.h"

/* Resultado da decomposicao do grafo em componentes de vias lentas. */
typedef void *Componentes;

/* Considera lentas as arestas com velocidade menor ou igual ao limite. */
Componentes calcular_componentes_lentas(Grafo grafo, double limite_velocidade);
void destruir_componentes(Componentes componentes);

/* Componentes sao numerados a partir de 0; vertices fora do resultado retornam -1. */
int obter_quantidade_componentes(Componentes componentes);
int obter_componente_vertice(Componentes componentes, VerticeGrafo vertice);

#endif
