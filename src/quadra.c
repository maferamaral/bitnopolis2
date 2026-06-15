#include "quadra.h"

#include <stdlib.h>
#include <string.h>

struct quadra {
    char *cep;
    double x;
    double y;
    double largura;
    double altura;
    double espessura;
    char *preenchimento;
    char *borda;
};

static char *copiar_texto(const char *texto)
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

static int definir_texto(char **destino, const char *texto)
{
    char *copia = copiar_texto(texto);

    if (copia == NULL) {
        return 0;
    }

    free(*destino);
    *destino = copia;
    return 1;
}

Quadra criar_quadra(const char *cep, double x, double y, double largura, double altura)
{
    struct quadra *quadra;

    if (cep == NULL) {
        return NULL;
    }

    quadra = malloc(sizeof(*quadra));
    if (quadra == NULL) {
        return NULL;
    }

    quadra->cep = copiar_texto(cep);
    quadra->preenchimento = copiar_texto("none");
    quadra->borda = copiar_texto("black");

    if (quadra->cep == NULL || quadra->preenchimento == NULL || quadra->borda == NULL) {
        destruir_quadra(quadra);
        return NULL;
    }

    quadra->x = x;
    quadra->y = y;
    quadra->largura = largura;
    quadra->altura = altura;
    quadra->espessura = 1.0;

    return quadra;
}

void destruir_quadra(Quadra quadra_generica)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL) {
        return;
    }

    free(quadra->cep);
    free(quadra->preenchimento);
    free(quadra->borda);
    free(quadra);
}

void definir_estilo_quadra(Quadra quadra_generica, double espessura, const char *preenchimento, const char *borda)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL || preenchimento == NULL || borda == NULL) {
        return;
    }

    if (!definir_texto(&quadra->preenchimento, preenchimento)) {
        return;
    }

    if (!definir_texto(&quadra->borda, borda)) {
        return;
    }

    quadra->espessura = espessura;
}

const char *obter_cep_quadra(Quadra quadra_generica)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL) {
        return NULL;
    }

    return quadra->cep;
}

double obter_x_quadra(Quadra quadra_generica)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL) {
        return 0.0;
    }

    return quadra->x;
}

double obter_y_quadra(Quadra quadra_generica)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL) {
        return 0.0;
    }

    return quadra->y;
}

double obter_largura_quadra(Quadra quadra_generica)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL) {
        return 0.0;
    }

    return quadra->largura;
}

double obter_altura_quadra(Quadra quadra_generica)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL) {
        return 0.0;
    }

    return quadra->altura;
}

double obter_espessura_quadra(Quadra quadra_generica)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL) {
        return 0.0;
    }

    return quadra->espessura;
}

const char *obter_preenchimento_quadra(Quadra quadra_generica)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL) {
        return NULL;
    }

    return quadra->preenchimento;
}

const char *obter_borda_quadra(Quadra quadra_generica)
{
    struct quadra *quadra = quadra_generica;

    if (quadra == NULL) {
        return NULL;
    }

    return quadra->borda;
}
