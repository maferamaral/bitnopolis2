#ifndef CIDADE_H
#define CIDADE_H

#include "quadra.h"

typedef void *Cidade;

Cidade criar_cidade(void);
void destruir_cidade(Cidade cidade);

int inserir_quadra_cidade(Cidade cidade, Quadra quadra);
Quadra buscar_quadra_cidade(Cidade cidade, const char *cep);
Quadra obter_quadra_cidade(Cidade cidade, int indice);
int obter_quantidade_quadras_cidade(Cidade cidade);

#endif
