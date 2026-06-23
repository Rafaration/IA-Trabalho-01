## Gerador de Labirintos para IA e Algoritmos de Busca
  
Gerador de labirintos em Python utilizando **DFS (Depth-First Search)** com saída em:
- Visual ASCII
- Matriz binária (0 e 1)
- Arquivos .csv

Também possui a implementação do algoritmo de busca **A\*** em C++, usado para resolver os labirintos gerados e coletar dados para análise comparativa.

## Características
- Geração procedural de labirintos
- Algoritmo DFS recursivo
- Exportação em CSV
- Representação binária para IA
- Visualização ASCII
- Geração em lote
- Suporte a labirintos quadrados e retangulares
- Leitura de labirintos salvos em arquivos .csv
- Implementação do algoritmo A\*
- Uso da heurística da distância de Manhattan
- Busca do caminho entre a entrada e a saída do labirinto
- Coleta de métricas da execução do A\*
- Armazenamento de dados úteis para interface e análise

## Estrutura do Projeto
.
├── AStar.cpp
├── maze_maker.py
├── labirintos/
│   ├── maze_4x4.csv
│   ├── maze_5x5.csv
│   └── ...
└── README.md

## Representação do Labirinto
  
O labirinto binário utiliza:

0 = caminho livre  
1 = parede  

Exemplo:

1,0,1,1,1  
1,0,0,0,1  
1,1,1,0,1  
1,0,0,0,1  
1,1,1,0,1

## Sobre o tamanho do labirinto
  
Um labirinto lógico 4x4 gera uma matriz real 9x9.  
Isso acontece porque a matriz também armazena:
- paredes
- bordas
- espaços entre células  

A fórmula utilizada é:

linhas = altura * 2 + 1  
colunas = largura * 2 + 1

## Como executar

### Clone o projeto

git clone https://github.com/Rafaration/IA-Trabalho-01.git

### Entre na pasta

cd IA-Trabalho-01

### Execute o gerador de labirintos

python maze_maker.py

### Compile o A\*

g++ AStar.cpp -o AStar

### Execute o A\*

./AStar

## Funções principais do gerador

### gerar_labirinto()
  
Gera um labirinto em ASCII.

#### Exemplo

print(gerar_labirinto(4, 4))

#### Saída

+  +--+--+--+  
|        |  |  
+  +--+  +  +  
|     |     |  
+--+--+--+  +

## gerar_labirinto_binario()
  
Gera um labirinto em matriz binária.

#### Exemplo

maze = gerar_labirinto_binario(4, 4)

#### Saída

[
 [1,0,1,1,1,1,1,1,1],
 [1,0,0,0,1,0,0,0,1],
 [1,1,1,0,1,0,1,1,1],
 [1,0,0,0,0,0,1,0,1],
 [1,0,1,1,1,1,1,0,1],
 [1,0,0,0,0,0,1,0,1],
 [1,1,1,1,1,0,1,0,1],
 [1,0,0,0,0,0,0,0,1],
 [1,1,1,1,1,1,1,0,1]
]

## converter_labirinto_visualizacao()
  
Converte o labirinto binário para visualização ASCII.

#### Exemplo

print(converter_labirinto_visualizacao(maze))

## armazena_labirinto()
  
Salva o labirinto em um arquivo .csv.

#### Exemplo

armazena_labirinto(
    "labirintos/maze_4x4.csv",
    'a',
    maze
)

## geracoes_labirintos()
  
Gera vários labirintos automaticamente.

### Parâmetros

<table>
<tr>
<th>  
Parâmetro
</th>
<th>  
Descrição
</th>
</tr>
<tr>
<td>  
quantidade_por_tamanho
</td>
<td>  
Quantidade de labirintos por tamanho
</td>
</tr>
<tr>
<td>  
tamanho_inicial
</td>
<td>  
Menor tamanho
</td>
</tr>
<tr>
<td>  
tamanho_final
</td>
<td>  
Maior tamanho
</td>
</tr>
<tr>
<td>  
apenas_quadrados
</td>
<td>  
Define se serão apenas NxN
</td>
</tr>
</table>


### Exemplo

geracoes_labirintos(
    quantidade_por_tamanho=5,
    tamanho_inicial=5,
    tamanho_final=20,
    apenas_quadrados=True
)

## Arquivos gerados
  
Exemplo:

labirintos/
├── maze_5x5.csv
├── maze_6x6.csv
├── maze_7x7.csv
└── ...  

Cada arquivo contém múltiplos labirintos.

## Exemplo simplificado do CSV

LABIRINTO 1  
1,0,1,1,1  
1,0,0,0,1  
1,1,1,0,1  

LABIRINTO 2  
1,0,1,1,1  
1,1,0,0,1  
1,1,1,0,1

## Implementação do A\*

O arquivo **AStar.cpp** implementa o algoritmo **A\*** para encontrar um caminho da entrada até a saída do labirinto.

A entrada padrão considerada pelo algoritmo é:

entrada.x = 0  
entrada.y = 1

A saída padrão considerada pelo algoritmo é:

saida.x = número de linhas - 1  
saida.y = número de colunas - 2

Essas posições seguem o padrão utilizado pelo gerador de labirintos, onde a entrada fica na parte superior e a saída fica na parte inferior da matriz.

## Funcionamento do A\*

O algoritmo utiliza duas listas principais:

- lista aberta: guarda os nós que ainda podem ser analisados
- lista fechada: guarda os nós que já foram visitados/expandidos

Para cada nó, são armazenados:

- posição atual no labirinto
- posição do pai
- custo g
- heurística h
- valor f = g + h

A cada iteração, o algoritmo escolhe da lista aberta o nó com menor valor de f.  
Se esse nó for a saída, o algoritmo termina com sucesso.  
Caso contrário, o nó é colocado na lista fechada e seus vizinhos são analisados.

## Heurística utilizada

A heurística utilizada no A\* é a distância de Manhattan:

h = |x_atual - x_saida| + |y_atual - y_saida|

Essa heurística foi escolhida porque o labirinto permite movimentos apenas nas quatro direções principais:

- cima
- baixo
- esquerda
- direita

## Dados úteis coletados pelo A\*

Foi criada uma struct para armazenar os dados da execução do algoritmo.

A struct guarda informações como:

- nome do arquivo utilizado
- número do labirinto escolhido
- quantidade de linhas
- quantidade de colunas
- posição de entrada
- posição de saída
- resultado da execução
- tempo de execução em milissegundos
- quantidade de nós gerados
- quantidade de nós expandidos
- quantidade de nós visitados
- custo da solução
- profundidade da solução
- tamanho do caminho encontrado
- maior tamanho atingido pela lista aberta
- maior tamanho atingido pela lista fechada
- fator de ramificação médio
- lista fechada final
- lista aberta final
- caminho solução

Essas informações foram adicionadas para facilitar a análise dos algoritmos e também para serem usadas futuramente na interface do trabalho.

## Métricas do A\*

### Nós gerados

Representa a quantidade de nós criados durante a busca.  
O nó inicial já conta como um nó gerado.

### Nós expandidos

Representa a quantidade de nós retirados da lista aberta e que tiveram seus vizinhos analisados.

### Nós visitados

Representa o tamanho da lista fechada.  
No caso do A\*, essa métrica fica diretamente relacionada com a quantidade de nós expandidos.

### Tempo de execução

Representa o tempo gasto pelo algoritmo para encontrar a solução ou concluir que não existe caminho até a saída.

### Custo da solução

Representa o valor de g do nó final.  
Como cada movimento tem custo 1, esse valor indica a quantidade de passos necessários para chegar até a saída.

### Profundidade da solução

Representa a quantidade de movimentos no caminho encontrado.

### Fator de ramificação médio

Representa uma estimativa da média de novos nós gerados por nó expandido.

## Funções principais do A\*

### lerLabirinto()

Lê um labirinto específico dentro de um arquivo .csv.

#### Exemplo

Matriz LABIRINTO = lerLabirinto("labirintos/maze_30x30.csv", 1);

## imprimirMatriz()

Imprime a matriz carregada no terminal.

#### Exemplo

imprimirMatriz(LABIRINTO);

## heuristica()

Calcula a distância de Manhattan entre a posição atual e a saída.

#### Exemplo

int hInicial = heuristica(entrada, saida);

## retirar_melhor_f()

Percorre a lista aberta e remove o nó com menor valor de f.

#### Exemplo

No atual = retirar_melhor_f(aberta);

## esta_na_lista()

Verifica se uma posição já está em uma lista.

#### Exemplo

bool existe = esta_na_lista(fechada, posVizinho);

## substituir_se_g_menor()

Atualiza um nó caso um caminho melhor até ele seja encontrado.

#### Exemplo

substituir_se_g_menor(aberta, posVizinho, novoG, novoH, atual.posicao);

## reconstruir_caminho()

Reconstrói o caminho da saída até a entrada usando os pais dos nós.

#### Exemplo

vector<Posicao> caminho = reconstruir_caminho(fechada, atual, entrada);

## imprimir_labirinto_caminho()

Imprime o labirinto com o caminho encontrado marcado na matriz.

#### Exemplo

imprimir_labirinto_caminho(LABIRINTO, fechada, atual, entrada);

## AStar()

Executa o algoritmo A\* e retorna uma struct com os dados úteis da execução.

#### Exemplo

DadosUteis dadosAStar = AStar(LABIRINTO, entrada, saida, nomeDoArquivo, numeroAlvo);

## Exemplo de saída do A\*

Durante a execução, o algoritmo mostra o nó atual analisado:

Atual: (0, 1) g=0 h=56 f=56  
Atual: (1, 1) g=1 h=55 f=56  
Atual: (2, 1) g=2 h=54 f=56

Ao final, exibe um resumo com as principais métricas:

Terminou com SUCESSO!!!!!!  
Total de nos gerados: 127  
Total de nos expandidos: 83  
Total de nos visitados: 83  
Custo da solucao: 58  
Profundidade da solucao: 58  
Tempo de execucao: 1.2345 ms  
Fator de ramificacao medio: 1.5181

## Objetivo da implementação

O objetivo do projeto é gerar instâncias de labirintos e resolver essas instâncias usando algoritmos de busca.

A geração dos labirintos é feita em Python.  
A resolução pelo algoritmo A\* é feita em C++.

As métricas coletadas serão utilizadas para comparar o desempenho dos algoritmos escolhidos pela equipe no trabalho de Inteligência Artificial.

## Referência
  
Código base para o gerador de labirinto inspirado e adaptado de:  
<https://rosettacode.org/wiki/Maze_generation#Python>