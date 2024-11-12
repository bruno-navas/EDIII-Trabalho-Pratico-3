#ifndef DECLARACAO_DE_CLASSES_E_FUNCOES_CRIADAS_H
#define DECLARACAO_DE_CLASSES_E_FUNCOES_CRIADAS_H

#include <iostream>
#include <set>
#include <string.h>
#include <vector>

using namespace std;

class Cabecalho{
private:
    int topo;                   // Armazena o RRN de um registro logicamente removido ou -1 caso nao haja                   4 bytes
    int nroRegRem;              // Armazena o nro de registros logicamene marcados como removidos, inicia como 0            4 bytes
    int nroPagDisco;            // Armazena o nro de paginas de disco ocupadas pelo arq. de dados                           4 bytes
    int qttCompacta;

public:
    int proxRRN;                // Armazena o valor do prox RRN disponivel, inicia como 0 e muda caso necessario            4 bytes
    char status;                // Indicacao da consistencia do arq. de dados, se for 1 esta consistente e se for 0 nao     1 byte


    explicit Cabecalho(FILE* arquivo);
};

class Dado_bin {
private:
    char removido;              //  Indica se o registro esta logicamente removido                    1 byte
    int encadeamento;           //  Armazena o RRN do proximo registro logicamente removido           4 bytes
    float tamanho;              //  Tamanho do individuo                                              4 bytes
    char unidadeMedida;         //  Unidade de medida da velocidade do individuo                      1 byte
    int velocidade;             //  Velocidade do individuo                                           4 bytes
    int populacao;              //  Qtd de individuos da especie que viviam em determinado lugar      4 bytes
    char variavel[142];         //  Bytes variaveis                                                   142 bytes
    //  Nome-Especie-Habitat-Tipo-Dieta-Alimento
    //  Cada campo variavel eh separado por um delimitador '#'
    //  As strings nao devem terminar com \0
    //  Se sobrar espaço preencher com '$'
public:
    explicit Dado_bin(FILE* arquivo);
    friend class Especie;
    friend class Presa;
};

class Especie{
public:
    string nome;
    string especie;
    string habitat;
    string dieta;
    string tipo;
    string presa;
    int populacao;
    int grau_entrada;
    int grau_saida;
    int grau;

    explicit Especie(FILE* arquivo);
    friend class Predador;
    friend class Grafo;
};

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

    //METODO DE EXIBICAO DA FUNCIONALIDADE 10
    void exibe_grafo() const;


    //METODOS DA FUNCIONALIDADE 13
    int Profundidade();
    void Profundidade_recursao(Predador vertice, int x,vector<vis> &visitado);

    //METODO DA FUNCIONALIDADE 14
    int dijkstra(char n_predador[61],char n_presa[61]);
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

//funcionalidades em ordem
void Cria_grafo_e_exibe();  
void Exibe_predadores();
void Identifica_ciclos();
void Analisa_conexoes();
void Relacao_presa_predador();

void scan_quote_string(char *str);
char* strsep(char** stringp, const char* delim);

#endif //DECLARACAO_DE_CLASSES_E_FUNCOES_CRIADAS_H
