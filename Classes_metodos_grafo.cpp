#include <cstring>
#include <utility>
#include <vector>
#include <queue>

#include "FuncoesAuxiliares.h"
#include "funcoes_arqbin.h"
#include "Classes_metodos_grafo.h"
using namespace std;

//CONSTRUTOR DO GRAFO DADO UM PONTEIRO PRA ARQUIVO, se tiver problema no inicio retorna o grafo com V=-1 para encerrar o programa
Grafo::Grafo(FILE* arquivo) {

    if(arquivo==nullptr){              //se o arquivo nao existir
        numero_de_vertices=-1;
        return;
    }

    Cabecalho cabecalho(arquivo); // Chama o construtor de Cabecalho e armazena os dados em cabecalho

    if(cabecalho.status=='0'){      //se  arquivo estiver inconsistente
        numero_de_vertices=-1;
        return;
    }

    // Inicializa as variáveis de contagem com 0
    numero_de_vertices = numero_de_ciclos = 0;

    // Pula o "lixo" do arquivo binario
    fseek(arquivo, 1600, SEEK_SET);

    // Loop para a leitura de todos os predadores do arquivo
    for (int i = 0; i < cabecalho.proxRRN; i++) {

        // Lê o predador do arquivo binário
        Predador p(arquivo);

        // Lê a presa do arquivo binário (os dados da presa já estão armazenados no Predador p para evitar leitura dupla do arquivo)
        Presa pr(p.predador.populacao, p.predador.presa);

        if(pr.populacao_do_predador==-1) {
            continue;
        } // Predador inconsistente

        // Procura o predador em vertices
        auto it_predador = vertices.find(p);

        // Se p estiver em vertices, it_predador vai apontar para sua posição
        if (it_predador!=vertices.end()) {
            auto& aux_predador = const_cast<Predador&>(*it_predador); // Copiamos seus dados para um auxiliar
            aux_predador.predador.grau_saida++; // Aumentamos seu grau de saída
            aux_predador.predador.grau++; // Aumentamos seu grau
            aux_predador.insere_presa(pr); // Inserimos a nova presa
        }
        // Adicionamos a presa lida ao predador e então, adicionamos o predador aos vertices
        else {
            p.insere_presa(pr);
            vertices.insert(p);
            numero_de_vertices++; // Temos um novo vértice
        }
    }

    //Loops para a definição do grau de entrada dos vértices
     for (const auto& x : vertices) {
         for (const auto& y : x.presas) {
              Predador busca_presa = x; // Predador auxiliar recebe os dados do Predador atual
              //cout << busca_presa << endl;
              // Como a busca em vertices se dá pelo nome do Predador, precisamos trocá-lo
              busca_presa.predador.nome = y.nome_da_presa; // Auxiliar recebe o nome da presa atual

              // Procura a presa em vértices (a presa também é predadora)
              auto it_presa = vertices.find(busca_presa);

              // Se a presa estiver em vertices, it_presa recebe sua posição
              if(it_presa!=vertices.end()) {
                  auto& aux_presa = const_cast<Predador&>(*it_presa); // Copiamos seus dados para um auxiliar
                  aux_presa.predador.grau_entrada++; // Aumentamos seu grau de entrada
                  aux_presa.predador.grau++; // Aumentamos seu grau
             }
         }
     }
}


//METODOS USADOS NA FUNCAO 10///////////////////////////////////////////////////////////////////////

// Metodo que chama o construtor do grafo com um ponteiro de arquivo binario inicializado de acordo com o nome dado
Grafo Cria_grafo() {
    string nome;
    cin >> nome;

    FILE *arq_bin = fopen(nome.c_str(), "rb");

    return Grafo(arq_bin);
}

void Grafo::exibe_grafo() const {

    if(numero_de_vertices==0)
    {
        cout << ERRO_PADRAO;
        return;
    } // Grafo vazio

    // Loops que percorem todos os Predadores e todas as suas Presas, exibindo seus dados
    for (const auto& x: vertices) {
        for (const auto& y: x.presas) {
            cout << x << y;
        }
    }
}


//METODOS USADOS NA FUNCAO 11///////////////////////////////////////////////////////////////////////

void Grafo::busca_predadores(char presa[]) const {
    // Armazena o resultado
    string saida;

    // Percorre todos os Predadores e suas Presas
    for(const auto& x:vertices) {
        for(const auto& y:x.presas) {
            // Se encontrar a presa buscada nas Presas do Predador, adiciona o nome do Predador à resposta
            if (y.nome_da_presa==presa) {
                saida += x.predador.nome + ", ";
            }
        }
    }

    // Se a saída não for vazia, exibe o novo da presa e de todos os seus predadores
    if(!saida.empty()) {
        saida.erase(saida.size()-2,2); // Remove as partes desnecessárias da resposta
        cout << presa << ": " << saida<<endl << endl;
    }
    else {
        cout << ERRO_REGISTRO << endl;
    }
}


//METODOS USADOS NA FUNCIONALIDADE 12/////////////////////////////////////////////////////////////

// Variáveis globais para maior abstração do código
int Branco = 0; // Predador não visitado
int Cinza = 1;  // Predador visitado
int Preto = 2;  // Predador fora da pilha

void Grafo::detecta_ciclos()  {

    // Vetor de inteiros que armazena o status de cada um dos Predadores
    vector<int> visitados(numero_de_vertices);
    int pos = 0;

    // Percorremos todos os predadores e se seu status for Branco, chamamos o metodo auxiliar
    for (const auto& x: vertices) {
        if (visitados[pos]==Branco) {
            auxiliar_ciclos(x, pos, visitados);
        }
        pos++;
    }

    cout << "Quantidade de ciclos: " << numero_de_ciclos << endl;
}

void Grafo::auxiliar_ciclos(const Predador& p, int pos, vector<int>& visitados) {
    // Se o Predador já foi visitado, temos um ciclo
    if (visitados[pos]==Cinza) {
        numero_de_ciclos++;
        return;
    }
    // Se o Predador está fora da pilha, já exploramos todos os seus caminhos
    if (visitados[pos]==Preto) {
        return;
    }

    // Indicamos que o Predador atual já foi visitado
    visitados[pos]=Cinza;

    // Percorremos todas as Presas do Predador atual
    for (const auto& x: p.presas) {
        Predador novo_p = p; // Auxiliar recebe os dados do Predador atual
        novo_p.predador.nome = x.nome_da_presa; // Trocamos seu nome pelo nome da Presa atual para a busca em vertices

        // Se a Presa atual estiver em vertices (também é predadora), chamamos o metodo auxiliar para ela
        auto it_presa = vertices.find(novo_p);
        if (it_presa!=vertices.end()) {
            novo_p = *it_presa; // Atualizamos o auxiliar com os dados da posição em que encontramos a Presa em vertices
            const int nova_pos = static_cast<int>(distance(vertices.begin(), it_presa)); // Calculamos a nova_pos pela distância de it_presa e o inicio de vertices
            auxiliar_ciclos(novo_p, nova_pos, visitados);
        }
    }

    // Alteramos o status do Predador para indicar que já visitamos todos os seus caminhos
    visitados[pos]=Preto;
}


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

void Grafo::Profundidade_recursao(const Predador& vertice, const int x, vector<vis> &visitado)
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

int Grafo::dijkstra(char n_predador[91], char n_presa[91]) const {

    if (!strcmp(n_predador, n_presa)) // caso obvio
        return 0;

    int i, v, u;
    int peso = 0;
    vector<dij> D; // vetor D que armazena os pesos para chegar ate cada vertice partindo de um vertice de predador
    dij temp;      // vetor auxiliar
    priority_queue<string> fila;
    // inicializa o vetor auxiliar D

    // inicializa o vetor auxiliar que sera utilizado na pesquisa, basicamente os pesos para chegar ao vertice iniciam como infinito exceto o inicial
    // e tambem eh destacado o nome da especie
    for (const auto &x : vertices)
    {
        temp.peso = __INT_MAX__; //infinito
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

                    for (u = 0; u < numero_de_vertices; u++){ // pega o indice do predador/presa na lista auxiliar
                        if (D[u].nome == proximo)
                            break;
                    }
                    int peso = presa.populacao_do_predador;

                    // se o caminho que passa por essa presa eh menor ou se ainda nao passou por la
                    if (D[u].peso > D[v].peso + peso)
                    {
                        // atualiza a distancia do caminho
                        D[u].peso = D[v].peso + peso;
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
