#ifndef FILA_PRIORIDADE_H
#define FILA_PRIORIDADE_H

/* Fila minima usada por algoritmos de caminhos; nao assume a posse dos itens. */
typedef void *FilaPrioridade;

FilaPrioridade criar_fila_prioridade(void);
void destruir_fila_prioridade(FilaPrioridade fila);

/* Menor valor de prioridade sai primeiro. */
int inserir_fila_prioridade(FilaPrioridade fila, void *item, double prioridade);
void *remover_minimo_fila_prioridade(FilaPrioridade fila);

int fila_prioridade_esta_vazia(FilaPrioridade fila);
int obter_tamanho_fila_prioridade(FilaPrioridade fila);

#endif
