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
	$(SRC_DIR)/lista.o

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

tstall: t_argumentos t_lista

clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(PROJ_NAME)
	rm -f $(TST_DIR)/*_run

.PHONY: all clean tstall t_argumentos t_lista
