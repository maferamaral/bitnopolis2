#include "argumentos.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    Argumentos argumentos = criar_argumentos();

    if (argumentos == NULL) {
        fprintf(stderr, "Erro: nao foi possivel iniciar os argumentos.\n");
        return 1;
    }

    if (!ler_argumentos(argumentos, argc, argv)) {
        fprintf(stderr, "Uso: ted [-e path] -f arq.geo [-q consulta.qry] [-v arqvias.via] -o dir\n");
        destruir_argumentos(argumentos);
        return 1;
    }

    destruir_argumentos(argumentos);
    return 0;
}
