#include "saida_svg.h"

#include "quadra.h"

#include <stdio.h>

static void calcular_limites_cidade(Cidade cidade, double *min_x, double *min_y, double *max_x, double *max_y)
{
    int i;
    int quantidade = obter_quantidade_quadras_cidade(cidade);

    *min_x = 0.0;
    *min_y = 0.0;
    *max_x = 100.0;
    *max_y = 100.0;

    if (quantidade <= 0) {
        return;
    }

    for (i = 0; i < quantidade; i++) {
        Quadra quadra = obter_quadra_cidade(cidade, i);
        double oeste = obter_x_quadra(quadra) - obter_largura_quadra(quadra);
        double leste = obter_x_quadra(quadra);
        double sul = obter_y_quadra(quadra);
        double norte = obter_y_quadra(quadra) + obter_altura_quadra(quadra);

        if (i == 0 || oeste < *min_x) {
            *min_x = oeste;
        }
        if (i == 0 || sul < *min_y) {
            *min_y = sul;
        }
        if (i == 0 || leste > *max_x) {
            *max_x = leste;
        }
        if (i == 0 || norte > *max_y) {
            *max_y = norte;
        }
    }
}

static void escrever_quadra_svg(FILE *arquivo, Quadra quadra)
{
    double x = obter_x_quadra(quadra) - obter_largura_quadra(quadra);
    double y = obter_y_quadra(quadra);

    fprintf(
        arquivo,
        "  <rect id=\"%s\" x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%.2f\" />\n",
        obter_cep_quadra(quadra),
        x,
        y,
        obter_largura_quadra(quadra),
        obter_altura_quadra(quadra),
        obter_preenchimento_quadra(quadra),
        obter_borda_quadra(quadra),
        obter_espessura_quadra(quadra)
    );
}

int iniciar_cidade_svg(const char *caminho_svg, Cidade cidade)
{
    FILE *arquivo;
    int i;
    int quantidade;
    double min_x;
    double min_y;
    double max_x;
    double max_y;
    double largura;
    double altura;

    if (caminho_svg == NULL || cidade == NULL) {
        return 0;
    }

    arquivo = fopen(caminho_svg, "w");
    if (arquivo == NULL) {
        return 0;
    }

    calcular_limites_cidade(cidade, &min_x, &min_y, &max_x, &max_y);
    largura = max_x - min_x;
    altura = max_y - min_y;

    fprintf(arquivo, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(
        arquivo,
        "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"%.2f %.2f %.2f %.2f\">\n",
        min_x - 10.0,
        min_y - 10.0,
        largura + 20.0,
        altura + 20.0
    );

    quantidade = obter_quantidade_quadras_cidade(cidade);
    for (i = 0; i < quantidade; i++) {
        escrever_quadra_svg(arquivo, obter_quadra_cidade(cidade, i));
    }

    fclose(arquivo);
    return 1;
}

int finalizar_svg(const char *caminho_svg)
{
    FILE *arquivo;

    if (caminho_svg == NULL) {
        return 0;
    }

    arquivo = fopen(caminho_svg, "a");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(arquivo, "</svg>\n");
    fclose(arquivo);
    return 1;
}

int escrever_cidade_svg(const char *caminho_svg, Cidade cidade)
{
    return iniciar_cidade_svg(caminho_svg, cidade) && finalizar_svg(caminho_svg);
}

int acrescentar_percurso_svg(const char *caminho_svg, ResultadoDijkstra resultado, const char *cor, const char *id, int animar)
{
    FILE *arquivo;
    int i;
    int quantidade;

    if (caminho_svg == NULL || resultado == NULL || id == NULL) {
        return 0;
    }

    if (!caminho_dijkstra_existe(resultado)) {
        return 1;
    }

    quantidade = obter_quantidade_vertices_dijkstra(resultado);
    if (quantidade <= 1) {
        return 1;
    }

    arquivo = fopen(caminho_svg, "a");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(
        arquivo,
        "  <path id=\"%s\" d=\"",
        id
    );

    for (i = 0; i < quantidade; i++) {
        VerticeGrafo vertice = obter_vertice_dijkstra(resultado, i);
        fprintf(
            arquivo,
            "%c %.2f %.2f ",
            i == 0 ? 'M' : 'L',
            obter_x_vertice_grafo(vertice),
            obter_y_vertice_grafo(vertice)
        );
    }

    fprintf(
        arquivo,
        "\" fill=\"none\" stroke=\"%s\" stroke-width=\"3.00\" stroke-linecap=\"round\" stroke-linejoin=\"round\" />\n",
        cor == NULL ? "black" : cor
    );

    if (animar) {
        fprintf(
            arquivo,
            "  <circle r=\"4.00\" fill=\"%s\"><animateMotion dur=\"6s\" repeatCount=\"indefinite\"><mpath href=\"#%s\" /></animateMotion></circle>\n",
            cor == NULL ? "black" : cor,
            id
        );
    }

    fclose(arquivo);
    return 1;
}

int acrescentar_marcadores_percurso_svg(const char *caminho_svg, ResultadoDijkstra resultado)
{
    FILE *arquivo;
    VerticeGrafo inicio;
    VerticeGrafo fim;
    int quantidade;

    if (caminho_svg == NULL || resultado == NULL) {
        return 0;
    }

    if (!caminho_dijkstra_existe(resultado)) {
        return 1;
    }

    quantidade = obter_quantidade_vertices_dijkstra(resultado);
    if (quantidade <= 0) {
        return 1;
    }

    inicio = obter_vertice_dijkstra(resultado, 0);
    fim = obter_vertice_dijkstra(resultado, quantidade - 1);

    arquivo = fopen(caminho_svg, "a");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(
        arquivo,
        "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"7.00\" fill=\"white\" stroke=\"black\" stroke-width=\"1.50\" />\n",
        obter_x_vertice_grafo(inicio),
        obter_y_vertice_grafo(inicio)
    );
    fprintf(
        arquivo,
        "  <text x=\"%.2f\" y=\"%.2f\" text-anchor=\"middle\" dominant-baseline=\"central\" font-size=\"8\" fill=\"black\">I</text>\n",
        obter_x_vertice_grafo(inicio),
        obter_y_vertice_grafo(inicio)
    );
    fprintf(
        arquivo,
        "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"7.00\" fill=\"white\" stroke=\"black\" stroke-width=\"1.50\" />\n",
        obter_x_vertice_grafo(fim),
        obter_y_vertice_grafo(fim)
    );
    fprintf(
        arquivo,
        "  <text x=\"%.2f\" y=\"%.2f\" text-anchor=\"middle\" dominant-baseline=\"central\" font-size=\"8\" fill=\"black\">F</text>\n",
        obter_x_vertice_grafo(fim),
        obter_y_vertice_grafo(fim)
    );

    fclose(arquivo);
    return 1;
}
