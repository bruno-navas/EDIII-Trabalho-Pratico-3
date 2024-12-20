#ifndef CLASSES_METODOS_GRAFO_H
#define CLASSES_METODOS_GRAFO_H

#include <iostream>
#include <set>
#include <vector>
#include "Classes_metodos_presa_e_predador.h"

class Grafo {
private:
    int numero_de_vertices;
    int numero_de_ciclos;
    set<Predador> vertices;
    
    typedef struct{
    bool visitado;
    string nome;

    }vis; //variavel auxiliar para fazer a pesquisa por profundidade, armazena o nome da especie e se foi visitada

    typedef struct{
    int peso;
    string nome;

    }dij; //variavel auxiliar para fazer a pesquisa com o algoritmo de dijkstra, armazena o nome da especie e o peso do caminho

    //METODO DE EXIBICAO DA FUNCIONALIDADE 10
    void exibe_grafo() const;

    //METODO DA FUNCIONALIDADE 11
    void busca_predadores(char presa[]) const;

    //METODOS DA FUNCIONALIDADE 12
    void detecta_ciclos();
    void auxiliar_ciclos(const Predador& p, int pos, vector<int>& visitados);

    //METODOS DA FUNCIONALIDADE 13
    int Profundidade();
    void Profundidade_recursao(const Predador& vertice, int x,vector<vis> &visitado);

    //METODO DA FUNCIONALIDADE 14
    int dijkstra(char n_predador[91],char n_presa[91]) const;
    
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
Grafo Cria_grafo();

#endif //CLASSES_METODOS_GRAFO_H
