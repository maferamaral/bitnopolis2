#include "saida_svg.h"

#include "quadra.h"

#include <stdio.h>
#include <stdlib.h>

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

static void incluir_ponto_limites(double x, double y, double *min_x, double *min_y, double *max_x, double *max_y, int *possui_ponto)
{
    if (!*possui_ponto || x < *min_x) {
        *min_x = x;
    }
    if (!*possui_ponto || y < *min_y) {
        *min_y = y;
    }
    if (!*possui_ponto || x > *max_x) {
        *max_x = x;
    }
    if (!*possui_ponto || y > *max_y) {
        *max_y = y;
    }

    *possui_ponto = 1;
}

static void calcular_limites_mapa(Cidade cidade, Grafo grafo, double *min_x, double *min_y, double *max_x, double *max_y)
{
    int i;
    int quantidade_vertices;
    int possui_ponto = 0;

    calcular_limites_cidade(cidade, min_x, min_y, max_x, max_y);
    possui_ponto = obter_quantidade_quadras_cidade(cidade) > 0;

    if (grafo == NULL) {
        return;
    }

    quantidade_vertices = obter_quantidade_vertices_grafo(grafo);
    for (i = 0; i < quantidade_vertices; i++) {
        VerticeGrafo vertice = obter_vertice_grafo(grafo, i);

        incluir_ponto_limites(
            obter_x_vertice_grafo(vertice),
            obter_y_vertice_grafo(vertice),
            min_x,
            min_y,
            max_x,
            max_y,
            &possui_ponto
        );
    }

    if (!possui_ponto) {
        *min_x = 0.0;
        *min_y = 0.0;
        *max_x = 100.0;
        *max_y = 100.0;
    }
}

static int obter_topo_svg(const char *caminho_svg, double *topo)
{
    FILE *arquivo;
    char linha[512];
    double min_x;
    double min_y;
    double largura;
    double altura;

    if (caminho_svg == NULL || topo == NULL) {
        return 0;
    }

    arquivo = fopen(caminho_svg, "r");
    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (sscanf(linha, "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"%lf %lf %lf %lf\"", &min_x, &min_y, &largura, &altura) == 4) {
            *topo = min_y;
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
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

static void escrever_grafo_svg(FILE *arquivo, Grafo grafo)
{
    int i;
    int j;
    int quantidade_vertices;

    if (grafo == NULL) {
        return;
    }

    quantidade_vertices = obter_quantidade_vertices_grafo(grafo);
    fprintf(arquivo, "  <g id=\"vias\" fill=\"none\" stroke=\"#666666\" stroke-width=\"1.50\" stroke-linecap=\"round\">\n");
    for (i = 0; i < quantidade_vertices; i++) {
        VerticeGrafo origem = obter_vertice_grafo(grafo, i);
        int quantidade_arestas = obter_grau_saida_vertice_grafo(origem);

        for (j = 0; j < quantidade_arestas; j++) {
            ArestaGrafo aresta = obter_aresta_vertice_grafo(origem, j);
            VerticeGrafo destino = obter_destino_aresta_grafo(aresta);

            fprintf(
                arquivo,
                "    <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" />\n",
                obter_x_vertice_grafo(origem),
                obter_y_vertice_grafo(origem),
                obter_x_vertice_grafo(destino),
                obter_y_vertice_grafo(destino)
            );
        }
    }
    fprintf(arquivo, "  </g>\n");

    fprintf(arquivo, "  <g id=\"cruzamentos\" fill=\"#333333\" stroke=\"none\">\n");
    for (i = 0; i < quantidade_vertices; i++) {
        VerticeGrafo vertice = obter_vertice_grafo(grafo, i);

        fprintf(
            arquivo,
            "    <circle cx=\"%.2f\" cy=\"%.2f\" r=\"1.80\" />\n",
            obter_x_vertice_grafo(vertice),
            obter_y_vertice_grafo(vertice)
        );
    }
    fprintf(arquivo, "  </g>\n");
}

int iniciar_mapa_svg(const char *caminho_svg, Cidade cidade, Grafo grafo)
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

    calcular_limites_mapa(cidade, grafo, &min_x, &min_y, &max_x, &max_y);
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
    escrever_grafo_svg(arquivo, grafo);

    fclose(arquivo);
    return 1;
}

int iniciar_cidade_svg(const char *caminho_svg, Cidade cidade)
{
    return iniciar_mapa_svg(caminho_svg, cidade, NULL);
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

int acrescentar_registrador_svg(const char *caminho_svg, int indice, double x, double y)
{
    FILE *arquivo;
    double topo;

    if (caminho_svg == NULL || indice < 0) {
        return 0;
    }

    if (!obter_topo_svg(caminho_svg, &topo)) {
        topo = y - 20.0;
    }

    arquivo = fopen(caminho_svg, "a");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(arquivo, "  <g id=\"registrador_R%d\">\n", indice);
    fprintf(
        arquivo,
        "    <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"red\" stroke-width=\"1.50\" stroke-dasharray=\"4 3\" />\n",
        x,
        y,
        x,
        topo
    );
    fprintf(
        arquivo,
        "    <text x=\"%.2f\" y=\"%.2f\" text-anchor=\"middle\" font-size=\"8\" fill=\"red\">R%d</text>\n",
        x,
        topo + 8.0,
        indice
    );
    fprintf(arquivo, "  </g>\n");

    fclose(arquivo);
    return 1;
}

int acrescentar_componentes_svg(const char *caminho_svg, Grafo grafo, Componentes componentes)
{
    static const char *cores[] = {
        "#ff6b6b",
        "#4dabf7",
        "#69db7c",
        "#ffd43b",
        "#b197fc",
        "#66d9e8"
    };
    FILE *arquivo;
    double *min_x;
    double *min_y;
    double *max_x;
    double *max_y;
    int *possui_vertice;
    int quantidade_componentes;
    int quantidade_vertices;
    int i;

    if (caminho_svg == NULL || grafo == NULL || componentes == NULL) {
        return 0;
    }

    quantidade_componentes = obter_quantidade_componentes(componentes);
    quantidade_vertices = obter_quantidade_vertices_grafo(grafo);
    if (quantidade_componentes <= 0) {
        return 1;
    }

    min_x = malloc((size_t) quantidade_componentes * sizeof(*min_x));
    min_y = malloc((size_t) quantidade_componentes * sizeof(*min_y));
    max_x = malloc((size_t) quantidade_componentes * sizeof(*max_x));
    max_y = malloc((size_t) quantidade_componentes * sizeof(*max_y));
    possui_vertice = calloc((size_t) quantidade_componentes, sizeof(*possui_vertice));

    if (min_x == NULL || min_y == NULL || max_x == NULL || max_y == NULL || possui_vertice == NULL) {
        free(min_x);
        free(min_y);
        free(max_x);
        free(max_y);
        free(possui_vertice);
        return 0;
    }

    for (i = 0; i < quantidade_vertices; i++) {
        VerticeGrafo vertice = obter_vertice_grafo(grafo, i);
        int componente = obter_componente_vertice(componentes, vertice);
        double x;
        double y;

        if (componente < 0 || componente >= quantidade_componentes) {
            continue;
        }

        x = obter_x_vertice_grafo(vertice);
        y = obter_y_vertice_grafo(vertice);
        incluir_ponto_limites(x, y, &min_x[componente], &min_y[componente], &max_x[componente], &max_y[componente], &possui_vertice[componente]);
    }

    arquivo = fopen(caminho_svg, "a");
    if (arquivo == NULL) {
        free(min_x);
        free(min_y);
        free(max_x);
        free(max_y);
        free(possui_vertice);
        return 0;
    }

    fprintf(arquivo, "  <g id=\"componentes_regs\">\n");
    for (i = 0; i < quantidade_componentes; i++) {
        double largura;
        double altura;

        if (!possui_vertice[i]) {
            continue;
        }

        largura = max_x[i] - min_x[i];
        altura = max_y[i] - min_y[i];

        fprintf(
            arquivo,
            "    <rect id=\"componente_%d\" x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" fill=\"%s\" fill-opacity=\"0.50\" stroke=\"%s\" stroke-width=\"1.50\" />\n",
            i,
            min_x[i],
            min_y[i],
            largura,
            altura,
            cores[i % (int) (sizeof(cores) / sizeof(cores[0]))],
            cores[i % (int) (sizeof(cores) / sizeof(cores[0]))]
        );
    }
    fprintf(arquivo, "  </g>\n");

    fclose(arquivo);
    free(min_x);
    free(min_y);
    free(max_x);
    free(max_y);
    free(possui_vertice);
    return 1;
}

int acrescentar_arestas_expandidas_svg(const char *caminho_svg, ArestaGrafo *arestas, int quantidade)
{
    FILE *arquivo;
    int i;

    if (caminho_svg == NULL || quantidade < 0 || (quantidade > 0 && arestas == NULL)) {
        return 0;
    }

    if (quantidade == 0) {
        return 1;
    }

    arquivo = fopen(caminho_svg, "a");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(arquivo, "  <g id=\"arestas_expandidas\" fill=\"none\" stroke=\"red\" stroke-width=\"4.00\" stroke-linecap=\"round\">\n");
    for (i = 0; i < quantidade; i++) {
        VerticeGrafo origem;
        VerticeGrafo destino;

        if (arestas[i] == NULL) {
            continue;
        }

        origem = obter_origem_aresta_grafo(arestas[i]);
        destino = obter_destino_aresta_grafo(arestas[i]);
        fprintf(
            arquivo,
            "    <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" />\n",
            obter_x_vertice_grafo(origem),
            obter_y_vertice_grafo(origem),
            obter_x_vertice_grafo(destino),
            obter_y_vertice_grafo(destino)
        );
    }
    fprintf(arquivo, "  </g>\n");

    fclose(arquivo);
    return 1;
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
