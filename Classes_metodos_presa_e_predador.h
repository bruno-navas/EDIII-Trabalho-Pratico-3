#ifndef CLASSES_METODOS_PRESA_E_PREDADOR_H
#define CLASSES_METODOS_PRESA_E_PREDADOR_H

#include <iostream>
#include <set>

#include "funcoes_arqbin.h"     //manipulacoes no arquivo binario

class Presa{
public:
    string nome_da_presa;
    int populacao_do_predador;

    explicit Presa(int populacao, string nome);
    bool operator<(const Presa& e) const {
        return this->nome_da_presa < e.nome_da_presa;
    } // Operador de comparação para ordenação no set
    friend ostream& operator<<(ostream& os, const Presa& e); // Operador para exibição dos dados da presa
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
    } // Operador de comparação para ordenação no set
    bool operator==(const Predador& outro) const {
        if (this->predador.nome == outro.predador.nome) {
            return true;
        }
        return false;
    } // Operador de igualdade para busca no set

    friend ostream& operator<<(ostream& out, const Predador& predador); // Operador para exibição dos dados do predador

    friend class Grafo;
    friend class Presa;
};

#endif //CLASSES_METODOS_PRESA_E_PREDADOR_H