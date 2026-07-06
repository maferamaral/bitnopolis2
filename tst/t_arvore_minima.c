#include "unity.h"
#include "arvore_minima.h"
#include "grafo.h"

void setUp(void)
{
}

void tearDown(void)
{
}

static Grafo criar_grafo_arvore(void)
{
    Grafo grafo = criar_grafo();

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "A", 0.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "B", 10.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "C", 20.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "D", 30.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "A", "B", "-", "-", 10.0, 4.0, "Rua_AB"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "B", "C", "-", "-", 10.0, 8.0, "Rua_BC"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "C", "D", "-", "-", 10.0, 2.0, "Rua_CD"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "A", "D", "-", "-", 100.0, 1.0, "Rua_AD"));
    return grafo;
}

void test_deve_expandir_arestas_lentas_da_arvore_minima(void)
{
    Grafo grafo = criar_grafo_arvore();
    VerticeGrafo a = buscar_vertice_grafo(grafo, "A");
    VerticeGrafo b = buscar_vertice_grafo(grafo, "B");
    VerticeGrafo c = buscar_vertice_grafo(grafo, "C");
    ArestaGrafo ab = obter_aresta_vertice_grafo(a, 0);
    ArestaGrafo bc = obter_aresta_vertice_grafo(b, 0);
    ArestaGrafo cd = obter_aresta_vertice_grafo(c, 0);

    TEST_ASSERT_EQUAL_INT(2, expandir_infraestrutura_arvore_minima(grafo, 5.0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 6.0, obter_velocidade_aresta_grafo(ab));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 8.0, obter_velocidade_aresta_grafo(bc));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 3.0, obter_velocidade_aresta_grafo(cd));

    destruir_grafo(grafo);
}

void test_deve_informar_arestas_expandidas(void)
{
    Grafo grafo = criar_grafo_arvore();
    VerticeGrafo a = buscar_vertice_grafo(grafo, "A");
    VerticeGrafo c = buscar_vertice_grafo(grafo, "C");
    ArestaGrafo ab = obter_aresta_vertice_grafo(a, 0);
    ArestaGrafo cd = obter_aresta_vertice_grafo(c, 0);
    ArestaGrafo expandidas[4];

    TEST_ASSERT_EQUAL_INT(2, expandir_infraestrutura_arvore_minima_com_arestas(grafo, 5.0, expandidas, 4));
    TEST_ASSERT_EQUAL_PTR(ab, expandidas[0]);
    TEST_ASSERT_EQUAL_PTR(cd, expandidas[1]);

    destruir_grafo(grafo);
}

void test_deve_ignorar_aresta_lenta_fora_da_arvore_minima(void)
{
    Grafo grafo = criar_grafo_arvore();
    VerticeGrafo a = buscar_vertice_grafo(grafo, "A");
    ArestaGrafo ad = obter_aresta_vertice_grafo(a, 1);

    TEST_ASSERT_EQUAL_INT(2, expandir_infraestrutura_arvore_minima(grafo, 5.0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 1.0, obter_velocidade_aresta_grafo(ad));

    destruir_grafo(grafo);
}

void test_deve_rejeitar_entradas_invalidas_arvore_minima(void)
{
    TEST_ASSERT_EQUAL_INT(0, expandir_infraestrutura_arvore_minima(NULL, 5.0));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_expandir_arestas_lentas_da_arvore_minima);
    RUN_TEST(test_deve_informar_arestas_expandidas);
    RUN_TEST(test_deve_ignorar_aresta_lenta_fora_da_arvore_minima);
    RUN_TEST(test_deve_rejeitar_entradas_invalidas_arvore_minima);
    return UNITY_END();
}
