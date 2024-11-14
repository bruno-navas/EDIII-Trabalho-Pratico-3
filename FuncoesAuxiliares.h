#ifndef FUNCOESPADRAO_H
#define FUNCOESPADRAO_H

#define ERRO_PADRAO     "Falha no processamento do arquivo\n"  //mensagem padrao de erro
#define ERRO_REGISTRO   "Registro Inexistente.\n"              //mensagem de erro para registro inexistente
#define ERRO_COMANDO    "O comando digitado nao existe\n"      //mensagem de erro caso o usuario digite um comando invalido
#define ERRO_CADASTRO   "Informação ja inserida no arquivo\n"  //mensagem de erro para dados ja cadastrados

void Seletor_de_funcionalidade (int selecao);
void scan_quote_string(char *str);
//char* strsep(char** stringp, const char* delim);

#endif //FUNCOESPADRAO_H
