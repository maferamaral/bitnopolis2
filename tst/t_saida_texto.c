#include "unity.h"
#include "saida_texto.h"
#include "cidade.h"
#include "quadra.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAMINHO_TXT_TESTE "/tmp/bitnopolis_t_saida.txt"

void setUp(void)
{
}

void tearDown(void)
{
    remove(CAMINHO_TXT_TESTE);
}

static int arquivo_contem(const char *trecho)
{
    FILE *arquivo = fopen(CAMINHO_TXT_TESTE, "r");
    char linha[256];

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

void test_deve_escrever_relatorio_textual_da_cidade(void)
{
    Cidade cidade = criar_cidade();

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_EQUAL_INT(1, inserir_quadra_cidade(cidade, criar_quadra("cep1", 100.0, 200.0, 40.0, 30.0)));
    TEST_ASSERT_EQUAL_INT(1, inserir_quadra_cidade(cidade, criar_quadra("cep2", 300.0, 400.0, 50.0, 60.0)));

    TEST_ASSERT_EQUAL_INT(1, escrever_relatorio_cidade_texto(CAMINHO_TXT_TESTE, cidade));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem("Quantidade de quadras: 2"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem("cep1"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem("cep2"));

    destruir_cidade(cidade);
}

void test_deve_acrescentar_linha_ao_arquivo_texto(void)
{
    TEST_ASSERT_EQUAL_INT(1, acrescentar_linha_texto(CAMINHO_TXT_TESTE, "Primeira linha"));
    TEST_ASSERT_EQUAL_INT(1, acrescentar_linha_texto(CAMINHO_TXT_TESTE, "Segunda linha"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem("Primeira linha"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem("Segunda linha"));
}

void test_deve_rejeitar_entradas_invalidas_texto(void)
{
    Cidade cidade = criar_cidade();

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_EQUAL_INT(0, escrever_relatorio_cidade_texto(NULL, cidade));
    TEST_ASSERT_EQUAL_INT(0, escrever_relatorio_cidade_texto(CAMINHO_TXT_TESTE, NULL));
    TEST_ASSERT_EQUAL_INT(0, acrescentar_linha_texto(NULL, "linha"));
    TEST_ASSERT_EQUAL_INT(0, acrescentar_linha_texto(CAMINHO_TXT_TESTE, NULL));

    destruir_cidade(cidade);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_escrever_relatorio_textual_da_cidade);
    RUN_TEST(test_deve_acrescentar_linha_ao_arquivo_texto);
    RUN_TEST(test_deve_rejeitar_entradas_invalidas_texto);
    return UNITY_END();
}
