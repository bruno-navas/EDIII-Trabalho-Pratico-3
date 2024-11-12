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
//CONSTRUTOR DO GRAFO DADO UM PONTEIRO PRA ARQUIVO
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



void Cria_grafo_e_exibe() {

    Grafo g = Cria_grafo();

    g.exibe_grafo();
}

Grafo Cria_grafo() {
    string nome;
    cin >> nome;

    FILE *arq_bin = fopen(nome.c_str(), "rb");

    return Grafo (arq_bin);
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

void Exibe_predadores() {
    Grafo g = Cria_grafo();

    if(g.numero_de_vertices==-1) //se houver problema na criacao do grafo
    {
        cout << ERRO_PADRAO;
        return;
    }

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


//FUNCIONALIDADE 13 - DFS

//a funcao Analisa_Conexoes() eh a funcao de pesquisa em profundidade do grafo
//essa funcao cria o grafo e chama a rotina Grafo::Profundidade para retornar a qtd
//de componentes conexos dentro do grafo para entao retornar o resultado

//as duas principais funcoes utilizadas aqui são Profundidade e Profundidade_recursao, explicadas a seguir

void Analisa_conexoes() {
    Grafo g = Cria_grafo();      //cria o grafo

    if(g.numero_de_vertices==-1) //se houver problema na criacao do grafo, da erro e nao executa a funcao
    {
        cout << ERRO_PADRAO;
        return;
    }

    int componente;     
    componente=g.Profundidade();    //chama a funcao de pesquisa em profundidade, necessaria para saber se um grafo eh ou nao eh fortemente conexo
                                    //essa funcao retorna a qtd de componentes conexos do grafo

    //se o numero de componentes conexos for igual ao nro de vertices o grafo eh fortemente conexo
    //assim partindo de qualquer vertice eh possivel chegar em todos os outros vertices do grafo

    //caso isso nao seja verdade ele nao eh fortemente conexo, mas ainda sim podem ter vertices que tenham a capacidade
    //de chegar em todos os outros vertices do grafo, so nao sao todos
    if(componente==g.numero_de_vertices)
        cout << "Sim, o grafo e fortemente conexo e possui 1 componente";
    else
        cout << "Nao, o grafo nao e fortemente conexo e possui " << componente << " componentes";
    
}

// funcao principal da pesquisa em profundidade
//a funcao realiza a pesquisa em profundidade de forma recursiva em todos os vertices do grafo

//para poder realizar a pesquisa um vetor auxiliar eh feito para que indices possam ser utilizados para acesso
//de outra forma ao nome das especies pesquisadas.
//essa foi uma abordagem que pensamos em usar por conta da forma de armazenar os vertices em formato <set>, que impede
//o acesso direto dos dados. assim eh possivel indexar de uma forma os nomes das presas na ordem que temos, sendo necessario um pouco de
//espaco auxiliar a mais

int Grafo::Profundidade()
{
    int componentes = 0; // nro de componentes conexos
    int aux;             // auxiliar para checar se um componente eh conexo
    int k;               // indice do vertice que entrara na recursao

    vector<vis> visitado; // vetor auxiliar
    vis temp;             // vetor auxiliar

    // inicializa o vetor auxiliar que sera utilizado na pesquisa, basicamente os vertices sao considerados nao visitados incialmente
    // e tambem eh destacado o nome da especie
    temp.visitado = false;
    for (const auto &x : vertices)
    {
        temp.nome = x.predador.nome;
        visitado.push_back(temp);
    }

    k = -1; // inicializa o valor referente ao indice de cada vertice original do vector auxiliar
            //  chama a funcao recursiva para cada um dos vertices do grafo dentro do loop
    for (const auto &vert_atual : vertices)
    {
        k++; // atualiza o indice do vertice que entrara na recursao
        aux = 0;
        Profundidade_recursao(vert_atual, k, visitado); // inicia a recursao pelo vertice atual do loop e coloca o indice inicial do vetor de visitados, alem do vector auxiliar em si
        cout << "Partindo de: " << vert_atual.predador.nome << "\n";

        // quando a recursao de um vertice acaba, a contagem de vertices visitados ocorre na variavel aux, ja que o vetor auxiliar foi modificado na funcao anterior
        for (int i = 0; i < numero_de_vertices; i++)
        { // conta os vertices que visitou e reinicia o booleano de visita para as proximas iteracoes
            cout << visitado[i].visitado << " " << visitado[i].nome << "\n";
            aux += visitado[i].visitado;
            visitado[i].visitado = false;
        }
        if (aux == numero_de_vertices) // se o nro de vertices visitados for igual ao nro de vertices o componente eh conexo
            componentes++;
    }

    return componentes;
}

// funcao recursiva do processo
// basicamente a recursao so vai parar quando todos os itens da "rede" que parte do vertice da iteracao forem visitados

void Grafo::Profundidade_recursao(Predador vertice, int x, vector<vis> &visitado)
{
    int i;
    visitado[x].visitado = true; // marca o vertice atual como visitado no vetor auxiliar

    for (const auto &presas_vertice : vertice.presas) // passa pela lista de presas do predador atual
    {
        for (const auto &lista_predador : vertices) // pasa pelos predadores de novo de acordo com os nomes na lista de presas do que veio antes
                                                    // eh nesse for que estao os predadores (com nome na lista de presas) que serao enviados para recursao novamente
        {
            if (presas_vertice.nome_da_presa == lista_predador.predador.nome) // se o nome fizer, tanto parte da lista de presas quanto a de vertices(predadores) faz as checagens
            {
                if (presas_vertice.nome_da_presa == vertice.predador.nome) // se a presa preda ela mesma da retorno, bicho burro
                    return;
                

                for (i = 0; i < numero_de_vertices; i++)                  // pega o indice do predador/presa na lista auxiliar
                    if (visitado[i].nome == lista_predador.predador.nome) // nao eh tao eficiente mas foi a forma pensada para indexar o <set>
                        break;

                if (visitado[i].visitado == false)                      // se esse vertice ainda nao foi visitado, chamada recursiva
                    Profundidade_recursao(lista_predador, i, visitado); // funcao recebe o vertice que ainda nao foi visitado, a lista auxiliar e o indice referente
            }
        }
    }
}

//FUNCIONALIDADE 14 - DIJKSTRA


void Relacao_presa_predador(){

    Grafo g = Cria_grafo();      //cria o grafo

    if(g.numero_de_vertices==-1) //se houver problema na criacao do grafo, da erro e nao executa a funcao
    {
        cout << ERRO_PADRAO;
        return;
    }

    int pesquisas;          //nro de pesquisas que serao feitas
    int peso_caminho;       //variavel de retorno da pesquisa, indica o peso do caminho
    char n_presa[61];       //presa da pesquisa
    char n_predador[61];    //predador da pesquisa

    cin >> pesquisas;   //quantas pesquisas serao feitas

    for(int i=0;i<pesquisas;i++)    //loop das pesquisas
    {   
        scan_quote_string(n_predador);  //le o primeiro e ultimo vertice da pesquisa
        scan_quote_string(n_presa);


        peso_caminho=g.dijkstra(n_predador,n_presa);//executa a rotina do algoritmo de dijkstra no grafo
                                                    //funcao acha o menor caminho entre dois vertices
                                                    //funcao retorna -1 se nao houver caminho entre os dois

        //ao fim da pesquisa atual checa se teve ou nao caminho e exibe o resultado
        if(peso_caminho==-1)    
            cout << "CAMINHO INEXISTENTE";
        else 
            cout << peso_caminho;
        cout << "\n";
    }
}

int Grafo::dijkstra(char n_predador[61],char n_presa[61])
{
    int peso=-1;

    return peso;
}




