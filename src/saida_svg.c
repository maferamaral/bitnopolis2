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

int escrever_cidade_svg(const char *caminho_svg, Cidade cidade)
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

    fprintf(arquivo, "</svg>\n");
    fclose(arquivo);
    return 1;
}
