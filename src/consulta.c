#include "consulta.h"

#include "arvore_minima.h"
#include "componentes.h"
#include "dijkstra.h"
#include "percurso.h"
#include "quadra.h"
#include "registrador.h"
#include "saida_svg.h"

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
    int quantidade = obter_quantidade_vertices_dijkstra(resultado);

    if (quantidade <= 1) {
        return escrever_linha_formatada(arquivo, "Origem e destino coincidem.");
    }

    for (i = 0; i < quantidade - 1; i++) {
        VerticeGrafo origem = obter_vertice_dijkstra(resultado, i);
        VerticeGrafo destino = obter_vertice_dijkstra(resultado, i + 1);
        ArestaGrafo aresta = buscar_aresta_entre_vertices_grafo(origem, destino);
        const char *nome_rua = obter_nome_aresta_grafo(aresta);

        if (nome_rua == NULL) {
            nome_rua = "via desconhecida";
        }

        if (fprintf(
            arquivo,
            "Siga pela %s de %s ate %s.\n",
            nome_rua,
            obter_id_vertice_grafo(origem),
            obter_id_vertice_grafo(destino)
        ) < 0) {
            return 0;
        }
    }

    return 1;
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

static int processar_comando_percurso(const char *linha, Grafo grafo, Registradores registradores, FILE *saida, const char *caminho_svg, int *numero_percurso)
{
    char origem[TAMANHO_TEXTO_CONSULTA];
    char destino[TAMANHO_TEXTO_CONSULTA];
    char cor_curto[TAMANHO_TEXTO_CONSULTA];
    char cor_rapido[TAMANHO_TEXTO_CONSULTA];
    char id_curto[TAMANHO_TEXTO_CONSULTA];
    char id_rapido[TAMANHO_TEXTO_CONSULTA];
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
    snprintf(id_curto, sizeof(id_curto), "percurso_curto_%d", *numero_percurso);
    snprintf(id_rapido, sizeof(id_rapido), "percurso_rapido_%d", *numero_percurso);
    (*numero_percurso)++;
    sucesso = sucesso && acrescentar_percurso_svg(caminho_svg, obter_percurso_curto(percurso), cor_curto, id_curto, 1);
    sucesso = sucesso && acrescentar_percurso_svg(caminho_svg, obter_percurso_rapido(percurso), cor_rapido, id_rapido, 0);
    sucesso = sucesso && acrescentar_marcadores_percurso_svg(caminho_svg, obter_percurso_curto(percurso));

    destruir_percurso(percurso);
    return sucesso;
}

static int processar_comando_mvm(const char *linha, Grafo grafo, FILE *saida)
{
    double velocidade_media;
    double x;
    double y;
    double largura;
    double altura;
    int alteradas;

    if (sscanf(linha, "mvm %lf %lf %lf %lf %lf", &velocidade_media, &x, &y, &largura, &altura) != 5) {
        return 1;
    }

    alteradas = atualizar_velocidade_arestas_regiao_grafo(grafo, x, y, largura, altura, velocidade_media);
    return fprintf(
        saida,
        "mvm %.2f %.2f %.2f %.2f %.2f -> %d aresta(s) atualizada(s)\n",
        velocidade_media,
        x,
        y,
        largura,
        altura,
        alteradas
    ) >= 0;
}

static int processar_comando_regs(const char *linha, Grafo grafo, FILE *saida)
{
    double limite_velocidade;
    Componentes componentes;
    int quantidade;

    if (sscanf(linha, "regs %lf", &limite_velocidade) != 1) {
        return 1;
    }

    componentes = calcular_componentes_lentas(grafo, limite_velocidade);
    if (componentes == NULL) {
        return 0;
    }

    quantidade = obter_quantidade_componentes(componentes);
    destruir_componentes(componentes);

    return fprintf(saida, "regs %.2f -> Numero de componentes conexos: %d\n", limite_velocidade, quantidade) >= 0;
}

static int processar_comando_exp(const char *linha, Grafo grafo, FILE *saida)
{
    double limite_velocidade;
    int expandidas;

    if (sscanf(linha, "exp %lf", &limite_velocidade) != 1) {
        return 1;
    }

    expandidas = expandir_infraestrutura_arvore_minima(grafo, limite_velocidade);
    return fprintf(saida, "exp %.2f -> %d aresta(s) expandida(s)\n", limite_velocidade, expandidas) >= 0;
}

int processar_arquivo_consulta(const char *caminho_qry, Cidade cidade, Grafo grafo, const char *caminho_txt, const char *caminho_svg)
{
    FILE *consulta;
    FILE *saida;
    Registradores registradores;
    char linha[TAMANHO_LINHA_CONSULTA];
    int sucesso = 1;
    int numero_percurso = 1;

    if (caminho_qry == NULL || cidade == NULL || grafo == NULL || caminho_txt == NULL || caminho_svg == NULL) {
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
            sucesso = processar_comando_percurso(linha, grafo, registradores, saida, caminho_svg, &numero_percurso);
        } else if (linha[0] == 'm' && linha[1] == 'v' && linha[2] == 'm') {
            sucesso = processar_comando_mvm(linha, grafo, saida);
        } else if (linha[0] == 'r' && linha[1] == 'e' && linha[2] == 'g' && linha[3] == 's') {
            sucesso = processar_comando_regs(linha, grafo, saida);
        } else if (linha[0] == 'e' && linha[1] == 'x' && linha[2] == 'p') {
            sucesso = processar_comando_exp(linha, grafo, saida);
        }
    }

    destruir_registradores(registradores);
    fclose(saida);
    fclose(consulta);
    return sucesso;
}
