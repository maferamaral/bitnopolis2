#include "arvore_minima.h"

#include <stdlib.h>

struct item_aresta_minima {
    ArestaGrafo aresta;
    int origem;
    int destino;
};

static int obter_indice_vertice_arvore(Grafo grafo, VerticeGrafo vertice)
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

static int comparar_arestas_por_comprimento(const void *a, const void *b)
{
    const struct item_aresta_minima *aresta_a = a;
    const struct item_aresta_minima *aresta_b = b;
    double comprimento_a = obter_comprimento_aresta_grafo(aresta_a->aresta);
    double comprimento_b = obter_comprimento_aresta_grafo(aresta_b->aresta);

    if (comprimento_a < comprimento_b) {
        return -1;
    }

    if (comprimento_a > comprimento_b) {
        return 1;
    }

    return 0;
}

static int encontrar_pai_arvore(int *pais, int indice)
{
    if (pais[indice] != indice) {
        pais[indice] = encontrar_pai_arvore(pais, pais[indice]);
    }

    return pais[indice];
}

static int unir_arvore(int *pais, int a, int b)
{
    int pai_a = encontrar_pai_arvore(pais, a);
    int pai_b = encontrar_pai_arvore(pais, b);

    if (pai_a == pai_b) {
        return 0;
    }

    pais[pai_b] = pai_a;
    return 1;
}

static struct item_aresta_minima *coletar_arestas(Grafo grafo, int *quantidade_coletada)
{
    int capacidade = obter_quantidade_arestas_grafo(grafo);
    int quantidade_vertices = obter_quantidade_vertices_grafo(grafo);
    struct item_aresta_minima *arestas;
    int i;

    *quantidade_coletada = 0;
    if (capacidade <= 0) {
        return NULL;
    }

    arestas = malloc((size_t) capacidade * sizeof(*arestas));
    if (arestas == NULL) {
        return NULL;
    }

    for (i = 0; i < quantidade_vertices; i++) {
        VerticeGrafo origem = obter_vertice_grafo(grafo, i);
        int j;
        int grau_saida = obter_grau_saida_vertice_grafo(origem);

        for (j = 0; j < grau_saida; j++) {
            ArestaGrafo aresta = obter_aresta_vertice_grafo(origem, j);
            int indice_origem = obter_indice_vertice_arvore(grafo, obter_origem_aresta_grafo(aresta));
            int indice_destino = obter_indice_vertice_arvore(grafo, obter_destino_aresta_grafo(aresta));

            if (indice_origem >= 0 && indice_destino >= 0 && *quantidade_coletada < capacidade) {
                arestas[*quantidade_coletada].aresta = aresta;
                arestas[*quantidade_coletada].origem = indice_origem;
                arestas[*quantidade_coletada].destino = indice_destino;
                (*quantidade_coletada)++;
            }
        }
    }

    return arestas;
}

int expandir_infraestrutura_arvore_minima_com_arestas(Grafo grafo, double limite_velocidade, ArestaGrafo *arestas_expandidas, int capacidade)
{
    struct item_aresta_minima *arestas;
    int *pais;
    int quantidade_arestas;
    int quantidade_vertices;
    int i;
    int selecionadas = 0;
    int expandidas = 0;

    if (grafo == NULL) {
        return 0;
    }

    quantidade_vertices = obter_quantidade_vertices_grafo(grafo);
    arestas = coletar_arestas(grafo, &quantidade_arestas);
    pais = malloc((size_t) quantidade_vertices * sizeof(*pais));

    if (pais == NULL) {
        free(arestas);
        return 0;
    }

    for (i = 0; i < quantidade_vertices; i++) {
        pais[i] = i;
    }

    qsort(arestas, (size_t) quantidade_arestas, sizeof(*arestas), comparar_arestas_por_comprimento);

    for (i = 0; i < quantidade_arestas && selecionadas < quantidade_vertices - 1; i++) {
        if (unir_arvore(pais, arestas[i].origem, arestas[i].destino)) {
            double velocidade = obter_velocidade_aresta_grafo(arestas[i].aresta);

            selecionadas++;
            if (velocidade < limite_velocidade) {
                definir_velocidade_aresta_grafo(arestas[i].aresta, velocidade * 1.5);
                if (arestas_expandidas != NULL && expandidas < capacidade) {
                    arestas_expandidas[expandidas] = arestas[i].aresta;
                }
                expandidas++;
            }
        }
    }

    free(pais);
    free(arestas);
    return expandidas;
}

int expandir_infraestrutura_arvore_minima(Grafo grafo, double limite_velocidade)
{
    return expandir_infraestrutura_arvore_minima_com_arestas(grafo, limite_velocidade, NULL, 0);
}
