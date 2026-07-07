#ifndef CIDADE_H
#define CIDADE_H

#include "quadra.h"

/* Colecao de quadras lidas do GEO. A cidade assume a posse das quadras inseridas. */
typedef void *Cidade;

Cidade criar_cidade(void);
void destruir_cidade(Cidade cidade);

/* Insere e consulta quadras por CEP ou pela ordem de leitura. */
int inserir_quadra_cidade(Cidade cidade, Quadra quadra);
Quadra buscar_quadra_cidade(Cidade cidade, const char *cep);
Quadra obter_quadra_cidade(Cidade cidade, int indice);
int obter_quantidade_quadras_cidade(Cidade cidade);

#endif
