#include "registrador.h"

#include <stdlib.h>

struct registrador {
    int definido;
    double x;
    double y;
};

struct registradores {
    struct registrador itens[QUANTIDADE_REGISTRADORES];
};

static int indice_valido(int indice)
{
    return indice >= 0 && indice < QUANTIDADE_REGISTRADORES;
}

Registradores criar_registradores(void)
{
    struct registradores *registradores = calloc(1, sizeof(*registradores));

    return registradores;
}

void destruir_registradores(Registradores registradores)
{
    free(registradores);
}

int definir_registrador_ponto(Registradores registradores_generico, int indice, double x, double y)
{
    struct registradores *registradores = registradores_generico;

    if (registradores == NULL || !indice_valido(indice)) {
        return 0;
    }

    registradores->itens[indice].definido = 1;
    registradores->itens[indice].x = x;
    registradores->itens[indice].y = y;

    return 1;
}

int definir_registrador_endereco(Registradores registradores, int indice, Quadra quadra, char face, double numero)
{
    double x_leste;
    double x_oeste;
    double y_sul;
    double y_norte;

    if (quadra == NULL) {
        return 0;
    }

    x_leste = obter_x_quadra(quadra);
    x_oeste = x_leste - obter_largura_quadra(quadra);
    y_sul = obter_y_quadra(quadra);
    y_norte = y_sul + obter_altura_quadra(quadra);

    if (face == 'N') {
        return definir_registrador_ponto(registradores, indice, x_leste - numero, y_norte);
    }

    if (face == 'S') {
        return definir_registrador_ponto(registradores, indice, x_leste - numero, y_sul);
    }

    if (face == 'L') {
        return definir_registrador_ponto(registradores, indice, x_leste, y_sul + numero);
    }

    if (face == 'O') {
        return definir_registrador_ponto(registradores, indice, x_oeste, y_sul + numero);
    }

    return 0;
}

int registrador_esta_definido(Registradores registradores_generico, int indice)
{
    struct registradores *registradores = registradores_generico;

    if (registradores == NULL || !indice_valido(indice)) {
        return 0;
    }

    return registradores->itens[indice].definido;
}

double obter_x_registrador(Registradores registradores_generico, int indice)
{
    struct registradores *registradores = registradores_generico;

    if (registradores == NULL || !indice_valido(indice) || !registradores->itens[indice].definido) {
        return 0.0;
    }

    return registradores->itens[indice].x;
}

double obter_y_registrador(Registradores registradores_generico, int indice)
{
    struct registradores *registradores = registradores_generico;

    if (registradores == NULL || !indice_valido(indice) || !registradores->itens[indice].definido) {
        return 0.0;
    }

    return registradores->itens[indice].y;
}
