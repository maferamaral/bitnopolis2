#ifndef PERCURSO_H
#define PERCURSO_H

#include "dijkstra.h"
#include "grafo.h"
#include "registrador.h"

/* Guarda os caminhos curto e rapido entre dois registradores. */
typedef void *Percurso;

Percurso calcular_percurso_registradores(Grafo grafo, Registradores registradores, int origem, int destino);
void destruir_percurso(Percurso percurso);

/* Resultados retornados pertencem ao Percurso e nao devem ser destruidos fora dele. */
int percurso_existe(Percurso percurso);
ResultadoDijkstra obter_percurso_curto(Percurso percurso);
ResultadoDijkstra obter_percurso_rapido(Percurso percurso);
VerticeGrafo obter_origem_percurso(Percurso percurso);
VerticeGrafo obter_destino_percurso(Percurso percurso);

#endif
