#include "Declaracao_de_classes_e_funcoes_criadas.h"
#include <cstring>
#include <utility>
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

    populacao = auxiliar.populacao;

    nome = strsep(&lin, "#");
    especie = strsep(&lin, "#");
    habitat = strsep(&lin, "#");
    dieta = strsep(&lin, "#");
    tipo = strsep(&lin, "#");
    presa = strsep(&lin, "#");

    grau_entrada = 0;
    grau_saida = 1;
    grau = 1;
}

Presa::Presa(int populacao, string nome) {
    populacao_do_predador = populacao;;
    nome_da_presa = std::move(nome);
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

    if(arquivo==NULL){              //se o arquivo nao existir
        numero_de_vertices=-1;
        return;
    }

    Cabecalho cabecalho(arquivo);

    if(cabecalho.status=='0'){      //se  arquivo estiver inconsistente
        numero_de_vertices=-1;
        return;
    }


    numero_de_vertices = 0;

    fseek(arquivo, 1600, SEEK_SET);

    for (int i = 0; i < cabecalho.proxRRN; i++) {

        // Lê o predador do arquivo binário
        Predador p(arquivo);

        // Lê a presa do arquivo binário
        //fseek(arquivo, 1600 + (160 * i), SEEK_SET); // Reposiciona para ler a presa
        Presa pr(p.predador.populacao, p.predador.presa);

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
        }
        else {
            p.insere_presa(pr);
            vertices.insert(p);
            numero_de_vertices++;
        }
    }

    fseek(arquivo, 1600, SEEK_SET);

    for (int i=0; i < cabecalho.proxRRN; i++) {

        // Lê o predador do arquivo binário
        Predador p(arquivo);

        // Lê a presa do arquivo binário
        Presa pr(p.predador.populacao, p.predador.presa);

        if(pr.populacao_do_predador==-1) {
            continue;
        }

        Predador busca_presa = p;
        busca_presa.predador.nome = busca_presa.predador.presa;

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

    if(numero_de_vertices==-1)
    {
        cout << ERRO_PADRAO;
        return;
    }

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

    if(g.numero_de_vertices==-1) //se houver problema na criacao do grafo
    {
        cout << ERRO_PADRAO;
        return;
    }

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

void Analisa_conexoes() {
    Grafo g = Cria_grafo();

    if(g.numero_de_vertices==-1) //se houver problema na criacao do grafo
    {
        cout << ERRO_PADRAO;
        return;
    }


    int componente;

    componente=g.Profundidade();

    //se o numero de componentes conexos for igual ao nro de vertices o grafo eh fortemente conexo
    //assim partindo de qualquer vertice eh possivel chegar em todos os outros vertices do grafo
    if(componente==g.numero_de_vertices)
        cout << "Sim, o grafo e fortemente conexo e possui 1 componente";
    else
        cout << "Nao, o grafo nao e fortemente conexo e possui " << componente << " componentes";
    
}

// Recursive function for DFS traversal
void Grafo::Profundidade_recursao(Predador vertice,int x, vector<vis> &visitado){
    int i;
    visitado[x].visitado=true;

    for (const auto &y : vertice.presas) // passa pelas presas do predador atual
    {
        for (const auto &z : vertices)  // pasa pelos predadores de novo de acordo com os nomes na lista de presas do que veio antes
        { 

            if (y.nome_da_presa == z.predador.nome) //se os nomes baterem, ve se ja foi visitado
            {
                if(y.nome_da_presa==vertice.predador.nome)
                    return;

                for(i=0;i<numero_de_vertices;i++)           //pega o indice do predador na lista auxiliar
                    if(visitado[i].nome==z.predador.nome)
                        break;

                if (visitado[i].visitado == false)
                    Profundidade_recursao(z, i, visitado);
                
            }
        }
    }
}

// funcao principal da pesquisa em profundidade
int Grafo::Profundidade(){
    int componentes=0, aux,k;
    vector<vis> visitado;
    vis temp;


    temp.visitado=false; 
    for (const auto &x : vertices){
        temp.nome=x.predador.nome;
        visitado.push_back(temp);
    }

    // chama a funcao recursiva para cada um dos vertices do grafo
    k=-1;
    for (const auto &x : vertices)
    {
        k++;
        aux=0;
        Profundidade_recursao(x,k,visitado);   //inicia a recursao pelo vertice atual do loop e coloca o indice inicial do vetor de visitados, alem da lista

        for(int i=0;i<numero_de_vertices;i++){   //soma os vertices que visitou e reinicia o vetor
            cout << visitado[i].visitado << " " << visitado[i].nome << "\n";
            aux+=visitado[i].visitado;
            visitado[i].visitado=false;
        }
        if(aux==numero_de_vertices)             //se o nro de vertices visitados for igual ao nro de vertices eh conexo
            componentes++;
    }

    return componentes;
}