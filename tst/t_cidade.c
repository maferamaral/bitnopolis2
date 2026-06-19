#include "unity.h"
#include "cidade.h"
#include "quadra.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_deve_criar_cidade_vazia(void)
{
    Cidade cidade = criar_cidade();

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_quadras_cidade(cidade));
    TEST_ASSERT_NULL(buscar_quadra_cidade(cidade, "cep1"));
    TEST_ASSERT_NULL(obter_quadra_cidade(cidade, 0));

    destruir_cidade(cidade);
}

void test_deve_inserir_e_obter_quadra(void)
{
    Cidade cidade = criar_cidade();
    Quadra quadra = criar_quadra("cep1", 10.0, 20.0, 30.0, 40.0);

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_NOT_NULL(quadra);
    TEST_ASSERT_EQUAL_INT(1, inserir_quadra_cidade(cidade, quadra));
    TEST_ASSERT_EQUAL_INT(1, obter_quantidade_quadras_cidade(cidade));
    TEST_ASSERT_EQUAL_PTR(quadra, obter_quadra_cidade(cidade, 0));

    destruir_cidade(cidade);
}

void test_deve_buscar_quadra_por_cep(void)
{
    Cidade cidade = criar_cidade();
    Quadra primeira = criar_quadra("cep1", 10.0, 20.0, 30.0, 40.0);
    Quadra segunda = criar_quadra("cep2", 50.0, 60.0, 70.0, 80.0);

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_NOT_NULL(primeira);
    TEST_ASSERT_NOT_NULL(segunda);
    TEST_ASSERT_EQUAL_INT(1, inserir_quadra_cidade(cidade, primeira));
    TEST_ASSERT_EQUAL_INT(1, inserir_quadra_cidade(cidade, segunda));
    TEST_ASSERT_EQUAL_PTR(primeira, buscar_quadra_cidade(cidade, "cep1"));
    TEST_ASSERT_EQUAL_PTR(segunda, buscar_quadra_cidade(cidade, "cep2"));
    TEST_ASSERT_NULL(buscar_quadra_cidade(cidade, "cep3"));

    destruir_cidade(cidade);
}

void test_deve_rejeitar_insercao_invalida(void)
{
    Cidade cidade = criar_cidade();
    Quadra quadra = criar_quadra("cep1", 10.0, 20.0, 30.0, 40.0);

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_NOT_NULL(quadra);
    TEST_ASSERT_EQUAL_INT(0, inserir_quadra_cidade(NULL, quadra));
    TEST_ASSERT_EQUAL_INT(0, inserir_quadra_cidade(cidade, NULL));

    destruir_quadra(quadra);
    destruir_cidade(cidade);
}

void test_deve_retornar_valores_neutros_para_cidade_nula(void)
{
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_quadras_cidade(NULL));
    TEST_ASSERT_NULL(buscar_quadra_cidade(NULL, "cep1"));
    TEST_ASSERT_NULL(buscar_quadra_cidade(NULL, NULL));
    TEST_ASSERT_NULL(obter_quadra_cidade(NULL, 0));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_criar_cidade_vazia);
    RUN_TEST(test_deve_inserir_e_obter_quadra);
    RUN_TEST(test_deve_buscar_quadra_por_cep);
    RUN_TEST(test_deve_rejeitar_insercao_invalida);
    RUN_TEST(test_deve_retornar_valores_neutros_para_cidade_nula);
    return UNITY_END();
}
