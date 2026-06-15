#include "unity.h"
#include "quadra.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_deve_criar_quadra_com_dados_geometricos(void)
{
    Quadra quadra = criar_quadra("cep15", 10.0, 20.0, 30.0, 40.0);

    TEST_ASSERT_NOT_NULL(quadra);
    TEST_ASSERT_EQUAL_STRING("cep15", obter_cep_quadra(quadra));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 10.0, obter_x_quadra(quadra));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 20.0, obter_y_quadra(quadra));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 30.0, obter_largura_quadra(quadra));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 40.0, obter_altura_quadra(quadra));

    destruir_quadra(quadra);
}

void test_deve_definir_estilo_da_quadra(void)
{
    Quadra quadra = criar_quadra("cep22", 1.0, 2.0, 3.0, 4.0);

    TEST_ASSERT_NOT_NULL(quadra);
    definir_estilo_quadra(quadra, 2.5, "yellow", "black");

    TEST_ASSERT_FLOAT_WITHIN(0.0001, 2.5, obter_espessura_quadra(quadra));
    TEST_ASSERT_EQUAL_STRING("yellow", obter_preenchimento_quadra(quadra));
    TEST_ASSERT_EQUAL_STRING("black", obter_borda_quadra(quadra));

    destruir_quadra(quadra);
}

void test_deve_ter_estilo_padrao(void)
{
    Quadra quadra = criar_quadra("cep30", 1.0, 2.0, 3.0, 4.0);

    TEST_ASSERT_NOT_NULL(quadra);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 1.0, obter_espessura_quadra(quadra));
    TEST_ASSERT_EQUAL_STRING("none", obter_preenchimento_quadra(quadra));
    TEST_ASSERT_EQUAL_STRING("black", obter_borda_quadra(quadra));

    destruir_quadra(quadra);
}

void test_deve_rejeitar_cep_nulo(void)
{
    TEST_ASSERT_NULL(criar_quadra(NULL, 1.0, 2.0, 3.0, 4.0));
}

void test_deve_retornar_valores_neutros_para_quadra_nula(void)
{
    TEST_ASSERT_NULL(obter_cep_quadra(NULL));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, obter_x_quadra(NULL));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, obter_y_quadra(NULL));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, obter_largura_quadra(NULL));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, obter_altura_quadra(NULL));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, obter_espessura_quadra(NULL));
    TEST_ASSERT_NULL(obter_preenchimento_quadra(NULL));
    TEST_ASSERT_NULL(obter_borda_quadra(NULL));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_criar_quadra_com_dados_geometricos);
    RUN_TEST(test_deve_definir_estilo_da_quadra);
    RUN_TEST(test_deve_ter_estilo_padrao);
    RUN_TEST(test_deve_rejeitar_cep_nulo);
    RUN_TEST(test_deve_retornar_valores_neutros_para_quadra_nula);
    return UNITY_END();
}
