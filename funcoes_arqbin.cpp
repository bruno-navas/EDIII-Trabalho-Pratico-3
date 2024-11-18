#include "funcoes_arqbin.h"
#include "FuncoesAuxiliares.h"

//le o cabecalho do arquivo de dados
Cabecalho::Cabecalho(FILE* arquivo) {
    fread(&status, 1, 1, arquivo);                 // Lê 1 byte para 'status'
    fread(&topo, sizeof(int), 1, arquivo);         // Lê 4 bytes para 'topo'
    fread(&proxRRN, sizeof(int), 1, arquivo);      // Lê 4 bytes para 'proxRRN'
    fread(&nroRegRem, sizeof(int), 1, arquivo);    // Lê 4 bytes para 'nroRegRem'
    fread(&nroPagDisco, sizeof(int), 1, arquivo);  // Lê 4 bytes para 'nroPagDisco'
    fread(&qttCompacta, sizeof(int), 1, arquivo);  // Lê 4 bytes para 'qttCompacta'
}

//le um dado do arquivo de dados
Dado_bin::Dado_bin(FILE *arquivo) {
    fread(&removido, 1, 1, arquivo);                       // Lê 1 byte para 'removido'
    fread(&encadeamento, 4, 1, arquivo);                   // Lê 4 bytes para 'encadeamento'
    fread(&populacao, 4, 1, arquivo);                      // Lê 4 bytes para 'populacao'
    fread(&tamanho, 4, 1, arquivo);                        // Lê 4 bytes para 'tamanho'
    fread(&unidadeMedida, 1, 1, arquivo);                  // Lê 1 byte para 'unidadeMedida'
    fread(&velocidade, 4, 1, arquivo);                     // Lê 4 bytes para 'velocidade'
    fread(&variavel, 1, 142, arquivo);                     // Lê 142 bytes para 'variavel'

}

//Construtor da classe Especie
Especie::Especie(FILE* arquivo) {
    Dado_bin auxiliar(arquivo); // Le os dados do arquivo bin com o uso do construtor de Dado_bin

    // Separa as informações recebidas ans varíáveis da classe
    char *lin = strdup(auxiliar.variavel); // Auxiliar

    populacao = auxiliar.populacao;

    // transfere os dados de auxiliar para as varíáveis com strsep e delimitador '#"
    nome = strsep(&lin, "#");
    especie = strsep(&lin, "#");
    habitat = strsep(&lin, "#");
    dieta = strsep(&lin, "#");
    tipo = strsep(&lin, "#");
    presa = strsep(&lin, "#");

    grau_entrada = 0;
    grau_saida = 1; // Se o vértice foi criado, ele possui ao menos uma presa
    grau = 1;
}

Especie::Especie(string NOME) {

    populacao=0;
    nome = NOME;
    especie = "NA";
    habitat = "NA";
    dieta = "NA";
    tipo = "NA";
    presa = "NA";
    grau_entrada = 0;
    grau_saida = -1;
    grau = 0;
}
