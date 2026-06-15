#include "unity.h"
#include "argumentos.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_deve_ler_argumentos_obrigatorios(void)
{
    char *argv[] = {"ted", "-f", "cidade.geo", "-o", "saida"};
    Argumentos argumentos = criar_argumentos();

    TEST_ASSERT_NOT_NULL(argumentos);
    TEST_ASSERT_EQUAL_INT(1, ler_argumentos(argumentos, 5, argv));
    TEST_ASSERT_NULL(obter_diretorio_entrada(argumentos));
    TEST_ASSERT_EQUAL_STRING("cidade.geo", obter_arquivo_geo(argumentos));
    TEST_ASSERT_EQUAL_STRING("saida", obter_diretorio_saida(argumentos));
    TEST_ASSERT_EQUAL_INT(0, argumentos_possuem_consulta(argumentos));
    TEST_ASSERT_EQUAL_INT(0, argumentos_possuem_vias(argumentos));

    destruir_argumentos(argumentos);
}

void test_deve_ler_argumentos_opcionais(void)
{
    char *argv[] = {
        "ted", "-e", "entrada", "-f", "cidade.geo",
        "-q", "consulta.qry", "-v", "vias.via", "-o", "saida"
    };
    Argumentos argumentos = criar_argumentos();

    TEST_ASSERT_NOT_NULL(argumentos);
    TEST_ASSERT_EQUAL_INT(1, ler_argumentos(argumentos, 11, argv));
    TEST_ASSERT_EQUAL_STRING("entrada", obter_diretorio_entrada(argumentos));
    TEST_ASSERT_EQUAL_STRING("cidade.geo", obter_arquivo_geo(argumentos));
    TEST_ASSERT_EQUAL_STRING("consulta.qry", obter_arquivo_consulta(argumentos));
    TEST_ASSERT_EQUAL_STRING("vias.via", obter_arquivo_vias(argumentos));
    TEST_ASSERT_EQUAL_STRING("saida", obter_diretorio_saida(argumentos));
    TEST_ASSERT_EQUAL_INT(1, argumentos_possuem_consulta(argumentos));
    TEST_ASSERT_EQUAL_INT(1, argumentos_possuem_vias(argumentos));

    destruir_argumentos(argumentos);
}

void test_deve_rejeitar_ausencia_de_geo(void)
{
    char *argv[] = {"ted", "-o", "saida"};
    Argumentos argumentos = criar_argumentos();

    TEST_ASSERT_NOT_NULL(argumentos);
    TEST_ASSERT_EQUAL_INT(0, ler_argumentos(argumentos, 3, argv));

    destruir_argumentos(argumentos);
}

void test_deve_rejeitar_ausencia_de_saida(void)
{
    char *argv[] = {"ted", "-f", "cidade.geo"};
    Argumentos argumentos = criar_argumentos();

    TEST_ASSERT_NOT_NULL(argumentos);
    TEST_ASSERT_EQUAL_INT(0, ler_argumentos(argumentos, 3, argv));

    destruir_argumentos(argumentos);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_ler_argumentos_obrigatorios);
    RUN_TEST(test_deve_ler_argumentos_opcionais);
    RUN_TEST(test_deve_rejeitar_ausencia_de_geo);
    RUN_TEST(test_deve_rejeitar_ausencia_de_saida);
    return UNITY_END();
}
