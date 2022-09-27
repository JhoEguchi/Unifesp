#include "globals.h"
#include "assembly.h"
#include "symtab.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int spPos = 0, GlobalPos = 0;

int line = 464;
int argcount=0;
int paramcount=0;

char *scope = "global";

typedef struct Data
{
    char *dataname;
    int size;
    int memorypos;
    char *scope;
    struct Data *next;
} DataList;	//Estrutura de dado que representa um elemento de memória, isto é, uma variável

typedef struct Memory
{
    char *scope;
    int scopesize;
    DataList *data;
    struct Memory *next;
} MemoryList;	// Estrutura de dado que faz uma tabela das variáveis, determinando a posição na pilha e o escopo

int isNumber(char *test)
{
    if (isdigit(test[0]) == 0)
        return 0;
    return 1;
}

MemoryList * IniciaMemoria()	// Iniciar a tabela das variáveis
{
    MemoryList *m = (MemoryList *)malloc(sizeof(MemoryList));
    m->scope = NULL;
    m->scopesize = 0;
    m->data = NULL;
    m->next = NULL;
    return m;
}

void InsereMemoria(MemoryList *head, char *scope, char *name, int size)	// Inserir na tabela
{
    MemoryList *pointer, *prepointer = NULL;
    DataList *new = (DataList *)malloc(sizeof(DataList));
    DataList *ap, *previous = NULL;
    pointer = head;
    new->dataname = name;
    new->size = size;
    new->scope = scope;
    new->next = NULL;
    if (pointer->scope == NULL) // Primeira inserção na lista de memórias
    {
        pointer->scope = scope;
        if(strcmp(scope, "main" ) == 0 || strcmp(scope, "global") == 0)
    		pointer->scopesize = 0;
    	else
    		pointer->scopesize = 1;
        new->memorypos = pointer->scopesize;
        pointer->scopesize += size;
        pointer->data = new;
        pointer->next = NULL;
        return;
    }
    while (pointer != NULL)
    {
        if (strcmp(scope, pointer->scope) == 0)
        {
            new->memorypos = pointer->scopesize;
            pointer->scopesize+=size;
            ap = pointer->data;
            while (ap != NULL)
            {
                previous = ap;
                ap = ap->next;
            }
            ap = new;
            if (previous != NULL)
                previous->next = new;
            return;
        }
        prepointer = pointer;
        pointer = pointer->next;
    }
    MemoryList *newmem = (MemoryList *)malloc(sizeof(MemoryList));
    newmem->scope = scope;
    if(strcmp(scope, "main") != 0)
    	newmem->scopesize = 1;
    else
    	newmem->scopesize = 0;
    new->memorypos = newmem->scopesize;
    newmem->scopesize += size;
    newmem->data = new;
    newmem->next = NULL;
    if (prepointer != NULL)
        prepointer->next = newmem;
}

DataList * FindMemory(MemoryList *head, char *scope, char *name){	// Encontrar a variável
    MemoryList *pointer;
    DataList *ap;
    pointer = head;
    if(strcmp(pointer->scope, "global") == 0){ //procura primeiro no global
        ap = pointer->data;
        while(ap != NULL)	
        {		
            if(strcmp(ap->dataname, name) == 0){
                return ap;
            }
            ap = ap->next;
        }
        pointer->next;
    }
    while(pointer != NULL){
        if(strcmp(pointer->scope, scope) == 0){
            ap = pointer->data;
            while(ap != NULL){
                if(strcmp(ap->dataname, name) == 0){
                    return ap;
                }
                ap = ap->next;
            }
         }
         pointer = pointer->next;
    }
}

QuadList * AlocaGlobal(QuadList *q, MemoryList *head){	// Caso a função tenha variáveis globais, iniciar a tabela com os elementos globais
	while(q->op != FUNC){
	    if(strcmp(q->arg2, "-") == 0){
                InsereMemoria(head, q->arg1, q->res, 1);
                GlobalPos++;
            }
            else{
                InsereMemoria(head, q->arg1, q->res, atoi(q->arg2));
                GlobalPos += atoi(q->arg2);
	    }
	    q = q->next;
	 }
	 return q;
}

void PrintMemory(MemoryList *m, FILE *a){	// Mostrar na tela a "pilha" do escopo
	DataList *ap;
	while(m != NULL){
		fprintf(a, "%s, %d\n\n", m->scope, m->scopesize);
		ap = m->data;
		while(ap != NULL){
			fprintf(a, "%s, %d\n", ap->dataname, ap->memorypos);
			ap = ap->next;
		}
		fprintf(a, "\n");
		m = m->next;
	}
}

void GenAssembly(QuadList *quad, MemoryList *head, AssemblyList *A, int flag)	// Funcao que gera o codigo assembly
{
    //IFF,GOTO,LAB,ASSIGN,STORE,ALLOC,END,RET,LOAD,PARAM,CALL,FUNC,ARG,ADD,SUB,MULT,DIV,GT,GTE,LT,LTE,EQUAL,DIF,HALT
    DataList *d;
    while (quad->next != NULL)
    {
        int flag1, flag2, flagres;
        flag1 = isNumber(quad->arg1);
        flag2 = isNumber(quad->arg2);
        flagres = isNumber(quad->res); 
        switch (quad->op)
        {
        case HALT: 
            InsereAssembly(A, "HLT", "", "", "", 0, line, 7, "");
            break;
        case ADD:
            if (flag1 && flag2)
            {
                InsereAssembly(A, "ADDI", quad->res, "$zero", "", atoi(quad->arg1), line, 2, "");
                line++;
                InsereAssembly(A, "ADDI", quad->res, quad->res, "", atoi(quad->arg2), line, 2, "");
                line++;
            }
            else
            {
                if (!flag1 && flag2)
                {
                    InsereAssembly(A, "ADDI", quad->res, quad->arg1, "", atoi(quad->arg2), line, 2, "");
                    line++;
                }
                else
                {
                    if (flag1 && !flag2)
                    {
                        InsereAssembly(A, "ADDI", quad->res, quad->arg2, "", atoi(quad->arg1), line, 2, "");
                        line++;
                    }
                    else
                    {
                        InsereAssembly(A, "ADD", quad->res, quad->arg1, quad->arg2, 0, line, 1, "");
                        line++;
                    }
                }
            }
            break;
        case SUB:
            if (flag1 && flag2)
            {
                InsereAssembly(A, "ADDI", quad->res, "$zero", "", atoi(quad->arg1), line, 2, "");
                line++;
                InsereAssembly(A, "SUBI", quad->res, quad->res, "", atoi(quad->arg2), line, 2, "");
                line++;
            }
            else
            {
                if (!flag1 && flag2)
                {
                    InsereAssembly(A, "SUBI", quad->res, quad->arg1, "", atoi(quad->arg2), line, 2, "");
                    line++;
                }
                else
                {
                    if (flag1 && !flag2)
                    {
                        InsereAssembly(A, "ADDI", quad->res, "$zero", "", atoi(quad->arg1), line, 2, "");
                        line++;
                        InsereAssembly(A, "SUBI", quad->res, quad->arg2, "", atoi(quad->arg1), line, 2, "");
                        line++;
                    }
                    else
                    {
                        InsereAssembly(A, "SUB", quad->res, quad->arg1, quad->arg2, 0, line, 1, "");
                        line++;
                    }
                }
            }
            break;
        case MULTI:
            if (flag1 && flag2)
            {
                InsereAssembly(A, "ADDI", " $h0", "$zero", "", atoi(quad->arg1), line, 2, "");
                line++;
                InsereAssembly(A, "ADDI", "$h1", "$zero", "", atoi(quad->arg2), line, 2, "");
                line++;
                InsereAssembly(A, "MULT", quad->res, "$h0", "$h1", 0, line, 1, "");
                line++;
            }
            else
            {
                if (!flag1 && flag2)
                {
                	InsereAssembly(A, "ADDI", " $h0", "$zero", "", atoi(quad->arg2), line, 2, "");
                	line++;
                	InsereAssembly(A, "MULT", quad->res, quad->arg1, "$h0", 0, line, 1, "");
                	line++;
                }
                else
                {
                    if (flag1 && !flag2)
                    {
                        InsereAssembly(A, "ADDI", " $h0", "$zero", "", atoi(quad->arg1), line, 2, "");
                	 line++;
                	 InsereAssembly(A, "MULT", quad->res, "$h0", quad->arg2, 0, line, 1, "");
                	 line++;
                    }
                    else
                    {
                        InsereAssembly(A, "MULT", quad->res, quad->arg1, quad->arg2, 0, line, 1, "");
                	 line++;
                    }
                }
            }
            break;
        case DIVI:
            if (flag1 && flag2)
            {
                InsereAssembly(A, "ADDI", " $h0", "$zero", "", atoi(quad->arg1), line, 2, "");
                line++;
                InsereAssembly(A, "ADDI", "$h1", "$zero", "", atoi(quad->arg2), line, 2, "");
                line++;
                InsereAssembly(A, "DIV", quad->res, "$h0", "$h1", 0, line, 1, "");
                line++;
            }
            else
            {
                if (!flag1 && flag2)
                {
                	InsereAssembly(A, "ADDI", " $h0", "$zero", "", atoi(quad->arg2), line, 2, "");
                	line++;
                	InsereAssembly(A, "DIV", quad->res, quad->arg1, "$h0", 0, line, 1, "");
                	line++;
                }
                else
                {
                    if (flag1 && !flag2)
                    {
                        InsereAssembly(A, "ADDI", " $h0", "$zero", "", atoi(quad->arg1), line, 2, "");
                	 line++;
                	 InsereAssembly(A, "DIV", quad->res, "$h0", quad->arg2, 0, line, 1, "");
                	 line++;
                    }
                    else
                    {
                        InsereAssembly(A, "DIV", quad->res, quad->arg1, quad->arg2, 0, line, 1, "");
                	 line++;
                    }
                }
            }
            break;
        case EQUAL:
            if (flag1 && flag2)
            {
                if (atoi(quad->arg1) != atoi(quad->arg2))
                {
                    InsereAssembly(A, "JUMP", "", "", "", 0, line, 2, quad->next->arg1);
                    line++;
                }
            }
            else
            {
                if (!flag1 && flag2)
                {
                    InsereAssembly(A, "LOADI", "$h0", "", "", atoi(quad->arg2), line, 4, "");
                    line++;
                    InsereAssembly(A, "BNE", quad->arg1, "$h0", "", 0, line, 2, quad->next->arg1);
                    line++;
                }
                else
                {
                    if (flag1 && !flag2)
                    {
                        InsereAssembly(A, "LOADI", "$h0", "", "", atoi(quad->arg2), line, 4, "");
                        line++;
                        InsereAssembly(A, "BNE", "$h0", quad->arg2, "", 0, line, 2, quad->next->arg1);
                        line++;
                    }
                    else
                    {
                        InsereAssembly(A, "BNE", quad->arg1, quad->arg2, "", 0, line, 2, quad->next->arg1);
                        line++;
                    }
                }
            }
            quad = quad->next;
            break;
        case DIFI:
            if (atoi(quad->arg1) == atoi(quad->arg2))
            {
                InsereAssembly(A, "JUMP", "", "", "", 0, line, 2, quad->next->arg1);
                line++;
            }
        else
        {
            if (!flag1 && flag2)
            {
                InsereAssembly(A, "LOADI", "$h0", "", "", atoi(quad->arg2), line, 4, "");
                line++;
                InsereAssembly(A, "BEQ", quad->arg1, "$h0", "", 0, line, 2, quad->next->arg1);
                line++;
            }
            else
            {
                if (flag1 && !flag2)
                {
                    InsereAssembly(A, "LOADI", "$h0", "", "", atoi(quad->arg2), line, 4, "");
                    line++;
                    InsereAssembly(A, "BEQ", "$h0", quad->arg2, "", 0, line, 2, quad->next->arg1);
                    line++;
                }
                else
                {
                    InsereAssembly(A, "BEQ", quad->arg1, quad->arg2, "", 0, line, 2, quad->next->arg1);
                    line++;
                }
            }
        }
        quad = quad->next;
        break;
    case LT: 
        if (flag1 && flag2)
        {
            if (atoi(quad->arg1) > atoi(quad->arg2))
            {
                InsereAssembly(A, "JUMP", "", "", "", 0, line, 2, quad->next->arg1);
                line++;
                quad = quad->next;
            }
        }
        else
        {
            if (!flag1 && flag2)
            {
                InsereAssembly(A, "SLTI", quad->res, quad->arg1, "", atoi(quad->arg2), line, 2, "");
                line++;
            }
            else
            {
                if (flag1 && !flag2)
                {
                    InsereAssembly(A, "LOADI", "$h0", "", "", atoi(quad->arg1), line, 4, "");
                    line++;
                    InsereAssembly(A, "SLT", quad->res, "$h0", quad->arg2, 0, line, 1, "");
                    line++;
                }
                else
                {
                    InsereAssembly(A, "SLT", quad->res, quad->arg1, quad->arg2, 0, line, 1, "");
                    line++;
                }
            }
        }
        break;
    case LTE:
        if (flag1 && flag2)
        {
            if (atoi(quad->arg1) >= atoi(quad->arg2))
            {
                InsereAssembly(A, "JUMP", "", "", "", 0, line, 2, quad->next->arg1);
                line++;
                quad = quad->next;
            }
        }
        else
        {
            if (!flag1 && flag2)
            {
                InsereAssembly(A, "SLTI", quad->res, quad->arg1, "", atoi(quad->arg2) + 1, line, 2, "");
                line++;
            }
            else
            {
                if (flag1 && !flag2)
                {
                    InsereAssembly(A, "LOADI", "$h0", "", "", atoi(quad->arg1), line, 4, "");
                    line++;
                    InsereAssembly(A, "ADDI", quad->arg2, "$zero", "", 1, line, 2, "");
                    line++;
                    InsereAssembly(A, "SLT", quad->res, "$h0", quad->arg2, 0, line, 1, "");
                    line++;
                }
                else
                {
                    InsereAssembly(A, "ADDI", quad->arg2, "$zero", "", 1, line, 2, "");
                    line++;
                    InsereAssembly(A, "SLT", quad->res, quad->arg1, quad->arg2, 0, line, 1, "");
                    line++;
                }
            }
        }
        break;
    case GT:
        if (flag1 && flag2)
        {
            if (atoi(quad->arg1) < atoi(quad->arg2))
            {
                InsereAssembly(A, "JUMP", "", "", "",0, line, 2, quad->next->arg1);
                line++;
                quad = quad->next;
            }
        }
        else
        {
            if (!flag1 && flag2)
            {
                InsereAssembly(A, "LOADI", "$h0", "", "", atoi(quad->arg2), line, 4, "");
                line++;
                InsereAssembly(A, "SLT", quad->res, "$h0", quad->arg1, 0, line, 1, "");
                line++;
            }
            else
            {
                if (flag1 && !flag2)
                {
                    InsereAssembly(A, "SLTI", quad->res, quad->arg2, "", atoi(quad->arg1), line, 2, "");
                    line++;
                }
                else
                {
                    InsereAssembly(A, "SLT", quad->res, quad->arg2, quad->arg1, 0, line, 1, "");
                    line++;
                }
            }
        }
        break;
    case GTE:
        if (flag1 && flag2)
        {
            if (atoi(quad->arg1) <= atoi(quad->arg2))
            {
                InsereAssembly(A, "JUMP", "", "", "", 0, line, 2, quad->next->arg1);
                line++;
                quad = quad->next;
            }
        }
        else
        {
            if (!flag1 && flag2)
            {
                InsereAssembly(A, "LOADI", "$h0", "", "", atoi(quad->arg2) + 1, line, 4, "");
                line++;
                InsereAssembly(A, "SLT", quad->res, "$h0", quad->arg1, 0, line, 1, "");
                line++;
            }
            else
            {
                if (flag1 && !flag2)
                {
                    InsereAssembly(A, "ADDI", quad->arg2, "$zero", "", 1, line, 2, "");
                    line++;
                    InsereAssembly(A, "SLTI", quad->res, quad->arg2, "", atoi(quad->arg1), line, 2, "");
                    line++;
                }
                else
                {
                    InsereAssembly(A, "ADDI", quad->arg2, "$zero", "", 1, line, 2, "");
                    line++;
                    InsereAssembly(A, "SLT", quad->res, quad->arg2, quad->arg1, 0, line, 1, "");
                    line++;
                }
            }
        }
        break;
    case GOTO:
        InsereAssembly(A, "JUMPI", "", "", "", 0, line, 6, quad->res);
        line++;
        break;
    case LAB:
        InsereAssembly(A, "LAB", "", "", "", 0, line, 0, quad->res);
        break;
    case ASSIGN:
        if (flag1 == 0)
        {
            InsereAssembly(A, "MOVE", quad->res, quad->arg1, "", 0, line, 3, "");
            line++;
        }
        else
        {
            InsereAssembly(A, "LOADI", quad->res, "", "", atoi(quad->arg1), line, 4, "");
            line++;
        }
        break;
    case RET:
        if (flagres == 0){
            InsereAssembly(A, "ADD", "$v0", "$zero", quad->res, 0, line, 1, "");
            line++;
        }
        else{
            InsereAssembly(A, "ADDI", "$v0", "$zero", "", atoi(quad->res), line, 2, "");
            line++;
        }
        if(flag == 1){
            argcount = 0;
            paramcount = 0;
        }
        else{
            argcount = 3;
            paramcount = 3;
        }
        if(strcmp(quad->res,"main") != 0){
            InsereAssembly(A, "LOAD", "$ra", "$sp", "", 0, line, 2, "");
            line++;
            InsereAssembly(A, "JUMP", "$ra", "", "", 0, line, 5, "");
            line++;
        }
        break;
    case END:
        if(flag == 1){
            argcount = 0;
            paramcount = 0;
        }
        else{
            argcount = 3;
            paramcount = 3;
        }
        if(strcmp(quad->res,"main") != 0){
            InsereAssembly(A, "LOAD", "$ra", "$sp", "", 0, line, 2, "");
            line++;
            InsereAssembly(A, "JUMP", "$ra", "", "", 0, line, 5, "");
            line++;
        }
        break;
    case LOAD:
        d = FindMemory(head, scope, quad->arg1);
        if (strcmp(d->scope, "global") != 0) // Checa global
        {
            if (strcmp(quad->arg2, "-") == 0)
                if (d->size == 1)	// Nao e um vetor
                {
                    InsereAssembly(A, "LOAD", quad->res, "$sp", "", d->memorypos, line, 2, "");
                    line++;
                }
                else{ // Um ponteiro de vetor, isto é, passagem como parametro de um vetor
                    InsereAssembly(A, "ADDI", quad->res, "$sp", "", d->memorypos, line, 2, "");
                    line++;
                }
            else // E vetor
            {
                    InsereAssembly(A, "LOAD", "$h0", "$sp", "", d->memorypos, line, 2, "");
                    line++;
                    InsereAssembly(A, "ADD", "$h0", "$h0", quad->arg2, 0, line, 1, "");
                    line++;
                    InsereAssembly(A, "LOAD", quad->res, "$h0", "", 0, line, 2, "");
                    line++;
            }
        }
        else 
        {
            if (strcmp(quad->arg2, "-") == 0)
                if (d->size == 1)
                {
                    InsereAssembly(A, "LOAD", quad->res, "$gp", "", d->memorypos, line, 2, "");
                    line++;
                }
                else{ 
                    InsereAssembly(A, "ADDI", quad->res, "$gp", "", d->memorypos, line, 2, "");
                    line++;
                }
            else 
            {
                InsereAssembly(A, "LOAD", "$h0", "$gp", "", d->memorypos, line, 2, "");
                line++;
                InsereAssembly(A, "ADD", "$h0", "$h0", quad->arg2, 0, line, 1, "");
                line++;
                InsereAssembly(A, "LOAD", quad->res, "$h0", "", 0, line, 2, "");
                line++;
            }
        }
        break;
    case STORE:
         d = FindMemory(head, scope, quad->res);
        if (strcmp(d->scope, "global") != 0) 
        {
            if (strcmp(quad->arg2, "-") == 0)
            {
                    InsereAssembly(A, "STORE", "$sp", quad->arg1, "", d->memorypos, line, 2, "");
                    line++;
            }
            else 
            {
                InsereAssembly(A, "LOAD", "$h0", "$sp", "", d->memorypos, line, 2, "");
                line++;
                InsereAssembly(A, "ADD", "$h0", "$h0", quad->arg2, 0, line, 1, "");
                line++;
                InsereAssembly(A, "STORE", "$h0", quad->arg1, "", 0, line, 2, "");
                line++;
            }
        }
        else 
        {
            if (strcmp(quad->arg2, "-") == 0){
                InsereAssembly(A, "STORE", "$gp", quad->arg1, "", d->memorypos, line, 2, "");
                line++;
            }
            else 
            {
                InsereAssembly(A, "LOAD", "$h0", "$gp", "", 0, line, 2, "");
                line++;
                InsereAssembly(A, "ADD", "$h0", "$h0", quad->arg2, 0, line, 1, "");
                line++;
                InsereAssembly(A, "STORE", "$h0", quad->arg1, "", 0, line, 2, "");
                line++;
            }
        }

        break;
    case ALLOC:	// Aqui se insere na tabela de memoria das variaveis
        if(strcmp(quad->arg1, "global") == 0){
            if(strcmp(quad->arg2, "-") == 0){
                InsereMemoria(head, quad->arg1, quad->res, 1);
                GlobalPos++;
            }
            else{
                InsereMemoria(head, quad->arg1, quad->res, atoi(quad->arg2));
                GlobalPos += atoi(quad->arg2);
            }
        }
        else{
            if(strcmp(quad->arg2, "-") == 0){
                InsereMemoria(head, quad->arg1, quad->res, 1);
                spPos++;	// Quantas variaveis foram alocadas na pilha
            }
            else{
                InsereMemoria(head, quad->arg1, quad->res, atoi(quad->arg2));
                spPos += atoi(quad->arg2);
            }
        }
        break;
    case ARG:		// Argumentos sao variaveis também, sendo assim, e necessario que se saiba a posicao dela na pilha
        InsereMemoria(head, quad->arg2, quad->arg1, 1);
        InsereAssembly(A, "STORE", "$sp", getString("$a%d", argcount), "", spPos, line, 2, "");
        argcount++;
        line++;
        spPos++;
        break;
    case CALL:
        if(strcmp(quad->arg1,"output") == 0){	// Chamada da funcao output
            InsereAssembly(A, "OUT", quad->res, "", "", 0, line, 5, "");
            if(flag == 1){
                argcount = 0;
                paramcount = 0;
            }
            else{
                argcount = 3;
                paramcount = 3;
            }
            line++;
            break;
        }
        if(strcmp(quad->arg1,"input") == 0){	// Chamada da funcao input
            InsereAssembly(A, "IN", quad->res, "", "", 0, line, 5, "");
            line++;
            break;
        }
        if(strcmp(quad->arg1, "CpyHDtoIM") == 0 || strcmp(quad->arg1, "CpyRegtoHD") == 0 || strcmp(quad->arg1, "CpyHDtoReg") == 0 || strcmp(quad->arg1, "CpyDMtoHD") == 0 || strcmp(quad->arg1, "CpyHDtoDM") == 0){	// Chamada das funcoes de movimento de dados
            InsereAssembly(A, quad->arg1, "$a0", "$a1", "$a2", 0, line, 1, "");
            if(flag == 1){
                argcount = 0;
                paramcount = 0;
            }
            else{
                argcount = 3;
                paramcount = 3;
            }
            line++;
            break;
        }
        if(strcmp(quad->arg1, "Display") == 0 || strcmp(quad->arg1, "DisplayProgram") == 0){	// Chamada das funcoes do display LCD
            InsereAssembly(A, quad->arg1, "$a0", "", "", 0, line, 5, "");
            if(flag == 1){
                argcount = 0;
                paramcount = 0;
            }
            else{
                argcount = 3;
                paramcount = 3;
            }
            line++;
            break;
        }
        if(strcmp(quad->arg1, "Execproc") == 0){	// Chamada da execução de processo
            InsereAssembly(A, quad->arg1, "$a0", "", "", 0, line, 5, "");
           if(flag == 1){
                argcount = 0;
                paramcount = 0;
            }
            else{
                argcount = 3;
                paramcount = 3;
            }
            line++;
            break;
        }
        if(strcmp(quad->arg1, "setCS") == 0 || strcmp(quad->arg1, "unSetCS") == 0){	// Chamada das funcoes de troca de contexto
            InsereAssembly(A, quad->arg1, "", "", "", 0, line, 7, "");
            if(flag == 1){
                argcount = 0;
                paramcount = 0;
            }
            else{
                argcount = 3;
                paramcount = 3;
            }
            line++;
            break;
        }
        if(strcmp(quad->arg1, "jumpProc") == 0 ){	// Chamada das execução de processo em troca de contexto
            InsereAssembly(A, quad->arg1, "$PC", "", "", 0, line, 5, "");
            if(flag == 1){
                argcount = 0;
                paramcount = 0;
            }
            else{
                argcount = 3;
                paramcount = 3;
            }
            line++;
            break;
        }
        InsereAssembly(A, "ADDI", "$sp", "$sp", "", spPos, line, 2, "");	// Soma a $sp com a quantidade de variaveis que ja foram inseridas na tabela, armazenada em spPos
        line++;
        InsereAssembly(A, "JAL", "", "", "", 0, line, 6, quad->arg1);	// Armazena a posição atual em $ra(nivel de processador) e em seguida pula para a linha designada
        line++;
        if(strcmp(st_lookup_typeID(quad->arg1, "global"), "inteiro") == 0){	// Verifica se a funcao chamada tem retorno (nao e void)
        	InsereAssembly(A, "ADD",quad->res,"$zero","$v0",0,line,1,"");
        	line++;
        }
        InsereAssembly(A, "SUBI", "$sp", "$sp", "", spPos, line, 2, "");	// Volta a $sp para a posição correta do escopo
        line++;
        if(flag == 1){
            argcount = 0;
            paramcount = 0;
        }
        else{
            argcount = 3;
            paramcount = 3;
        }
        break;
    case PARAM:	// Carrega os parametros da funcao
        if(flagres){
            InsereAssembly(A, "LOADI", getString("$a%d", paramcount), "", "", atoi(quad->res), line, 4, "");
            line++;
            paramcount++;
        }
        else{
            InsereAssembly(A, "MOVE", getString("$a%d", paramcount), quad->res, "", 0, line, 3, "");
            line++;
            paramcount++;
        }
        break;
    case FUNC:	// Inicio de funcao
        if(flag == 1){
            argcount = 0;
            paramcount = 0;
        }
        else{
            argcount = 3;
            paramcount = 3;
        }
        scope = quad->arg1;
        //printf("%s\n", scope);
        InsereAssembly(A, "LAB", scope, "", "", 0, line, 0, scope);
        spPos = 0;
        if(strcmp(scope,"main") != 0){	//caso a funcao nao seja a main e necessario guardar a ra na memoria em caso de recursao
             InsereAssembly(A, "STORE", "$sp", "$ra", "", 0, line, 2, "");
             line++;
             spPos++;
        }
        break;
    case IFF:	// Essa condicao ocorre apenas apos operacoes de GT, GTE, LT e LTE, como SLT coloca 1 no registrador quad->res caso seja codicao verdadeira somente pulara em caso de nao igualdade
    	InsereAssembly(A, "ADDI", "$h0", "$zero", "", 1, line, 2, "");
        line++;
	InsereAssembly(A, "BNE", quad->res, "$h0", "", 0, line, 2, quad->arg1);
        line++;
        break;
    default:
        printf("Quadrupla desconhecida");
        break;
        }
    quad = quad->next;
    }
}

AssemblyList * AssemblyGenerator(QuadList * Q, int flag){
	AssemblyList *headA;
	MemoryList *headM;
	FILE *Memoria;	// Para printar a memoria
	headM = IniciaMemoria();
    if(flag == 1)
        headA = InsereAssembly(headA, "ADDI", "$gp", "$zero", "", 0, line, 2, "main");
    else
        headA = InsereAssembly(headA, "ADDI", "$gp", "$zero", "", 32, line, 2, "main");
	line++;
	Q = AlocaGlobal(Q, headM);
	InsereAssembly(headA, "ADDI", "$sp", "$gp", "", GlobalPos, line, 2, "main");
	line++;
	InsereAssembly(headA, "JUMPI", "", "", "", 0, line, 6, "main");
	line++;
	GenAssembly(Q,headM,headA,flag);
	Memoria = fopen("memoria.txt", "w+"); // Printar Memoria
	PrintMemory(headM, Memoria);
	return headA;
}

