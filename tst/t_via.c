#include "unity.h"
#include "via.h"
#include "grafo.h"

#include <stdio.h>
#include <stdlib.h>

#define CAMINHO_VIA_TESTE "/tmp/bitnopolis_t_via.via"

void setUp(void)
{
}

void tearDown(void)
{
    remove(CAMINHO_VIA_TESTE);
}

static void escrever_via(const char *conteudo)
{
    FILE *arquivo = fopen(CAMINHO_VIA_TESTE, "w");

    TEST_ASSERT_NOT_NULL(arquivo);
    fputs(conteudo, arquivo);
    fclose(arquivo);
}

void test_deve_ler_vertices_e_arestas(void)
{
    Grafo grafo;
    VerticeGrafo v1;
    VerticeGrafo v2;
    ArestaGrafo aresta;

    escrever_via(
        "2\n"
        "v v1 10 20\n"
        "v v2 30 40\n"
        "e v1 v2 cep1 cep2 100 5 Rua_A\n"
    );

    grafo = ler_arquivo_via(CAMINHO_VIA_TESTE);
    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(2, obter_quantidade_vertices_grafo(grafo));
    TEST_ASSERT_EQUAL_INT(1, obter_quantidade_arestas_grafo(grafo));

    v1 = buscar_vertice_grafo(grafo, "v1");
    v2 = buscar_vertice_grafo(grafo, "v2");
    aresta = obter_aresta_vertice_grafo(v1, 0);

    TEST_ASSERT_NOT_NULL(v1);
    TEST_ASSERT_NOT_NULL(v2);
    TEST_ASSERT_NOT_NULL(aresta);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 10.0, obter_x_vertice_grafo(v1));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 40.0, obter_y_vertice_grafo(v2));
    TEST_ASSERT_EQUAL_PTR(v2, obter_destino_aresta_grafo(aresta));
    TEST_ASSERT_EQUAL_STRING("cep1", obter_cep_direita_aresta_grafo(aresta));
    TEST_ASSERT_EQUAL_STRING("cep2", obter_cep_esquerda_aresta_grafo(aresta));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 100.0, obter_comprimento_aresta_grafo(aresta));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 5.0, obter_velocidade_aresta_grafo(aresta));
    TEST_ASSERT_EQUAL_STRING("Rua_A", obter_nome_aresta_grafo(aresta));

    destruir_grafo(grafo);
}

void test_deve_rejeitar_aresta_com_vertice_inexistente(void)
{
    escrever_via(
        "1\n"
        "v v1 10 20\n"
        "e v1 v2 - - 100 5 Rua_A\n"
    );

    TEST_ASSERT_NULL(ler_arquivo_via(CAMINHO_VIA_TESTE));
}

void test_deve_ignorar_linhas_desconhecidas(void)
{
    Grafo grafo;

    escrever_via(
        "1\n"
        "x qualquer coisa\n"
        "v v1 10 20\n"
    );

    grafo = ler_arquivo_via(CAMINHO_VIA_TESTE);
    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, obter_quantidade_vertices_grafo(grafo));
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_arestas_grafo(grafo));

    destruir_grafo(grafo);
}

void test_deve_retornar_nulo_para_arquivo_inexistente(void)
{
    TEST_ASSERT_NULL(ler_arquivo_via("/tmp/bitnopolis_via_inexistente.via"));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_ler_vertices_e_arestas);
    RUN_TEST(test_deve_rejeitar_aresta_com_vertice_inexistente);
    RUN_TEST(test_deve_ignorar_linhas_desconhecidas);
    RUN_TEST(test_deve_retornar_nulo_para_arquivo_inexistente);
    return UNITY_END();
}
