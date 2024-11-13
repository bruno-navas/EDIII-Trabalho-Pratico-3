#ifndef CLASSES_METODOS_GRAFO_H
#define CLASSES_METODOS_GRAFO_H

#include <iostream>
#include <set>
#include <string.h>
#include <vector>

#include "funcoes_arqbin.h"     //manipulacoes no arquivo binario
#include "Funcionalidades.h"    //funcionalidades do programa

using namespace std;

class Presa{
public:
    string nome_da_presa;
    int populacao_do_predador;

    explicit Presa(int populacao, string nome);
    bool operator<(const Presa& e) const {
        return this->nome_da_presa < e.nome_da_presa;
    }
    friend ostream& operator<<(ostream& os, const Presa& e);
};

class Predador {
private:
    Especie predador;
    set<Presa> presas;

public:
    explicit Predador(FILE* arquivo);
    void insere_presa(const Presa& p);

    bool operator<(const Predador& e) const {
        return this->predador.nome < e.predador.nome;
    }
    bool operator==(const Predador& outro) const {
        if (this->predador.nome == outro.predador.nome) {
            return true;
        }
        return false;
    }

    friend ostream& operator<<(ostream& out, const Predador& predador);
    friend class Grafo;
    friend class Presa;
    void friend Exibe_predadores();
    void friend Analisa_conexoes();
};

class Grafo {
private:
    int numero_de_vertices;
    set<Predador> vertices;
    
    typedef struct{     //variavel auxiliar para fazer a pesquisa por profundidade, armazena o nome da especie e se foi visitada
    bool visitado;
    string nome;

    }vis;

    typedef struct{     //variavel auxiliar para fazer a pesquisa com o algoritmo de dijkstra, armazena o nome da especie e o peso do caminho
    int peso;
    string nome;

    }dij;

    //METODO DE EXIBICAO DA FUNCIONALIDADE 10
    void exibe_grafo() const;

    //METODOS DA FUNCIONALIDADE 13
    int Profundidade();
    void Profundidade_recursao(Predador vertice, int x,vector<vis> &visitado);

    //METODO DA FUNCIONALIDADE 14
    int dijkstra(char n_predador[91],char n_presa[91]);
    
public:
    explicit Grafo(FILE* arquivo);
    Grafo friend Cria_grafo();

    void friend Cria_grafo_e_exibe();           //funcionalidade 10
    void friend Exibe_predadores();             //funcionalidade 11
    //void friend Identifica_ciclos();            //funcionalidade 12 
    void friend Analisa_conexoes();             //funcionalidade 13
    void friend Relacao_presa_predador();       //funcionalidade 14


};
//funcao que le o nome do arquivo, cria o ponteiro pra arquivo e chama o construtor
Grafo Cria_grafo();


#endif //DECLARACAO_CLASSES_GRAFO_H
