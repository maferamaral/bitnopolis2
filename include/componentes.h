#ifndef COMPONENTES_H
#define COMPONENTES_H

#include "grafo.h"

typedef void *Componentes;

Componentes calcular_componentes_lentas(Grafo grafo, double limite_velocidade);
void destruir_componentes(Componentes componentes);

int obter_quantidade_componentes(Componentes componentes);
int obter_componente_vertice(Componentes componentes, VerticeGrafo vertice);

#endif
