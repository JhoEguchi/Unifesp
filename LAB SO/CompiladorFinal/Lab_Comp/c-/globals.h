#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef YYPARSER
#include "yacc.tab.h"
#define ENDFILE 0
#endif
extern int ERRO;
int lineno;

FILE* listing;

typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfK,WhileK,VariableK,AssignK,FunctionK,ReturnK,ParamK} StmtKind;
typedef enum {OpK,ConstK,IdK,VetorK,TypeK,CallK} ExpKind;
typedef int TokenType; 
typedef enum {IFF,GOTO,LAB,ASSIGN,STORE,ALLOC,END,RET,LOAD,PARAM,CALL,FUNC,ARG,ADD,SUB,MULTI,DIVI,GT,GTE,LT,LTE,EQUAL,DIFI,HALT} Op;


typedef enum {Void,Integer,Array} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;} kind;
     char *scope;
     int len;
     union { TokenType op;
             int val;
             char * name; } attr;

     ExpType type;
   } TreeNode;
   
typedef struct Quadruple
   {
   	Op op;
   	char *res;
   	char *arg1;
   	char *arg2;
   	struct Quadruple *next;
   	struct Quadruple *last;
   }QuadList;

typedef struct Assembly
   {
   	char *op;
   	char *arg1;
   	char *arg2;
   	char *arg3;
   	int imed;
   	int lineNo;
   	int format;
   	char *label;
   	struct Assembly *next;
   }AssemblyList;

typedef struct Binario
   {
   	int* operando;
   	int* RD;
   	int* R1;
   	int* R2;
   	int* Imediato17;
   	int* Imediato22;
   	int format;
   	struct Binario *next;
   }BinaryList;
   


   #endif
