#include <iostream>
#include <fstream>
using namespace std;

#include "./FuncoesPadrao.h"

int main() {
    int funcao_selecionada;
    cin >> funcao_selecionada;

    Seletor_de_funcionalidade(funcao_selecionada);

    return 0;
}
