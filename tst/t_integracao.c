#include "unity.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define DIR_ENTRADA "/tmp/bitnopolis_integracao"
#define DIR_SAIDA "/tmp/bitnopolis_integracao_saida"
#define CAMINHO_GEO DIR_ENTRADA "/cidade.geo"
#define CAMINHO_VIA DIR_ENTRADA "/cidade.via"
#define CAMINHO_QRY DIR_ENTRADA "/cidade.qry"
#define CAMINHO_TXT DIR_SAIDA "/cidade.txt"
#define CAMINHO_SVG DIR_SAIDA "/cidade.svg"

static void limpar_arquivos_integracao(void)
{
    remove(CAMINHO_GEO);
    remove(CAMINHO_VIA);
    remove(CAMINHO_QRY);
    remove(CAMINHO_TXT);
    remove(CAMINHO_SVG);
    rmdir(DIR_ENTRADA);
    rmdir(DIR_SAIDA);
}

void setUp(void)
{
    limpar_arquivos_integracao();
    TEST_ASSERT_EQUAL_INT(0, mkdir(DIR_ENTRADA, 0777));
    TEST_ASSERT_EQUAL_INT(0, mkdir(DIR_SAIDA, 0777));
}

void tearDown(void)
{
    limpar_arquivos_integracao();
}

static void escrever_arquivo(const char *caminho, const char *conteudo)
{
    FILE *arquivo = fopen(caminho, "w");

    TEST_ASSERT_NOT_NULL(arquivo);
    fputs(conteudo, arquivo);
    fclose(arquivo);
}

static int arquivo_contem(const char *caminho, const char *trecho)
{
    FILE *arquivo = fopen(caminho, "r");
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

void test_deve_executar_programa_com_geo_via_qry(void)
{
    escrever_arquivo(
        CAMINHO_GEO,
        "cq 1 yellow black\n"
        "q cep1 0 0 10 10\n"
        "q cep2 20 0 10 10\n"
    );
    escrever_arquivo(
        CAMINHO_VIA,
        "4\n"
        "v A 0 0\n"
        "v B 10 0\n"
        "v C 0 10\n"
        "v D 20 0\n"
        "e A B - - 10 1 Rua_AB\n"
        "e B D - - 10 1 Rua_BD\n"
        "e A C - - 30 30 Rua_AC\n"
        "e C D - - 30 30 Rua_CD\n"
    );
    escrever_arquivo(
        CAMINHO_QRY,
        "@o? R0 cep1 L 0\n"
        "@o? R1 cep2 L 0\n"
        "p? R0 R1 red blue\n"
        "regs 5\n"
        "exp 5\n"
    );

    TEST_ASSERT_EQUAL_INT(
        0,
        system("./ted -e " DIR_ENTRADA " -f cidade.geo -v cidade.via -q cidade.qry -o " DIR_SAIDA)
    );

    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_TXT, "@o? R0 cep1 L 0.00 -> (0.00, 0.00)"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_TXT, "Percurso mais curto: custo 20.00"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_TXT, "Siga na direcao leste pela Rua_AB ate B."));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_TXT, "regs 5.00 -> Numero de componentes conexos: 1"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_TXT, "exp 5.00 -> 2 aresta(s) expandida(s)"));

    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_SVG, "id=\"vias\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_SVG, "id=\"registrador_R0\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_SVG, "id=\"percurso_curto_1\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_SVG, "animateMotion"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_SVG, "id=\"componentes_regs\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_contem(CAMINHO_SVG, "id=\"arestas_expandidas\""));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_executar_programa_com_geo_via_qry);
    return UNITY_END();
}
