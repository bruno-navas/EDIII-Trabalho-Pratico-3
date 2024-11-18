#ifndef CLASSES_METODOS_GRAFO_H
#define CLASSES_METODOS_GRAFO_H

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <stack>
#include "Classes_metodos_presa_e_predador.h"
#include "funcoes_arqbin.h"     //manipulacoes no arquivo binario

using namespace std;


class Grafo {
private:
    int numero_de_vertices;
    int numero_de_ciclos;
    set<Predador> vertices;
    
    typedef struct{
    int low;
    int disc;
    bool stack;

    }vis; //variavel auxiliar para fazer a pesquisa por profundidade, armazena o nome da especie e se foi visitada


    //METODO DE EXIBICAO DA FUNCIONALIDADE 10
    void exibe_grafo() const;

    //METODO DA FUNCIONALIDADE 11
    void busca_predadores(char presa[]) const;

    //METODOS DA FUNCIONALIDADE 12
    void detecta_ciclos();
    void auxiliar_ciclos(const Predador& p, int pos, vector<int>& visitados);

    //METODOS DA FUNCIONALIDADE 13
    int Profundidade();
    void Profundidade_recursao(const Predador &vertice, map<string,vis> &visitado, stack<string> &pilha, int &comp);

    //METODO DA FUNCIONALIDADE 14
    int dijkstra(string n_predador, string n_presa);
    
public:
    explicit Grafo(FILE* arquivo);
    Grafo friend Cria_grafo();

    void friend Cria_grafo_e_exibe();           //funcionalidade 10
    void friend Exibe_predadores();             //funcionalidade 11
    void friend Identifica_ciclos();            //funcionalidade 12
    void friend Analisa_conexoes();             //funcionalidade 13
    void friend Relacao_presa_predador();       //funcionalidade 14
};

//funcao que le o nome do arquivo, cria o ponteiro pra arquivo e chama o construtor
//o bool determina se a correcao dos alimentos ocorrera ou nao
Grafo Cria_grafo();

#endif //CLASSES_METODOS_GRAFO_H
