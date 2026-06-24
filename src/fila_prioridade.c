#include "fila_prioridade.h"

#include <stdlib.h>

struct item_fila_prioridade {
    void *item;
    double prioridade;
};

struct fila_prioridade {
    struct item_fila_prioridade *itens;
    int tamanho;
    int capacidade;
};

static int garantir_capacidade(FilaPrioridade fila_generica)
{
    struct fila_prioridade *fila = fila_generica;
    struct item_fila_prioridade *novos_itens;
    int nova_capacidade;

    if (fila->tamanho < fila->capacidade) {
        return 1;
    }

    nova_capacidade = fila->capacidade == 0 ? 8 : fila->capacidade * 2;
    novos_itens = realloc(fila->itens, (size_t) nova_capacidade * sizeof(*novos_itens));
    if (novos_itens == NULL) {
        return 0;
    }

    fila->itens = novos_itens;
    fila->capacidade = nova_capacidade;
    return 1;
}

static void trocar_itens(struct item_fila_prioridade *a, struct item_fila_prioridade *b)
{
    struct item_fila_prioridade temporario = *a;

    *a = *b;
    *b = temporario;
}

static void subir_item(struct fila_prioridade *fila, int indice)
{
    while (indice > 0) {
        int pai = (indice - 1) / 2;

        if (fila->itens[pai].prioridade <= fila->itens[indice].prioridade) {
            return;
        }

        trocar_itens(&fila->itens[pai], &fila->itens[indice]);
        indice = pai;
    }
}

static void descer_item(struct fila_prioridade *fila, int indice)
{
    while (1) {
        int esquerda = indice * 2 + 1;
        int direita = indice * 2 + 2;
        int menor = indice;

        if (esquerda < fila->tamanho && fila->itens[esquerda].prioridade < fila->itens[menor].prioridade) {
            menor = esquerda;
        }

        if (direita < fila->tamanho && fila->itens[direita].prioridade < fila->itens[menor].prioridade) {
            menor = direita;
        }

        if (menor == indice) {
            return;
        }

        trocar_itens(&fila->itens[indice], &fila->itens[menor]);
        indice = menor;
    }
}

FilaPrioridade criar_fila_prioridade(void)
{
    struct fila_prioridade *fila = calloc(1, sizeof(*fila));

    return fila;
}

void destruir_fila_prioridade(FilaPrioridade fila_generica)
{
    struct fila_prioridade *fila = fila_generica;

    if (fila == NULL) {
        return;
    }

    free(fila->itens);
    free(fila);
}

int inserir_fila_prioridade(FilaPrioridade fila_generica, void *item, double prioridade)
{
    struct fila_prioridade *fila = fila_generica;

    if (fila == NULL) {
        return 0;
    }

    if (!garantir_capacidade(fila)) {
        return 0;
    }

    fila->itens[fila->tamanho].item = item;
    fila->itens[fila->tamanho].prioridade = prioridade;
    subir_item(fila, fila->tamanho);
    fila->tamanho++;

    return 1;
}

void *remover_minimo_fila_prioridade(FilaPrioridade fila_generica)
{
    struct fila_prioridade *fila = fila_generica;
    void *item;

    if (fila == NULL || fila->tamanho == 0) {
        return NULL;
    }

    item = fila->itens[0].item;
    fila->tamanho--;

    if (fila->tamanho > 0) {
        fila->itens[0] = fila->itens[fila->tamanho];
        descer_item(fila, 0);
    }

    return item;
}

int fila_prioridade_esta_vazia(FilaPrioridade fila_generica)
{
    struct fila_prioridade *fila = fila_generica;

    return fila == NULL || fila->tamanho == 0;
}

int obter_tamanho_fila_prioridade(FilaPrioridade fila_generica)
{
    struct fila_prioridade *fila = fila_generica;

    if (fila == NULL) {
        return 0;
    }

    return fila->tamanho;
}
