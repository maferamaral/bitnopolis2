#include "unity.h"
#include "geo.h"
#include "cidade.h"
#include "quadra.h"

#include <stdio.h>
#include <stdlib.h>

#define CAMINHO_GEO_TESTE "/tmp/bitnopolis_t_geo.geo"

void setUp(void)
{
}

void tearDown(void)
{
    remove(CAMINHO_GEO_TESTE);
}

static void escrever_geo(const char *conteudo)
{
    FILE *arquivo = fopen(CAMINHO_GEO_TESTE, "w");

    TEST_ASSERT_NOT_NULL(arquivo);
    fputs(conteudo, arquivo);
    fclose(arquivo);
}

void test_deve_ler_quadras_do_geo(void)
{
    Cidade cidade;
    Quadra primeira;
    Quadra segunda;

    escrever_geo(
        "q cep1 10 20 30 40\n"
        "q cep2 50.5 60.5 70.5 80.5\n"
    );

    cidade = ler_arquivo_geo(CAMINHO_GEO_TESTE);
    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_EQUAL_INT(2, obter_quantidade_quadras_cidade(cidade));

    primeira = buscar_quadra_cidade(cidade, "cep1");
    segunda = buscar_quadra_cidade(cidade, "cep2");

    TEST_ASSERT_NOT_NULL(primeira);
    TEST_ASSERT_NOT_NULL(segunda);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 10.0, obter_x_quadra(primeira));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 20.0, obter_y_quadra(primeira));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 30.0, obter_largura_quadra(primeira));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 40.0, obter_altura_quadra(primeira));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 50.5, obter_x_quadra(segunda));

    destruir_cidade(cidade);
}

void test_deve_aplicar_estilo_cq_nas_quadras_seguintes(void)
{
    Cidade cidade;
    Quadra primeira;
    Quadra segunda;

    escrever_geo(
        "cq 2.5 yellow black\n"
        "q cep1 10 20 30 40\n"
        "cq 1 red blue\n"
        "q cep2 50 60 70 80\n"
    );

    cidade = ler_arquivo_geo(CAMINHO_GEO_TESTE);
    TEST_ASSERT_NOT_NULL(cidade);

    primeira = buscar_quadra_cidade(cidade, "cep1");
    segunda = buscar_quadra_cidade(cidade, "cep2");

    TEST_ASSERT_NOT_NULL(primeira);
    TEST_ASSERT_NOT_NULL(segunda);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 2.5, obter_espessura_quadra(primeira));
    TEST_ASSERT_EQUAL_STRING("yellow", obter_preenchimento_quadra(primeira));
    TEST_ASSERT_EQUAL_STRING("black", obter_borda_quadra(primeira));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 1.0, obter_espessura_quadra(segunda));
    TEST_ASSERT_EQUAL_STRING("red", obter_preenchimento_quadra(segunda));
    TEST_ASSERT_EQUAL_STRING("blue", obter_borda_quadra(segunda));

    destruir_cidade(cidade);
}

void test_deve_ignorar_linhas_desconhecidas(void)
{
    Cidade cidade;

    escrever_geo(
        "x comando desconhecido\n"
        "q cep1 10 20 30 40\n"
    );

    cidade = ler_arquivo_geo(CAMINHO_GEO_TESTE);
    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_EQUAL_INT(1, obter_quantidade_quadras_cidade(cidade));

    destruir_cidade(cidade);
}

void test_deve_retornar_nulo_para_arquivo_inexistente(void)
{
    TEST_ASSERT_NULL(ler_arquivo_geo("/tmp/bitnopolis_geo_inexistente.geo"));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_ler_quadras_do_geo);
    RUN_TEST(test_deve_aplicar_estilo_cq_nas_quadras_seguintes);
    RUN_TEST(test_deve_ignorar_linhas_desconhecidas);
    RUN_TEST(test_deve_retornar_nulo_para_arquivo_inexistente);
    return UNITY_END();
}
