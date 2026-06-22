#include "saida_texto.h"

#include "quadra.h"

#include <stdio.h>

int escrever_relatorio_cidade_texto(const char *caminho_txt, Cidade cidade)
{
    FILE *arquivo;
    int i;
    int quantidade;

    if (caminho_txt == NULL || cidade == NULL) {
        return 0;
    }

    arquivo = fopen(caminho_txt, "w");
    if (arquivo == NULL) {
        return 0;
    }

    quantidade = obter_quantidade_quadras_cidade(cidade);
    fprintf(arquivo, "Quantidade de quadras: %d\n", quantidade);

    for (i = 0; i < quantidade; i++) {
        Quadra quadra = obter_quadra_cidade(cidade, i);

        fprintf(
            arquivo,
            "Quadra %s: x=%.2f y=%.2f largura=%.2f altura=%.2f\n",
            obter_cep_quadra(quadra),
            obter_x_quadra(quadra),
            obter_y_quadra(quadra),
            obter_largura_quadra(quadra),
            obter_altura_quadra(quadra)
        );
    }

    fclose(arquivo);
    return 1;
}

int acrescentar_linha_texto(const char *caminho_txt, const char *linha)
{
    FILE *arquivo;

    if (caminho_txt == NULL || linha == NULL) {
        return 0;
    }

    arquivo = fopen(caminho_txt, "a");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(arquivo, "%s\n", linha);
    fclose(arquivo);
    return 1;
}
