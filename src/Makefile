PROJ_NAME = ted

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration
LDFLAGS = -O0

SRC_DIR = src
TST_DIR = tst

OBJS = \
	$(SRC_DIR)/main.o \
	$(SRC_DIR)/path.o \
	$(SRC_DIR)/geo.o \
	$(SRC_DIR)/via.o \
	$(SRC_DIR)/qry.o \
	$(SRC_DIR)/quadra.o \
	$(SRC_DIR)/ponto.o \
	$(SRC_DIR)/grafo.o \
	$(SRC_DIR)/vertice.o \
	$(SRC_DIR)/aresta.o \
	$(SRC_DIR)/lista.o \
	$(SRC_DIR)/fila_prioridade.o \
	$(SRC_DIR)/dijkstra.o \
	$(SRC_DIR)/componentes.o \
	$(SRC_DIR)/mst.o \
	$(SRC_DIR)/registrador.o \
	$(SRC_DIR)/svg.o \
	$(SRC_DIR)/txt.o

all: ted

ted: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(SRC_DIR)/$(PROJ_NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(SRC_DIR)/$(PROJ_NAME)
	rm -f $(TST_DIR)/*.o
	rm -f $(TST_DIR)/*_run

# =========================
# TESTES UNITÁRIOS
# =========================

t_grafo: $(SRC_DIR)/grafo.o $(SRC_DIR)/lista.o $(SRC_DIR)/vertice.o $(SRC_DIR)/aresta.o $(TST_DIR)/t_grafo.c
	$(CC) $(CFLAGS) $^ -o $(TST_DIR)/t_grafo_run
	./$(TST_DIR)/t_grafo_run

t_fila_prioridade: $(SRC_DIR)/fila_prioridade.o $(TST_DIR)/t_fila_prioridade.c
	$(CC) $(CFLAGS) $^ -o $(TST_DIR)/t_fila_prioridade_run
	./$(TST_DIR)/t_fila_prioridade_run

t_dijkstra: $(SRC_DIR)/dijkstra.o $(SRC_DIR)/grafo.o $(SRC_DIR)/lista.o $(SRC_DIR)/fila_prioridade.o $(SRC_DIR)/vertice.o $(SRC_DIR)/aresta.o $(TST_DIR)/t_dijkstra.c
	$(CC) $(CFLAGS) $^ -o $(TST_DIR)/t_dijkstra_run
	./$(TST_DIR)/t_dijkstra_run

t_componentes: $(SRC_DIR)/componentes.o $(SRC_DIR)/grafo.o $(SRC_DIR)/lista.o $(SRC_DIR)/vertice.o $(SRC_DIR)/aresta.o $(TST_DIR)/t_componentes.c
	$(CC) $(CFLAGS) $^ -o $(TST_DIR)/t_componentes_run
	./$(TST_DIR)/t_componentes_run

t_mst: $(SRC_DIR)/mst.o $(SRC_DIR)/grafo.o $(SRC_DIR)/lista.o $(SRC_DIR)/fila_prioridade.o $(SRC_DIR)/vertice.o $(SRC_DIR)/aresta.o $(TST_DIR)/t_mst.c
	$(CC) $(CFLAGS) $^ -o $(TST_DIR)/t_mst_run
	./$(TST_DIR)/t_mst_run

t_geo: $(SRC_DIR)/geo.o $(SRC_DIR)/quadra.o $(SRC_DIR)/ponto.o $(SRC_DIR)/lista.o $(TST_DIR)/t_geo.c
	$(CC) $(CFLAGS) $^ -o $(TST_DIR)/t_geo_run
	./$(TST_DIR)/t_geo_run

t_via: $(SRC_DIR)/via.o $(SRC_DIR)/grafo.o $(SRC_DIR)/lista.o $(SRC_DIR)/vertice.o $(SRC_DIR)/aresta.o $(TST_DIR)/t_via.c
	$(CC) $(CFLAGS) $^ -o $(TST_DIR)/t_via_run
	./$(TST_DIR)/t_via_run

t_path: $(SRC_DIR)/path.o $(TST_DIR)/t_path.c
	$(CC) $(CFLAGS) $^ -o $(TST_DIR)/t_path_run
	./$(TST_DIR)/t_path_run

tstall: t_path t_grafo t_fila_prioridade t_dijkstra t_componentes t_mst t_geo t_via

.PHONY: all clean ted tstall t_grafo t_fila_prioridade t_dijkstra t_componentes t_mst t_geo t_via t_path