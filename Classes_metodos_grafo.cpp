#include <cstring>
#include <utility>
#include <vector>
#include <queue>

#include "FuncoesAuxiliares.h"
#include "funcoes_arqbin.h"
#include "Classes_metodos_grafo.h"

using namespace std;

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
//CONSTRUTOR DO GRAFO DADO UM PONTEIRO PRA ARQUIVO, se tiver problema no inicio retorna o grafo com V=-1 para encerrar o programa
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
//METODOS USADOS NA FUNCAO 10///////////////////////////////////////////////////////////////////////

//metodo que chama o construtor do grafo com um ponteiro de arquivo binario inicializado de acordo com o nome dado
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

//A FUNCIONALIDADE 11 NAO USA NENHUM METODO DA CLASSE DO GRAFO, APENAS OS DADOS DELE////////////

//METODOS USADOS NA FUNCIONALIDADE 12/////////////////////////////////////////////////////////////


//METODOS USADOS NA FUNCIONALIDADE 13///////////////////////////////////////////////////////////

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

//METODO USADO NA FUNCIONALIDADE 14////////////////////////////////////////////////////////////////

//o algoritmo recebe o vertice inicial e o alvo, o algoritmo guloso
//funciona da maneira determinada nos slides da AULA17

//caso nao exista um caminho entre o vertice inicial e o alvo o peso
//no vetor D vale -1

int Grafo::dijkstra(char n_predador[91], char n_presa[91])
{

    if (!strcmp(n_predador, n_presa)) // caso obvio
        return 0;

    int i, v, u;
    int peso = 0;
    vector<dij> D; // vetor D que armazena os pesos para chegar ate cada vertice partindo de um vertice de predador
    dij temp;      // vetor auxiliar
    priority_queue<string> fila;
    // inicializa o vetor auxiliar D

    // inicializa o vetor auxiliar que sera utilizado na pesquisa, basicamente os pesos para chegar ao vertice iniciam como -1 exceto o inicial
    // e tambem eh destacado o nome da especie
    for (const auto &x : vertices)
    {
        temp.peso = -1;
        temp.nome = x.predador.nome;
        if (temp.nome == n_predador) // peso do vertice de partida eh zero
            temp.peso = 0;
        D.push_back(temp);
    }

    fila.push(n_predador); // insere raiz da pesquisa na fila

    while (!fila.empty()) // enquanto a fila n estiver vazia
    {
        string atual = fila.top(); // pega o vertice, remove ele da fila e salva o indice do vetor auxiliar D no proximo for
        fila.pop();

        v = 0;
        for (auto &topo : vertices) // acha o vertice atual para passar por todas as presas dele
        {
            if (topo.predador.nome == atual)
            {

                // passa por todas as presas do vertice atual
                for (auto &presa : topo.presas)
                {
                    string proximo = presa.nome_da_presa; // pega o nome da presa e o peso, e salva o indice no vetor auxiliar

                    for (u = 0; u < numero_de_vertices; u++) // pega o indice do predador/presa na lista auxiliar
                        if (D[u].nome == proximo)
                            break;

                    int peso = presa.populacao_do_predador;

                    // se o caminho que passa por essa presa eh menor
                    if (D[v].peso > D[u].peso + peso)
                    {
                        // atualiza a distancia do caminho
                        D[v].peso = D[u].peso + peso;
                        fila.push(proximo);
                    }
                }
            }
            v++; // atualiza indice do vetor auxiliar
        }
    }

    // retorna o peso do caminho para chegar na presa/alimento a partir do predador dado
    for (i = 0; i < numero_de_vertices; i++)
        if (D[i].nome == n_presa)
            break;
    return D[i].peso;
}
