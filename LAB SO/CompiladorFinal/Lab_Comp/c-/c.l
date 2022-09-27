%option noyywrap
%option nounput
%{
#include "stdio.h"
#include "globals.h"
#include "string.h"

extern int lineno;
extern FILE *yyin;
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t\r]+

%%
"else" {return ELSE;}
"if" {return IF;}
"int" {return INT;}
"return" {return RETURN;}
"void" {return VOID;}
"while" {return WHILE;}
"+" {return MAIS;}
"-" {return MENOS;}
"*" {return MULT;}
"/" {return DIV;}
"<" {return MENOR;}
"<=" {return MENORIGUAL;}
">" {return MAIOR;}
">=" {return MAIORIGUAL;}
"==" {return IGUAL;}
"!=" {return DIF;}
"=" {return RECEBE;}
";" {return PV;}
"," {return VIR;}
"(" {return ABRP;}
")" {return FECP;}
"[" {return ABRCO;}
"]" {return FECCO;}
"{" {return ABRC;}
"}" {return FECC;}
{whitespace}  {}
<<EOF>> {return ENDFILE;}
{newline} {lineno++;}
{identifier} {return ID;}
{number}  {return NUM;}
"/*" {
        char c;
        while(1){
          c = input();
          if(c == '*'){
            if(input() == '/') break;
          }
          if(c == '\n') lineno++;
        }
      }
. {return ERROR;}


%%
int getToken(){
  int token;
  token = yylex();
  return token;
}
void analise_lexica (){
  FILE *resultado = fopen("./Saidas/lexico.txt","w+");
  int Token;
  Token = yylex();
  while(Token!= ENDFILE){

	switch(Token){

	case IF:
		fprintf(resultado,"IF: %s\n ",yytext);
		break;
	case ELSE:
		fprintf(resultado,"ELSE: %s\n ",yytext);
		break;
	case INT:
		fprintf(resultado,"INT: %s\n ",yytext);
		break;
	case RETURN:
		fprintf(resultado,"RETURN: %s\n ",yytext);
		break;
	case VOID:
		fprintf(resultado,"VOID: %s\n ",yytext);
		break;
	case WHILE:
		fprintf(resultado,"WHILE: %s\n ",yytext);
		break;
	case MAIS:
		fprintf(resultado,"MAIS: %s\n ",yytext);
		break;
	case MENOS:
		fprintf(resultado,"MENOS: %s\n ",yytext);
		break;
	case MULT:
		fprintf(resultado,"MULT: %s\n ",yytext);
		break;
	case DIV:
		fprintf(resultado,"DIV: %s\n ",yytext);
		break;
	case MENOR:
		fprintf(resultado,"MENOR: %s\n ",yytext);
		break;
	case MENORIGUAL:
		fprintf(resultado,"MENORIGUAL: %s\n ",yytext);
		break;
	case MAIOR:
		fprintf(resultado,"MAIOR: %s\n ",yytext);
		break;
	case MAIORIGUAL:
		fprintf(resultado,"MAIORIGUAL: %s\n ",yytext);
		break;
	case IGUAL:
		fprintf(resultado,"IGUAL: %s\n ",yytext);
		break;
	case DIF:
		fprintf(resultado,"DIF: %s\n ",yytext);
		break;
	case RECEBE:
		fprintf(resultado,"RECEBE: %s\n ",yytext);
		break;
	case PV:
		fprintf(resultado,"PV: %s\n ",yytext);
		break;
	case VIR:
		fprintf(resultado,"VIR: %s\n ",yytext);
		break;
	case ABRP:
		fprintf(resultado,"ABRP: %s\n ",yytext);
		break;
	case FECP:
		fprintf(resultado,"FECP: %s\n ",yytext);
		break;
	case ABRCO:
		fprintf(resultado,"ABRCO: %s\n ",yytext);
		break;
	case FECCO:
		fprintf(resultado,"FECCO: %s\n ",yytext);
		break;
	case ABRC:
		fprintf(resultado,"ABRC: %s\n ",yytext);
		break;
	case FECC:
		fprintf(resultado,"FECC: %s\n ",yytext);
		break;
	case ID:
		fprintf(resultado,"ID: %s\n ",yytext);
		break;
	case NUM:
		fprintf(resultado,"NUM: %s\n ",yytext);
		break;
	case ERROR:
		fprintf(resultado,"ERROR ");
    printf("ERRO LÉXICO: %s LINHA: %d", yytext, lineno);
    ERRO = 1;
		break;
	case ENDFILE:
		fprintf(resultado,"ENDFILE ");
		break;
	default:
		fprintf(resultado,"Token não reconhecido");

	}
	Token = yylex();
  }
}
