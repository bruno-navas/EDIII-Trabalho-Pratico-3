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
            Analisa_conexoes();             //busca em profundidade 
                                            //é fortemente conexo se, e somente se, cada vertice eh raiz de uma subarvore geradora do mesmo grafo inicial
                                            //realizar o processo de criacao visto em aula para todos os vertices, ja que eh direcionado
                                            //se o grafo nao fosse direcionado precisaria fazer isso uma vez so
        case 14:
            //Relacao_presa_predador();     //busca em largura
        default:
            cout << ERRO_COMANDO;
    }
}



