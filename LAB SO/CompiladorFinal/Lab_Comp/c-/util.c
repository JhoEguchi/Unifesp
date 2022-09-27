#include "globals.h"
#include "util.h"


void DefineScope(TreeNode *t, char *scopename)
{
  int i;
  while (t!=NULL) {
    for(i = 0; i<MAXCHILDREN; i++){
      t->scope = scopename;
      DefineScope(t->child[i], scopename);
    }
    t = t->sibling;
  }
}

void printToken( TokenType token, const char* tokenString, FILE *listing )
{ switch (token)
  { case IF:
    case ELSE:
    case INT:
    case VOID:
    case RETURN:
    case WHILE:
      fprintf(listing, "reserved word: %s\n",tokenString);
      break;
    case RECEBE: fprintf(listing,"=\n"); break;
    case MAIOR: fprintf(listing,">\n"); break;
    case MAIORIGUAL: fprintf(listing,">=\n"); break;
    case MENOR: fprintf(listing,"<\n"); break;
    case MENORIGUAL: fprintf(listing,"<=\n"); break;
    case IGUAL: fprintf(listing,"==\n"); break;
    case DIF: fprintf(listing,"!=\n"); break;
    case ABRP: fprintf(listing,"(\n"); break;
    case FECP: fprintf(listing,")\n"); break;
    case ABRCO: fprintf(listing,"[\n"); break;
    case FECCO: fprintf(listing,"]\n"); break;
    case ABRC: fprintf(listing,"{\n"); break;
    case FECC: fprintf(listing,"}\n"); break;
    case PV: fprintf(listing,";\n"); break;
    case VIR: fprintf(listing,",\n"); break;
    case MAIS: fprintf(listing,"+\n"); break;
    case MENOS: fprintf(listing,"-\n"); break;
    case MULT: fprintf(listing,"*\n"); break;
    case DIV: fprintf(listing,"/\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing, "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing, "ID, name= %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing, "ERROR: %s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    /*fprintf(listing,"Out of memory error at line %d\n",lineno);*/
    return 0;
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
    t->scope="global";
    t->len=0;
  }
  return t;
}

TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    /*fprintf(listing,"Out of memory error at line %d\n",lineno);*/
    return 0;
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->scope="global";
    t->len=0;
    t->type = 0;
  }
  return t;
}

QuadList * startQuad(){
	QuadList *q1 = (QuadList *) malloc(sizeof(QuadList));
	QuadList *q2 = (QuadList *) malloc(sizeof(QuadList));
	if(q1 == NULL || q2 == NULL)
		return 0;
	else{
		q1->op = 0;
   		q1->res = NULL;
   		q1->arg1 = NULL;
   		q1->arg2 = NULL;
   		q1->next = NULL;
   		q1->last = NULL;
   		q2->op = 0;
   		q2->res = NULL;
   		q2->arg1 = NULL;
   		q2->arg2 = NULL;
   		q2->next = NULL;
   		q2->last = q1;
	}
	return q1;
}

void newQuad(QuadList *q, Op op, char *res, char *arg1, char *arg2)
{
	QuadList *q1 = (QuadList *) malloc(sizeof(QuadList));
	if(q1 == NULL)
		return 0;
	else{
		q->op = op;
   		q->res = res;
   		q->arg1 = arg1;
   		q->arg2 = arg2;
   		q->next = q1;
   		//printf("\n(%s,%s,%s,%s)",op, res, arg1, arg2);
   		q1->op = 0;
   		q1->res = NULL;
   		q1->arg1 = NULL;
   		q1->arg2 = NULL;
   		q1->next = NULL;
   		q1->last = q;
	}
}

AssemblyList * InsereAssembly(AssemblyList *head, char *op, char *arg1, char *arg2, char *arg3, int imed, int lineNo, int format, char *label){
    AssemblyList *a = (AssemblyList*) malloc(sizeof(AssemblyList));
    AssemblyList *pointer;
    pointer = head;
    a->op = op;
    a->arg1 = arg1;
    a->arg2 = arg2;
    a->arg3 = arg3;
    a->imed = imed;
    a->lineNo = lineNo;
    a->format = format;
    a->label = label;
    a->next = NULL;
    if(head == NULL){
    	head = a;
    	return head; 
    }
    while(pointer->next != NULL){
    	pointer = pointer->next;
    }
    pointer->next = a;
}

BinaryList * InsereBinario(BinaryList *head, int *op, int *RD, int *R1, int *R2, int *imed17, int *imed22, int format){
    BinaryList *b = (BinaryList*) malloc(sizeof(BinaryList));
    BinaryList *pointer;
    pointer = head;
    b->operando = op;
    b->RD = RD;
    b->R1 = R1;
    b->R2 = R2;
    b->Imediato17 = imed17;
    b->Imediato22 = imed22;
    b->format = format;
    b->next = NULL;
    if(head == NULL){
    	head = b;
    	return head; 
    }
    while(pointer->next != NULL){
    	pointer = pointer->next;
    }
    pointer->next = b;
}

char * getString(const char *format, int num){
	char *Srtrn = malloc(10*sizeof(char));
	sprintf(Srtrn, format, num);
	return Srtrn;
}

static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=4
#define UNINDENT indentno-=4

/* printSpaces indents by printing spaces */
static void printSpaces(FILE *arv)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(arv," ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */

void printTree( TreeNode *tree, FILE *arv )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces(arv);
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case VariableK:
          if(tree->type == 1)
	  fprintf(arv,"Variável: %s\n",tree->attr.name);
	  else{
	  	if(tree->type == 2)
	  	fprintf(arv,"Vetor: %s\n",tree->attr.name);
	  }
	  break;
	  case ParamK:
          if(tree->type == 1)
	  fprintf(arv,"Variável: %s\n",tree->attr.name);
	  else{
	  	if(tree->type == 2)
	  	fprintf(arv,"Vetor: %s\n",tree->attr.name);
	  }
	  break;
	case IfK:
          fprintf(arv,"If\n");
          break;
        case WhileK:
          fprintf(arv,"While\n");
          break;
        case AssignK:
          fprintf(arv,"Atribuição\n");
          break;
        case FunctionK:
          fprintf(arv,"Função: %s\n",tree->attr.name);
          break;
        case ReturnK:
          fprintf(arv,"Return: \n");
          break;
        default:
          fprintf(arv,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
          case TypeK:
	  fprintf(arv,"Tipo: %s\n",tree->attr.name);
	  break;
	  case OpK:
          fprintf(arv,"Op: ");
	  printToken(tree->attr.op,"\0", arv);
          break;
	case VetorK:
          fprintf(arv,"Vetor: %s\n",tree->attr.name);
          break;
        case ConstK:
          fprintf(arv,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(arv,"Id: %s\n",tree->attr.name);
          break;
        case CallK:
          fprintf(arv,"Chamada da função %s: \n", tree->attr.name);
          break;
        default:
          fprintf(arv,"Unknown ExpNode kind\n");
          break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i], arv);
    tree = tree->sibling;
  }
  UNINDENT;
}

void printQuad (QuadList * Q, FILE * Qo){
	while(Q->last != NULL)
		Q = Q->last;
	while(Q->next != NULL){
		switch(Q->op){
			//IFF,GOTO,LAB,ASSIGN,STORE,ALLOC,END,RET,LOAD,PARAM,CALL,FUNC,ARG,ADD,SUB,MULT,DIV,GT,GTE,LT,LTE,EQUAL,DIF,HALT
			case IFF: fprintf(Qo, "(IFF,"); break;
			case GOTO: fprintf(Qo, "(GOTO,"); break;
			case LAB: fprintf(Qo, "(LAB,"); break;
			case ASSIGN: fprintf(Qo, "(ASSIGN,"); break;
			case STORE: fprintf(Qo, "(STORE,"); break;
			case ALLOC: fprintf(Qo, "(ALLOC,"); break;
			case END: fprintf(Qo, "(END,"); break;
			case RET: fprintf(Qo, "(RET,"); break;
			case LOAD: fprintf(Qo, "(LOAD,"); break;
			case PARAM: fprintf(Qo, "(PARAM,"); break;
			case CALL: fprintf(Qo, "(CALL,"); break;
			case FUNC: fprintf(Qo, "(FUNC,"); break;
			case ARG: fprintf(Qo, "(ARG,"); break;
			case ADD: fprintf(Qo, "(ADD,"); break;
			case SUB: fprintf(Qo, "(SUB,"); break;
			case MULTI: fprintf(Qo, "(MULT,"); break;
			case DIVI: fprintf(Qo, "(DIV,"); break;
			case GT: fprintf(Qo, "(GT,"); break;
			case GTE: fprintf(Qo, "(GTE,"); break;
			case LT: fprintf(Qo, "(LT,"); break;
			case LTE: fprintf(Qo, "(LTE,"); break;
			case EQUAL: fprintf(Qo, "(EQUAL,"); break;
			case DIFI: fprintf(Qo, "(DIF,"); break;
			case HALT: fprintf(Qo, "(HALT,"); break;
		}
		fprintf(Qo, "%s,", Q->res);
		fprintf(Qo, "%s,", Q->arg1);
		fprintf(Qo, "%s)\n", Q->arg2);
		Q = Q->next;
	}
}

void printAssembly (AssemblyList *head, FILE * Qo){
	AssemblyList *A;
	A = head;
	while(A != NULL){
		switch(A->format){
		case 0:
			fprintf(Qo, ".%s\n", A->label);
			break;
		case 1:
			fprintf(Qo, "%d:\t", A->lineNo);
			fprintf(Qo, "%s ", A->op);
			fprintf(Qo, "%s,", A->arg1);
			fprintf(Qo, "%s,", A->arg2);
			fprintf(Qo, "%s\n", A->arg3);
			break;
		case 2:
			if(strcmp(A->op,"BNE") == 0 || strcmp(A->op,"BEQ") == 0){
				fprintf(Qo, "%d:\t", A->lineNo);
				fprintf(Qo, "%s ", A->op);
				fprintf(Qo, "%s,", A->arg1);
				fprintf(Qo, "%s,", A->arg2);
				fprintf(Qo, "%s\n", A->label);
			}
			else{
				fprintf(Qo, "%d:\t", A->lineNo);
				fprintf(Qo, "%s ", A->op);
				fprintf(Qo, "%s,", A->arg1);
				fprintf(Qo, "%s,", A->arg2);
				fprintf(Qo, "%d\n", A->imed);
			}
			break;
		case 3:
			fprintf(Qo, "%d:\t", A->lineNo);
			fprintf(Qo, "%s ", A->op);
			fprintf(Qo, "%s,", A->arg1);
			fprintf(Qo, "%s\n", A->arg2);
			break;
		case 4:
			fprintf(Qo, "%d:\t", A->lineNo);
			fprintf(Qo, "%s ", A->op);
			fprintf(Qo, "%s,", A->arg1);
			fprintf(Qo, "%d\n", A->imed);
			break;
		case 5:
			fprintf(Qo, "%d:\t", A->lineNo);
			fprintf(Qo, "%s ", A->op);
			fprintf(Qo, "%s\n", A->arg1);
			break;
		case 6:
			fprintf(Qo, "%d:\t", A->lineNo);
			fprintf(Qo, "%s ", A->op);
			fprintf(Qo, "%s\n", A->label);
			break;
		case 7:
			fprintf(Qo, "%d:\t", A->lineNo);
			fprintf(Qo, "%s\n", A->op);
			break;
		default:
			fprintf(Qo, "Instrução desconhecida");
			break;
		}
		A = A->next;
		
	}
}

void printBinario(BinaryList *b, FILE *f){
	int i, cont = 128;
	while(b != NULL){
		switch(b->format){
		case 1:
			fprintf(f, "	ram[%d] = 32'b", cont);
			cont++;
			for(i = 0; i < 6; i++) fprintf(f, "%d", b->operando[i]);
			//fprintf(f, "_");
			for(i = 0; i < 5; i++) fprintf(f, "%d", b->RD[i]);
			//fprintf(f, "_");
			for(i = 0; i < 5; i++) fprintf(f, "%d", b->R1[i]);
			//fprintf(f, "_");
			for(i = 0; i < 5; i++) fprintf(f, "%d", b->R2[i]);
			//fprintf(f, "_");
			fprintf(f, "00000000000");
			fprintf(f, ";\n");
			break;
		case 2:
			fprintf(f, "	ram[%d] = 32'b", cont);
			cont++;
			for(i = 0; i < 6; i++) fprintf(f, "%d", b->operando[i]);
			//fprintf(f, "_");
			for(i = 0; i < 5; i++) fprintf(f, "%d", b->RD[i]);
			//fprintf(f, "_");
			for(i = 0; i < 5; i++) fprintf(f, "%d", b->R1[i]);
			//fprintf(f, "_");
			for(i = 0; i < 16; i++) fprintf(f, "%d", b->Imediato17[i]);
			fprintf(f, ";\n");
			break;
		case 3:
			fprintf(f, "	ram[%d] = 32'b", cont);
			cont++;
			for(i = 0; i < 6; i++) fprintf(f, "%d", b->operando[i]);
			//fprintf(f, "_");
			for(i = 0; i < 5; i++) fprintf(f, "%d", b->RD[i]);
			//fprintf(f, "_");
			for(i = 0; i < 5; i++) fprintf(f, "%d", b->R1[i]);
			//fprintf(f, "_");
			fprintf(f, "0000000000000000");
			fprintf(f, ";\n");
			break;
		case 4:
			fprintf(f, "	ram[%d] = 32'b", cont);
			cont++;
			for(i = 0; i < 6; i++) fprintf(f, "%d", b->operando[i]);
			//fprintf(f, "_");
			for(i = 0; i < 5; i++) fprintf(f, "%d", b->RD[i]);
			//fprintf(f, "_");
			for(i = 0; i < 21; i++) fprintf(f, "%d", b->Imediato22[i]);
			fprintf(f, ";\n");
			break;
		case 5:
			fprintf(f, "	ram[%d] = 32'b", cont);
			cont++;
			for(i = 0; i < 6; i++) fprintf(f, "%d", b->operando[i]);
			//fprintf(f, "_");
			for(i = 0; i < 5; i++) fprintf(f, "%d", b->RD[i]);
			//fprintf(f, "_");
			fprintf(f, "000000000000000000000");
			fprintf(f, ";\n");
			break;
		case 6:
			fprintf(f, "	ram[%d] = 32'b", cont);
			cont++;
			for(i = 0; i < 6; i++) fprintf(f, "%d", b->operando[i]);
			//fprintf(f, "_");
			fprintf(f, "00000");
			//fprintf(f, "_");
			for(i = 0; i < 21; i++) fprintf(f, "%d", b->Imediato22[i]);
			fprintf(f, ";\n");
			break;
		case 7:
			fprintf(f, "	ram[%d] = 32'b", cont);
			cont++;
			for(i = 0; i < 6; i++) fprintf(f, "%d", b->operando[i]);
			//fprintf(f, "_");
			fprintf(f, "00000000000000000000000000");
			fprintf(f, ";\n");
			break;
		default:
			break;
		}
		b = b->next;
	}
}

  char * copyString(char * s)
  { int n;
    char * t;
    if (s==NULL) return NULL;
    n = strlen(s)+1;
    t = malloc(n);
    if (t==NULL)
      fprintf(listing,"Out of memory error at line %d\n",lineno);
    else strcpy(t,s);
    return t;
  }
