#ifndef FUNCOES_ARQBIN_H
#define FUNCOES_ARQBIN_H

using namespace std;
#include <iostream>
#include <cstring>

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
    int populacao; // Do predador
    int grau_entrada; // Indica o número de predadores que predam o vértice
    int grau_saida; // Indica o número de presas no vértice
    int grau; // Soma dos graus de saída e de entrada

    explicit Especie(FILE* arquivo);
    explicit Especie(string NOME);
    //friend class Predador;
    //friend class Grafo;
};

#endif //FUNCOES_ARQBIN_H
