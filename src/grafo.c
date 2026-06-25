#include "grafo.h"

#include "lista.h"

#include <stdlib.h>
#include <string.h>

struct vertice_grafo {
    char *id;
    double x;
    double y;
    Lista arestas_saida;
};

struct aresta_grafo {
    struct vertice_grafo *origem;
    struct vertice_grafo *destino;
    char *cep_direita;
    char *cep_esquerda;
    double comprimento;
    double velocidade_media;
    char *nome;
};

struct grafo {
    Lista vertices;
    int quantidade_arestas;
};

static char *copiar_texto_grafo(const char *texto)
{
    char *copia;
    size_t tamanho;

    if (texto == NULL) {
        return NULL;
    }

    tamanho = strlen(texto) + 1;
    copia = malloc(tamanho);
    if (copia == NULL) {
        return NULL;
    }

    memcpy(copia, texto, tamanho);
    return copia;
}

static void destruir_aresta(void *aresta_generica)
{
    struct aresta_grafo *aresta = aresta_generica;

    if (aresta == NULL) {
        return;
    }

    free(aresta->cep_direita);
    free(aresta->cep_esquerda);
    free(aresta->nome);
    free(aresta);
}

static void destruir_vertice(void *vertice_generico)
{
    struct vertice_grafo *vertice = vertice_generico;

    if (vertice == NULL) {
        return;
    }

    destruir_lista(vertice->arestas_saida, destruir_aresta);
    free(vertice->id);
    free(vertice);
}

static struct vertice_grafo *criar_vertice(const char *id, double x, double y)
{
    struct vertice_grafo *vertice;

    if (id == NULL) {
        return NULL;
    }

    vertice = malloc(sizeof(*vertice));
    if (vertice == NULL) {
        return NULL;
    }

    vertice->id = copiar_texto_grafo(id);
    vertice->arestas_saida = criar_lista();
    if (vertice->id == NULL || vertice->arestas_saida == NULL) {
        destruir_vertice(vertice);
        return NULL;
    }

    vertice->x = x;
    vertice->y = y;

    return vertice;
}

static struct aresta_grafo *criar_aresta(
    struct vertice_grafo *origem,
    struct vertice_grafo *destino,
    const char *cep_direita,
    const char *cep_esquerda,
    double comprimento,
    double velocidade_media,
    const char *nome
)
{
    struct aresta_grafo *aresta;

    if (origem == NULL || destino == NULL || cep_direita == NULL || cep_esquerda == NULL || nome == NULL) {
        return NULL;
    }

    aresta = malloc(sizeof(*aresta));
    if (aresta == NULL) {
        return NULL;
    }

    aresta->origem = origem;
    aresta->destino = destino;
    aresta->cep_direita = copiar_texto_grafo(cep_direita);
    aresta->cep_esquerda = copiar_texto_grafo(cep_esquerda);
    aresta->nome = copiar_texto_grafo(nome);
    aresta->comprimento = comprimento;
    aresta->velocidade_media = velocidade_media;

    if (aresta->cep_direita == NULL || aresta->cep_esquerda == NULL || aresta->nome == NULL) {
        destruir_aresta(aresta);
        return NULL;
    }

    return aresta;
}

Grafo criar_grafo(void)
{
    struct grafo *grafo = malloc(sizeof(*grafo));

    if (grafo == NULL) {
        return NULL;
    }

    grafo->vertices = criar_lista();
    if (grafo->vertices == NULL) {
        free(grafo);
        return NULL;
    }

    grafo->quantidade_arestas = 0;
    return grafo;
}

void destruir_grafo(Grafo grafo_generico)
{
    struct grafo *grafo = grafo_generico;

    if (grafo == NULL) {
        return;
    }

    destruir_lista(grafo->vertices, destruir_vertice);
    free(grafo);
}

int inserir_vertice_grafo(Grafo grafo_generico, const char *id, double x, double y)
{
    struct grafo *grafo = grafo_generico;
    struct vertice_grafo *vertice;

    if (grafo == NULL || id == NULL || buscar_vertice_grafo(grafo, id) != NULL) {
        return 0;
    }

    vertice = criar_vertice(id, x, y);
    if (vertice == NULL) {
        return 0;
    }

    if (!inserir_lista(grafo->vertices, vertice)) {
        destruir_vertice(vertice);
        return 0;
    }

    return 1;
}

int inserir_aresta_grafo(
    Grafo grafo_generico,
    const char *origem,
    const char *destino,
    const char *cep_direita,
    const char *cep_esquerda,
    double comprimento,
    double velocidade_media,
    const char *nome
)
{
    struct grafo *grafo = grafo_generico;
    struct vertice_grafo *vertice_origem;
    struct vertice_grafo *vertice_destino;
    struct aresta_grafo *aresta;

    if (grafo == NULL) {
        return 0;
    }

    vertice_origem = buscar_vertice_grafo(grafo, origem);
    vertice_destino = buscar_vertice_grafo(grafo, destino);
    if (vertice_origem == NULL || vertice_destino == NULL) {
        return 0;
    }

    aresta = criar_aresta(vertice_origem, vertice_destino, cep_direita, cep_esquerda, comprimento, velocidade_media, nome);
    if (aresta == NULL) {
        return 0;
    }

    if (!inserir_lista(vertice_origem->arestas_saida, aresta)) {
        destruir_aresta(aresta);
        return 0;
    }

    grafo->quantidade_arestas++;
    return 1;
}

VerticeGrafo buscar_vertice_grafo(Grafo grafo_generico, const char *id)
{
    struct grafo *grafo = grafo_generico;
    int i;
    int quantidade;

    if (grafo == NULL || id == NULL) {
        return NULL;
    }

    quantidade = obter_tamanho_lista(grafo->vertices);
    for (i = 0; i < quantidade; i++) {
        struct vertice_grafo *vertice = obter_item_lista(grafo->vertices, i);

        if (vertice != NULL && strcmp(vertice->id, id) == 0) {
            return vertice;
        }
    }

    return NULL;
}

VerticeGrafo buscar_vertice_mais_proximo_grafo(Grafo grafo_generico, double x, double y)
{
    struct grafo *grafo = grafo_generico;
    struct vertice_grafo *mais_proximo = NULL;
    double menor_distancia = 0.0;
    int i;
    int quantidade;

    if (grafo == NULL) {
        return NULL;
    }

    quantidade = obter_tamanho_lista(grafo->vertices);
    for (i = 0; i < quantidade; i++) {
        struct vertice_grafo *vertice = obter_item_lista(grafo->vertices, i);
        double dx = vertice->x - x;
        double dy = vertice->y - y;
        double distancia = dx * dx + dy * dy;

        if (mais_proximo == NULL || distancia < menor_distancia) {
            mais_proximo = vertice;
            menor_distancia = distancia;
        }
    }

    return mais_proximo;
}

VerticeGrafo obter_vertice_grafo(Grafo grafo_generico, int indice)
{
    struct grafo *grafo = grafo_generico;

    if (grafo == NULL) {
        return NULL;
    }

    return obter_item_lista(grafo->vertices, indice);
}

ArestaGrafo obter_aresta_vertice_grafo(VerticeGrafo vertice_generico, int indice)
{
    struct vertice_grafo *vertice = vertice_generico;

    if (vertice == NULL) {
        return NULL;
    }

    return obter_item_lista(vertice->arestas_saida, indice);
}

int obter_quantidade_vertices_grafo(Grafo grafo_generico)
{
    struct grafo *grafo = grafo_generico;

    if (grafo == NULL) {
        return 0;
    }

    return obter_tamanho_lista(grafo->vertices);
}

int obter_quantidade_arestas_grafo(Grafo grafo_generico)
{
    struct grafo *grafo = grafo_generico;

    if (grafo == NULL) {
        return 0;
    }

    return grafo->quantidade_arestas;
}

int obter_grau_saida_vertice_grafo(VerticeGrafo vertice_generico)
{
    struct vertice_grafo *vertice = vertice_generico;

    if (vertice == NULL) {
        return 0;
    }

    return obter_tamanho_lista(vertice->arestas_saida);
}

const char *obter_id_vertice_grafo(VerticeGrafo vertice_generico)
{
    struct vertice_grafo *vertice = vertice_generico;

    if (vertice == NULL) {
        return NULL;
    }

    return vertice->id;
}

double obter_x_vertice_grafo(VerticeGrafo vertice_generico)
{
    struct vertice_grafo *vertice = vertice_generico;

    if (vertice == NULL) {
        return 0.0;
    }

    return vertice->x;
}

double obter_y_vertice_grafo(VerticeGrafo vertice_generico)
{
    struct vertice_grafo *vertice = vertice_generico;

    if (vertice == NULL) {
        return 0.0;
    }

    return vertice->y;
}

VerticeGrafo obter_origem_aresta_grafo(ArestaGrafo aresta_generica)
{
    struct aresta_grafo *aresta = aresta_generica;

    if (aresta == NULL) {
        return NULL;
    }

    return aresta->origem;
}

VerticeGrafo obter_destino_aresta_grafo(ArestaGrafo aresta_generica)
{
    struct aresta_grafo *aresta = aresta_generica;

    if (aresta == NULL) {
        return NULL;
    }

    return aresta->destino;
}

const char *obter_cep_direita_aresta_grafo(ArestaGrafo aresta_generica)
{
    struct aresta_grafo *aresta = aresta_generica;

    if (aresta == NULL) {
        return NULL;
    }

    return aresta->cep_direita;
}

const char *obter_cep_esquerda_aresta_grafo(ArestaGrafo aresta_generica)
{
    struct aresta_grafo *aresta = aresta_generica;

    if (aresta == NULL) {
        return NULL;
    }

    return aresta->cep_esquerda;
}

double obter_comprimento_aresta_grafo(ArestaGrafo aresta_generica)
{
    struct aresta_grafo *aresta = aresta_generica;

    if (aresta == NULL) {
        return 0.0;
    }

    return aresta->comprimento;
}

double obter_velocidade_aresta_grafo(ArestaGrafo aresta_generica)
{
    struct aresta_grafo *aresta = aresta_generica;

    if (aresta == NULL) {
        return 0.0;
    }

    return aresta->velocidade_media;
}

const char *obter_nome_aresta_grafo(ArestaGrafo aresta_generica)
{
    struct aresta_grafo *aresta = aresta_generica;

    if (aresta == NULL) {
        return NULL;
    }

    return aresta->nome;
}
