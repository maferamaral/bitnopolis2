#include "consulta.h"

#include "dijkstra.h"
#include "percurso.h"
#include "quadra.h"
#include "registrador.h"

#include <stdio.h>

#define TAMANHO_LINHA_CONSULTA 512
#define TAMANHO_TEXTO_CONSULTA 128

static int escrever_linha_formatada(FILE *arquivo, const char *linha)
{
    return fprintf(arquivo, "%s\n", linha) >= 0;
}

static int obter_indice_registrador(const char *texto)
{
    int indice;
    char resto;

    if (texto == NULL) {
        return -1;
    }

    if (sscanf(texto, "R%d%c", &indice, &resto) == 1 && indice >= 0 && indice < QUANTIDADE_REGISTRADORES) {
        return indice;
    }

    return -1;
}

static int escrever_caminho_dijkstra(FILE *arquivo, ResultadoDijkstra resultado)
{
    int i;

    for (i = 0; i < obter_quantidade_vertices_dijkstra(resultado); i++) {
        VerticeGrafo vertice = obter_vertice_dijkstra(resultado, i);

        if (fprintf(arquivo, "%s%s", i == 0 ? "" : " -> ", obter_id_vertice_grafo(vertice)) < 0) {
            return 0;
        }
    }

    return fprintf(arquivo, "\n") >= 0;
}

static int escrever_resultado_percurso(FILE *arquivo, const char *titulo, ResultadoDijkstra resultado)
{
    if (!caminho_dijkstra_existe(resultado)) {
        return escrever_linha_formatada(arquivo, "Destino inacessivel");
    }

    if (fprintf(arquivo, "%s: custo %.2f\n", titulo, obter_custo_dijkstra(resultado)) < 0) {
        return 0;
    }

    return escrever_caminho_dijkstra(arquivo, resultado);
}

static int processar_comando_origem(const char *linha, Cidade cidade, Registradores registradores, FILE *saida)
{
    char registrador[TAMANHO_TEXTO_CONSULTA];
    char cep[TAMANHO_TEXTO_CONSULTA];
    char face;
    double numero;
    int indice;
    Quadra quadra;

    if (sscanf(linha, "@o? %127s %127s %c %lf", registrador, cep, &face, &numero) != 4) {
        return 1;
    }

    indice = obter_indice_registrador(registrador);
    quadra = buscar_quadra_cidade(cidade, cep);

    if (indice < 0 || quadra == NULL || !definir_registrador_endereco(registradores, indice, quadra, face, numero)) {
        return fprintf(saida, "@o? %s %s %c %.2f -> endereco invalido\n", registrador, cep, face, numero) >= 0;
    }

    return fprintf(
        saida,
        "@o? %s %s %c %.2f -> (%.2f, %.2f)\n",
        registrador,
        cep,
        face,
        numero,
        obter_x_registrador(registradores, indice),
        obter_y_registrador(registradores, indice)
    ) >= 0;
}

static int processar_comando_percurso(const char *linha, Grafo grafo, Registradores registradores, FILE *saida)
{
    char origem[TAMANHO_TEXTO_CONSULTA];
    char destino[TAMANHO_TEXTO_CONSULTA];
    char cor_curto[TAMANHO_TEXTO_CONSULTA];
    char cor_rapido[TAMANHO_TEXTO_CONSULTA];
    int indice_origem;
    int indice_destino;
    Percurso percurso;
    int sucesso = 1;

    if (sscanf(linha, "p? %127s %127s %127s %127s", origem, destino, cor_curto, cor_rapido) != 4) {
        return 1;
    }

    indice_origem = obter_indice_registrador(origem);
    indice_destino = obter_indice_registrador(destino);

    fprintf(saida, "p? %s %s\n", origem, destino);

    if (indice_origem < 0 || indice_destino < 0) {
        return escrever_linha_formatada(saida, "Destino inacessivel");
    }

    percurso = calcular_percurso_registradores(grafo, registradores, indice_origem, indice_destino);
    if (percurso == NULL || !percurso_existe(percurso)) {
        destruir_percurso(percurso);
        return escrever_linha_formatada(saida, "Destino inacessivel");
    }

    sucesso = sucesso && escrever_resultado_percurso(saida, "Percurso mais curto", obter_percurso_curto(percurso));
    sucesso = sucesso && escrever_resultado_percurso(saida, "Percurso mais rapido", obter_percurso_rapido(percurso));

    destruir_percurso(percurso);
    return sucesso;
}

int processar_arquivo_consulta(const char *caminho_qry, Cidade cidade, Grafo grafo, const char *caminho_txt)
{
    FILE *consulta;
    FILE *saida;
    Registradores registradores;
    char linha[TAMANHO_LINHA_CONSULTA];
    int sucesso = 1;

    if (caminho_qry == NULL || cidade == NULL || grafo == NULL || caminho_txt == NULL) {
        return 0;
    }

    consulta = fopen(caminho_qry, "r");
    if (consulta == NULL) {
        return 0;
    }

    saida = fopen(caminho_txt, "w");
    if (saida == NULL) {
        fclose(consulta);
        return 0;
    }

    registradores = criar_registradores();
    if (registradores == NULL) {
        fclose(saida);
        fclose(consulta);
        return 0;
    }

    while (fgets(linha, sizeof(linha), consulta) != NULL && sucesso) {
        if (linha[0] == '@' && linha[1] == 'o' && linha[2] == '?') {
            sucesso = processar_comando_origem(linha, cidade, registradores, saida);
        } else if (linha[0] == 'p' && linha[1] == '?') {
            sucesso = processar_comando_percurso(linha, grafo, registradores, saida);
        }
    }

    destruir_registradores(registradores);
    fclose(saida);
    fclose(consulta);
    return sucesso;
}
