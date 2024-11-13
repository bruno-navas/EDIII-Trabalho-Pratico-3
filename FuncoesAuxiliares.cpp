#include "FuncoesAuxiliares.h"
#include "Funcionalidades.h"
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

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
            break;
        case 13:
            Analisa_conexoes();             //busca em profundidade 
                                            //eh fortemente conexo se, e somente se, cada vertice eh raiz de uma subarvore geradora do mesmo grafo inicial
                                            //realizar o processo de busca visto em aula para todos os vertices, ja que eh direcionado
                                            //se o grafo nao fosse direcionado precisaria fazer isso uma vez so
            break;
        case 14:
            Relacao_presa_predador();       //busca em largura (dijkstra)
                                            //algoritmo que procura o menor caminho entre duas especies considerando o peso
            break;
        default:
            cout << ERRO_COMANDO;
            break;
    }
}

void scan_quote_string(char *str) {

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

//strsep para uso em sistemas windows 
char* strsep(char** stringp, const char* delim)
{
    char* start = *stringp;

    char *p = (start != nullptr) ? strpbrk(start, delim) : nullptr;

    if (p == nullptr)
    {
        *stringp = nullptr;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}



