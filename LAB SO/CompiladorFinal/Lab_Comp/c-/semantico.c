#include "globals.h"
#include "symtab.h"
#include "semantico.h"
#include "string.h"
extern int ERRO;

static void typeError(TreeNode * t, char * message)
{
    fprintf(stderr,"ERRO SEMÂNTICO: %s NA LINHA %d: %s\n",t->attr.name, t->lineno, message);
    ERRO = 1;
}

static int location = 0;

static void traverse(TreeNode * t, void (* preProc) (TreeNode *), void (* postProc) (TreeNode *))
{

    if (t != NULL)
    {
        preProc(t);
        {
            int i;
            for (i=0; i < MAXCHILDREN; i++)
                traverse(t->child[i],preProc,postProc);
        }
        postProc(t);
        traverse(t->sibling,preProc,postProc);
    }
}

static void nullProc(TreeNode * t)
{
    if (t==NULL)
        return;
    else
        return;
}

static void insertNode( TreeNode * t)
{

    switch (t->nodekind)
    {

        case StmtK:
            switch (t->kind.stmt)
            {
                case VariableK:
                    if (st_lookup(t->attr.name,t->scope) == -1 && st_lookup(t->attr.name,"global") == -1 )
                    {
                    	switch(t->type){
                    	case 0: break;
                    	case 1:
                        	st_insert(t->attr.name,t->lineno,location++,  t->scope, "variável", "inteiro");
				break;
			case 2:
                        	st_insert(t->attr.name,t->lineno,location++,  t->scope, "variável", "vetor");
				break;
                    	}
                    }
                    else
                        typeError(t, "VARIÁVEL JÁ DECLARADA.");
                    break;
                 
                case ParamK:
                    if (st_lookup(t->attr.name,t->scope) == -1 && st_lookup(t->attr.name,"global") == -1 )
                    {
                    	switch(t->type){
                    	case 0: break;
                    	case 1:
                        	st_insert(t->attr.name,t->lineno,location++,  t->scope, "variável", "inteiro");
				break;
			case 2:
                        	st_insert(t->attr.name,t->lineno,location++,  t->scope, "variável", "vetor");
				break;
                    	}
                    }
                    else
                        typeError(t, "VARIÁVEL JÁ DECLARADA.");
                    break;

                case FunctionK:
                    if (st_lookup(t->attr.name,t->scope) == -1)
                    {
                        if(t->type == 1)
                            st_insert(t->attr.name,t->lineno,location++, t->scope,"função", "inteiro");
                        else
                            st_insert(t->attr.name,t->lineno,location++, t->scope,"função", "void");
                    }
                    else
                        typeError(t, "FUNÇÃO JÁ DECLARADA.");
                    break;

                case ReturnK:
                    break;

                default:
                    break;
            }
            break;

        case ExpK:
            switch (t->kind.exp)
            {
                case IdK:
		    if( st_lookup(t->attr.name,"global") != -1)
		     st_insert(t->attr.name,t->lineno,0, "global", "variável", "inteiro");
                    else if (st_lookup(t->attr.name,t->scope) == -1)
                        typeError(t,"IDENTIFICADOR NÃO DECLARADO");
                    else
                        st_insert(t->attr.name,t->lineno,0, t->scope, "variável", "inteiro");
                    break;

                case VetorK:
		    if( st_lookup(t->attr.name,"global") != -1)
			st_insert(t->attr.name,t->lineno,0, "global", "vetor", "inteiro");
                    else if (st_lookup(t->attr.name,t->scope) == -1)
                        typeError(t,"VETOR NÃO DECLARADO");
                    else
                        st_insert(t->attr.name,t->lineno,0, t->scope, "vetor", "inteiro");
                    break;
                
                case CallK:
                    if (st_lookup(t->attr.name,"global") == -1)
                        typeError(t, "FUNÇÃO NÃO DECLARADA.");
                    else
                    {
			switch(t->type){
			case 0:
                        	st_insert(t->attr.name,t->lineno,location++, "global", "função", "void");
				break;
			case 1:
                        	st_insert(t->attr.name,t->lineno,location++, "global", "função", "inteiro");
				break;
			default: break;
			}
		     }
		     break;

                case TypeK:
                    break;

                default:
                    break;
        }
        break;

        default:
            break;
    }
}

void buildSymtab(TreeNode * syntaxTree)
{
    FILE * listing;
    listing = fopen("./Saidas/tabeladesimbolo.txt","w+");
    st_insert("output",0,location++, "global","função", "inteiro");
    st_insert("input",0,location++, "global","função", "inteiro");
    st_insert("CpyHDtoIM",0,location++, "global","função", "inteiro");
    st_insert("CpyRegtoHD",0,location++, "global","função", "inteiro");
    st_insert("CpyHDtoReg",0,location++, "global","função", "inteiro");
    st_insert("CpyDMtoHD",0,location++, "global","função", "inteiro");
    st_insert("CpyHDtoDM",0,location++, "global","função", "inteiro");
    st_insert("Display",0,location++, "global","função", "inteiro");
    st_insert("DisplayProgram",0,location++, "global","função", "inteiro");
    st_insert("Execproc",0,location++, "global","função", "inteiro");
    st_insert("setCS",0,location++, "global","função", "inteiro");
    st_insert("unSetCS",0,location++, "global","função", "inteiro");
    st_insert("jumpProc",0,location++, "global","função", "inteiro");
    traverse(syntaxTree,insertNode,nullProc);
    if(st_lookup("main","global") == -1)
    {
        printf("FUNÇÃO \'main\' NÃO DECLARADA.");
        ERRO = 1;
    }
   printSymTab(listing);

}

static void checkNode(TreeNode * t)
{
    switch (t->nodekind)
    {

        case StmtK:
            switch (t->kind.stmt)
            {
                case IfK:
                    if (t->child[0]->type == 1 && t->child[1]->type == 1)
                        typeError(t->child[0],"COMPARAÇÃO IF INVÁLIDA(VOID)");
                    break;

                case AssignK:
                    if(t->child[1]->kind.exp == CallK)
                    	 if(strcmp(st_lookup_type(t->child[1]->attr.name,"global"), "void") == 0)
                            typeError(t->child[1],"RECEBENDO PARAMETRO DE UMA FUNÇÃO VOID");
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

void typeCheck(TreeNode * syntaxTree)
{
    traverse(syntaxTree,nullProc,checkNode);
}
