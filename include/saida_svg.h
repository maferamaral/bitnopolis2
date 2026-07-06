#ifndef SAIDA_SVG_H
#define SAIDA_SVG_H

#include "cidade.h"
#include "componentes.h"
#include "dijkstra.h"

int iniciar_cidade_svg(const char *caminho_svg, Cidade cidade);
int iniciar_mapa_svg(const char *caminho_svg, Cidade cidade, Grafo grafo);
int finalizar_svg(const char *caminho_svg);
int escrever_cidade_svg(const char *caminho_svg, Cidade cidade);
int acrescentar_registrador_svg(const char *caminho_svg, int indice, double x, double y);
int acrescentar_componentes_svg(const char *caminho_svg, Grafo grafo, Componentes componentes);
int acrescentar_arestas_expandidas_svg(const char *caminho_svg, ArestaGrafo *arestas, int quantidade);
int acrescentar_percurso_svg(const char *caminho_svg, ResultadoDijkstra resultado, const char *cor, const char *id, int animar);
int acrescentar_marcadores_percurso_svg(const char *caminho_svg, ResultadoDijkstra resultado);

#endif
