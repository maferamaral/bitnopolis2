#include "lista.h"

#include <stdlib.h>

struct no_lista {
    void *item;
    struct no_lista *proximo;
};

struct lista {
    struct no_lista *primeiro;
    struct no_lista *ultimo;
    int tamanho;
};

Lista criar_lista(void)
{
    struct lista *lista = malloc(sizeof(*lista));

    if (lista == NULL) {
        return NULL;
    }

    lista->primeiro = NULL;
    lista->ultimo = NULL;
    lista->tamanho = 0;

    return lista;
}

void destruir_lista(Lista lista_generica, void (*destruir_item)(void *item))
{
    struct lista *lista = lista_generica;
    struct no_lista *atual;

    if (lista == NULL) {
        return;
    }

    atual = lista->primeiro;
    while (atual != NULL) {
        struct no_lista *proximo = atual->proximo;

        if (destruir_item != NULL) {
            destruir_item(atual->item);
        }

        free(atual);
        atual = proximo;
    }

    free(lista);
}

int inserir_lista(Lista lista_generica, void *item)
{
    struct lista *lista = lista_generica;
    struct no_lista *novo;

    if (lista == NULL) {
        return 0;
    }

    novo = malloc(sizeof(*novo));
    if (novo == NULL) {
        return 0;
    }

    novo->item = item;
    novo->proximo = NULL;

    if (lista->ultimo == NULL) {
        lista->primeiro = novo;
    } else {
        lista->ultimo->proximo = novo;
    }

    lista->ultimo = novo;
    lista->tamanho++;

    return 1;
}

void *obter_item_lista(Lista lista_generica, int indice)
{
    struct lista *lista = lista_generica;
    struct no_lista *atual;
    int i;

    if (lista == NULL || indice < 0 || indice >= lista->tamanho) {
        return NULL;
    }

    atual = lista->primeiro;
    for (i = 0; i < indice; i++) {
        atual = atual->proximo;
    }

    return atual->item;
}

int obter_tamanho_lista(Lista lista_generica)
{
    struct lista *lista = lista_generica;

    if (lista == NULL) {
        return 0;
    }

    return lista->tamanho;
}

int lista_esta_vazia(Lista lista_generica)
{
    return obter_tamanho_lista(lista_generica) == 0;
}
