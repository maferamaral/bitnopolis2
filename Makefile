PROJ_NAME = src/ted

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration
CPPFLAGS = -Iinclude
LDFLAGS = -O0

SRC_DIR = src

OBJS = \
	$(SRC_DIR)/main.o \
	$(SRC_DIR)/argumentos.o \
	$(SRC_DIR)/lista.o \
	$(SRC_DIR)/quadra.o \
	$(SRC_DIR)/cidade.o \
	$(SRC_DIR)/geo.o \
	$(SRC_DIR)/grafo.o \
	$(SRC_DIR)/via.o \
	$(SRC_DIR)/registrador.o \
	$(SRC_DIR)/saida_texto.o \
	$(SRC_DIR)/saida_svg.o \
	$(SRC_DIR)/fila_prioridade.o \
	$(SRC_DIR)/dijkstra.o \
	$(SRC_DIR)/percurso.o \
	$(SRC_DIR)/consulta.o \
	$(SRC_DIR)/componentes.o \
	$(SRC_DIR)/arvore_minima.o

all: ted

ted: $(PROJ_NAME)

$(PROJ_NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(PROJ_NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(PROJ_NAME)

.PHONY: all ted clean
