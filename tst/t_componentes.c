#include "unity.h"
#include "componentes.h"
#include "grafo.h"

void setUp(void)
{
}

void tearDown(void)
{
}

static Grafo criar_grafo_componentes(void)
{
    Grafo grafo = criar_grafo();

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "A", 0.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "B", 10.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "C", 20.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "D", 30.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "E", 40.0, 0.0));

    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "A", "B", "-", "-", 10.0, 3.0, "Rua_AB"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "B", "C", "-", "-", 10.0, 8.0, "Rua_BC"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "C", "D", "-", "-", 10.0, 2.0, "Rua_CD"));

    return grafo;
}

void test_deve_calcular_componentes_de_arestas_lentas(void)
{
    Grafo grafo = criar_grafo_componentes();
    Componentes componentes = calcular_componentes_lentas(grafo, 5.0);
    int componente_a;
    int componente_b;
    int componente_c;
    int componente_d;

    TEST_ASSERT_NOT_NULL(componentes);
    TEST_ASSERT_EQUAL_INT(2, obter_quantidade_componentes(componentes));

    componente_a = obter_componente_vertice(componentes, buscar_vertice_grafo(grafo, "A"));
    componente_b = obter_componente_vertice(componentes, buscar_vertice_grafo(grafo, "B"));
    componente_c = obter_componente_vertice(componentes, buscar_vertice_grafo(grafo, "C"));
    componente_d = obter_componente_vertice(componentes, buscar_vertice_grafo(grafo, "D"));

    TEST_ASSERT_TRUE(componente_a >= 0);
    TEST_ASSERT_EQUAL_INT(componente_a, componente_b);
    TEST_ASSERT_TRUE(componente_c >= 0);
    TEST_ASSERT_EQUAL_INT(componente_c, componente_d);
    TEST_ASSERT_NOT_EQUAL(componente_a, componente_c);
    TEST_ASSERT_EQUAL_INT(-1, obter_componente_vertice(componentes, buscar_vertice_grafo(grafo, "E")));

    destruir_componentes(componentes);
    destruir_grafo(grafo);
}

void test_deve_retornar_zero_quando_nao_ha_arestas_lentas(void)
{
    Grafo grafo = criar_grafo_componentes();
    Componentes componentes = calcular_componentes_lentas(grafo, 1.0);

    TEST_ASSERT_NOT_NULL(componentes);
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_componentes(componentes));
    TEST_ASSERT_EQUAL_INT(-1, obter_componente_vertice(componentes, buscar_vertice_grafo(grafo, "A")));

    destruir_componentes(componentes);
    destruir_grafo(grafo);
}

void test_deve_rejeitar_entradas_invalidas_componentes(void)
{
    TEST_ASSERT_NULL(calcular_componentes_lentas(NULL, 5.0));
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_componentes(NULL));
    TEST_ASSERT_EQUAL_INT(-1, obter_componente_vertice(NULL, NULL));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_calcular_componentes_de_arestas_lentas);
    RUN_TEST(test_deve_retornar_zero_quando_nao_ha_arestas_lentas);
    RUN_TEST(test_deve_rejeitar_entradas_invalidas_componentes);
    return UNITY_END();
}
