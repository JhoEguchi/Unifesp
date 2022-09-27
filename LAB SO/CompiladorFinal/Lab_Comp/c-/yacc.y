%{

#define YYPARSER


#include <stdio.h>
#include "globals.h"
#include "util.h"
#define YYSTYPE TreeNode *


static TreeNode * savedTree; /* stores syntax tree for later return */

extern int lineno;
extern FILE *yyin;
char *yytext;
extern int ERRO;

static int yylex();
void yyerror(char *s);
TokenType getToken(void);
%}


%start programa
%token ELSE IF INT RETURN VOID WHILE 
%left MAIS MENOS MULT DIV
%left MENOR MENORIGUAL MAIOR MAIORIGUAL IGUAL DIF
%token PV VIR ABRP FECP ABRCO FECCO ABRC FECC RECEBE
%token ID NUM
%token ERROR

%%

  programa      : decl_lista{
                              savedTree = $1;
                            };
  decl_lista    : decl_lista decl{
                                YYSTYPE t = $1;
                                 if (t->child[0] != NULL)
                                 { while (t->child[0]->sibling != NULL)
                                 	t = t->child[0]->sibling;
                                  t->child[0]->sibling = $2;
                                      //t = t->child[2];
                                  // t->child[2] = $2;
                                   //$$ = t->child[0]; }
                                   $$ = $1;}
                                   else $$ = $2;
                                  }
                | decl{
                  $$ = $1;
                }
                ;
  decl          : var_decl{
                            $$ = $1;
                          }
                | fun_decl{
                          $$ = $1;
                }
                ;
  var_decl      : INT id PV{
                            $$ = newExpNode(TypeK);
                            $$->type = 1;
                            $$->attr.name = "int";
                            $$->child[0] = $2;
                            $2->nodekind = StmtK;
                            $2->kind.stmt = VariableK;
                            $2->type = 1;

                }
                | INT id ABRCO num FECCO PV{
                  $$ = newExpNode(TypeK);
                  $$->type = 1;
                  $$->attr.name = "int";
                  $$->child[0] = $2;
                  $2->nodekind = StmtK;
                  $2->kind.stmt = VariableK;
                  $2->type = 2;
                  $2->len = $4->attr.val;
                }
                ;
  fun_decl      : INT id ABRP params FECP composto_decl{

                                              $$ = newExpNode(TypeK);
                                              $$->type = 1;
                                              $$->attr.name = "int";
                                              $$->child[0] = $2;
                                              $2->child[0] = $4;
                                              $2->child[1] = $6;
                                              $2->type = 1;
                                              $2->kind.stmt = FunctionK;
                                              $2->nodekind = StmtK;
					      DefineScope($2->child[0], $2->attr.name);
                                              DefineScope($2->child[1], $2->attr.name);
					      

                                }
              		| VOID id ABRP params FECP composto_decl{

                                              $$ = newExpNode(TypeK);
                                              $$->attr.name = "void";
                                              $$->child[0] = $2;
                                              $2->child[0] = $4;
                                              $2->child[1] = $6;
                                              $$->type = 0;
                                              $2->kind.stmt = FunctionK;
                                              $2->nodekind = StmtK;
                                              DefineScope($2->child[0], $2->attr.name);
					      DefineScope($2->child[1], $2->attr.name);
					      
              		 };
  params        : param_lista{
                  $$ = $1;
                }
                | VOID  {
                }
                ;
  param_lista   : param_lista VIR param{
                                YYSTYPE t = $1;
                                 if (t != NULL)
                                 { while (t->sibling != NULL)
                                      t = t->sibling;
                                   t->sibling = $3;
                                   $$ = $1; }
                                   else $$ = $3;
                                  }
                | param{
                    $$ = $1;
                }
                ;
  param         : INT id{
                          $$ = newExpNode(TypeK);
                          $$->type = 1;
                          $$->attr.name = "integer";
                          $$->child[0] = $2;
                          $2->type = 1;
                          $2->nodekind = StmtK;
                          $2->kind.stmt = ParamK;
                        }
                | INT id ABRCO FECCO{
                          $$ = newExpNode(TypeK);
                          $$->type = 1;
                          $$->attr.name = "integer";
                          $$->child[0] = $2;
                          $2->type = 2;
                          $2->nodekind = StmtK;
                          $2->kind.stmt = ParamK;
                          }
                ;
  composto_decl : ABRC local_decl stmt_list FECC{
                                  YYSTYPE t = $2;
                                   if (t != NULL)
                                   { while (t->sibling != NULL)
                                        t = t->sibling;
                                     t->sibling = $3;
                                     $$ = $2; }
                                     else {
                                        $$ = $3;
                                      }
                                     }
                  | ABRC local_decl FECC{
                    $$ = $2;
                  }
                  | ABRC stmt_list FECC{
                    $$ = $2;
                  }
		  | ABRC FECC{};
  local_decl    : local_decl var_decl{
                        YYSTYPE t = $1;
                         if (t != NULL)
                         { while (t->sibling != NULL)
                              t = t->sibling;
                           t->sibling = $2;
                           $$ = $1; }
                           else $$ = $2;
                  }
		| var_decl{ 
			$$ = $1;
		}


                | {
                	$$ = NULL;
                }
                ;
  stmt_list     : stmt_list stmt{
                          YYSTYPE t = $1;
                           if (t != NULL)
                           { while (t->sibling != NULL)
                                t = t->sibling;
                             t->sibling = $2;
                             $$ = $1; }
                             else $$ = $2;
    }
                | stmt{	
			$$=$1;
		}
		| {
                    $$ = NULL;
                  }
                ;
  stmt          : exp_decl
                {
                  $$ = $1;
                }
                | composto_decl
                {
                  $$ = $1;
                }
                | sel_decl
                {
                  $$ = $1;
                }
                | it_decl
                {
                  $$ = $1;
                }
                | retorno_decl
                {
                  $$ = $1;
                }
                ;
  exp_decl      : exp PV
                  {
                    $$ = $1;
                  }
                | PV
                ;
  sel_decl      : IF ABRP exp FECP stmt
                {
                $$ = newStmtNode(IfK);
                $$->child[0] = $3;
                $$->child[1] = $5;
                }
                | IF ABRP exp FECP stmt ELSE stmt
                {
                $$ = newStmtNode(IfK);
                $$->child[0] = $3;
                $$->child[1] = $5;
                $$->child[2] = $7;
                }
                ;
  it_decl       : WHILE ABRP exp FECP stmt
                  {
                  $$ = newStmtNode(WhileK);
                  $$->child[0] = $3;
                  $$->child[1] = $5;
                  };
  retorno_decl  : RETURN PV
                  {
                  $$ = newStmtNode(ReturnK);
                  $$->type = 0;
                  }
                | RETURN exp PV
                  {
                  $$ = newStmtNode(ReturnK);
                  $$->child[0] = $2;
                  }
                ;
  exp           : var RECEBE exp
                {
                $$ = newStmtNode(AssignK);
                $$->child[0] = $1;
                $$->child[1] = $3;
                }
                | simples_exp
                {
                $$ = $1;
                }
                ;
  var           : id
                  {
 	                 $$ = $1;
			 $$->type = 1;
                  }
                | id ABRCO exp FECCO
                  {
                  $$ = $1;
                  $$->type = 2;
                  $$->kind.exp = IdK;
                  $$->child[0] = $3;
                  };
  simples_exp   : soma_exp rel soma_exp
                  {
                  $$ = $2;
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                  }
                | soma_exp
                  {
                  $$ = $1;
                  }
                ;
  rel           : MENOR
                  {
                  $$ = newExpNode(OpK);
                  $$->attr.op = MENOR;
		  $$->type = 1;
                  }
                | MENORIGUAL
                {
                $$ = newExpNode(OpK);
                $$->attr.op = MENORIGUAL;
		$$->type = 1;
                }
                | MAIOR
                {
                $$ = newExpNode(OpK);
                $$->attr.op = MAIOR;
		$$->type = 1;
                }
                | MAIORIGUAL
                {
                $$ = newExpNode(OpK);
                $$->attr.op = MAIORIGUAL;
		$$->type = 1;
                }
                | IGUAL
                {
                $$ = newExpNode(OpK);
                $$->attr.op = IGUAL;
		$$->type = 1;
                }
                | DIF
                {
                $$ = newExpNode(OpK);
                $$->attr.op = DIF;
		$$->type = 1;
                }
                ;
  soma_exp      : soma_exp sum term
                {
                $$ = $2;
                $$->child[0] = $1;
                $$->child[1] = $3;
                }
                | term
                {
                $$ = $1;
                }
                ;
  sum           : MAIS
                  {
                  $$ = newExpNode(OpK);
                  $$->attr.op = MAIS;
		  $$->type = 1;
                  }
                | MENOS
                  {
                  $$ = newExpNode(OpK);
                  $$->attr.op = MENOS;
		  $$->type = 1;
                  }
                ;
  term          : term mult fator
                  {
                  $$ = $2;
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                  }
                | fator
                  {
                  $$ = $1;
                  }
                ;
  mult          : MULT
                  {
                  $$ = newExpNode(OpK);
                  $$->attr.op = MULT;
		  $$->type = 1;
                  }
                | DIV
                  {
                  $$ = newExpNode(OpK);
                  $$->attr.op = DIV;
 		  $$->type = 1;
                  }
                ;
  fator         : ABRP exp FECP
                  {
                  $$ = $2;
                  }
                | var
                  {
                  $$ = $1;
                  }
                | atv
                  {
                  $$ = $1;
                  }
                | num
                  {
                  $$ = $1;
                  }
                ;
  atv           : id ABRP args FECP
                  {
                  $$ = $1;
                  $$->child[0] = $3;
                  $$->kind.exp = CallK;
                  }
  args          : arg_lista
                  {
                  $$ = $1;
                  }
                |
                {
                $$ = NULL;
                }
                ;
  arg_lista     : arg_lista VIR exp
                  {
                  YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $3;
                     $$ = $1; }
                     else $$ = $3;
                  }
                | exp
                {
                $$ = $1;
                }
                ;

  num            : NUM
                  {
                  $$ = newExpNode(ConstK);
                  $$->attr.val = atoi(yytext);
                  $$->type = 1;
                  };
  id             : ID
                  {
                  $$ = newExpNode(IdK);
                  $$->attr.name = copyString(yytext);
                  };

%%

void yyerror(char * msg)
{
	extern char* yytext;
	printf("ERROR! %s on line %d. (%s)\n", msg, lineno, yytext);
	ERRO = 1;
}

TreeNode * parse(void)
{
	yyparse();
	return savedTree;
}

static int yylex(void)
{	
	TokenType t = getToken();
	return t;
}
