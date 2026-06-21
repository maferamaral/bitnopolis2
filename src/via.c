#include "via.h"

#include <stdio.h>

#define TAMANHO_LINHA_VIA 512
#define TAMANHO_TEXTO_VIA 128

static int linha_e_quantidade_vertices(const char *linha)
{
    int quantidade;
    char resto;

    return sscanf(linha, " %d %c", &quantidade, &resto) == 1;
}

static int ler_vertice_via(const char *linha, Grafo grafo)
{
    char id[TAMANHO_TEXTO_VIA];
    double x;
    double y;

    if (sscanf(linha, "v %127s %lf %lf", id, &x, &y) != 3) {
        return 1;
    }

    return inserir_vertice_grafo(grafo, id, x, y);
}

static int ler_aresta_via(const char *linha, Grafo grafo)
{
    char origem[TAMANHO_TEXTO_VIA];
    char destino[TAMANHO_TEXTO_VIA];
    char cep_direita[TAMANHO_TEXTO_VIA];
    char cep_esquerda[TAMANHO_TEXTO_VIA];
    char nome[TAMANHO_TEXTO_VIA];
    double comprimento;
    double velocidade_media;

    if (sscanf(
        linha,
        "e %127s %127s %127s %127s %lf %lf %127s",
        origem,
        destino,
        cep_direita,
        cep_esquerda,
        &comprimento,
        &velocidade_media,
        nome
    ) != 7) {
        return 1;
    }

    return inserir_aresta_grafo(
        grafo,
        origem,
        destino,
        cep_direita,
        cep_esquerda,
        comprimento,
        velocidade_media,
        nome
    );
}

Grafo ler_arquivo_via(const char *caminho_via)
{
    FILE *arquivo;
    Grafo grafo;
    char linha[TAMANHO_LINHA_VIA];

    if (caminho_via == NULL) {
        return NULL;
    }

    arquivo = fopen(caminho_via, "r");
    if (arquivo == NULL) {
        return NULL;
    }

    grafo = criar_grafo();
    if (grafo == NULL) {
        fclose(arquivo);
        return NULL;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha_e_quantidade_vertices(linha)) {
            continue;
        }

        if (linha[0] == 'v') {
            if (!ler_vertice_via(linha, grafo)) {
                destruir_grafo(grafo);
                fclose(arquivo);
                return NULL;
            }
        } else if (linha[0] == 'e') {
            if (!ler_aresta_via(linha, grafo)) {
                destruir_grafo(grafo);
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    return grafo;
}
