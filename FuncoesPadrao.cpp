#include "FuncoesPadrao.h"
#include "Declaracao_de_classes_e_funcoes_criadas.h"

void Seletor_de_funcionalidade (int selecao) {
    switch (selecao) {
        case 10:
            Cria_grafo_e_exibe();
            break;
        case 11:
            Exibe_predadores();
            break;
        case 12:
            //Identifica_ciclos();
        case 13:
            //Analisa_conexoes();
        case 14:
            //Relacao_presa_predador();
        default:
            cout << ERRO_COMANDO;
    }
}


