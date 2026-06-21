PROJ_NAME = ted

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration
CPPFLAGS = -I../include -I../unity
LDFLAGS = -O0

SRC_DIR = .
TST_DIR = ../tst
UNITY_DIR = ../unity

OBJS = \
	$(SRC_DIR)/main.o \
	$(SRC_DIR)/argumentos.o \
	$(SRC_DIR)/lista.o \
	$(SRC_DIR)/quadra.o \
	$(SRC_DIR)/cidade.o \
	$(SRC_DIR)/geo.o \
	$(SRC_DIR)/grafo.o \
	$(SRC_DIR)/via.o \
	$(SRC_DIR)/registrador.o

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(PROJ_NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

t_argumentos: $(SRC_DIR)/argumentos.o $(TST_DIR)/t_argumentos.c $(UNITY_DIR)/unity.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(TST_DIR)/t_argumentos_run
	$(TST_DIR)/t_argumentos_run

t_lista: $(SRC_DIR)/lista.o $(TST_DIR)/t_lista.c $(UNITY_DIR)/unity.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(TST_DIR)/t_lista_run
	$(TST_DIR)/t_lista_run

t_quadra: $(SRC_DIR)/quadra.o $(TST_DIR)/t_quadra.c $(UNITY_DIR)/unity.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(TST_DIR)/t_quadra_run
	$(TST_DIR)/t_quadra_run

t_cidade: $(SRC_DIR)/cidade.o $(SRC_DIR)/lista.o $(SRC_DIR)/quadra.o $(TST_DIR)/t_cidade.c $(UNITY_DIR)/unity.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(TST_DIR)/t_cidade_run
	$(TST_DIR)/t_cidade_run

t_geo: $(SRC_DIR)/geo.o $(SRC_DIR)/cidade.o $(SRC_DIR)/lista.o $(SRC_DIR)/quadra.o $(TST_DIR)/t_geo.c $(UNITY_DIR)/unity.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(TST_DIR)/t_geo_run
	$(TST_DIR)/t_geo_run

t_grafo: $(SRC_DIR)/grafo.o $(SRC_DIR)/lista.o $(TST_DIR)/t_grafo.c $(UNITY_DIR)/unity.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(TST_DIR)/t_grafo_run
	$(TST_DIR)/t_grafo_run

t_via: $(SRC_DIR)/via.o $(SRC_DIR)/grafo.o $(SRC_DIR)/lista.o $(TST_DIR)/t_via.c $(UNITY_DIR)/unity.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(TST_DIR)/t_via_run
	$(TST_DIR)/t_via_run

t_registrador: $(SRC_DIR)/registrador.o $(SRC_DIR)/quadra.o $(TST_DIR)/t_registrador.c $(UNITY_DIR)/unity.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(TST_DIR)/t_registrador_run
	$(TST_DIR)/t_registrador_run

tstall: t_argumentos t_lista t_quadra t_cidade t_geo t_grafo t_via t_registrador

clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(PROJ_NAME)
	rm -f $(TST_DIR)/*_run

.PHONY: all clean tstall t_argumentos t_lista t_quadra t_cidade t_geo t_grafo t_via t_registrador
