#ifndef _UTIL_H_
#define _UTIL_H_

void DefineScope(TreeNode*, char*);

TreeNode * newStmtNode(StmtKind);

TreeNode * newExpNode(ExpKind);

QuadList * startQuad();

void newQuad(QuadList *, Op, char*, char*, char*);

AssemblyList * InsereAssembly(AssemblyList *, char *, char *, char *, char *, int , int , int , char *);

BinaryList * InsereBinario(BinaryList *, int *, int *, int *, int *, int *, int *, int);

char * getString(const char *, int );

void printTree(TreeNode *, FILE *);

void printToken (TokenType,const char*, FILE *);

void printQuad (QuadList *, FILE *);

void printAssembly (AssemblyList *, FILE *);

void printBinario(BinaryList *, FILE *);

char * copyString(char *);
#endif
