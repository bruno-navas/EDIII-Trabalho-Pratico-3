#include <iostream>
#include "Funcionalidades.h"
#include "FuncoesAuxiliares.h"
#include "Classes_metodos_grafo.h"
using namespace std;

void Cria_grafo_e_exibe() {
    Grafo g = Cria_grafo();
    g.exibe_grafo();
}

void Exibe_predadores() {
    Grafo g = Cria_grafo();

    if(g.numero_de_vertices==-1) //se houver problema na criacao do grafo
    {
        cout << ERRO_PADRAO;
        return;
    }

    int numero_de_buscas = 3;
    cin >> numero_de_buscas;

    fflush(stdin);

    string input;
    string busca_atual;
    vector<string> busca;
    bool dentro_aspas = false;

    getline(cin, input);

    for (const char c : input) {
        if (c == '"') {
            if (dentro_aspas) {
                busca.push_back(busca_atual);
                busca_atual.clear();
            }
            dentro_aspas = !dentro_aspas;
        }
        else if (dentro_aspas) {
            busca_atual += c;
        }
    }

    for(int i=0; i<numero_de_buscas; i++) {
        string saida;

        for(const auto& x:g.vertices) {
            for(const auto& y:x.presas) {
                if (y.nome_da_presa==busca[i]) {
                    saida += x.predador.nome + ", ";
                }
            }
        }
        if(!saida.empty()) {
            saida.erase(saida.size()-2,2);
            cout << busca[i] << ": " << saida<<endl << endl;
        }
        else {
            cout << ERRO_REGISTRO << endl;
        }
    }
}

void Identifica_ciclos() {
    Grafo g = Cria_grafo();

    if(g.numero_de_vertices==-1) //se houver problema na criacao do grafo
    {
        cout << ERRO_PADRAO;
        return;
    }

    g.detecta_ciclos();
}

//FUNCIONALIDADE 13 - DFS

//a funcao Analisa_Conexoes() eh a funcao de pesquisa em profundidade do grafo
//essa funcao cria o grafo e chama a rotina Grafo::Profundidade para retornar a qtd
//de componentes conexos dentro do grafo para entao retornar o resultado

//as duas principais funcoes utilizadas aqui são Profundidade e Profundidade_recursao, explicadas a seguir

void Analisa_conexoes() {
    Grafo g = Cria_grafo();      //cria o grafo

    if(g.numero_de_vertices==-1) //se houver problema na criacao do grafo, da erro e nao executa a funcao
    {
        cout << ERRO_PADRAO;
        return;
    }

    int componente = g.Profundidade();    //chama a funcao de pesquisa em profundidade, necessaria para saber se um grafo eh ou nao eh fortemente conexo
                                          //essa funcao retorna a qtd de componentes conexos do grafo

    //se o numero de componentes conexos for igual ao nro de vertices o grafo eh fortemente conexo
    //assim partindo de qualquer vertice eh possivel chegar em todos os outros vertices do grafo

    //caso isso nao seja verdade ele nao eh fortemente conexo, mas ainda sim podem ter vertices que tenham a capacidade
    //de chegar em todos os outros vertices do grafo, so nao sao todos
    if(componente==g.numero_de_vertices)
        cout << "Sim, o grafo e fortemente conexo e possui 1 componente";
    else
        cout << "Nao, o grafo nao e fortemente conexo e possui " << componente << " componentes";
    
}

//FUNCIONALIDADE 14 - DIJKSTRA

//a funcao realiza um loop de pesquisas que recebe
//um vertice de inicio e um alimento alvo e determina o peso
// minimo do caminho que parte do inicio e vai ate o alvo
//isso eh feito com o algoritmo de dijkstra, que segue abaixo

void Relacao_presa_predador(){

    Grafo g = Cria_grafo();      //cria o grafo

    if(g.numero_de_vertices==-1) //se houver problema na criacao do grafo, da erro e nao executa a funcao
    {
        cout << ERRO_PADRAO;
        return;
    }

    int pesquisas;          //nro de pesquisas que serao feitas
    int peso_caminho;       //variavel de retorno da pesquisa, indica o peso do caminho
    char n_presa[91];       //presa da pesquisa
    char n_predador[91];    //predador da pesquisa

    cin >> pesquisas;   //quantas pesquisas serao feitas

    for(int i=0;i<pesquisas;i++)    //loop das pesquisas
    {   
        scan_quote_string(n_predador);  //le o primeiro e ultimo vertice da pesquisa
        scan_quote_string(n_presa);


        peso_caminho=g.dijkstra(n_predador,n_presa);//executa a rotina do algoritmo de dijkstra no grafo
                                                    //funcao acha o menor caminho entre dois vertices
                                                    //funcao retorna -1 se nao houver caminho entre os dois

        //ao fim da pesquisa atual checa se teve ou nao caminho e exibe o resultado
        cout << n_predador << " " << n_presa << ": ";
        if(peso_caminho==INT_MAX)
            cout << "CAMINHO INEXISTENTE";
        else 
            cout << peso_caminho;
        cout << "\n";
    }
}
