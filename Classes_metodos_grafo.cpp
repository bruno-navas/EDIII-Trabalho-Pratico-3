#include <cstring>
#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <map>

#include "FuncoesAuxiliares.h"
#include "funcoes_arqbin.h"
#include "Classes_metodos_grafo.h"
#include "Classes_metodos_presa_e_predador.h"


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

    // Loops para a definição do grau de entrada dos vértices
    for (const auto &x : vertices)
    {
        for (const auto &y : x.presas)
        {
            Predador busca_presa = x; // Predador auxiliar recebe os dados do Predador atual
            // cout << busca_presa << endl;
            //  Como a busca em vertices se dá pelo nome do Predador, precisamos trocá-lo
            busca_presa.predador.nome = y.nome_da_presa; // Auxiliar recebe o nome da presa atual

            // Procura a presa em vértices (a presa também é predadora)
            auto it_presa = vertices.find(busca_presa);

            // Se a presa estiver em vertices, it_presa recebe sua posição
            if (it_presa != vertices.end())
            {
                auto &aux_presa = const_cast<Predador &>(*it_presa); // Copiamos seus dados para um auxiliar
                aux_presa.predador.grau_entrada++;                   // Aumentamos seu grau de entrada
                aux_presa.predador.grau++;                           // Aumentamos seu grau
            }


            if (it_presa == vertices.end()) // se a comida nn fizer parte dos vertices, insere: funcoes 13 e 14 eh necessario
                vertices.insert(Predador(y.nome_da_presa));
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

    // Loops que percorem todos os Predadores e todas as suas Presas, exibindo seus dados
    for (const auto& x: vertices) {
        if(x.predador.grau_saida!=-1){          //so exibe se o vertice nao for tipo "sunlight"
            for (const auto& y: x.presas) {
                cout << x << y;
            }
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
        if (auto it_presa = vertices.find(novo_p); it_presa!=vertices.end()) {
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

//a pesquisa eh feita com base no algoritmo de tarjan, abordagem que usa apenas uma travessia pelo grafo
//vale ressaltar que para a funcao funcioar devidamente foi preciso acescentar os vertices
// de alimentos que aida ao estivessem nos vertices, como "sunlight"

//pagina encontrada que ajudou a construir o codigo
//https://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/

int Grafo::Profundidade()
{  
    int componentes = 0; // nro de componentes conexos
    map<string,vis> visit;//map, auxiliar que indexa as informacoes com o nome das especies
    vis temp;             //auxiliar
    stack<string> pilha;  //pilha que sera usada no processo de recursao

    // inicializa o vetor auxiliar que sera utilizado na pesquisa, basicamente os vertices sao considerados nao visitados incialmente
    // e tambem eh destacado o nome da especie
    temp.low = -1;
    temp.disc=-1;
    temp.stack=false;
    for (const auto &x : vertices)      //inicializa o map auxiliar
        visit[x.predador.nome]=temp;
    

    //  chama a funcao recursiva para cada um dos vertices do grafo dentro do loop
    for (const auto &vert_atual : vertices)
        if(visit[vert_atual.predador.nome].disc==-1)    //so faz a visita se nao tiver sido visitado
            Profundidade_recursao(vert_atual, visit,pilha,componentes); // inicia a recursao pelo vertice atual do loop e coloca o indice inicial do vetor de visitados, alem do vector auxiliar em si
    

    return componentes; // a qtd de componentes eh alterada dentro da funcao recursiva
}

//parte recursiva do processo
static int tempo = 0;   //variavel global para abstacao do codigo

void Grafo::Profundidade_recursao(const Predador &vertice, map<string,vis> &visitado, stack<string> &pilha, int &comp)
{
    string atual=vertice.predador.nome; //armazena o nome do vertice, ajuda na leitura do codigo

    visitado[atual].low = visitado[atual].disc = ++tempo;   // a variavel global tempo ajuda na abstracao do codigo e significa quantos passos
    visitado[atual].stack = true;                           // foram precisos para chegar no vertice "atual" dentro do algoritmo
    pilha.push(atual);                                      // coloca o vertice na pilha

    for (const auto &presas_vertice : vertice.presas) // passa pela lista de presas do predador atual
    {
        auto &lista_predador = *vertices.find(Predador(presas_vertice.nome_da_presa));
        // eh nesse find q estao os predadores (com nome na lista de presas) que serao enviados para recursao novamente (ou nao)
        string prox = lista_predador.predador.nome;

        if (visitado[prox].disc == -1)
        {                                                                       // se esse vertice ainda nao foi visitado, chamada recursiva
            Profundidade_recursao(lista_predador, visitado, pilha, comp);       // funcao recebe o vertice que ainda nao foi visitado, a lista auxiliar e o indice referente
            visitado[atual].low = min(visitado[atual].low, visitado[prox].low); // escolhe o menor camiho enntre o atual e o que veio da recursao
        }
        else if (visitado[prox].stack)                                             // se o item ja fizer parte da stack (foi visitado e ainda nao foi removido da pilha)
            visitado[atual].low = (min(visitado[atual].low, visitado[prox].disc)); // escolhe o menor camiho enntre o atual e o da pilha
    }

    // vertice raiz de um componente conexo? tira da pilha os membros,atualiza as flags e aumenta a contagem de componentes connexos
    string w;
    if (visitado[atual].low == visitado[atual].disc)
    {
        while (pilha.top() != atual)
        {
            w = pilha.top();
            visitado[w].stack = false;
            pilha.pop();
        }
        w = pilha.top();
        visitado[w].stack = false;
        pilha.pop();

        comp++;
    }
}

//METODO USADO NA FUNCIONALIDADE 14////////////////////////////////////////////////////////////////

//o algoritmo recebe o vertice inicial e o alvo, o algoritmo guloso
//funciona da maneira determinada nos slides da AULA17

//usa o conceito de fila de prioridade para realizar a pesquisa e atualizar os caminhos

//caso nao exista um caminho entre o vertice inicial e o alvo o peso
//no vetor D vale int_max("infinito")

int Grafo::dijkstra(string n_predador, string n_presa)  {

    //checa se ambos os membros fazem parte da lista de vertices
    auto pred = vertices.find(Predador(n_predador));
    auto alim = vertices.find(Predador(n_presa));

    if(pred==vertices.end() || alim==vertices.end())  //se o inicio ou o fim n estiver na lista de vertices n tem caminho
        return __INT_MAX__;

    if (n_predador == n_presa)                          //predador e presa iguais
    {                                                   //caso obvio, tem que estar na lista de presa
        auto v = vertices.find(Predador(n_predador));   //acha o vertice
        auto presa = v->presas.find(Presa(0,n_presa));  //procura por um alimento com o nome n_presa==n_predador

        if (presa!=v->presas.end()) // se o bixo se comer, tem caminho
            return presa->populacao_do_predador;
                
        return __INT_MAX__; // se o bixo nao se comer, retorna que nao tem caminho
    }


    int peso = 0;
    map<string, int> Di;    //map, indexa de acordo com o nome um vetor de inteiros que sera o peso de cada vertice
    priority_queue<string> fila;

    // inicializa o vetor auxiliar Di
    // que sera utilizado na pesquisa, basicamente os pesos para chegar ao vertice iniciam como infinito exceto o inicial
    for (const auto &x : vertices)
    {
        Di[x.predador.nome]=__INT_MAX__;
        if (x.predador.nome == n_predador) // peso do vertice de partida eh zero
            Di[x.predador.nome]=0;
    }

    fila.push(n_predador); // insere raiz da pesquisa na fila

    while (!fila.empty()) // enquanto a fila n estiver vazia
    {
        string atual = fila.top(); // pega o vertice, remove ele da fila e salva o indice do vetor auxiliar Di no proximo for
        fila.pop();

        auto &topo = *vertices.find(Predador(atual));   //pega o vertice referente ao topo da fila que foi removido

        // passa por todas as presas do vertice atual
        for (auto &presa : topo.presas)
        {
            string proximo = presa.nome_da_presa; // pega o nome da presa e o peso, e salva o indice no vetor auxiliar

            int peso = presa.populacao_do_predador; //peso de incremento do caminho

            // se o caminho que passa por essa presa eh menor ou se ainda nao passou por la
            if (Di[proximo] > Di[atual] + peso)
            {
                // atualiza a distancia do caminho e a fila
                Di[proximo] = Di[atual] + peso;
                fila.push(proximo);
            }
        }
    }

    // retorna o peso do caminho para chegar na presa/alimento a partir do predador dado
    return Di[n_presa];
}
