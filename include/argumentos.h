#ifndef ARGUMENTOS_H
#define ARGUMENTOS_H

/* TAD responsavel por validar e guardar as opcoes da linha de comando. */
typedef void *Argumentos;

Argumentos criar_argumentos(void);
void destruir_argumentos(Argumentos argumentos);

/* Retorna 1 quando os argumentos obrigatorios foram informados. */
int ler_argumentos(Argumentos argumentos, int argc, char **argv);

/* Os ponteiros retornados apontam para strings recebidas em argv. */
const char *obter_diretorio_entrada(Argumentos argumentos);
const char *obter_arquivo_geo(Argumentos argumentos);
const char *obter_arquivo_consulta(Argumentos argumentos);
const char *obter_arquivo_vias(Argumentos argumentos);
const char *obter_diretorio_saida(Argumentos argumentos);

/* Indicam a presenca dos parametros opcionais -q e -v. */
int argumentos_possuem_consulta(Argumentos argumentos);
int argumentos_possuem_vias(Argumentos argumentos);

#endif
