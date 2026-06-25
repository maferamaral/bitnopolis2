#ifndef PERCURSO_H
#define PERCURSO_H

#include "dijkstra.h"
#include "grafo.h"
#include "registrador.h"

typedef void *Percurso;

Percurso calcular_percurso_registradores(Grafo grafo, Registradores registradores, int origem, int destino);
void destruir_percurso(Percurso percurso);

int percurso_existe(Percurso percurso);
ResultadoDijkstra obter_percurso_curto(Percurso percurso);
ResultadoDijkstra obter_percurso_rapido(Percurso percurso);
VerticeGrafo obter_origem_percurso(Percurso percurso);
VerticeGrafo obter_destino_percurso(Percurso percurso);

#endif
