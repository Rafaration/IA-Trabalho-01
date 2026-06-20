# Gerador de labirintos para IA/Algorítmos de busca

Gerador de labirintos em Python utilizando **DFS (Depth-First Search)** com saída em:

* Visual ASCII
* Matriz binária (`0` e `1`)
* Arquivos `.csv`

# Características

* Geração procedural de labirintos  
* Algoritmo DFS recursivo  
* Exportação em CSV  
* Representação binária para IA  
* Visualização ASCII  
* Geração em lote  
* Suporte a labirintos quadrados e retangulares  

# Estrutura do Projeto

```text
.
├── maze_maker.py
├── labirintos/
│   ├── maze_4x4.csv
│   ├── maze_5x5.csv
│   └── ...
└── README.md
```

# Representação do Labirinto

O labirinto binário utiliza:

```text
0 = caminho livre
1 = parede
```

Exemplo:

```text
1,0,1,1,1
1,0,0,0,1
1,1,1,0,1
1,0,0,0,1
1,1,1,1,1
```

# Sobre o tamanho do labirinto

Um labirinto lógico `4x4` gera uma matriz real `9x9`.

Isso acontece porque a matriz também armazena:

* paredes
* bordas
* espaços entre células

A fórmula utilizada é:

```python
linhas = altura * 2 + 1
colunas = largura * 2 + 1
```
# Como executar

## Clone o projeto

```bash
git clone https://github.com/seu-usuario/seu-repositorio.git
```

## Entre na pasta

```bash
cd seu-repositorio
```

## Execute o programa

```bash
python maze_maker.py
```

# Funções principais

## `gerar_labirinto()`

Gera um labirinto em ASCII.

### Exemplo

```python
print(gerar_labirinto(4, 4))
```

### Saída

```text
+--+--+--+--+
|        |  |
+  +--+  +  +
|     |     |
+--+--+--+--+
```

# `gerar_labirinto_binario()`

Gera um labirinto em matriz binária.

### Exemplo

```python
maze = gerar_labirinto_binario(4, 4)
```

### Saída

```python
[
 [1,1,1,1,1],
 [0,0,0,0,1],
 [1,1,1,0,1]
]
```

# `converter_labirinto_visualizacao()`

Converte o labirinto binário para visualização ASCII.

### Exemplo

```python
print(converter_labirinto_visualizacao(maze))
```

# `armazena_labirinto()`

Salva o labirinto em um arquivo `.csv`.

### Exemplo

```python
armazena_labirinto(
    "labirintos/maze_4x4.csv",
    'a',
    maze
)
```

# `geracoes_labirintos()`

Gera vários labirintos automaticamente.

## Parâmetros

| Parâmetro                | Descrição                            |
| ------------------------ | ------------------------------------ |
| `quantidade_por_tamanho` | Quantidade de labirintos por tamanho |
| `tamanho_inicial`        | Menor tamanho                        |
| `tamanho_final`          | Maior tamanho                        |
| `apenas_quadrados`       | Define se serão apenas NxN           |

## Exemplo

```python
geracoes_labirintos(
    quantidade_por_tamanho=5,
    tamanho_inicial=5,
    tamanho_final=20,
    apenas_quadrados=True
)
```

# Arquivos gerados

Exemplo:

```text
labirintos/
├── maze_5x5.csv
├── maze_6x6.csv
├── maze_7x7.csv
└── ...
```

Cada arquivo contém múltiplos labirintos.

# Exemplo de conteúdo do CSV

```text
LABIRINTO 1
1,1,1,1,1
0,0,0,0,1
1,1,1,0,1

LABIRINTO 2
1,1,1,1,1
0,1,0,0,1
1,1,1,0,1
```

# Referência

Código base adaptado de:

Rosetta Code — Maze Generation  
<https://rosettacode.org/wiki/Maze_generation#Python>