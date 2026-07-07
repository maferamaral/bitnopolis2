#include "componentes.h"

#include <stdlib.h>

struct componentes {
    Grafo grafo;
    int *componente_por_vertice;
    int *tamanho_por_componente;
    int quantidade_vertices;
    int quantidade_componentes;
};

struct contexto_componentes {
    struct componentes *componentes;
    int *indices;
    int *menores;
    int *pilha;
    int *esta_na_pilha;
    int proximo_indice;
    int topo_pilha;
    double limite_velocidade;
};

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

static int menor_inteiro(int a, int b)
{
    return a < b ? a : b;
}

static int inicializar_contexto(struct contexto_componentes *contexto, struct componentes *componentes, double limite_velocidade)
{
    int i;
    int quantidade = componentes->quantidade_vertices;

    contexto->componentes = componentes;
    contexto->limite_velocidade = limite_velocidade;
    contexto->proximo_indice = 0;
    contexto->topo_pilha = 0;
    contexto->indices = malloc((size_t) quantidade * sizeof(*contexto->indices));
    contexto->menores = malloc((size_t) quantidade * sizeof(*contexto->menores));
    contexto->pilha = malloc((size_t) quantidade * sizeof(*contexto->pilha));
    contexto->esta_na_pilha = calloc((size_t) quantidade, sizeof(*contexto->esta_na_pilha));

    if (contexto->indices == NULL || contexto->menores == NULL || contexto->pilha == NULL || contexto->esta_na_pilha == NULL) {
        free(contexto->indices);
        free(contexto->menores);
        free(contexto->pilha);
        free(contexto->esta_na_pilha);
        return 0;
    }

    for (i = 0; i < quantidade; i++) {
        contexto->indices[i] = -1;
        contexto->menores[i] = -1;
        componentes->componente_por_vertice[i] = -1;
    }

    return 1;
}

static void liberar_contexto(struct contexto_componentes *contexto)
{
    free(contexto->indices);
    free(contexto->menores);
    free(contexto->pilha);
    free(contexto->esta_na_pilha);
}

static void desempilhar_componente(struct contexto_componentes *contexto, int indice_raiz)
{
    int indice;
    int tamanho = 0;

    do {
        contexto->topo_pilha--;
        indice = contexto->pilha[contexto->topo_pilha];
        contexto->esta_na_pilha[indice] = 0;
        contexto->componentes->componente_por_vertice[indice] = contexto->componentes->quantidade_componentes;
        tamanho++;
    } while (indice != indice_raiz && contexto->topo_pilha > 0);

    contexto->componentes->tamanho_por_componente[contexto->componentes->quantidade_componentes] = tamanho;
    contexto->componentes->quantidade_componentes++;
}

static void visitar_vertice(struct contexto_componentes *contexto, int indice)
{
    VerticeGrafo vertice;
    int i;
    int grau_saida;

    contexto->indices[indice] = contexto->proximo_indice;
    contexto->menores[indice] = contexto->proximo_indice;
    contexto->proximo_indice++;
    contexto->pilha[contexto->topo_pilha] = indice;
    contexto->topo_pilha++;
    contexto->esta_na_pilha[indice] = 1;

    vertice = obter_vertice_grafo(contexto->componentes->grafo, indice);
    grau_saida = obter_grau_saida_vertice_grafo(vertice);
    for (i = 0; i < grau_saida; i++) {
        ArestaGrafo aresta = obter_aresta_vertice_grafo(vertice, i);
        int indice_destino;

        if (obter_velocidade_aresta_grafo(aresta) < contexto->limite_velocidade) {
            continue;
        }

        indice_destino = obter_indice_vertice_componentes(contexto->componentes->grafo, obter_destino_aresta_grafo(aresta));
        if (indice_destino < 0) {
            continue;
        }

        if (contexto->indices[indice_destino] < 0) {
            visitar_vertice(contexto, indice_destino);
            contexto->menores[indice] = menor_inteiro(contexto->menores[indice], contexto->menores[indice_destino]);
        } else if (contexto->esta_na_pilha[indice_destino]) {
            contexto->menores[indice] = menor_inteiro(contexto->menores[indice], contexto->indices[indice_destino]);
        }
    }

    if (contexto->menores[indice] == contexto->indices[indice]) {
        desempilhar_componente(contexto, indice);
    }
}

Componentes calcular_componentes_lentas(Grafo grafo, double limite_velocidade)
{
    struct componentes *componentes;
    struct contexto_componentes contexto;
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
    componentes->tamanho_por_componente = calloc((size_t) quantidade_vertices, sizeof(*componentes->tamanho_por_componente));
    if (componentes->componente_por_vertice == NULL || componentes->tamanho_por_componente == NULL || !inicializar_contexto(&contexto, componentes, limite_velocidade)) {
        destruir_componentes(componentes);
        return NULL;
    }

    for (i = 0; i < quantidade_vertices; i++) {
        if (contexto.indices[i] < 0) {
            visitar_vertice(&contexto, i);
        }
    }

    liberar_contexto(&contexto);
    return componentes;
}

void destruir_componentes(Componentes componentes_generico)
{
    struct componentes *componentes = componentes_generico;

    if (componentes == NULL) {
        return;
    }

    free(componentes->componente_por_vertice);
    free(componentes->tamanho_por_componente);
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

int obter_quantidade_componentes_unico_vertice(Componentes componentes_generico)
{
    struct componentes *componentes = componentes_generico;
    int quantidade = 0;
    int i;

    if (componentes == NULL) {
        return 0;
    }

    for (i = 0; i < componentes->quantidade_componentes; i++) {
        if (componentes->tamanho_por_componente[i] == 1) {
            quantidade++;
        }
    }

    return quantidade;
}

int obter_quantidade_componentes_multiplos_vertices(Componentes componentes_generico)
{
    struct componentes *componentes = componentes_generico;
    int quantidade = 0;
    int i;

    if (componentes == NULL) {
        return 0;
    }

    for (i = 0; i < componentes->quantidade_componentes; i++) {
        if (componentes->tamanho_por_componente[i] > 1) {
            quantidade++;
        }
    }

    return quantidade;
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
