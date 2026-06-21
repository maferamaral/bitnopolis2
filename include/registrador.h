#ifndef REGISTRADOR_H
#define REGISTRADOR_H

#include "quadra.h"

#define QUANTIDADE_REGISTRADORES 11

typedef void *Registradores;

Registradores criar_registradores(void);
void destruir_registradores(Registradores registradores);

int definir_registrador_ponto(Registradores registradores, int indice, double x, double y);
int definir_registrador_endereco(Registradores registradores, int indice, Quadra quadra, char face, double numero);

int registrador_esta_definido(Registradores registradores, int indice);
double obter_x_registrador(Registradores registradores, int indice);
double obter_y_registrador(Registradores registradores, int indice);

#endif
