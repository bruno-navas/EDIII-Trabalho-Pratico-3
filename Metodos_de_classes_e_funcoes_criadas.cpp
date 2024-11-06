#include "Declaracao_de_classes_e_funcoes_criadas.h"
#include <cstring>
#include <vector>

#include "FuncoesPadrao.h"
using namespace std;

Cabecalho::Cabecalho(FILE* arquivo) {
    fread(&status, 1, 1, arquivo);                 // Lê 1 byte para 'status'
    fread(&topo, sizeof(int), 1, arquivo);         // Lê 4 bytes para 'topo'
    fread(&proxRRN, sizeof(int), 1, arquivo);      // Lê 4 bytes para 'proxRRN'
    fread(&nroRegRem, sizeof(int), 1, arquivo);    // Lê 4 bytes para 'nroRegRem'
    fread(&nroPagDisco, sizeof(int), 1, arquivo);  // Lê 4 bytes para 'nroPagDisco'
    fread(&qttCompacta, sizeof(int), 1, arquivo);  // Lê 4 bytes para 'qttCompacta'
}

Dado_bin::Dado_bin(FILE *arquivo) {
    fread(&removido, 1, 1, arquivo);                       // Lê 1 byte para 'removido'
    fread(&encadeamento, 4, 1, arquivo);                   // Lê 4 bytes para 'encadeamento'
    fread(&populacao, 4, 1, arquivo);                      // Lê 4 bytes para 'populacao'
    fread(&tamanho, 4, 1, arquivo);                        // Lê 4 bytes para 'tamanho'
    fread(&unidadeMedida, 1, 1, arquivo);                  // Lê 1 byte para 'unidadeMedida'
    fread(&velocidade, 4, 1, arquivo);                     // Lê 4 bytes para 'velocidade'
    fread(&variavel, 1, 142, arquivo);                     // Lê 142 bytes para 'variavel'

    //cout << variavel << endl;
}

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

Especie::Especie(FILE* arquivo) {
    Dado_bin auxiliar(arquivo);
    char *lin = strdup(auxiliar.variavel);

    nome = strsep(&lin, "#");
    especie = strsep(&lin, "#");
    habitat = strsep(&lin, "#");
    dieta = strsep(&lin, "#");
    tipo = strsep(&lin, "#");

    grau_entrada = 0;
    grau_saida = 1;
    grau = 1;
}

Presa::Presa(FILE *arquivo) {
    Dado_bin auxiliar(arquivo);
    char *lin = strdup(auxiliar.variavel);

    populacao_do_predador = auxiliar.populacao;
    for(int i=0; i<5; i++){strsep(&lin, "#");}
    nome_da_presa = strsep(&lin, "#");
}

ostream& operator<<(ostream &os, const Presa &presa) {
    os << presa.nome_da_presa << " " << presa.populacao_do_predador << endl;
    return os;
}

Predador::Predador(FILE* arquivo): predador(arquivo) {
}

void Predador::insere_presa(const Presa& p) {
    presas.insert(p);
}

ostream& operator<<(ostream& out, const Predador& predador) {
    out << predador.predador.nome << " " << predador.predador.especie << " " << predador.predador.habitat << " " << predador.predador.tipo << " " << predador.predador.dieta << " " << predador.predador.grau_entrada << " " << predador.predador.grau_saida << " " << predador.predador.grau << " ";

    return out;
}

Grafo::Grafo(FILE* arquivo) {
    Cabecalho cabecalho(arquivo);

    numero_de_vertices = 0;

    for (int i = 0; i < cabecalho.proxRRN; i++) {
        // Calcula a posição do predador e da presa no arquivo binário
        fseek(arquivo, 1600 + (160 * i), SEEK_SET);

        // Lê o predador do arquivo binário
        Predador p(arquivo);

        // Lê a presa do arquivo binário
        fseek(arquivo, 1600 + (160 * i), SEEK_SET); // Reposiciona para ler a presa
        Presa pr(arquivo);

        if(pr.populacao_do_predador==-1) {
            continue;
        }

        // Tenta inserir o predador no conjunto
        auto it_predador = vertices.find(p);

        if (it_predador!=vertices.end()) {
            Predador aux_predador = *it_predador;
            aux_predador.predador.grau_saida++;
            aux_predador.predador.grau++;
            aux_predador.insere_presa(pr);

            vertices.erase(*it_predador);
            vertices.insert(aux_predador);


            numero_de_vertices++;
        }
        else {
            p.insere_presa(pr);
            vertices.insert(p);
        }
    }

    for (int i=0; i < cabecalho.proxRRN; i++) {
        // Calcula a posição do predador e da presa no arquivo binário
        fseek(arquivo, 1600 + (160 * i), SEEK_SET);

        // Lê o predador do arquivo binário
        Predador p(arquivo);

        // Lê a presa do arquivo binário
        fseek(arquivo, 1600 + (160 * i), SEEK_SET); // Reposiciona para ler a presa
        Presa pr(arquivo);

        if(pr.populacao_do_predador==-1) {
            continue;
        }

        Predador busca_presa = p;
        busca_presa.predador.nome = pr.nome_da_presa;

        //cout << busca_presa << endl;
        //cout << pr << endl;
        //system("pause");

        auto it_presa = vertices.find(busca_presa);

        if(it_presa!=vertices.end()) {
            Predador aux_presa = *it_presa;
            aux_presa.predador.grau_entrada++;
            aux_presa.predador.grau++;

            vertices.erase(*it_presa);
            vertices.insert(aux_presa);
        }
    }
}

void Grafo::exibe_grafo() const {
    for (const auto& x: vertices) {
        for (const auto& y: x.presas) {
            cout << x << y;
        }
    }
}

void Cria_grafo_e_exibe() {
    string nome;
    cin >> nome;

    FILE *arq_bin = fopen(nome.c_str(), "rb");

    Grafo g(arq_bin);

    g.exibe_grafo();
}

Grafo Cria_grafo() {
    string nome;
    cin >> nome;

    FILE *arq_bin = fopen(nome.c_str(), "rb");

    return Grafo (arq_bin);
}

void Exibe_predadores() {
    Grafo g = Cria_grafo();

    int numero_de_buscas = 3;
    cin >> numero_de_buscas;

    fflush(stdin);

    string input;
    string busca_atual;
    vector<string> busca;
    bool dentro_aspas = false;

    getline(cin, input);

    for (const char c : input) {
        if (c == '"') {
            if (dentro_aspas) {
                busca.push_back(busca_atual);
                busca_atual.clear();
            }
            dentro_aspas = !dentro_aspas;
        }
        else if (dentro_aspas) {
            busca_atual += c;
        }
    }

    for(int i=0; i<numero_de_buscas; i++) {
        string saida;

        for(const auto& x:g.vertices) {
            for(const auto& y:x.presas) {
                if (y.nome_da_presa==busca[i]) {
                    saida += x.predador.nome + ", ";
                }
            }
        }
        if(!saida.empty()) {
            saida.erase(saida.size()-2,2);
            cout << busca[i] << ": " << saida<<endl << endl;
        }
        else {
            cout << ERRO_REGISTRO << endl;
        }
        }
}
