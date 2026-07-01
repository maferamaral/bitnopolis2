# Bitnopolis 2

Trabalho 2 de Estrutura de Dados II: sistema viario inteligente para leitura de cidade, grafo viario, consultas e geracao de saidas TXT/SVG.

## Estrutura

```text
include/   Headers dos TADs, sempre com ponteiros opacos
src/       Codigo-fonte e Makefile exigido pelo professor
tst/       Testes unitarios com Unity
unity/     Biblioteca Unity
```

Os headers nao declaram `struct`. Os TADs usam tipos opacos com `void *`, e as estruturas internas ficam apenas nos arquivos `.c`.

## Compilacao

O `Makefile` fica dentro de `src`.

```bash
cd src
make
```

O executavel gerado e:

```text
src/ted
```

Para limpar arquivos de compilacao:

```bash
cd src
make clean
```

## Execucao

Formato:

```bash
./ted [-e path] -f arq.geo [-q consulta.qry] [-v arqvias.via] -o dir
```

Exemplo:

```bash
cd src
./ted -e ../data/exemplos -f cidade.geo -v cidade.via -q cidade.qry -o ../out
```

As saidas usam o mesmo nome-base do arquivo GEO:

```text
out/cidade.txt
out/cidade.svg
```

## Testes

Para executar todos os testes unitarios:

```bash
cd src
make tstall
```

Tambem existem alvos individuais, por exemplo:

```bash
make t_grafo
make t_dijkstra
make t_consulta
```

## Comandos implementados

- `cq`: atributos graficos de quadras.
- `q`: leitura de quadras do GEO.
- `@o?`: registrador geografico por endereco.
- `p?`: percurso mais curto e mais rapido entre registradores.
- `mvm`: atualizacao regional de velocidade media.
- `regs`: componentes conexos por arestas inadequadas.
- `exp`: expansao de infraestrutura via arvore geradora minima.
