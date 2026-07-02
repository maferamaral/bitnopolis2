#include "argumentos.h"
#include "consulta.h"
#include "geo.h"
#include "grafo.h"
#include "saida_svg.h"
#include "saida_texto.h"
#include "via.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *copiar_texto_main(const char *texto)
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

static int caminho_absoluto(const char *caminho)
{
    return caminho != NULL && caminho[0] == '/';
}

static char *montar_caminho_entrada(const char *diretorio, const char *arquivo)
{
    char *caminho;
    size_t tamanho;
    int precisa_barra;

    if (arquivo == NULL) {
        return NULL;
    }

    if (diretorio == NULL || caminho_absoluto(arquivo)) {
        return copiar_texto_main(arquivo);
    }

    precisa_barra = diretorio[strlen(diretorio) - 1] != '/';
    tamanho = strlen(diretorio) + strlen(arquivo) + (precisa_barra ? 2 : 1);
    caminho = malloc(tamanho);
    if (caminho == NULL) {
        return NULL;
    }

    snprintf(caminho, tamanho, "%s%s%s", diretorio, precisa_barra ? "/" : "", arquivo);
    return caminho;
}

static char *obter_nome_base(const char *caminho)
{
    const char *inicio;
    const char *ponto;
    char *base;
    size_t tamanho;

    if (caminho == NULL) {
        return NULL;
    }

    inicio = strrchr(caminho, '/');
    inicio = inicio == NULL ? caminho : inicio + 1;
    ponto = strrchr(inicio, '.');
    tamanho = ponto == NULL ? strlen(inicio) : (size_t) (ponto - inicio);

    base = malloc(tamanho + 1);
    if (base == NULL) {
        return NULL;
    }

    memcpy(base, inicio, tamanho);
    base[tamanho] = '\0';
    return base;
}

static char *montar_caminho_saida(const char *diretorio, const char *base, const char *extensao)
{
    char *caminho;
    size_t tamanho;
    int precisa_barra;

    if (diretorio == NULL || base == NULL || extensao == NULL) {
        return NULL;
    }

    precisa_barra = diretorio[strlen(diretorio) - 1] != '/';
    tamanho = strlen(diretorio) + strlen(base) + strlen(extensao) + (precisa_barra ? 3 : 2);
    caminho = malloc(tamanho);
    if (caminho == NULL) {
        return NULL;
    }

    snprintf(caminho, tamanho, "%s%s%s.%s", diretorio, precisa_barra ? "/" : "", base, extensao);
    return caminho;
}

static void liberar_recursos(
    Argumentos argumentos,
    Cidade cidade,
    Grafo grafo,
    char *caminho_geo,
    char *caminho_via,
    char *caminho_qry,
    char *base_saida,
    char *caminho_txt,
    char *caminho_svg
)
{
    destruir_grafo(grafo);
    destruir_cidade(cidade);
    destruir_argumentos(argumentos);
    free(caminho_geo);
    free(caminho_via);
    free(caminho_qry);
    free(base_saida);
    free(caminho_txt);
    free(caminho_svg);
}

int main(int argc, char **argv)
{
    Argumentos argumentos = criar_argumentos();
    Cidade cidade = NULL;
    Grafo grafo = NULL;
    char *caminho_geo = NULL;
    char *caminho_via = NULL;
    char *caminho_qry = NULL;
    char *base_saida = NULL;
    char *caminho_txt = NULL;
    char *caminho_svg = NULL;
    const char *diretorio_entrada;

    if (argumentos == NULL) {
        fprintf(stderr, "Erro: nao foi possivel iniciar os argumentos.\n");
        return 1;
    }

    if (!ler_argumentos(argumentos, argc, argv)) {
        fprintf(stderr, "Uso: ted [-e path] -f arq.geo [-q consulta.qry] [-v arqvias.via] -o dir\n");
        destruir_argumentos(argumentos);
        return 1;
    }

    diretorio_entrada = obter_diretorio_entrada(argumentos);
    caminho_geo = montar_caminho_entrada(diretorio_entrada, obter_arquivo_geo(argumentos));
    base_saida = obter_nome_base(obter_arquivo_geo(argumentos));
    caminho_txt = montar_caminho_saida(obter_diretorio_saida(argumentos), base_saida, "txt");
    caminho_svg = montar_caminho_saida(obter_diretorio_saida(argumentos), base_saida, "svg");

    if (caminho_geo == NULL || base_saida == NULL || caminho_txt == NULL || caminho_svg == NULL) {
        fprintf(stderr, "Erro: nao foi possivel montar os caminhos de entrada/saida.\n");
        liberar_recursos(argumentos, cidade, grafo, caminho_geo, caminho_via, caminho_qry, base_saida, caminho_txt, caminho_svg);
        return 1;
    }

    cidade = ler_arquivo_geo(caminho_geo);
    if (cidade == NULL) {
        fprintf(stderr, "Erro: nao foi possivel ler o arquivo GEO: %s\n", caminho_geo);
        liberar_recursos(argumentos, cidade, grafo, caminho_geo, caminho_via, caminho_qry, base_saida, caminho_txt, caminho_svg);
        return 1;
    }

    if (argumentos_possuem_vias(argumentos)) {
        caminho_via = montar_caminho_entrada(diretorio_entrada, obter_arquivo_vias(argumentos));
        grafo = ler_arquivo_via(caminho_via);
        if (grafo == NULL) {
            fprintf(stderr, "Erro: nao foi possivel ler o arquivo VIA: %s\n", caminho_via);
            liberar_recursos(argumentos, cidade, grafo, caminho_geo, caminho_via, caminho_qry, base_saida, caminho_txt, caminho_svg);
            return 1;
        }
    } else {
        grafo = criar_grafo();
        if (grafo == NULL) {
            fprintf(stderr, "Erro: nao foi possivel criar o grafo viario.\n");
            liberar_recursos(argumentos, cidade, grafo, caminho_geo, caminho_via, caminho_qry, base_saida, caminho_txt, caminho_svg);
            return 1;
        }
    }

    if (!iniciar_cidade_svg(caminho_svg, cidade)) {
        fprintf(stderr, "Erro: nao foi possivel gerar o SVG: %s\n", caminho_svg);
        liberar_recursos(argumentos, cidade, grafo, caminho_geo, caminho_via, caminho_qry, base_saida, caminho_txt, caminho_svg);
        return 1;
    }

    if (argumentos_possuem_consulta(argumentos)) {
        caminho_qry = montar_caminho_entrada(diretorio_entrada, obter_arquivo_consulta(argumentos));
        if (caminho_qry == NULL || !processar_arquivo_consulta(caminho_qry, cidade, grafo, caminho_txt, caminho_svg)) {
            fprintf(stderr, "Erro: nao foi possivel processar o arquivo QRY: %s\n", caminho_qry == NULL ? "(nulo)" : caminho_qry);
            liberar_recursos(argumentos, cidade, grafo, caminho_geo, caminho_via, caminho_qry, base_saida, caminho_txt, caminho_svg);
            return 1;
        }
    } else if (!escrever_relatorio_cidade_texto(caminho_txt, cidade)) {
        fprintf(stderr, "Erro: nao foi possivel gerar o TXT: %s\n", caminho_txt);
        liberar_recursos(argumentos, cidade, grafo, caminho_geo, caminho_via, caminho_qry, base_saida, caminho_txt, caminho_svg);
        return 1;
    }

    if (!finalizar_svg(caminho_svg)) {
        fprintf(stderr, "Erro: nao foi possivel finalizar o SVG: %s\n", caminho_svg);
        liberar_recursos(argumentos, cidade, grafo, caminho_geo, caminho_via, caminho_qry, base_saida, caminho_txt, caminho_svg);
        return 1;
    }

    liberar_recursos(argumentos, cidade, grafo, caminho_geo, caminho_via, caminho_qry, base_saida, caminho_txt, caminho_svg);
    return 0;
}
