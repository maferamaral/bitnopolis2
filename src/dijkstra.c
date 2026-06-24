#include "dijkstra.h"

#include "fila_prioridade.h"

#include <float.h>
#include <stdlib.h>

struct no_dijkstra {
    int indice;
};

struct resultado_dijkstra {
    int existe;
    double custo;
    VerticeGrafo *vertices;
    int quantidade_vertices;
};

static int obter_indice_vertice(Grafo grafo, VerticeGrafo vertice)
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

static struct no_dijkstra *criar_no_dijkstra(int indice)
{
    struct no_dijkstra *no = malloc(sizeof(*no));

    if (no == NULL) {
        return NULL;
    }

    no->indice = indice;
    return no;
}

static struct resultado_dijkstra *criar_resultado_vazio(void)
{
    struct resultado_dijkstra *resultado = calloc(1, sizeof(*resultado));

    return resultado;
}

static int construir_caminho(
    struct resultado_dijkstra *resultado,
    Grafo grafo,
    int *anteriores,
    int indice_origem,
    int indice_destino,
    double custo
)
{
    int quantidade = 1;
    int atual = indice_destino;
    int posicao;

    while (atual != indice_origem) {
        atual = anteriores[atual];
        quantidade++;
    }

    resultado->vertices = malloc((size_t) quantidade * sizeof(*resultado->vertices));
    if (resultado->vertices == NULL) {
        return 0;
    }

    atual = indice_destino;
    posicao = quantidade - 1;
    while (posicao >= 0) {
        resultado->vertices[posicao] = obter_vertice_grafo(grafo, atual);
        atual = anteriores[atual];
        posicao--;
    }

    resultado->existe = 1;
    resultado->custo = custo;
    resultado->quantidade_vertices = quantidade;
    return 1;
}

static int preparar_vetores(int quantidade, double **distancias, int **anteriores, int **visitados)
{
    int i;

    *distancias = malloc((size_t) quantidade * sizeof(**distancias));
    *anteriores = malloc((size_t) quantidade * sizeof(**anteriores));
    *visitados = calloc((size_t) quantidade, sizeof(**visitados));

    if (*distancias == NULL || *anteriores == NULL || *visitados == NULL) {
        free(*distancias);
        free(*anteriores);
        free(*visitados);
        return 0;
    }

    for (i = 0; i < quantidade; i++) {
        (*distancias)[i] = DBL_MAX;
        (*anteriores)[i] = -1;
    }

    return 1;
}

ResultadoDijkstra calcular_dijkstra(Grafo grafo, const char *origem, const char *destino, FuncaoPesoAresta calcular_peso)
{
    VerticeGrafo vertice_origem;
    VerticeGrafo vertice_destino;
    int indice_origem;
    int indice_destino;
    int quantidade;
    double *distancias = NULL;
    int *anteriores = NULL;
    int *visitados = NULL;
    FilaPrioridade fila;
    struct resultado_dijkstra *resultado;

    if (grafo == NULL || origem == NULL || destino == NULL || calcular_peso == NULL) {
        return NULL;
    }

    vertice_origem = buscar_vertice_grafo(grafo, origem);
    vertice_destino = buscar_vertice_grafo(grafo, destino);
    if (vertice_origem == NULL || vertice_destino == NULL) {
        return NULL;
    }

    quantidade = obter_quantidade_vertices_grafo(grafo);
    indice_origem = obter_indice_vertice(grafo, vertice_origem);
    indice_destino = obter_indice_vertice(grafo, vertice_destino);

    if (!preparar_vetores(quantidade, &distancias, &anteriores, &visitados)) {
        return NULL;
    }

    fila = criar_fila_prioridade();
    resultado = criar_resultado_vazio();
    if (fila == NULL || resultado == NULL) {
        destruir_fila_prioridade(fila);
        free(resultado);
        free(distancias);
        free(anteriores);
        free(visitados);
        return NULL;
    }

    distancias[indice_origem] = 0.0;
    inserir_fila_prioridade(fila, criar_no_dijkstra(indice_origem), 0.0);

    while (!fila_prioridade_esta_vazia(fila)) {
        struct no_dijkstra *no = remover_minimo_fila_prioridade(fila);
        int indice_atual;
        VerticeGrafo vertice_atual;
        int grau_saida;
        int i;

        if (no == NULL) {
            continue;
        }

        indice_atual = no->indice;
        free(no);

        if (visitados[indice_atual]) {
            continue;
        }

        visitados[indice_atual] = 1;
        if (indice_atual == indice_destino) {
            break;
        }

        vertice_atual = obter_vertice_grafo(grafo, indice_atual);
        grau_saida = obter_grau_saida_vertice_grafo(vertice_atual);

        for (i = 0; i < grau_saida; i++) {
            ArestaGrafo aresta = obter_aresta_vertice_grafo(vertice_atual, i);
            VerticeGrafo vizinho = obter_destino_aresta_grafo(aresta);
            int indice_vizinho = obter_indice_vertice(grafo, vizinho);
            double peso = calcular_peso(aresta);
            double nova_distancia;

            if (indice_vizinho < 0 || peso < 0.0 || distancias[indice_atual] == DBL_MAX) {
                continue;
            }

            nova_distancia = distancias[indice_atual] + peso;
            if (nova_distancia < distancias[indice_vizinho]) {
                struct no_dijkstra *novo_no;

                distancias[indice_vizinho] = nova_distancia;
                anteriores[indice_vizinho] = indice_atual;

                novo_no = criar_no_dijkstra(indice_vizinho);
                if (novo_no != NULL) {
                    inserir_fila_prioridade(fila, novo_no, nova_distancia);
                }
            }
        }
    }

    while (!fila_prioridade_esta_vazia(fila)) {
        free(remover_minimo_fila_prioridade(fila));
    }

    if (distancias[indice_destino] != DBL_MAX) {
        if (!construir_caminho(resultado, grafo, anteriores, indice_origem, indice_destino, distancias[indice_destino])) {
            destruir_resultado_dijkstra(resultado);
            resultado = NULL;
        }
    }

    destruir_fila_prioridade(fila);
    free(distancias);
    free(anteriores);
    free(visitados);
    return resultado;
}

void destruir_resultado_dijkstra(ResultadoDijkstra resultado_generico)
{
    struct resultado_dijkstra *resultado = resultado_generico;

    if (resultado == NULL) {
        return;
    }

    free(resultado->vertices);
    free(resultado);
}

double peso_distancia_aresta(ArestaGrafo aresta)
{
    return obter_comprimento_aresta_grafo(aresta);
}

double peso_tempo_aresta(ArestaGrafo aresta)
{
    double velocidade = obter_velocidade_aresta_grafo(aresta);

    if (velocidade <= 0.0) {
        return DBL_MAX / 4.0;
    }

    return obter_comprimento_aresta_grafo(aresta) / velocidade;
}

int caminho_dijkstra_existe(ResultadoDijkstra resultado_generico)
{
    struct resultado_dijkstra *resultado = resultado_generico;

    return resultado != NULL && resultado->existe;
}

double obter_custo_dijkstra(ResultadoDijkstra resultado_generico)
{
    struct resultado_dijkstra *resultado = resultado_generico;

    if (resultado == NULL || !resultado->existe) {
        return 0.0;
    }

    return resultado->custo;
}

int obter_quantidade_vertices_dijkstra(ResultadoDijkstra resultado_generico)
{
    struct resultado_dijkstra *resultado = resultado_generico;

    if (resultado == NULL || !resultado->existe) {
        return 0;
    }

    return resultado->quantidade_vertices;
}

VerticeGrafo obter_vertice_dijkstra(ResultadoDijkstra resultado_generico, int indice)
{
    struct resultado_dijkstra *resultado = resultado_generico;

    if (resultado == NULL || !resultado->existe || indice < 0 || indice >= resultado->quantidade_vertices) {
        return NULL;
    }

    return resultado->vertices[indice];
}
