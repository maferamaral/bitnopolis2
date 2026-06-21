#ifndef GRAFO_H
#define GRAFO_H

typedef void *Grafo;
typedef void *VerticeGrafo;
typedef void *ArestaGrafo;

Grafo criar_grafo(void);
void destruir_grafo(Grafo grafo);

int inserir_vertice_grafo(Grafo grafo, const char *id, double x, double y);
int inserir_aresta_grafo(
    Grafo grafo,
    const char *origem,
    const char *destino,
    const char *cep_direita,
    const char *cep_esquerda,
    double comprimento,
    double velocidade_media,
    const char *nome
);

VerticeGrafo buscar_vertice_grafo(Grafo grafo, const char *id);
VerticeGrafo obter_vertice_grafo(Grafo grafo, int indice);
ArestaGrafo obter_aresta_vertice_grafo(VerticeGrafo vertice, int indice);

int obter_quantidade_vertices_grafo(Grafo grafo);
int obter_quantidade_arestas_grafo(Grafo grafo);
int obter_grau_saida_vertice_grafo(VerticeGrafo vertice);

const char *obter_id_vertice_grafo(VerticeGrafo vertice);
double obter_x_vertice_grafo(VerticeGrafo vertice);
double obter_y_vertice_grafo(VerticeGrafo vertice);

VerticeGrafo obter_origem_aresta_grafo(ArestaGrafo aresta);
VerticeGrafo obter_destino_aresta_grafo(ArestaGrafo aresta);
const char *obter_cep_direita_aresta_grafo(ArestaGrafo aresta);
const char *obter_cep_esquerda_aresta_grafo(ArestaGrafo aresta);
double obter_comprimento_aresta_grafo(ArestaGrafo aresta);
double obter_velocidade_aresta_grafo(ArestaGrafo aresta);
const char *obter_nome_aresta_grafo(ArestaGrafo aresta);

#endif
