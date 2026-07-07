#ifndef COMPONENTES_H
#define COMPONENTES_H

#include "grafo.h"

/* Resultado da decomposicao em componentes fortemente conexos. */
typedef void *Componentes;

/* Desconsidera arestas com velocidade menor que o limite durante o calculo. */
Componentes calcular_componentes_lentas(Grafo grafo, double limite_velocidade);
void destruir_componentes(Componentes componentes);

/* Componentes sao numerados a partir de 0; vertices fora do grafo retornam -1. */
int obter_quantidade_componentes(Componentes componentes);
int obter_quantidade_componentes_unico_vertice(Componentes componentes);
int obter_quantidade_componentes_multiplos_vertices(Componentes componentes);
int obter_componente_vertice(Componentes componentes, VerticeGrafo vertice);

#endif
