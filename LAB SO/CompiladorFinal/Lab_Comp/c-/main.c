#include <stdio.h>
#include "globals.h"
#include "util.h"
#include "assembly.h"
#include "binario.h"
#include "binarioSO.h"
#include "yacc.h"
#include "semantico.h"
#include "cgen.h"

extern FILE *yyin;
int ERRO = 0;
int lineno = 1;
TokenType GetToken();
void analise_lexica();


int main(int argc, char *argv[]){
    FILE *entrada, *saida, *saida1, *saida2, *saida3;
    TreeNode *arvore_sintatica;
    QuadList *Q;
    AssemblyList *A;
    BinaryList *B;

    char path[50];
    int flag = 0;

     /* Input file path */
    printf("Enter file path: ");
    scanf("%s", path);

    if(strcmp(path, "SO.txt") == 0)
      flag = 1;

    /* Try to open file */
    entrada = fopen(path, "r");

    /* Exit if file not opened successfully */
    if (entrada == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read previleges.\n");

        exit(EXIT_FAILURE);
    }
    
    yyin = entrada;
    printf("\nCriando arquivo de saída da Análise léxica...\n" );
    analise_lexica();
    if(ERRO){
        printf("\nErro ao criar arquivo de saída da Análise léxica\n" );
        return 0;
    }
    else{
      printf("\nCriado com Sucesso!\n" );
    }
    saida = fopen("./Saidas/arvoresintatica.txt","w+");
    rewind(yyin);
    lineno = 1;
    printf("\nCriando arquivo de saída da Árvore Sintática...\n" );
    arvore_sintatica = parse();
    if(ERRO){
        printf("\nErro ao criar arquivo de saída da Árvore Sintática\n" );
        return 0;
    }
    else{
      printf("\nCriado com Sucesso!\n" );
    }
    printTree(arvore_sintatica, saida);
    printf("\nCriando arquivo de saída da Tabela de Símbolos...\n" );
    buildSymtab(arvore_sintatica);
    if(ERRO){
        printf("\nErro ao criar arquivo de saída da Tabela de Símbolos\n" );
        return 0;
    }
    else{
      printf("\nCriado com Sucesso!\n" );
    }
    typeCheck(arvore_sintatica);
    printf("\nCriando arquivo de saída das quadruplas...\n" );
    Q = codeGen(arvore_sintatica, flag);
    saida1 = fopen("./Saidas/cgen.txt","w+");
    printQuad(Q, saida1);
    printf("\nCriado com Sucesso!\n" );
    while(Q->last != NULL)
	  Q = Q->last;
    printf("\nCriando arquivo de saída do codigo Assembly...\n" );
    A = AssemblyGenerator(Q, flag);
    saida2 = fopen("./Saidas/assembly.txt","w+");
    printAssembly(A, saida2);
    printf("\nCriado com Sucesso!\n" );
    printf("\nCriando arquivo de saída do codigo Binario...\n" );
    if(flag == 0)
      B = BinaryGenerator(A);
    else 
      B = BinaryGeneratorSO(A);
    saida3 = fopen("./Saidas/binario.txt", "w+");
    printBinario(B, saida3);
     printf("\nCriado com Sucesso!\n" );
    return 0;
}
