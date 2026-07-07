#include "geo.h"

#include "quadra.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TAMANHO_LINHA 512
#define TAMANHO_TEXTO 128

struct estilo_geo {
    double espessura;
    char preenchimento[TAMANHO_TEXTO];
    char borda[TAMANHO_TEXTO];
};

static void iniciar_estilo(struct estilo_geo *estilo)
{
    estilo->espessura = 1.0;
    snprintf(estilo->preenchimento, sizeof(estilo->preenchimento), "%s", "none");
    snprintf(estilo->borda, sizeof(estilo->borda), "%s", "black");
}

static int ler_comando_cq(const char *linha, struct estilo_geo *estilo)
{
    double espessura;
    char texto_espessura[TAMANHO_TEXTO];
    char *fim_numero;
    char preenchimento[TAMANHO_TEXTO];
    char borda[TAMANHO_TEXTO];

    if (sscanf(linha, "cq %127s %127s %127s", texto_espessura, preenchimento, borda) != 3) {
        return 0;
    }

    espessura = strtod(texto_espessura, &fim_numero);
    if (fim_numero == texto_espessura || (*fim_numero != '\0' && strcmp(fim_numero, "px") != 0)) {
        return 0;
    }

    estilo->espessura = espessura;
    snprintf(estilo->preenchimento, sizeof(estilo->preenchimento), "%s", preenchimento);
    snprintf(estilo->borda, sizeof(estilo->borda), "%s", borda);
    return 1;
}

static int ler_comando_q(const char *linha, Cidade cidade, const struct estilo_geo *estilo)
{
    char cep[TAMANHO_TEXTO];
    double x;
    double y;
    double largura;
    double altura;
    Quadra quadra;

    if (sscanf(linha, "q %127s %lf %lf %lf %lf", cep, &x, &y, &largura, &altura) != 5) {
        return 1;
    }

    quadra = criar_quadra(cep, x, y, largura, altura);
    if (quadra == NULL) {
        return 0;
    }

    definir_estilo_quadra(quadra, estilo->espessura, estilo->preenchimento, estilo->borda);
    if (!inserir_quadra_cidade(cidade, quadra)) {
        destruir_quadra(quadra);
        return 0;
    }

    return 1;
}

Cidade ler_arquivo_geo(const char *caminho_geo)
{
    FILE *arquivo;
    Cidade cidade;
    char linha[TAMANHO_LINHA];
    struct estilo_geo estilo;

    if (caminho_geo == NULL) {
        return NULL;
    }

    arquivo = fopen(caminho_geo, "r");
    if (arquivo == NULL) {
        return NULL;
    }

    cidade = criar_cidade();
    if (cidade == NULL) {
        fclose(arquivo);
        return NULL;
    }

    iniciar_estilo(&estilo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] == 'c' && linha[1] == 'q') {
            ler_comando_cq(linha, &estilo);
        } else if (linha[0] == 'q') {
            if (!ler_comando_q(linha, cidade, &estilo)) {
                destruir_cidade(cidade);
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    return cidade;
}
