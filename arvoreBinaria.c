/* Universidade de Brasília - Faculdade do Gama
Engenharia de Software

Disciplina: Estrutura de dados e Algoritmos

Professor: Dr. Nilton Correia da Silva

GRUPO G- CO2 Emissions Around the World     
ALUNOS:
Vinicius de Oliveira Santos -202017263
Mayara Alves de Oliveira - 200025058
Dara Maria Barbosa de Sousa - 202046040 */


#include <stdio.h>
#include <stdlib.h>     // BIBLIOTECAS
#include <string.h>

//Variavel global para guardar o ponteiro do arquivo
FILE *myfile;
//Variavel global com o nome do arquivo a ser aberto
char filename[50];

//Estrutura que guarda os campos do registro a ser inserido na arvore
typedef struct tipoReg {
  char chave[1000];
  int linha;
} tipoReg;

typedef struct tipoElem {
  //Estrutura que define o que vai ser um elemento da arvore (um no) e cada
  //elemento vai ter um registro e 2 ponteiros, um para esquerda e para direita
  tipoReg Reg;
  struct tipoElem *esq, *dir;

} tipoElem;

typedef tipoElem *tipoArvore; // definindo a arvore, que vai ser um ponteiro do tipoElem  

tipoArvore raiz; //variavel global que representa a arvore
tipoReg registro;  //variavel global que representa a estrutura de registro


// Declaracao de funcoes
void criaArvore(tipoArvore *raiz);
int vaziaArvore(tipoArvore raiz);
int insereArvore(tipoArvore *raiz, tipoReg Reg);
void procuraNoArquivo(int linha);
void emOrdem(tipoArvore raiz);
void liberaArvore(tipoElem *raiz);
int carregarArquivos(char filename[50]);
void limpa_buffer();

// Inicio da main
int main() {
  criaArvore(&raiz);
  
  int opcao;
  
  do{
    printf("Opcao 1: Carregar Arquivos de Dados\n");
    printf("Opcao 2: Emitir Relatorio\n");
    printf("Opcao 3: Sair\n");

    scanf("%d", &opcao);
    //limpando o buffer de leitura
    limpa_buffer();

    switch(opcao){
      case 1:
        
        printf("Digite o nome do arquivo: ");
        if(vaziaArvore(raiz)){
          criaArvore(&raiz);
        }
        else{
          liberaArvore(raiz);
          criaArvore(&raiz);
        }
        scanf("%s",filename);
        limpa_buffer();
        carregarArquivos(filename);
        break;

      case 2:
        if(vaziaArvore(raiz)){
          printf("Não existem dados \n");
        } 
        else{
          emOrdem(raiz);
        }
        break;

      case 3:
        //free na raiz, libera toda a memoria que ela ocupa, incluindo as dos seus filhos
        liberaArvore(raiz);
        printf("Programa Encerrado.\n");
        return 0;
    }
  }while(opcao != 3);

  
  return 0;
}

void criaArvore(tipoArvore *raiz) {
  (*raiz) = NULL;
}

// Essa funcao faz a leitura do arquivo e a montagem da arvore
int carregarArquivos(char filename[50]){
  char line[500]; // linha para ler o arquivo
  char countryName[500], region[500]; // strings auxiliares
  int linha = 0; // numero da linha do registro
  int idx = 0; // variavel auxiliar para fazer a separacao dos campos na linha lida
  int aux,idxChave,i,countryNameIdx,regionIdx; // variaveis auxiliares

  myfile = fopen(filename, "r");
    
  if(myfile == NULL){
    printf("Erro ao abrir arquivo\n");
    printf("Arquivo vazio ou nao encontrado \n");
    return 0 ;
  } 
    
  
  while (fgets(line, 500, myfile) != NULL){
    countryNameIdx = 0;
    regionIdx = 0;

    // se o contador de linha for = 0, estamos no cabecalho do arquivo, que nao deve ser lido
    if (linha == 0) {
      linha += 1;
      continue;
    }

    // tratar caso do Country Name comecar com aspas
    i = 0;
    if (line[i] == '"') {
      for (i = 1;; i++) {
        if (line[i] == '"') {
          idx = i + 2;
          break;
        }
        countryName[countryNameIdx] = line[i];
        countryNameIdx += 1;
      }
    } else {
      for (i = 0;; i++) {
        if (line[i] == ',') {
          idx = i + 1;
          break;
        }
        countryName[countryNameIdx] = line[i];
        countryNameIdx += 1;
      }
    }
    
    //adicionando final de string
    countryName[countryNameIdx] = '\0';

    // faz a leitura do segundo campo, a leitura serve apenas para caminhar com o ponteiro na linha ate chegar no proximo campo
    for (i = idx;; i++) {
      if (line[i] == ',') {
        idx = i + 1;
        break;
      }
    }

    // leitura do campo Region
    for (i = idx;; i++) {
      if (line[i] == ',') {
        idx = i + 1;
        break;
      }
      region[regionIdx] = line[i];
      regionIdx += 1;
    }

    //adicionando final de string
    region[regionIdx] = '\0';

    // se a leitura for vazia, encerramos a leitura, uma garantia a mais para o caso da linha exitir com espacos em branco
    if (countryName[0] == ' ' && region[0] == ' ') {
      break;
    }
    
    //colocando o Region no campo chave do registro
    idxChave = 0;
    aux = 0;
    while(region[aux] != '\0'){
      registro.chave[idxChave] = region[aux];
      aux+=1;
      idxChave+=1;
    }

     //concatenando o campo countryName no campo chave do registro
    aux = 0;
    while(countryName[aux] != '\0'){
      registro.chave[idxChave] = countryName[aux];
      aux+=1;
      idxChave+=1;
    }

    registro.chave[idxChave] = '\0'; //adicionando final de string

    registro.linha = linha; //colocando a linha no registro

    insereArvore(&raiz, registro); //inserindo na arvore

    //limpando as duas variaveis das strings
    i = 0;
    while (countryName[i]) {
      countryName[i] = '\0';
      i += 1;
    }
    i = 0;
    while (region[i]) {
      region[i] = '\0';
      i += 1;
    }
    linha += 1;
  }

  fclose(myfile);
  printf("Arquivo lido com sucesso \n");
  return 1;
}

// Esta funcao verifica se a arvores estao vazias
int vaziaArvore(tipoArvore raiz) {
  return (raiz == NULL);
}

//Esta funcao insere um registro na arvore
int insereArvore(tipoArvore *raiz, tipoReg Reg) {

  if (vaziaArvore(*raiz)) {
    (*raiz) = malloc(sizeof(tipoElem));
    (*raiz)->Reg = Reg;
    (*raiz)->esq = NULL;
    (*raiz)->dir = NULL;
    return 1;
  } else if (strcmp((*raiz)->Reg.chave, Reg.chave) < 0) {
    return insereArvore(&((*raiz)->dir), Reg);
  } else if (strcmp((*raiz)->Reg.chave, Reg.chave) > 0) {
    return insereArvore(&((*raiz)->esq), Reg);
  } else
    return 0;
}

// Mostra a arvore em ordem, no metodo de busca ECD
void emOrdem(tipoArvore raiz) {
  if (!vaziaArvore(raiz)) {
    emOrdem(raiz->esq);
    procuraNoArquivo(raiz->Reg.linha);
    printf("%s - %d \n", raiz->Reg.chave, raiz->Reg.linha);
    printf("\n");
    emOrdem(raiz->dir);
  }
}

// Essa funcao procura no arquivo a linha do registro salvo na arvore
void procuraNoArquivo(int linha){
  FILE *f = fopen(filename, "r");
  FILE *arquivoSaida = fopen("arquivoSaida.csv", "a+");
  char line[500];
  int contador = 0;
  while (fgets(line, 500, myfile) != NULL){
    if(contador == linha ){
      //printf("%s \n", line); //mostra as linhas no terminal
      fprintf(arquivoSaida, "%s", line);
      break;
    }
    contador+=1;
  }
  fclose(f);
  fclose(arquivoSaida);
}

// Libera a memoria dos nos
void liberaArvore(tipoElem *raiz){
  if(!vaziaArvore(raiz)){
    liberaArvore(raiz->esq);
    liberaArvore(raiz->dir);
    // printf("liberando\n");
    free(raiz);
  }
}

void limpa_buffer() {
	#ifdef _WIN32
		fflush(stdin);
	#else
		__fpurge(stdin);
	#endif
}