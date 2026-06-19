#include "cidade.h"

#include "lista.h"

#include <string.h>
#include <stdlib.h>

struct cidade {
    Lista quadras;
};

static void destruir_item_quadra(void *quadra)
{
    destruir_quadra(quadra);
}

Cidade criar_cidade(void)
{
    struct cidade *cidade = malloc(sizeof(*cidade));

    if (cidade == NULL) {
        return NULL;
    }

    cidade->quadras = criar_lista();
    if (cidade->quadras == NULL) {
        free(cidade);
        return NULL;
    }

    return cidade;
}

void destruir_cidade(Cidade cidade_generica)
{
    struct cidade *cidade = cidade_generica;

    if (cidade == NULL) {
        return;
    }

    destruir_lista(cidade->quadras, destruir_item_quadra);
    free(cidade);
}

int inserir_quadra_cidade(Cidade cidade_generica, Quadra quadra)
{
    struct cidade *cidade = cidade_generica;

    if (cidade == NULL || quadra == NULL) {
        return 0;
    }

    return inserir_lista(cidade->quadras, quadra);
}

Quadra buscar_quadra_cidade(Cidade cidade_generica, const char *cep)
{
    struct cidade *cidade = cidade_generica;
    int i;
    int quantidade;

    if (cidade == NULL || cep == NULL) {
        return NULL;
    }

    quantidade = obter_tamanho_lista(cidade->quadras);
    for (i = 0; i < quantidade; i++) {
        Quadra quadra = obter_item_lista(cidade->quadras, i);
        const char *cep_quadra = obter_cep_quadra(quadra);

        if (cep_quadra != NULL && strcmp(cep_quadra, cep) == 0) {
            return quadra;
        }
    }

    return NULL;
}

Quadra obter_quadra_cidade(Cidade cidade_generica, int indice)
{
    struct cidade *cidade = cidade_generica;

    if (cidade == NULL) {
        return NULL;
    }

    return obter_item_lista(cidade->quadras, indice);
}

int obter_quantidade_quadras_cidade(Cidade cidade_generica)
{
    struct cidade *cidade = cidade_generica;

    if (cidade == NULL) {
        return 0;
    }

    return obter_tamanho_lista(cidade->quadras);
}
