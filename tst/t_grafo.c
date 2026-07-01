#include "unity.h"
#include "grafo.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_deve_criar_grafo_vazio(void)
{
    Grafo grafo = criar_grafo();

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_vertices_grafo(grafo));
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_arestas_grafo(grafo));
    TEST_ASSERT_NULL(buscar_vertice_grafo(grafo, "v1"));

    destruir_grafo(grafo);
}

void test_deve_inserir_e_buscar_vertices(void)
{
    Grafo grafo = criar_grafo();
    VerticeGrafo vertice;

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v1", 10.0, 20.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v2", 30.0, 40.0));
    TEST_ASSERT_EQUAL_INT(0, inserir_vertice_grafo(grafo, "v1", 50.0, 60.0));
    TEST_ASSERT_EQUAL_INT(2, obter_quantidade_vertices_grafo(grafo));

    vertice = buscar_vertice_grafo(grafo, "v1");
    TEST_ASSERT_NOT_NULL(vertice);
    TEST_ASSERT_EQUAL_STRING("v1", obter_id_vertice_grafo(vertice));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 10.0, obter_x_vertice_grafo(vertice));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 20.0, obter_y_vertice_grafo(vertice));
    TEST_ASSERT_EQUAL_PTR(vertice, obter_vertice_grafo(grafo, 0));

    destruir_grafo(grafo);
}

void test_deve_inserir_aresta_direcionada(void)
{
    Grafo grafo = criar_grafo();
    VerticeGrafo origem;
    VerticeGrafo destino;
    ArestaGrafo aresta;

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v1", 10.0, 20.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v2", 30.0, 40.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "v1", "v2", "cep1", "cep2", 100.0, 5.0, "Rua_A"));

    origem = buscar_vertice_grafo(grafo, "v1");
    destino = buscar_vertice_grafo(grafo, "v2");
    aresta = obter_aresta_vertice_grafo(origem, 0);

    TEST_ASSERT_EQUAL_INT(1, obter_quantidade_arestas_grafo(grafo));
    TEST_ASSERT_EQUAL_INT(1, obter_grau_saida_vertice_grafo(origem));
    TEST_ASSERT_EQUAL_INT(0, obter_grau_saida_vertice_grafo(destino));
    TEST_ASSERT_NOT_NULL(aresta);
    TEST_ASSERT_EQUAL_PTR(origem, obter_origem_aresta_grafo(aresta));
    TEST_ASSERT_EQUAL_PTR(destino, obter_destino_aresta_grafo(aresta));
    TEST_ASSERT_EQUAL_STRING("cep1", obter_cep_direita_aresta_grafo(aresta));
    TEST_ASSERT_EQUAL_STRING("cep2", obter_cep_esquerda_aresta_grafo(aresta));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 100.0, obter_comprimento_aresta_grafo(aresta));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 5.0, obter_velocidade_aresta_grafo(aresta));
    TEST_ASSERT_EQUAL_STRING("Rua_A", obter_nome_aresta_grafo(aresta));
    TEST_ASSERT_EQUAL_PTR(aresta, buscar_aresta_entre_vertices_grafo(origem, destino));
    TEST_ASSERT_NULL(buscar_aresta_entre_vertices_grafo(destino, origem));

    destruir_grafo(grafo);
}

void test_deve_rejeitar_aresta_com_vertices_inexistentes(void)
{
    Grafo grafo = criar_grafo();

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v1", 10.0, 20.0));
    TEST_ASSERT_EQUAL_INT(0, inserir_aresta_grafo(grafo, "v1", "v2", "-", "-", 100.0, 5.0, "Rua_A"));
    TEST_ASSERT_EQUAL_INT(0, inserir_aresta_grafo(grafo, "v2", "v1", "-", "-", 100.0, 5.0, "Rua_A"));
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_arestas_grafo(grafo));

    destruir_grafo(grafo);
}

void test_deve_buscar_vertice_mais_proximo(void)
{
    Grafo grafo = criar_grafo();

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v1", 0.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v2", 100.0, 100.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v3", 20.0, 10.0));

    TEST_ASSERT_EQUAL_STRING("v3", obter_id_vertice_grafo(buscar_vertice_mais_proximo_grafo(grafo, 18.0, 12.0)));
    TEST_ASSERT_EQUAL_STRING("v2", obter_id_vertice_grafo(buscar_vertice_mais_proximo_grafo(grafo, 90.0, 95.0)));
    TEST_ASSERT_NULL(buscar_vertice_mais_proximo_grafo(NULL, 0.0, 0.0));

    destruir_grafo(grafo);
}

void test_deve_atualizar_velocidade_das_arestas_em_regiao(void)
{
    Grafo grafo = criar_grafo();
    VerticeGrafo v1;
    VerticeGrafo v2;
    VerticeGrafo v3;
    ArestaGrafo dentro;
    ArestaGrafo fora;

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v1", 10.0, 10.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v2", 20.0, 20.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "v3", 100.0, 100.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "v1", "v2", "-", "-", 10.0, 5.0, "Rua_Dentro"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "v2", "v3", "-", "-", 10.0, 5.0, "Rua_Fora"));

    TEST_ASSERT_EQUAL_INT(1, atualizar_velocidade_arestas_regiao_grafo(grafo, 0.0, 0.0, 30.0, 30.0, 12.0));

    v1 = buscar_vertice_grafo(grafo, "v1");
    v2 = buscar_vertice_grafo(grafo, "v2");
    v3 = buscar_vertice_grafo(grafo, "v3");
    dentro = obter_aresta_vertice_grafo(v1, 0);
    fora = obter_aresta_vertice_grafo(v2, 0);

    TEST_ASSERT_NOT_NULL(v3);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 12.0, obter_velocidade_aresta_grafo(dentro));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 5.0, obter_velocidade_aresta_grafo(fora));

    destruir_grafo(grafo);
}

void test_deve_retornar_valores_neutros_para_entradas_nulas(void)
{
    TEST_ASSERT_EQUAL_INT(0, inserir_vertice_grafo(NULL, "v1", 10.0, 20.0));
    TEST_ASSERT_EQUAL_INT(0, inserir_aresta_grafo(NULL, "v1", "v2", "-", "-", 1.0, 1.0, "Rua"));
    TEST_ASSERT_NULL(buscar_vertice_grafo(NULL, "v1"));
    TEST_ASSERT_NULL(obter_vertice_grafo(NULL, 0));
    TEST_ASSERT_NULL(obter_aresta_vertice_grafo(NULL, 0));
    TEST_ASSERT_NULL(buscar_aresta_entre_vertices_grafo(NULL, NULL));
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_vertices_grafo(NULL));
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_arestas_grafo(NULL));
    TEST_ASSERT_EQUAL_INT(0, obter_grau_saida_vertice_grafo(NULL));
    TEST_ASSERT_EQUAL_INT(0, atualizar_velocidade_arestas_regiao_grafo(NULL, 0.0, 0.0, 10.0, 10.0, 5.0));
    TEST_ASSERT_NULL(obter_id_vertice_grafo(NULL));
    TEST_ASSERT_NULL(obter_origem_aresta_grafo(NULL));
    TEST_ASSERT_NULL(obter_destino_aresta_grafo(NULL));
    TEST_ASSERT_NULL(obter_nome_aresta_grafo(NULL));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_criar_grafo_vazio);
    RUN_TEST(test_deve_inserir_e_buscar_vertices);
    RUN_TEST(test_deve_inserir_aresta_direcionada);
    RUN_TEST(test_deve_rejeitar_aresta_com_vertices_inexistentes);
    RUN_TEST(test_deve_buscar_vertice_mais_proximo);
    RUN_TEST(test_deve_atualizar_velocidade_das_arestas_em_regiao);
    RUN_TEST(test_deve_retornar_valores_neutros_para_entradas_nulas);
    return UNITY_END();
}
