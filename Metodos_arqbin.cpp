#include "Metodos_arqbin.h"
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
