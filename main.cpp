#include <iostream>
using namespace std;

#include "FuncoesAuxiliares.h"  //seletor de funcionalidade

int main() {
    int funcao_selecionada=12;
    cin >> funcao_selecionada;

    Seletor_de_funcionalidade(funcao_selecionada);

    return 0;
}
