#include "unity.h"
#include "saida_svg.h"
#include "cidade.h"
#include "grafo.h"
#include "quadra.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAMINHO_SVG_TESTE "/tmp/bitnopolis_t_saida.svg"

void setUp(void)
{
}

void tearDown(void)
{
    remove(CAMINHO_SVG_TESTE);
}

static int arquivo_contem_svg(const char *trecho)
{
    FILE *arquivo = fopen(CAMINHO_SVG_TESTE, "r");
    char linha[512];

    TEST_ASSERT_NOT_NULL(arquivo);
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strstr(linha, trecho) != NULL) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

void test_deve_escrever_svg_com_quadras(void)
{
    Cidade cidade = criar_cidade();
    Quadra quadra = criar_quadra("cep1", 100.0, 200.0, 40.0, 30.0);

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_NOT_NULL(quadra);
    definir_estilo_quadra(quadra, 2.0, "yellow", "black");
    TEST_ASSERT_EQUAL_INT(1, inserir_quadra_cidade(cidade, quadra));

    TEST_ASSERT_EQUAL_INT(1, escrever_cidade_svg(CAMINHO_SVG_TESTE, cidade));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("<svg"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("<rect"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("cep1"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("fill=\"yellow\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("stroke=\"black\""));

    destruir_cidade(cidade);
}

void test_deve_escrever_svg_com_grafo_viario(void)
{
    Cidade cidade = criar_cidade();
    Grafo grafo = criar_grafo();

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "A", 0.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "B", 40.0, 20.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "A", "B", "-", "-", 44.0, 10.0, "Rua_AB"));

    TEST_ASSERT_EQUAL_INT(1, iniciar_mapa_svg(CAMINHO_SVG_TESTE, cidade, grafo));
    TEST_ASSERT_EQUAL_INT(1, finalizar_svg(CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("id=\"vias\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("<line"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("x1=\"0.00\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("x2=\"40.00\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("id=\"cruzamentos\""));

    destruir_grafo(grafo);
    destruir_cidade(cidade);
}

void test_deve_desenhar_registrador_ate_o_topo_do_svg(void)
{
    Cidade cidade = criar_cidade();

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_EQUAL_INT(1, iniciar_mapa_svg(CAMINHO_SVG_TESTE, cidade, NULL));
    TEST_ASSERT_EQUAL_INT(1, acrescentar_registrador_svg(CAMINHO_SVG_TESTE, 0, 50.0, 80.0));
    TEST_ASSERT_EQUAL_INT(1, finalizar_svg(CAMINHO_SVG_TESTE));

    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("id=\"registrador_R0\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("x1=\"50.00\" y1=\"80.00\" x2=\"50.00\" y2=\"-10.00\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg("x=\"50.00\" y=\"-2.00\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem_svg(">R0<"));

    destruir_cidade(cidade);
}

void test_deve_rejeitar_entradas_invalidas_svg(void)
{
    Cidade cidade = criar_cidade();

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_EQUAL_INT(0, escrever_cidade_svg(NULL, cidade));
    TEST_ASSERT_EQUAL_INT(0, escrever_cidade_svg(CAMINHO_SVG_TESTE, NULL));

    destruir_cidade(cidade);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_escrever_svg_com_quadras);
    RUN_TEST(test_deve_escrever_svg_com_grafo_viario);
    RUN_TEST(test_deve_desenhar_registrador_ate_o_topo_do_svg);
    RUN_TEST(test_deve_rejeitar_entradas_invalidas_svg);
    return UNITY_END();
}
