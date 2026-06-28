#include "componentes.h"

#include <stdlib.h>

struct componentes {
    Grafo grafo;
    int *componente_por_vertice;
    int quantidade_vertices;
    int quantidade_componentes;
};

static int encontrar_pai(int *pais, int indice)
{
    if (pais[indice] != indice) {
        pais[indice] = encontrar_pai(pais, pais[indice]);
    }

    return pais[indice];
}

static void unir_vertices(int *pais, int a, int b)
{
    int pai_a = encontrar_pai(pais, a);
    int pai_b = encontrar_pai(pais, b);

    if (pai_a != pai_b) {
        pais[pai_b] = pai_a;
    }
}

static int obter_indice_vertice_componentes(Grafo grafo, VerticeGrafo vertice)
{
    int i;
    int quantidade = obter_quantidade_vertices_grafo(grafo);

    for (i = 0; i < quantidade; i++) {
        if (obter_vertice_grafo(grafo, i) == vertice) {
            return i;
        }
    }

    return -1;
}

static int inicializar_vetores(int quantidade, int **pais, int **possui_aresta_lenta)
{
    int i;

    *pais = malloc((size_t) quantidade * sizeof(**pais));
    *possui_aresta_lenta = calloc((size_t) quantidade, sizeof(**possui_aresta_lenta));

    if (*pais == NULL || *possui_aresta_lenta == NULL) {
        free(*pais);
        free(*possui_aresta_lenta);
        return 0;
    }

    for (i = 0; i < quantidade; i++) {
        (*pais)[i] = i;
    }

    return 1;
}

static int atribuir_componentes(struct componentes *componentes, int *pais, int *possui_aresta_lenta)
{
    int *mapa_raiz;
    int i;

    mapa_raiz = malloc((size_t) componentes->quantidade_vertices * sizeof(*mapa_raiz));
    if (mapa_raiz == NULL) {
        return 0;
    }

    for (i = 0; i < componentes->quantidade_vertices; i++) {
        componentes->componente_por_vertice[i] = -1;
        mapa_raiz[i] = -1;
    }

    for (i = 0; i < componentes->quantidade_vertices; i++) {
        int raiz;

        if (!possui_aresta_lenta[i]) {
            continue;
        }

        raiz = encontrar_pai(pais, i);
        if (mapa_raiz[raiz] < 0) {
            mapa_raiz[raiz] = componentes->quantidade_componentes;
            componentes->quantidade_componentes++;
        }

        componentes->componente_por_vertice[i] = mapa_raiz[raiz];
    }

    free(mapa_raiz);
    return 1;
}

Componentes calcular_componentes_lentas(Grafo grafo, double limite_velocidade)
{
    struct componentes *componentes;
    int *pais = NULL;
    int *possui_aresta_lenta = NULL;
    int i;
    int quantidade_vertices;

    if (grafo == NULL) {
        return NULL;
    }

    quantidade_vertices = obter_quantidade_vertices_grafo(grafo);
    componentes = calloc(1, sizeof(*componentes));
    if (componentes == NULL) {
        return NULL;
    }

    componentes->grafo = grafo;
    componentes->quantidade_vertices = quantidade_vertices;
    componentes->componente_por_vertice = malloc((size_t) quantidade_vertices * sizeof(*componentes->componente_por_vertice));

    if (componentes->componente_por_vertice == NULL || !inicializar_vetores(quantidade_vertices, &pais, &possui_aresta_lenta)) {
        destruir_componentes(componentes);
        return NULL;
    }

    for (i = 0; i < quantidade_vertices; i++) {
        VerticeGrafo origem = obter_vertice_grafo(grafo, i);
        int j;
        int grau_saida = obter_grau_saida_vertice_grafo(origem);

        for (j = 0; j < grau_saida; j++) {
            ArestaGrafo aresta = obter_aresta_vertice_grafo(origem, j);

            if (obter_velocidade_aresta_grafo(aresta) < limite_velocidade) {
                int indice_origem = obter_indice_vertice_componentes(grafo, obter_origem_aresta_grafo(aresta));
                int indice_destino = obter_indice_vertice_componentes(grafo, obter_destino_aresta_grafo(aresta));

                if (indice_origem >= 0 && indice_destino >= 0) {
                    possui_aresta_lenta[indice_origem] = 1;
                    possui_aresta_lenta[indice_destino] = 1;
                    unir_vertices(pais, indice_origem, indice_destino);
                }
            }
        }
    }

    if (!atribuir_componentes(componentes, pais, possui_aresta_lenta)) {
        destruir_componentes(componentes);
        componentes = NULL;
    }

    free(pais);
    free(possui_aresta_lenta);
    return componentes;
}

void destruir_componentes(Componentes componentes_generico)
{
    struct componentes *componentes = componentes_generico;

    if (componentes == NULL) {
        return;
    }

    free(componentes->componente_por_vertice);
    free(componentes);
}

int obter_quantidade_componentes(Componentes componentes_generico)
{
    struct componentes *componentes = componentes_generico;

    if (componentes == NULL) {
        return 0;
    }

    return componentes->quantidade_componentes;
}

int obter_componente_vertice(Componentes componentes_generico, VerticeGrafo vertice)
{
    struct componentes *componentes = componentes_generico;
    int indice;

    if (componentes == NULL || vertice == NULL) {
        return -1;
    }

    indice = obter_indice_vertice_componentes(componentes->grafo, vertice);
    if (indice < 0 || indice >= componentes->quantidade_vertices) {
        return -1;
    }

    return componentes->componente_por_vertice[indice];
}
