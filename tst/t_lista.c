#include "unity.h"
#include "lista.h"

#include <stdlib.h>

static int itens_destruidos;

void setUp(void)
{
    itens_destruidos = 0;
}

void tearDown(void)
{
}

static void destruir_inteiro(void *item)
{
    free(item);
    itens_destruidos++;
}

static int *criar_inteiro(int valor)
{
    int *item = malloc(sizeof(*item));

    TEST_ASSERT_NOT_NULL(item);
    *item = valor;
    return item;
}

void test_deve_criar_lista_vazia(void)
{
    Lista lista = criar_lista();

    TEST_ASSERT_NOT_NULL(lista);
    TEST_ASSERT_EQUAL_INT(0, obter_tamanho_lista(lista));
    TEST_ASSERT_EQUAL_INT(1, lista_esta_vazia(lista));

    destruir_lista(lista, NULL);
}

void test_deve_inserir_e_obter_itens(void)
{
    int primeiro = 10;
    int segundo = 20;
    Lista lista = criar_lista();

    TEST_ASSERT_NOT_NULL(lista);
    TEST_ASSERT_EQUAL_INT(1, inserir_lista(lista, &primeiro));
    TEST_ASSERT_EQUAL_INT(1, inserir_lista(lista, &segundo));
    TEST_ASSERT_EQUAL_INT(2, obter_tamanho_lista(lista));
    TEST_ASSERT_EQUAL_INT(0, lista_esta_vazia(lista));
    TEST_ASSERT_EQUAL_PTR(&primeiro, obter_item_lista(lista, 0));
    TEST_ASSERT_EQUAL_PTR(&segundo, obter_item_lista(lista, 1));

    destruir_lista(lista, NULL);
}

void test_deve_retornar_nulo_para_indice_invalido(void)
{
    int item = 5;
    Lista lista = criar_lista();

    TEST_ASSERT_NOT_NULL(lista);
    TEST_ASSERT_EQUAL_INT(1, inserir_lista(lista, &item));
    TEST_ASSERT_NULL(obter_item_lista(lista, -1));
    TEST_ASSERT_NULL(obter_item_lista(lista, 1));

    destruir_lista(lista, NULL);
}

void test_deve_destruir_itens_quando_funcao_for_informada(void)
{
    Lista lista = criar_lista();

    TEST_ASSERT_NOT_NULL(lista);
    TEST_ASSERT_EQUAL_INT(1, inserir_lista(lista, criar_inteiro(1)));
    TEST_ASSERT_EQUAL_INT(1, inserir_lista(lista, criar_inteiro(2)));

    destruir_lista(lista, destruir_inteiro);

    TEST_ASSERT_EQUAL_INT(2, itens_destruidos);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_criar_lista_vazia);
    RUN_TEST(test_deve_inserir_e_obter_itens);
    RUN_TEST(test_deve_retornar_nulo_para_indice_invalido);
    RUN_TEST(test_deve_destruir_itens_quando_funcao_for_informada);
    return UNITY_END();
}
