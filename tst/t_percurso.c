#include "unity.h"
#include "percurso.h"
#include "grafo.h"
#include "registrador.h"

void setUp(void)
{
}

void tearDown(void)
{
}

static Grafo criar_grafo_percurso(void)
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

void test_deve_calcular_percurso_curto_e_rapido_entre_registradores(void)
{
    Grafo grafo = criar_grafo_percurso();
    Registradores registradores = criar_registradores();
    Percurso percurso;
    ResultadoDijkstra curto;
    ResultadoDijkstra rapido;

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_EQUAL_INT(1, definir_registrador_ponto(registradores, 0, 1.0, 1.0));
    TEST_ASSERT_EQUAL_INT(1, definir_registrador_ponto(registradores, 1, 19.0, 1.0));

    percurso = calcular_percurso_registradores(grafo, registradores, 0, 1);
    TEST_ASSERT_NOT_NULL(percurso);
    TEST_ASSERT_EQUAL_INT(1, percurso_existe(percurso));
    TEST_ASSERT_EQUAL_STRING("A", obter_id_vertice_grafo(obter_origem_percurso(percurso)));
    TEST_ASSERT_EQUAL_STRING("D", obter_id_vertice_grafo(obter_destino_percurso(percurso)));

    curto = obter_percurso_curto(percurso);
    rapido = obter_percurso_rapido(percurso);

    TEST_ASSERT_NOT_NULL(curto);
    TEST_ASSERT_NOT_NULL(rapido);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 20.0, obter_custo_dijkstra(curto));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 2.0, obter_custo_dijkstra(rapido));
    TEST_ASSERT_EQUAL_STRING("B", obter_id_vertice_grafo(obter_vertice_dijkstra(curto, 1)));
    TEST_ASSERT_EQUAL_STRING("C", obter_id_vertice_grafo(obter_vertice_dijkstra(rapido, 1)));

    destruir_percurso(percurso);
    destruir_registradores(registradores);
    destruir_grafo(grafo);
}

void test_deve_rejeitar_registrador_nao_definido(void)
{
    Grafo grafo = criar_grafo_percurso();
    Registradores registradores = criar_registradores();

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_EQUAL_INT(1, definir_registrador_ponto(registradores, 0, 1.0, 1.0));
    TEST_ASSERT_NULL(calcular_percurso_registradores(grafo, registradores, 0, 1));

    destruir_registradores(registradores);
    destruir_grafo(grafo);
}

void test_deve_rejeitar_entradas_invalidas(void)
{
    Grafo grafo = criar_grafo_percurso();
    Registradores registradores = criar_registradores();

    TEST_ASSERT_NOT_NULL(registradores);
    TEST_ASSERT_NULL(calcular_percurso_registradores(NULL, registradores, 0, 1));
    TEST_ASSERT_NULL(calcular_percurso_registradores(grafo, NULL, 0, 1));
    TEST_ASSERT_EQUAL_INT(0, percurso_existe(NULL));
    TEST_ASSERT_NULL(obter_percurso_curto(NULL));
    TEST_ASSERT_NULL(obter_percurso_rapido(NULL));
    TEST_ASSERT_NULL(obter_origem_percurso(NULL));
    TEST_ASSERT_NULL(obter_destino_percurso(NULL));

    destruir_registradores(registradores);
    destruir_grafo(grafo);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deve_calcular_percurso_curto_e_rapido_entre_registradores);
    RUN_TEST(test_deve_rejeitar_registrador_nao_definido);
    RUN_TEST(test_deve_rejeitar_entradas_invalidas);
    return UNITY_END();
}
