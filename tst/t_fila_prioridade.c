#include "unity.h"
#include "fila_prioridade.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_deve_criar_fila_vazia(void)
{
    FilaPrioridade fila = criar_fila_prioridade();

    TEST_ASSERT_NOT_NULL(fila);
    TEST_ASSERT_EQUAL_INT(1, fila_prioridade_esta_vazia(fila));
    TEST_ASSERT_EQUAL_INT(0, obter_tamanho_fila_prioridade(fila));
    TEST_ASSERT_NULL(remover_minimo_fila_prioridade(fila));

    destruir_fila_prioridade(fila);
}

void test_deve_remover_itens_por_menor_prioridade(void)
{
    int a = 10;
    int b = 20;
    int c = 30;
    FilaPrioridade fila = criar_fila_prioridade();

    TEST_ASSERT_NOT_NULL(fila);
    TEST_ASSERT_EQUAL_INT(1, inserir_fila_prioridade(fila, &a, 3.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_fila_prioridade(fila, &b, 1.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_fila_prioridade(fila, &c, 2.0));
    TEST_ASSERT_EQUAL_INT(3, obter_tamanho_fila_prioridade(fila));

    TEST_ASSERT_EQUAL_PTR(&b, remover_minimo_fila_prioridade(fila));
    TEST_ASSERT_EQUAL_PTR(&c, remover_minimo_fila_prioridade(fila));
    TEST_ASSERT_EQUAL_PTR(&a, remover_minimo_fila_prioridade(fila));
    TEST_ASSERT_EQUAL_INT(1, fila_prioridade_esta_vazia(fila));

    destruir_fila_prioridade(fila);
}

void test_deve_aceitar_prioridade_negativa(void)
{
    int a = 10;
    int b = 20;
    FilaPrioridade fila = criar_fila_prioridade();

    TEST_ASSERT_NOT_NULL(fila);
    TEST_ASSERT_EQUAL_INT(1, inserir_fila_prioridade(fila, &a, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_fila_prioridade(fila, &b, -5.0));
    TEST_ASSERT_EQUAL_PTR(&b, remover_minimo_fila_prioridade(fila));
    TEST_ASSERT_EQUAL_PTR(&a, remover_minimo_fila_prioridade(fila));

    destruir_fila_prioridade(fila);
}

void test_deve_rejeitar_fila_nula(void)
{
    int item = 1;

    TEST_ASSERT_EQUAL_INT(0, inserir_fila_prioridade(NULL, &item, 1.0));
    TEST_ASSERT_NULL(remover_minimo_fila_prioridade(NULL));
    TEST_ASSERT_EQUAL_INT(1, fila_prioridade_esta_vazia(NULL));
    TEST_ASSERT_EQUAL_INT(0, obter_tamanho_fila_prioridade(NULL));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_criar_fila_vazia);
    RUN_TEST(test_deve_remover_itens_por_menor_prioridade);
    RUN_TEST(test_deve_aceitar_prioridade_negativa);
    RUN_TEST(test_deve_rejeitar_fila_nula);
    return UNITY_END();
}
