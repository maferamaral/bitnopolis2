#include "unity.h"
#include "registrador.h"
#include "quadra.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_deve_criar_registradores_vazios(void)
{
    Registradores registradores = criar_registradores();

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_EQUAL_INT(0, registrador_esta_definido(registradores, 0));
    TEST_ASSERT_EQUAL_INT(0, registrador_esta_definido(registradores, 10));
    TEST_ASSERT_EQUAL_INT(0, registrador_esta_definido(registradores, -1));
    TEST_ASSERT_EQUAL_INT(0, registrador_esta_definido(registradores, 11));

    destruir_registradores(registradores);
}

void test_deve_definir_registrador_por_ponto(void)
{
    Registradores registradores = criar_registradores();

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_EQUAL_INT(1, definir_registrador_ponto(registradores, 3, 10.5, 20.5));
    TEST_ASSERT_EQUAL_INT(1, registrador_esta_definido(registradores, 3));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 10.5, obter_x_registrador(registradores, 3));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 20.5, obter_y_registrador(registradores, 3));

    destruir_registradores(registradores);
}

void test_deve_calcular_endereco_na_face_norte(void)
{
    Registradores registradores = criar_registradores();
    Quadra quadra = criar_quadra("cep1", 100.0, 200.0, 40.0, 30.0);

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_NOT_NULL(quadra);
    TEST_ASSERT_EQUAL_INT(1, definir_registrador_endereco(registradores, 0, quadra, 'N', 15.0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 85.0, obter_x_registrador(registradores, 0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 230.0, obter_y_registrador(registradores, 0));

    destruir_quadra(quadra);
    destruir_registradores(registradores);
}

void test_deve_calcular_endereco_na_face_sul(void)
{
    Registradores registradores = criar_registradores();
    Quadra quadra = criar_quadra("cep1", 100.0, 200.0, 40.0, 30.0);

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_NOT_NULL(quadra);
    TEST_ASSERT_EQUAL_INT(1, definir_registrador_endereco(registradores, 1, quadra, 'S', 15.0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 85.0, obter_x_registrador(registradores, 1));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 200.0, obter_y_registrador(registradores, 1));

    destruir_quadra(quadra);
    destruir_registradores(registradores);
}

void test_deve_calcular_endereco_na_face_leste(void)
{
    Registradores registradores = criar_registradores();
    Quadra quadra = criar_quadra("cep1", 100.0, 200.0, 40.0, 30.0);

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_NOT_NULL(quadra);
    TEST_ASSERT_EQUAL_INT(1, definir_registrador_endereco(registradores, 2, quadra, 'L', 10.0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 100.0, obter_x_registrador(registradores, 2));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 210.0, obter_y_registrador(registradores, 2));

    destruir_quadra(quadra);
    destruir_registradores(registradores);
}

void test_deve_calcular_endereco_na_face_oeste(void)
{
    Registradores registradores = criar_registradores();
    Quadra quadra = criar_quadra("cep1", 100.0, 200.0, 40.0, 30.0);

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_NOT_NULL(quadra);
    TEST_ASSERT_EQUAL_INT(1, definir_registrador_endereco(registradores, 4, quadra, 'O', 10.0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 60.0, obter_x_registrador(registradores, 4));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 210.0, obter_y_registrador(registradores, 4));

    destruir_quadra(quadra);
    destruir_registradores(registradores);
}

void test_deve_rejeitar_entradas_invalidas(void)
{
    Registradores registradores = criar_registradores();
    Quadra quadra = criar_quadra("cep1", 100.0, 200.0, 40.0, 30.0);

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_NOT_NULL(quadra);
    TEST_ASSERT_EQUAL_INT(0, definir_registrador_ponto(NULL, 0, 1.0, 2.0));
    TEST_ASSERT_EQUAL_INT(0, definir_registrador_ponto(registradores, -1, 1.0, 2.0));
    TEST_ASSERT_EQUAL_INT(0, definir_registrador_ponto(registradores, 11, 1.0, 2.0));
    TEST_ASSERT_EQUAL_INT(0, definir_registrador_endereco(registradores, 0, NULL, 'N', 1.0));
    TEST_ASSERT_EQUAL_INT(0, definir_registrador_endereco(registradores, 0, quadra, 'X', 1.0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, obter_x_registrador(NULL, 0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, obter_y_registrador(registradores, 9));

    destruir_quadra(quadra);
    destruir_registradores(registradores);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_criar_registradores_vazios);
    RUN_TEST(test_deve_definir_registrador_por_ponto);
    RUN_TEST(test_deve_calcular_endereco_na_face_norte);
    RUN_TEST(test_deve_calcular_endereco_na_face_sul);
    RUN_TEST(test_deve_calcular_endereco_na_face_leste);
    RUN_TEST(test_deve_calcular_endereco_na_face_oeste);
    RUN_TEST(test_deve_rejeitar_entradas_invalidas);
    return UNITY_END();
}
