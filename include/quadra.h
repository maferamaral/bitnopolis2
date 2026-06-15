#ifndef QUADRA_H
#define QUADRA_H

typedef void *Quadra;

Quadra criar_quadra(const char *cep, double x, double y, double largura, double altura);
void destruir_quadra(Quadra quadra);

void definir_estilo_quadra(Quadra quadra, double espessura, const char *preenchimento, const char *borda);

const char *obter_cep_quadra(Quadra quadra);
double obter_x_quadra(Quadra quadra);
double obter_y_quadra(Quadra quadra);
double obter_largura_quadra(Quadra quadra);
double obter_altura_quadra(Quadra quadra);
double obter_espessura_quadra(Quadra quadra);
const char *obter_preenchimento_quadra(Quadra quadra);
const char *obter_borda_quadra(Quadra quadra);

#endif
