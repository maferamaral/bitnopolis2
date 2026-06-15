#include "argumentos.h"

#include <stdlib.h>
#include <string.h>

struct argumentos {
    const char *diretorio_entrada;
    const char *arquivo_geo;
    const char *arquivo_consulta;
    const char *arquivo_vias;
    const char *diretorio_saida;
};

static int eh_opcao_com_valor(const char *opcao)
{
    return strcmp(opcao, "-e") == 0 ||
           strcmp(opcao, "-f") == 0 ||
           strcmp(opcao, "-q") == 0 ||
           strcmp(opcao, "-v") == 0 ||
           strcmp(opcao, "-o") == 0;
}

Argumentos criar_argumentos(void)
{
    struct argumentos *argumentos = malloc(sizeof(*argumentos));

    if (argumentos == NULL) {
        return NULL;
    }

    argumentos->diretorio_entrada = NULL;
    argumentos->arquivo_geo = NULL;
    argumentos->arquivo_consulta = NULL;
    argumentos->arquivo_vias = NULL;
    argumentos->diretorio_saida = NULL;

    return argumentos;
}

void destruir_argumentos(Argumentos argumentos)
{
    free(argumentos);
}

int ler_argumentos(Argumentos argumentos_generico, int argc, char **argv)
{
    struct argumentos *argumentos = argumentos_generico;
    int i;

    if (argumentos == NULL || argv == NULL) {
        return 0;
    }

    argumentos->diretorio_entrada = NULL;
    argumentos->arquivo_geo = NULL;
    argumentos->arquivo_consulta = NULL;
    argumentos->arquivo_vias = NULL;
    argumentos->diretorio_saida = NULL;

    for (i = 1; i < argc; i += 2) {
        if (i + 1 >= argc || !eh_opcao_com_valor(argv[i])) {
            return 0;
        }

        if (strcmp(argv[i], "-e") == 0) {
            argumentos->diretorio_entrada = argv[i + 1];
        } else if (strcmp(argv[i], "-f") == 0) {
            argumentos->arquivo_geo = argv[i + 1];
        } else if (strcmp(argv[i], "-q") == 0) {
            argumentos->arquivo_consulta = argv[i + 1];
        } else if (strcmp(argv[i], "-v") == 0) {
            argumentos->arquivo_vias = argv[i + 1];
        } else if (strcmp(argv[i], "-o") == 0) {
            argumentos->diretorio_saida = argv[i + 1];
        }
    }

    return argumentos->arquivo_geo != NULL && argumentos->diretorio_saida != NULL;
}

const char *obter_diretorio_entrada(Argumentos argumentos_generico)
{
    struct argumentos *argumentos = argumentos_generico;

    if (argumentos == NULL) {
        return NULL;
    }

    return argumentos->diretorio_entrada;
}

const char *obter_arquivo_geo(Argumentos argumentos_generico)
{
    struct argumentos *argumentos = argumentos_generico;

    if (argumentos == NULL) {
        return NULL;
    }

    return argumentos->arquivo_geo;
}

const char *obter_arquivo_consulta(Argumentos argumentos_generico)
{
    struct argumentos *argumentos = argumentos_generico;

    if (argumentos == NULL) {
        return NULL;
    }

    return argumentos->arquivo_consulta;
}

const char *obter_arquivo_vias(Argumentos argumentos_generico)
{
    struct argumentos *argumentos = argumentos_generico;

    if (argumentos == NULL) {
        return NULL;
    }

    return argumentos->arquivo_vias;
}

const char *obter_diretorio_saida(Argumentos argumentos_generico)
{
    struct argumentos *argumentos = argumentos_generico;

    if (argumentos == NULL) {
        return NULL;
    }

    return argumentos->diretorio_saida;
}

int argumentos_possuem_consulta(Argumentos argumentos_generico)
{
    struct argumentos *argumentos = argumentos_generico;

    return argumentos != NULL && argumentos->arquivo_consulta != NULL;
}

int argumentos_possuem_vias(Argumentos argumentos_generico)
{
    struct argumentos *argumentos = argumentos_generico;

    return argumentos != NULL && argumentos->arquivo_vias != NULL;
}
