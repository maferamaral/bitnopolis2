#ifndef SAIDA_TEXTO_H
#define SAIDA_TEXTO_H

#include "cidade.h"

int escrever_relatorio_cidade_texto(const char *caminho_txt, Cidade cidade);
int acrescentar_linha_texto(const char *caminho_txt, const char *linha);

#endif
