#include "unity.h"
#include "consulta.h"
#include "cidade.h"
#include "grafo.h"
#include "quadra.h"
#include "saida_svg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAMINHO_QRY_TESTE "/tmp/bitnopolis_t_consulta.qry"
#define CAMINHO_TXT_TESTE "/tmp/bitnopolis_t_consulta.txt"
#define CAMINHO_SVG_TESTE "/tmp/bitnopolis_t_consulta.svg"

void setUp(void)
{
}

void tearDown(void)
{
    remove(CAMINHO_QRY_TESTE);
    remove(CAMINHO_TXT_TESTE);
    remove(CAMINHO_SVG_TESTE);
}

static void escrever_qry(const char *conteudo)
{
    FILE *arquivo = fopen(CAMINHO_QRY_TESTE, "w");

    TEST_ASSERT_NOT_NULL(arquivo);
    fputs(conteudo, arquivo);
    fclose(arquivo);
}

static int arquivo_txt_contem(const char *trecho)
{
    FILE *arquivo = fopen(CAMINHO_TXT_TESTE, "r");
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

static int arquivo_svg_contem(const char *trecho)
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

static Cidade criar_cidade_consulta(void)
{
    Cidade cidade = criar_cidade();

    TEST_ASSERT_NOT_NULL(cidade);
    TEST_ASSERT_EQUAL_INT(1, inserir_quadra_cidade(cidade, criar_quadra("cep1", 0.0, 0.0, 10.0, 10.0)));
    TEST_ASSERT_EQUAL_INT(1, inserir_quadra_cidade(cidade, criar_quadra("cep2", 20.0, 0.0, 10.0, 10.0)));
    return cidade;
}

static Grafo criar_grafo_consulta(void)
{
    Grafo grafo = criar_grafo();

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "A", 0.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "B", 10.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "C", 0.0, 10.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "D", 20.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "A", "B", "-", "-", 10.0, 1.0, "Rua_AB"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "B", "D", "-", "-", 10.0, 1.0, "Rua_BD"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "A", "C", "-", "-", 30.0, 30.0, "Rua_AC"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "C", "D", "-", "-", 30.0, 30.0, "Rua_CD"));
    return grafo;
}

void test_deve_processar_origem_geografica_e_percurso(void)
{
    Cidade cidade = criar_cidade_consulta();
    Grafo grafo = criar_grafo_consulta();

    escrever_qry(
        "@o? R0 cep1 L 0\n"
        "@o? R1 cep2 L 0\n"
        "p? R0 R1 red blue\n"
    );

    TEST_ASSERT_EQUAL_INT(1, iniciar_cidade_svg(CAMINHO_SVG_TESTE, cidade));
    TEST_ASSERT_EQUAL_INT(1, processar_arquivo_consulta(CAMINHO_QRY_TESTE, cidade, grafo, CAMINHO_TXT_TESTE, CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(1, finalizar_svg(CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("@o? R0 cep1 L 0.00 -> (0.00, 0.00)"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("@o? R1 cep2 L 0.00 -> (20.00, 0.00)"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("p? R0 R1"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("Percurso mais curto: custo 20.00"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("Siga na direcao leste pela Rua_AB ate B."));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("Siga em frente na direcao leste pela Rua_BD ate D."));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("Percurso mais rapido: custo 2.00"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("Siga na direcao norte pela Rua_AC ate C."));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("Vire a direita na direcao leste pela Rua_CD ate D."));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("id=\"registrador_R0\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("id=\"registrador_R1\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("stroke=\"red\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("stroke-dasharray=\"4 3\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem(">R0<"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem(">R1<"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("id=\"percurso_curto_1\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("stroke=\"red\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("id=\"percurso_rapido_1\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("stroke=\"blue\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("animateMotion"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem(">I<"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem(">F<"));

    destruir_grafo(grafo);
    destruir_cidade(cidade);
}

void test_deve_informar_destino_inacessivel_quando_percurso_nao_existir(void)
{
    Cidade cidade = criar_cidade_consulta();
    Grafo grafo = criar_grafo();

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "A", 0.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "D", 20.0, 0.0));

    escrever_qry(
        "@o? R0 cep1 L 0\n"
        "@o? R1 cep2 L 0\n"
        "p? R0 R1 red blue\n"
    );

    TEST_ASSERT_EQUAL_INT(1, processar_arquivo_consulta(CAMINHO_QRY_TESTE, cidade, grafo, CAMINHO_TXT_TESTE, CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("Destino inacessivel"));

    destruir_grafo(grafo);
    destruir_cidade(cidade);
}

void test_deve_processar_mvm_e_alterar_velocidades(void)
{
    Cidade cidade = criar_cidade_consulta();
    Grafo grafo = criar_grafo_consulta();
    VerticeGrafo origem;
    ArestaGrafo aresta;

    escrever_qry("mvm 9 0 0 15 5\n");

    TEST_ASSERT_EQUAL_INT(1, processar_arquivo_consulta(CAMINHO_QRY_TESTE, cidade, grafo, CAMINHO_TXT_TESTE, CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("mvm 9.00 0.00 0.00 15.00 5.00 -> 1 aresta(s) atualizada(s)"));

    origem = buscar_vertice_grafo(grafo, "A");
    aresta = obter_aresta_vertice_grafo(origem, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 9.0, obter_velocidade_aresta_grafo(aresta));

    destruir_grafo(grafo);
    destruir_cidade(cidade);
}

void test_deve_processar_regs_e_informar_componentes(void)
{
    Cidade cidade = criar_cidade_consulta();
    Grafo grafo = criar_grafo_consulta();

    escrever_qry("regs 5\n");

    TEST_ASSERT_EQUAL_INT(1, processar_arquivo_consulta(CAMINHO_QRY_TESTE, cidade, grafo, CAMINHO_TXT_TESTE, CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("regs 5.00 -> Numero de componentes conexos: 1"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("id=\"componentes_regs\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("id=\"componente_0\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("fill-opacity=\"0.50\""));

    destruir_grafo(grafo);
    destruir_cidade(cidade);
}

void test_deve_processar_exp_e_expandir_arestas(void)
{
    Cidade cidade = criar_cidade_consulta();
    Grafo grafo = criar_grafo_consulta();
    VerticeGrafo origem = buscar_vertice_grafo(grafo, "A");
    ArestaGrafo aresta = obter_aresta_vertice_grafo(origem, 0);

    escrever_qry("exp 5\n");

    TEST_ASSERT_EQUAL_INT(1, processar_arquivo_consulta(CAMINHO_QRY_TESTE, cidade, grafo, CAMINHO_TXT_TESTE, CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(1, arquivo_txt_contem("exp 5.00 -> 2 aresta(s) expandida(s)"));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("id=\"arestas_expandidas\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("stroke=\"red\""));
    TEST_ASSERT_EQUAL_INT(1, arquivo_svg_contem("stroke-width=\"4.00\""));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 1.5, obter_velocidade_aresta_grafo(aresta));

    destruir_grafo(grafo);
    destruir_cidade(cidade);
}

void test_deve_rejeitar_entradas_invalidas_consulta(void)
{
    Cidade cidade = criar_cidade_consulta();
    Grafo grafo = criar_grafo_consulta();

    escrever_qry("@o? R0 cep1 L 0\n");

    TEST_ASSERT_EQUAL_INT(0, processar_arquivo_consulta(NULL, cidade, grafo, CAMINHO_TXT_TESTE, CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(0, processar_arquivo_consulta(CAMINHO_QRY_TESTE, NULL, grafo, CAMINHO_TXT_TESTE, CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(0, processar_arquivo_consulta(CAMINHO_QRY_TESTE, cidade, NULL, CAMINHO_TXT_TESTE, CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(0, processar_arquivo_consulta(CAMINHO_QRY_TESTE, cidade, grafo, NULL, CAMINHO_SVG_TESTE));
    TEST_ASSERT_EQUAL_INT(0, processar_arquivo_consulta(CAMINHO_QRY_TESTE, cidade, grafo, CAMINHO_TXT_TESTE, NULL));

    destruir_grafo(grafo);
    destruir_cidade(cidade);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_processar_origem_geografica_e_percurso);
    RUN_TEST(test_deve_informar_destino_inacessivel_quando_percurso_nao_existir);
    RUN_TEST(test_deve_processar_mvm_e_alterar_velocidades);
    RUN_TEST(test_deve_processar_regs_e_informar_componentes);
    RUN_TEST(test_deve_processar_exp_e_expandir_arestas);
    RUN_TEST(test_deve_rejeitar_entradas_invalidas_consulta);
    return UNITY_END();
}
