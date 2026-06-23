/*
    Código que implementa o algorítmo de busca A* em labirintos para a matéria de IA desenvolvido pela dupla:
    . Rafael Loureiro Fockink
    . Eduardo Nogueira Korte

    Equipe do trabalho:
    . Rafael Loureiro Fockink,
    . Vitor Krieser Roehrs
    . Eduardo Nogueira Korte
    . Felipe Kuznik Thome
*/

// Aqui vou utilizar funções já implementadas
// pelos colegas da outra dupla para ler e imprimir os labirintos.

#include <iostream>
#include <fstream>
#include <vector>
#include <queue> // não utilizado
#include <string>
#include <sstream>
#include <algorithm>
#include <list>
#include <chrono> // medir o tempo de execução
#include <iomanip>

using namespace std;

using Matriz = vector<vector<int>>;


/////////////////////////////////////////////////////////////
// Funções implementadas no código de Busca primeiro melhor:
/////////////////////////////////////////////////////////////

////// FUNÇÃO APENAS PARA LER O LABIRINTO /////
Matriz lerLabirinto(const string& nomeArquivo, int numeroDesejado) {
    ifstream arquivo(nomeArquivo);
    Matriz matrizEncontrada;
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return matrizEncontrada;
    }

    string linha;
    
    while (getline(arquivo, linha)) {

        if (linha.find_first_not_of(" \t\r\n") == string::npos) continue;

        if (linha.find("LABIRINTO") != string::npos) {
            stringstream ss(linha);
            string nome;
            int numero;

            if (ss >> nome >> numero) {
                
                if (numero == numeroDesejado) {
                    while (getline(arquivo, linha)) {
                        
                        if (linha.find("LABIRINTO") != string::npos || linha.find_first_not_of(" \t\r\n") == string::npos) {
                            break;
                        }
                        
                        vector<int> linhaMatriz;
                        stringstream ssMatriz(linha);
                        string valorTexto;
                        
                        while (getline(ssMatriz, valorTexto, ',')) {
                            if (valorTexto.find_first_not_of(" \t\r\n") == string::npos) continue;
                            linhaMatriz.push_back(stoi(valorTexto));
                        }
                        
                        if (!linhaMatriz.empty()) {
                            matrizEncontrada.push_back(linhaMatriz);
                        }
                    }
                    
                    arquivo.close();
                    return matrizEncontrada;
                }
            }
        }
    }

    arquivo.close();
    cout << "Labirinto com o numero " << numeroDesejado << " nao foi encontrado." << endl;
    return matrizEncontrada;
}


///////////////////////////////////////////////////////////////////////////////////
////// FUNÇÃO APENAS PARA IMPRIMIR O LABIRINTO (POSSIVELMENTE IREMOS EXCLUIR) /////
///////////////////////////////////////////////////////////////////////////////////

void imprimirMatriz(const Matriz& matriz) {
    for (const auto& linha : matriz) {
        for (int valor : linha) {
            if (valor == 2) {
                cout << "* ";
            } else {
                cout << valor << " ";
            }
        }
        cout << endl;
    }
}
///////////////////////////////////////////////////////////////////////////////////

class Posicao {
    public:
    int x, y;
    
    bool eh_igual(const Posicao& outra) const {
        return x == outra.x && y == outra.y;
    }

    bool eh_valida(const Matriz &LABIRINTO) const {
        int n = LABIRINTO.size();
        int m = LABIRINTO[0].size();
        
        // Verifica se está dentro dos limites da matriz
        if (x < 0 || x >= n || y < 0 || y >= m) {
            return false;
        }

        // Verifica se é caminho livre, se não é parede
        if (LABIRINTO[x][y] != 0) {
            return false;
        }

        return true;
    }
};


class No {
    public:
    Posicao posicao;
    
    // guarda a posição do pai
    Posicao pai;
    bool temPai;

    // custo do início até o nó atual
    int g;
    // estimativa até o destino
    int h;
    // prioridade na busca
    int f;
    
    
    No(Posicao pos, int gValor = 0, int hValor = 0, Posicao posPai = Posicao{-1, -1}, bool temPaiValor = false) {
        posicao = pos;

        g = gValor;
        h = hValor;
        f = g + h;
        
        pai = posPai;
        temPai = temPaiValor;
    }

};

// Funcção que calcula a heurística da distância do ponto atual até a saída
int heuristica (Posicao atual, Posicao saida) {
    return abs(atual.x - saida.x) + abs(atual.y - saida.y);
}


// Funcao para pegar o nó com o menor f na lista aberta
No retirar_melhor_f(list<No> &aberta) {
    
    // Assumo que o primeiro nó é o melhor inicialmente
    // utilizo o tipo auto pq a função .beguin() retorna
    // um iterador, quase um ponteiro...
    auto melhor = aberta.begin();

    // Percorre todos os nós da lista aberta
    for (auto it = aberta.begin(); it != aberta.end() ; it ++) {
        // Se o nó atual tiver um f menor que o melhor até agora, ele vira o novo melhor.
        if (it->f < melhor->f)
        melhor = it;
    }
    
    // Copia o melhor nó
    No escolhido = *melhor;
    // remove ele da lista aberta
    aberta.erase(melhor);
    
    return escolhido;
}



// Função que verifica se o No/Posicao está na lista
bool esta_na_lista(const list<No> &lista, Posicao posicao) {
    for (No no : lista) {
        if (no.posicao.eh_igual(posicao)) {
            return true;
        }
    }
    
    return false;
}


// Função que substitui o nó pelo novo valor mais baixo de g,
// caso o novo caminho seja melhor
bool substituir_se_g_menor(list<No>& lista, Posicao posicao, int novoG, int novoH, Posicao novoPai) {
    for (No &no : lista) {
        if (no.posicao.eh_igual(posicao)) {

            // Verifica se o novo g é menor que o g armazenado
            if (novoG < no.g) {
                no.g = novoG;
                no.h = novoH;
                no.f = no.g + no.h;

                // O nó atual agora é o pai do vizinho
                no.pai = novoPai;
                no.temPai = true;
                
                return true;
            }
            
            return false;
        }
    }
    
    return false;
}

// Função que busca um nó em uma lista pela posição, parecido com a 
// função de esta na lista mas essa busca por uma posição específica
bool obter_no_da_lista(const list<No> &lista, Posicao posicao, No &noEncontrado) {
    for (No no : lista) {
        if (no.posicao.eh_igual(posicao)) {
            noEncontrado = no;
            return true;
        }
    }
    
    return false;
}

// Função que imprime o labirinto final com o caminho encontrado
// o caminho será mostrado com "#"
void imprimir_labirinto_caminho(const Matriz &LABIRINTO, const list<No> &fechada, No noFinal, Posicao entrada) {

    // Cria uma cópia do labirinto original
    Matriz labirintoFinal = LABIRINTO;
    
    // começa pelo nó final, ou seja, pela saída
    No atual = noFinal;
    
    // marca a saída como parte do caminho
    labirintoFinal[atual.posicao.x][atual.posicao.y] = 2;
    
    // Volta pelo caminho usando os pais dos nós
    while (!atual.posicao.eh_igual(entrada)) {
        
        // Se o nó não tiver paim para
        if (!atual.temPai) {
            break;
        }

        // Pega a posição do pai do nó atual
        Posicao posPai = atual.pai;

        // Cria uma variável para armazenar o nó pai encontrado
        No noPai (Posicao {-1, -1});
        
        // Busca o pai na lista fechada
        bool encontrouPai = obter_no_da_lista(fechada, posPai, noPai);
        
        // Se não encontrou o pai, para a reconstrução
        if (!encontrouPai){
            break;
        }

        // agora o pai vira o nó autal
        atual = noPai;

        // marca essa posiçãpo como parte do caminho
        labirintoFinal[atual.posicao.x][atual.posicao.y] = 2;
        
    }
    
    cout << "\nLabirinto final com o caminho encontrado:\n" << endl;
    
    for (const auto& linha : labirintoFinal) {
        for (int valor : linha) {
            if (valor == 2) {
                cout << ". ";
            } else {
                cout << valor << " ";
            }
        }
        cout << endl;
    }
    
}

// Cria o tipo Resultado que pode ser SUCESSO ou FALHA
enum Resultado { SUCESSO, FALHA };

struct DadosUteis {
    // identificação
    string nome_arquivo;
    int numero_labirinto;

    // Dimensões do labirinto
    int linhas  = 0;
    int colunas = 0;

    // Entrada e saída (são fixas mas é bom armazenar)
    Posicao entrada;
    Posicao saida;

    // Resultado da execução
    Resultado resultado = FALHA;
    bool encontrou_solucao = false;

    // Métricas principais
    int nos_gerados    = 0; // Nós criados/adicionados a àrvore de busca
    int nos_expandidos = 0; // Nós removidos da aberta e tiveram seus vizinhos analizados
    int nos_visitados  = 0; // Tamanho da lista fechada
    // nosVisitados = tamanho da lista fechada (equivale aos nós expandidos)
    double tempo_execucao_ms = 0.0;

    // Métricas "complementares"
    int custo_solucao        = -1; // Valor g do nó final
    int profundidade_solucao = -1; // Quantidades de passos no caminho
    int tamanho_caminho      =  0; // Quantidade de células no caminho
    int tamanho_max_lista_aberta = 0;
    int tamanho_max_lista_fechada = 0;
    double fator_ramificacao_medio = 0.0;
    // ramificação = nós gerados / nós expandidos

    // Dados uteis para a interface e analize
    list<No> lista_fechada;
    list<No> lista_aberta_final;
    vector<Posicao> caminho_solucao;
};

vector<Posicao> reconstruir_caminho (const list<No> &fechada, No noFinal, Posicao entrada) {
    
    vector<Posicao> caminho;

    No atual = noFinal;
    caminho.push_back(atual.posicao);

    while (!atual.posicao.eh_igual(entrada)) {

        if (!atual.temPai) {
            break;
        }

        Posicao posPai = atual.pai;
        No noPai (Posicao {-1, -1});

        bool achouPai = obter_no_da_lista(fechada, posPai, noPai);

        if (!achouPai) {
            break;
        }

        atual = noPai;
        caminho.push_back(atual.posicao);
    }

    // inverte a ordem da lista para não precisar percorer o caminho de trás para frente
    reverse (caminho.begin(), caminho.end());

    return caminho;
}

// Função que executa o algoritmo de busca do A* do tipo Dados Uteis para retornar os resultados,
// métricas e dados para interface e analise
DadosUteis AStar(const Matriz &LABIRINTO, Posicao entrada, Posicao saida, string nome_do_arquivo, int numero_do_labirinto) {
    
    // [Dados] Aqui crio a estrutura para armazenar as informações e no final retornar ela
    DadosUteis dados;

    // [Dado] ínicio da mediçãodo tempo de execução
    auto inicioTempo = chrono::high_resolution_clock::now();

    // [Dado] Armazeno informações do arquivo/Labirinto
    dados.nome_arquivo = nome_do_arquivo;
    dados.numero_labirinto = numero_do_labirinto;

    // [Inicialização A*] Inicializa a posição da entrada do labirinto
    entrada.x = 0;
    entrada.y = 1;
    
    // [Inicialização A*] Pega as dimensões do labirinto e inicializa a posição de saída do labirinto
    int n = LABIRINTO.size();
    int m = LABIRINTO[0].size();
    
    saida.x = n - 1;
    saida.y = m - 2;

    // [Dado] armazena o tam do labirinto e posições entrada/saída
    dados.linhas = n;
    dados.colunas = m;
    dados.entrada = entrada;
    dados.saida = saida;

    // [Inicialização A*] Calcula a heurística da entrada até a saída
    int hInicial = heuristica(entrada, saida);

    // [Inicialização A*] Cria o nó inicial
    No atual(entrada, 0, hInicial);

    // [Dados] Conta o total de nós criados/gerados na árvore
    // Começa em 1 porque o nó inicial já foi criado
    dados.nos_gerados = 1;

    // [Dado] Inicialização das métricas complementares
    dados.nos_expandidos = 0;
    dados.nos_visitados = 0;
    dados.tamanho_max_lista_aberta = 1;
    dados.tamanho_max_lista_fechada = 0;

    // [A*]
    // [Pseudocódigo] faça uma lista aberta contendo apenas o nó inicial
    list<No> aberta;
    aberta.push_back(atual);

    // [A*]
    // [Pseudocódigo] faça uma lista fechada vazia
    list<No> fechada;

    // [Controle A*] Variável para controlar se o destino foi alcançado
    bool destinoAlcancado = false;

    // [A*]
    // [Pseudocódigo] enquanto (o nó de destino não foi alcançado):
    while (!destinoAlcancado) {

        // [Verifica Erro] Se a lista aberta ficou vazia, não existe caminho até a saída
        if (aberta.empty()) {
            // [Dados] Finaliza medição do tempo
            auto fimTempo = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duracao = fimTempo - inicioTempo;
            
            // [Dados] Armazena métrocas finais em caso de falha
            dados.tempo_execucao_ms = duracao.count();
            dados.resultado = FALHA;
            dados.encontrou_solucao = false;
            dados.lista_fechada = fechada;
            dados.lista_aberta_final = aberta;
            dados.nos_visitados = fechada.size();
            dados.tamanho_max_lista_fechada = max(dados.tamanho_max_lista_fechada, (int)fechada.size());
            
            // [Dados] Calcula fator de ramificação médio
            if (dados.nos_gerados > 0) {
                dados.fator_ramificacao_medio = (double)(dados.nos_gerados - 1) / dados.nos_expandidos;
            }

            cout << "Terminou com FALHA: nao existe caminho ate a saida." << endl;
            cout << "Total de nos gerados: " << dados.nos_gerados << endl;
            cout << "Total de nos expandidos: " << dados.nos_expandidos << endl;
            cout << "Tempo de execucao: " << fixed << setprecision(4) 
                 << dados.tempo_execucao_ms << " ms" << endl;

            return dados;
            
        }

        // [A*]
        // [Pseudocódigo] considere o nó com a menor pontuação f na lista aberta
        atual = retirar_melhor_f(aberta);

        cout << "Atual: (" 
             << atual.posicao.x << ", " 
             << atual.posicao.y << ") "
             << "g=" << atual.g << " "
             << "h=" << atual.h << " "
             << "f=" << atual.f << endl;

        // [A*]
        // [Pseudocódigo] se (este nó é o nó de destino):
        if (atual.posicao.eh_igual(saida)) {

            // [Dado] Finalizo a medição do tempo
            auto fimTempo = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duracao = fimTempo - inicioTempo;

            // [Dado] Armazena resultado e métricas principais
            dados.tempo_execucao_ms = duracao.count();
            dados.resultado = SUCESSO;
            dados.encontrou_solucao = true;
            dados.custo_solucao = atual.g;

            // [Dado] Reconstroe e armazena o caminho apra a interface
            dados.caminho_solucao = reconstruir_caminho(fechada, atual, entrada);
            dados.tamanho_caminho = dados.caminho_solucao.size();

            // [Dado] Calcula profundidade da solução
            if (dados.tamanho_caminho > 0) {
                dados.profundidade_solucao = dados.tamanho_caminho - 1;
            }

            // [Dado] Armazena listas finais para análise/interface
            dados.lista_fechada = fechada;
            dados.lista_aberta_final = aberta;
            dados.nos_visitados = fechada.size();

            // [Dado] Calcula fator deramificação médio
            if (dados.nos_expandidos > 0) {
                dados.fator_ramificacao_medio = (double)(dados.nos_gerados - 1) / dados.nos_expandidos;
            }


            cout << "\nTerminou com SUCESSO!!!!!!" << endl;
            cout << "Total de nos gerados: " << dados.nos_gerados << endl;
            cout << "Total de nos expandidos: " << dados.nos_expandidos << endl;
            cout << "Total de nos visitados: " << dados.nos_visitados << endl;
            cout << "Custo da solucao: " << dados.custo_solucao << endl;
            cout << "Profundidade da solucao: " << dados.profundidade_solucao << endl;
            cout << "Tempo de execucao: " << fixed << setprecision(4) 
                 << dados.tempo_execucao_ms << " ms" << endl;
            cout << "Fator de ramificacao medio: " << fixed << setprecision(4) 
                 << dados.fator_ramificacao_medio << endl;


            // [Função auxiliar]
            // [Representação visual A*] Printa o labirinto final com . mostrando o caminho
            imprimir_labirinto_caminho(LABIRINTO, fechada, atual, entrada);

            destinoAlcancado = true;

            return dados;
        }

        // [A*]
        // [Pseudocódigo] senão:
        else {

            // [A*]
            // [Pseudocódigo] coloque o nó atual na lista fechada e observe todos os seus vizinhos
            fechada.push_back(atual);

            // [Dado] O nó atual será expandido, pois seus vizinhos serão "adentadros"
            dados.nos_expandidos++;

            // [Dado] Atualiza métricas da lista fechada
            dados.nos_visitados = fechada.size();
            dados.tamanho_max_lista_fechada = max(dados.tamanho_max_lista_fechada, (int)fechada.size());

            // [Inicialização A*] Possiveis direções/vizinhos do nó atual
            // [Pseudocódigo] e observe todos os seus vizinhos
            vector<Posicao> direcoes = {
                {-1, 0}, // Cima
                {1,  0}, // Baixo
                {0, -1}, // Esquerda
                {0,  1}  // Direita
            };

            // [A*]
            // [Pseudocódigo] para (cada vizinho do nó atual):
            for (Posicao direcao : direcoes) {

                Posicao posVizinho;

                posVizinho.x = atual.posicao.x + direcao.x;
                posVizinho.y = atual.posicao.y + direcao.y;

                // [Verificações A*]
                // Se o vizinho não está dentro do labirinto ou é parede,
                // pula para o próximo vizinho
                if (!posVizinho.eh_valida(LABIRINTO)) {
                    continue;
                }

                // [A*]
                // Calcula o novo valor de g do vizinho
                int novoG = atual.g + 1;

                // [A*]
                // Calcula o novo valor de h do vizinho
                int novoH = heuristica(posVizinho, saida);

                // [A*]
                // Verifica se esse vizinho está na lista fechada
                bool vizinhoEstaNaFechada = esta_na_lista(fechada, posVizinho);

                // [A*]
                // Verifica se esse vizinho está na lista aberta
                bool vizinhoEstaNaAberta = esta_na_lista(aberta, posVizinho);

                // [A*]
                // [Pseudocódigo] se (vizinho tem um valor g menor que o atual e está na lista fechada):
                if (vizinhoEstaNaFechada && 
                    substituir_se_g_menor(fechada, posVizinho, novoG, novoH, atual.posicao)) {

                    // [Pseudocódigo] - Substitua o vizinho pelo novo valor mais baixo de g
                    // Isso já foi feito dentro da função substituir_se_g_menor que também retorna a verificação

                    // [Pseudocódigo] - O nó atual agora é o pai do vizinho
                    // Isso também já foi feito dentro da função substituir_se_g_menor
                }

                // [A*]
                // [Pseudocódigo] caso contrário
                else {

                    // [A*]
                    // [Pseudocódigo] se (o valor atual de g for menor e esse vizinho estiver na lista aberta):
                    if (vizinhoEstaNaAberta && 
                        substituir_se_g_menor(aberta, posVizinho, novoG, novoH, atual.posicao)) {

                        // [Pseudocódigo] - Substitua o vizinho pelo novo valor mais baixo de g
                        // Isso já foi feito dentro da função substituir_se_g_menor que também retorna a verificação

                        // [Pseudocódigo] - Alterar o pai do vizinho para o nosso nó atual
                        // Isso também já foi feito dentro da função substituir_se_g_menor
                    }

                    // [A*]
                    // [Pseudocódigo] caso contrário
                    else {

                        // [A*]
                        // [Pseudocódigo] se esse vizinho não estiver nas duas listas:
                        if (!vizinhoEstaNaFechada && !vizinhoEstaNaAberta) {

                            // [A*]
                            // [Pseudocódigo] - Adicione-o à lista aberta e defina seu g.
                            No vizinho(posVizinho, novoG, novoH, atual.posicao, true);
                            aberta.push_back(vizinho);

                            // [Dados] Conta mais um nó gerado
                            dados.nos_gerados++;
                        }
                    }
                }
            }

            // [Dados] Atualiza o maior tamanho atingido pela lista aberta
            dados.tamanho_max_lista_aberta = max(dados.tamanho_max_lista_aberta, (int)aberta.size());
        }
    }

    // [Dados] Retorno que NÃO É PARA OCORRER
    dados.resultado = FALHA;
    dados.encontrou_solucao = false;

    return dados;
}





int main() {
    string nomeDoArquivo = "labirintos/maze_30x30.csv";
    int numeroAlvo = 1;

    Matriz LABIRINTO = lerLabirinto(nomeDoArquivo, numeroAlvo);

    if (!LABIRINTO.empty()) {
        cout << "Matriz \"" << numeroAlvo << "\" carregada com sucesso!\n" << endl;
        imprimirMatriz(LABIRINTO);
        cout << "--------------------------------------------------------------------\n" << endl;
    } else {
        return 1; 
    }

    Posicao entrada;
    Posicao saida;

    DadosUteis dados_AStar = AStar(LABIRINTO, entrada, saida, nomeDoArquivo, numeroAlvo);

    cout << "\n############### RESUMO A* ################\n";
    cout << "Arquivo: " << dados_AStar.nome_arquivo << endl;
    cout << "Labirinto numero: " << dados_AStar.numero_labirinto << endl;
    cout << "Tamanho \"real\": " << dados_AStar.linhas << "x" << dados_AStar.colunas << endl;
    cout << "Resultado: " << (dados_AStar.encontrou_solucao ? "SUCESSO" : "FALHA") << endl;
    cout << "Nos gerados: " << dados_AStar.nos_gerados << endl;
    cout << "Nos expandidos: " << dados_AStar.nos_expandidos << endl;
    cout << "Nos visitados: " << dados_AStar.nos_visitados << endl;
    cout << "Custo da solucao: " << dados_AStar.custo_solucao << endl;
    cout << "Profundidade da solucao: " << dados_AStar.profundidade_solucao << endl;
    cout << "Tempo: " << fixed << setprecision(4) << dados_AStar.tempo_execucao_ms << " ms" << endl;
    cout << "Maior tamanho da lista aberta: " << dados_AStar.tamanho_max_lista_aberta << endl;
    cout << "Maior tamanho da lista fechada: " << dados_AStar.tamanho_max_lista_fechada << endl;
    cout << "Fator de ramificacao medio: " << fixed << setprecision(4) 
        << dados_AStar.fator_ramificacao_medio << endl;
    cout << "############################################\n";

    return 0;
}