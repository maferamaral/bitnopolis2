#ifndef CONSULTA_H
#define CONSULTA_H

#include "cidade.h"
#include "grafo.h"

/* Processa comandos QRY e acrescenta seus efeitos aos arquivos TXT e SVG. */
int processar_arquivo_consulta(const char *caminho_qry, Cidade cidade, Grafo grafo, const char *caminho_txt, const char *caminho_svg);

#endif
