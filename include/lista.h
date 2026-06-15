#ifndef LISTA_H
#define LISTA_H

typedef void *Lista;

Lista criar_lista(void);
void destruir_lista(Lista lista, void (*destruir_item)(void *item));

int inserir_lista(Lista lista, void *item);
void *obter_item_lista(Lista lista, int indice);
int obter_tamanho_lista(Lista lista);
int lista_esta_vazia(Lista lista);

#endif
