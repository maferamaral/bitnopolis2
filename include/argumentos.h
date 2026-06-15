#ifndef ARGUMENTOS_H
#define ARGUMENTOS_H

typedef void *Argumentos;

Argumentos criar_argumentos(void);
void destruir_argumentos(Argumentos argumentos);

int ler_argumentos(Argumentos argumentos, int argc, char **argv);

const char *obter_diretorio_entrada(Argumentos argumentos);
const char *obter_arquivo_geo(Argumentos argumentos);
const char *obter_arquivo_consulta(Argumentos argumentos);
const char *obter_arquivo_vias(Argumentos argumentos);
const char *obter_diretorio_saida(Argumentos argumentos);

int argumentos_possuem_consulta(Argumentos argumentos);
int argumentos_possuem_vias(Argumentos argumentos);

#endif
