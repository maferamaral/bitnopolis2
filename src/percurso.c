#include "percurso.h"

#include <stdlib.h>

struct percurso {
    VerticeGrafo origem;
    VerticeGrafo destino;
    ResultadoDijkstra curto;
    ResultadoDijkstra rapido;
};

Percurso calcular_percurso_registradores(Grafo grafo, Registradores registradores, int origem, int destino)
{
    struct percurso *percurso;
    double origem_x;
    double origem_y;
    double destino_x;
    double destino_y;
    const char *id_origem;
    const char *id_destino;

    if (grafo == NULL || registradores == NULL) {
        return NULL;
    }

    if (!registrador_esta_definido(registradores, origem) || !registrador_esta_definido(registradores, destino)) {
        return NULL;
    }

    percurso = calloc(1, sizeof(*percurso));
    if (percurso == NULL) {
        return NULL;
    }

    origem_x = obter_x_registrador(registradores, origem);
    origem_y = obter_y_registrador(registradores, origem);
    destino_x = obter_x_registrador(registradores, destino);
    destino_y = obter_y_registrador(registradores, destino);

    percurso->origem = buscar_vertice_mais_proximo_grafo(grafo, origem_x, origem_y);
    percurso->destino = buscar_vertice_mais_proximo_grafo(grafo, destino_x, destino_y);

    if (percurso->origem == NULL || percurso->destino == NULL) {
        destruir_percurso(percurso);
        return NULL;
    }

    id_origem = obter_id_vertice_grafo(percurso->origem);
    id_destino = obter_id_vertice_grafo(percurso->destino);

    percurso->curto = calcular_dijkstra(grafo, id_origem, id_destino, peso_distancia_aresta);
    percurso->rapido = calcular_dijkstra(grafo, id_origem, id_destino, peso_tempo_aresta);

    if (percurso->curto == NULL || percurso->rapido == NULL) {
        destruir_percurso(percurso);
        return NULL;
    }

    return percurso;
}

void destruir_percurso(Percurso percurso_generico)
{
    struct percurso *percurso = percurso_generico;

    if (percurso == NULL) {
        return;
    }

    destruir_resultado_dijkstra(percurso->curto);
    destruir_resultado_dijkstra(percurso->rapido);
    free(percurso);
}

int percurso_existe(Percurso percurso_generico)
{
    struct percurso *percurso = percurso_generico;

    return percurso != NULL &&
           caminho_dijkstra_existe(percurso->curto) &&
           caminho_dijkstra_existe(percurso->rapido);
}

ResultadoDijkstra obter_percurso_curto(Percurso percurso_generico)
{
    struct percurso *percurso = percurso_generico;

    if (percurso == NULL) {
        return NULL;
    }

    return percurso->curto;
}

ResultadoDijkstra obter_percurso_rapido(Percurso percurso_generico)
{
    struct percurso *percurso = percurso_generico;

    if (percurso == NULL) {
        return NULL;
    }

    return percurso->rapido;
}

VerticeGrafo obter_origem_percurso(Percurso percurso_generico)
{
    struct percurso *percurso = percurso_generico;

    if (percurso == NULL) {
        return NULL;
    }

    return percurso->origem;
}

VerticeGrafo obter_destino_percurso(Percurso percurso_generico)
{
    struct percurso *percurso = percurso_generico;

    if (percurso == NULL) {
        return NULL;
    }

    return percurso->destino;
}
