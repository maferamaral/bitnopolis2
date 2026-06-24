#include "unity.h"
#include "dijkstra.h"
#include "grafo.h"

void setUp(void)
{
}

void tearDown(void)
{
}

static Grafo criar_grafo_de_rotas(void)
{
    Grafo grafo = criar_grafo();

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "A", 0.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "B", 1.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "C", 2.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "D", 3.0, 0.0));
    TEST_ASSERT_EQUAL_INT(1, inserir_vertice_grafo(grafo, "E", 4.0, 0.0));

    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "A", "B", "-", "-", 10.0, 1.0, "Rua_AB"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "B", "D", "-", "-", 10.0, 1.0, "Rua_BD"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "A", "C", "-", "-", 30.0, 30.0, "Rua_AC"));
    TEST_ASSERT_EQUAL_INT(1, inserir_aresta_grafo(grafo, "C", "D", "-", "-", 30.0, 30.0, "Rua_CD"));

    return grafo;
}

void test_deve_calcular_menor_caminho_por_distancia(void)
{
    Grafo grafo = criar_grafo_de_rotas();
    ResultadoDijkstra resultado = calcular_dijkstra(grafo, "A", "D", peso_distancia_aresta);

    TEST_ASSERT_NOT_NULL(resultado);
    TEST_ASSERT_EQUAL_INT(1, caminho_dijkstra_existe(resultado));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 20.0, obter_custo_dijkstra(resultado));
    TEST_ASSERT_EQUAL_INT(3, obter_quantidade_vertices_dijkstra(resultado));
    TEST_ASSERT_EQUAL_STRING("A", obter_id_vertice_grafo(obter_vertice_dijkstra(resultado, 0)));
    TEST_ASSERT_EQUAL_STRING("B", obter_id_vertice_grafo(obter_vertice_dijkstra(resultado, 1)));
    TEST_ASSERT_EQUAL_STRING("D", obter_id_vertice_grafo(obter_vertice_dijkstra(resultado, 2)));

    destruir_resultado_dijkstra(resultado);
    destruir_grafo(grafo);
}

void test_deve_calcular_menor_caminho_por_tempo(void)
{
    Grafo grafo = criar_grafo_de_rotas();
    ResultadoDijkstra resultado = calcular_dijkstra(grafo, "A", "D", peso_tempo_aresta);

    TEST_ASSERT_NOT_NULL(resultado);
    TEST_ASSERT_EQUAL_INT(1, caminho_dijkstra_existe(resultado));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 2.0, obter_custo_dijkstra(resultado));
    TEST_ASSERT_EQUAL_INT(3, obter_quantidade_vertices_dijkstra(resultado));
    TEST_ASSERT_EQUAL_STRING("A", obter_id_vertice_grafo(obter_vertice_dijkstra(resultado, 0)));
    TEST_ASSERT_EQUAL_STRING("C", obter_id_vertice_grafo(obter_vertice_dijkstra(resultado, 1)));
    TEST_ASSERT_EQUAL_STRING("D", obter_id_vertice_grafo(obter_vertice_dijkstra(resultado, 2)));

    destruir_resultado_dijkstra(resultado);
    destruir_grafo(grafo);
}

void test_deve_indicar_destino_inacessivel(void)
{
    Grafo grafo = criar_grafo_de_rotas();
    ResultadoDijkstra resultado = calcular_dijkstra(grafo, "A", "E", peso_distancia_aresta);

    TEST_ASSERT_NOT_NULL(resultado);
    TEST_ASSERT_EQUAL_INT(0, caminho_dijkstra_existe(resultado));
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_vertices_dijkstra(resultado));
    TEST_ASSERT_NULL(obter_vertice_dijkstra(resultado, 0));

    destruir_resultado_dijkstra(resultado);
    destruir_grafo(grafo);
}

void test_deve_rejeitar_entradas_invalidas(void)
{
    Grafo grafo = criar_grafo_de_rotas();

    TEST_ASSERT_NULL(calcular_dijkstra(NULL, "A", "D", peso_distancia_aresta));
    TEST_ASSERT_NULL(calcular_dijkstra(grafo, NULL, "D", peso_distancia_aresta));
    TEST_ASSERT_NULL(calcular_dijkstra(grafo, "A", NULL, peso_distancia_aresta));
    TEST_ASSERT_NULL(calcular_dijkstra(grafo, "A", "D", NULL));
    TEST_ASSERT_NULL(calcular_dijkstra(grafo, "X", "D", peso_distancia_aresta));
    TEST_ASSERT_EQUAL_INT(0, caminho_dijkstra_existe(NULL));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, obter_custo_dijkstra(NULL));
    TEST_ASSERT_EQUAL_INT(0, obter_quantidade_vertices_dijkstra(NULL));
    TEST_ASSERT_NULL(obter_vertice_dijkstra(NULL, 0));

    destruir_grafo(grafo);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_calcular_menor_caminho_por_distancia);
    RUN_TEST(test_deve_calcular_menor_caminho_por_tempo);
    RUN_TEST(test_deve_indicar_destino_inacessivel);
    RUN_TEST(test_deve_rejeitar_entradas_invalidas);
    return UNITY_END();
}
