#include "funcoes_arqbin.h"
#include "Classes_metodos_presa_e_predador.h"
using namespace std;

// Metodos da Presa

// Construtor da presa, apenas transfere os dados recebidos para as variáveis da classe
Presa::Presa(int populacao, string nome) {
    populacao_do_predador = populacao;;
    nome_da_presa = std::move(nome);
}

// Operador exibe o nome da Presa seguido da população do seu predador (peso da aresta)
ostream& operator<<(ostream &os, const Presa &presa) {
    os << presa.nome_da_presa << " " << presa.populacao_do_predador << endl;
    return os;
}

// Metodos do Predador

// Construtor do predador utiliza o construtor da Especie para receber os dados do arquivo binario
Predador::Predador(FILE* arquivo): predador(arquivo) {
}

// Metodo auxiliar para inserir presas novas nos sets de Presa do Predador
void Predador::insere_presa(const Presa& p) {
    presas.insert(p);
}

// Exibe os dados do Predador na ordem desejada
ostream& operator<<(ostream& out, const Predador& predador) {
    out << predador.predador.nome << " " << predador.predador.especie << " " << predador.predador.habitat << " " << predador.predador.tipo << " " << predador.predador.dieta << " " << predador.predador.grau_entrada << " " << predador.predador.grau_saida << " " << predador.predador.grau << " ";

    return out;
}

