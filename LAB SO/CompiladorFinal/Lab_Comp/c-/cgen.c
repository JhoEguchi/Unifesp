/****************************************************/
/* File: cgen.c                                     */
/* The code generator implementation                */
/* for the TINY compiler                            */
/* (generates code for the TM machine)              */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "util.h"
#include "cgen.h"

/* tmpOffset is the memory offset for temps
   It is decremented each time a temp is
   stored, and incremeted when loaded again
*/
static int Registercnt = -1;

static int Labelcnt = 0;

static QuadList * Q;

int IncrementaReg();

int RetornaUltReg();

/* prototype for internal recursive code generator */
static void cGen (TreeNode * tree, int);

static void cGenWoS (TreeNode * tree, int);

/* Procedure genStmt generates code at a statement node */
static void genStmt( TreeNode * tree, int flag)
{ TreeNode * p1, * p2, * p3;
  char * t1, * t2;
  int savedLabel, savedRegister;
  switch (tree->kind.stmt) {

      case IfK :
         p1 = tree->child[0] ;
         p2 = tree->child[1] ;
         p3 = tree->child[2] ;
         cGen(p1, flag);
         newQuad(Q, IFF, getString("$t%d",RetornaUltReg()), getString("L%d",Labelcnt), "-");
         Q = Q->next;
         savedLabel = Labelcnt;
         if(p3 != NULL){
         	Labelcnt+= 2;
         }
         else{ 
         	Labelcnt+= 1;
         }
         cGen(p2, flag);      
         if(p3 != NULL){
         	newQuad(Q, GOTO, getString("L%d",++savedLabel), "-", "-");
         	Q = Q->next;
         	savedLabel--;
         }
         newQuad(Q, LAB, getString("L%d",savedLabel), "-", "-");
         Q = Q->next;
         if(p3 != NULL){
         	cGen(p3, flag);
        	newQuad(Q, LAB, getString("L%d",++savedLabel), "-", "-");
        	Q = Q->next;
         }
         break;

      case WhileK:
         p1 = tree->child[0] ;
         p2 = tree->child[1] ;
         newQuad(Q, LAB, getString("L%d",Labelcnt), "-", "-");
         Q = Q->next;
         savedLabel = Labelcnt;
         Labelcnt+= 2;
         cGen(p1, flag);
         newQuad(Q, IFF, getString("$t%d",RetornaUltReg()), getString("L%d",++savedLabel), "-");
         Q = Q->next;
         cGen(p2, flag);
         newQuad(Q, GOTO, getString("L%d",--savedLabel), "-", "-");
         Q = Q->next;
         newQuad(Q, LAB, getString("L%d",++savedLabel), "-", "-");
         Q = Q->next;
         
         break; /* repeat */

      case AssignK:
         p1 = tree->child[0] ;
         p2 = tree->child[1] ;
         cGen(p1, flag);
         t1 = getString("$t%d", RetornaUltReg());
         cGen(p2, flag);
         switch(p2->kind.exp){
         	case IdK:
         	case OpK:
         	case CallK:
         		t2 = getString("$t%d", RetornaUltReg());
         		break;
         	case ConstK:
         		t2 = getString("%d", p2->attr.val);
         		break;
            case VetorK: break;
            case TypeK: break;
         }
         newQuad(Q, ASSIGN, t1, t2, "-");
         Q = Q->next;
         switch(p1->type){
         	case 0: break;
         	case 1:
         		newQuad(Q, STORE, p1->attr.name, t1, "-");
         		break;
         	case 2: 
         		cGen(p1->child[0], flag);
         		newQuad(Q, STORE, p1->attr.name, t1, getString("$t%d", RetornaUltReg()));
         		break;
         }
         Q = Q->next;
         
         break; /* assign_k */

      case VariableK:
         switch(tree->type){
         	case 0: break;
         	case 1:
         		newQuad(Q, ALLOC, tree->attr.name, tree->scope, "-");
         		Q = Q->next;
         		break;
         	case 2:
         		newQuad(Q, ALLOC, tree->attr.name, tree->scope, getString("%d",tree->len));
         		Q = Q->next;
         		break;
         }
         
         break;
         
      case FunctionK:
         p1 = tree->child[0] ;
         p2 = tree->child[1] ;
         cGen(p1, flag);
         cGen(p2, flag);
         newQuad(Q, END, tree->attr.name, "-", "-");
         Q = Q->next;
         break;
      case ReturnK: //arrumar casos
         p1 = tree->child[0];
         if(p1 != NULL){
         	cGen(p1, flag);
         	switch(p1->kind.exp){
         	case IdK:
         	case OpK:
         	case CallK:
         		newQuad(Q, RET, getString("$t%d",RetornaUltReg()), "-", "-");
         		break;
         	case ConstK:
         		newQuad(Q, RET, getString("%d", p1->attr.val), "-", "-"); 
         		break;
         	case TypeK: break;
           	case VetorK: break;
         	}
         	Q = Q->next;
         }
         else{
         	newQuad(Q, RET, NULL, "-", "-");
         	Q = Q->next;
         }
         break;
      case ParamK:
         //newQuad(Q, LOAD, getString("$t%d",IncrementaReg()), tree->attr.name, "-");
         //Q = Q->next;
         break;
      default:
         break;
    }
} /* genStmt */

/* Procedure genExp generates code at an expression node */
static void genExp( TreeNode * tree, int flag)
{ int contador = 0;;
  TreeNode * p1, * p2;
  char * t1, * t2, * t3;
  switch (tree->kind.exp) {

    case ConstK :
      break; /* ConstK */
    
    case CallK:
         p1 = tree->child[0];
         while(p1 != NULL){
         	cGenWoS(p1, flag);
         	if(p1->kind.exp == ConstK){
         		newQuad(Q, PARAM, getString("%d", p1->attr.val), "-", "-");
         	}
         	else
         		newQuad(Q, PARAM, getString("$t%d",RetornaUltReg()), "-", "-");
         	Q = Q->next;
         	p1 = p1->sibling;
         	contador++;
         }
         if(strcmp(tree->attr.name, "output") == 0 || strcmp(tree->attr.name, "CpyHDtoIM") == 0 || strcmp(tree->attr.name, "CpyRegtoHD") == 0 || strcmp(tree->attr.name, "CpyHDtoReg") == 0 || strcmp(tree->attr.name, "CpyDMtoHD") == 0 || strcmp(tree->attr.name, "CpyHDtoDM") == 0 || strcmp(tree->attr.name, "Display") == 0 || strcmp(tree->attr.name, "DisplayProgram") == 0 || strcmp(tree->attr.name, "Execproc") == 0 || strcmp(tree->attr.name, "setCS") == 0 || strcmp(tree->attr.name, "unSetCS") == 0 || strcmp(tree->attr.name, "jumpProc") == 0)
         	newQuad(Q, CALL, getString("$t%d",RetornaUltReg()), tree->attr.name, getString("%d",contador));
         else
         	newQuad(Q, CALL, getString("$t%d",IncrementaReg(flag)), tree->attr.name, getString("%d",contador));
         Q = Q->next;
         break;
    
    case IdK :
         p1 = tree->child[0];
         
         if(p1 == NULL){
         	newQuad(Q, LOAD, getString("$t%d",IncrementaReg(flag)), tree->attr.name, "-");
         	Q = Q->next;
         }
         else{
         	cGen(p1, flag);
         	switch(p1->kind.exp){
         	case IdK:
         	case OpK:
         	case CallK:
         		t1 = getString("$t%d", RetornaUltReg());
         		break;
         	case ConstK:
         		t1 = getString("%d", p1->attr.val);
         		break;
         	case TypeK: break;
            	case VetorK: break;
         	}
         	newQuad(Q, LOAD, getString("$t%d", IncrementaReg(flag)), tree->attr.name, t1);
         	Q = Q->next;
         }
      break; /* IdK */
    case TypeK :
         p1 = tree->child[0];
         
         switch(p1->kind.stmt){
         	case IfK: break;
         	case WhileK: break;
         	case VariableK:
         		cGen(p1, flag);
         		break;
         	case AssignK: break;
         	case FunctionK:
         		newQuad(Q, FUNC, tree->attr.name, p1->attr.name, "-");
         		Q = Q->next;
         		cGen(p1, flag);
         		break;
         	case ReturnK: break;
         	case ParamK:
         		newQuad(Q, ARG, tree->attr.name, p1->attr.name, p1->scope);
         		Q = Q->next;
         		cGen(p1, flag);
         		break;
         }
         cGen(tree->child[2], flag);
      break; 
    case OpK :
         p1 = tree->child[0];
         p2 = tree->child[1];
         /* gen code for ac = left arg */
         cGen(p1, flag);
         /* gen code to push left operand */
         switch(p1->kind.exp){
         	case IdK:
         	case OpK:
         		t2 = getString("$t%d", RetornaUltReg());
         		break;
         	case CallK:
         		t2 = getString("$t%d", RetornaUltReg());
         		break;
         	case ConstK:
         		t2 = getString("%d", p2->attr.val);
         		break;
		      case TypeK: break;
            case VetorK: break;
         	}
         /* gen code for ac = right operand */
         cGen(p2, flag);
         switch(p2->kind.exp){
         	case IdK:
         	case OpK:
         		t3 = getString("$t%d", RetornaUltReg());
         		break;
         	case CallK:
         		t3 = getString("$t%d", RetornaUltReg());
         		break;
         	case ConstK:
         		t3 = getString("%d", p2->attr.val);
         		break;
		      case TypeK: break;
            case VetorK: break;
         	}
         t1 = getString("$t%d", IncrementaReg(flag));
         /* now load left operand */
         switch (tree->attr.op) {
            case MAIS :
               newQuad(Q, ADD, t1, t2, t3);
               Q = Q->next;
               break;
            case MENOS :
               newQuad(Q, SUB, t1, t2, t3);
               Q = Q->next;
               break;
            case MULT :
               newQuad(Q, MULTI, t1, t2, t3);
               Q = Q->next;
               break;
            case DIV :
               newQuad(Q, DIVI, t1, t2, t3);
               Q = Q->next;
               break;
            case MAIOR :
               newQuad(Q, GT, t1, t2, t3);
               Q = Q->next;
               break;
            case MAIORIGUAL :
               newQuad(Q, GTE, t1, t2, t3);
               Q = Q->next;
               break;
            case MENOR :
               newQuad(Q, LT, t1, t2, t3);
               Q = Q->next;               
               break;
            case MENORIGUAL :
               newQuad(Q, LTE, t1, t2, t3);
               Q = Q->next;               
               break;
            case IGUAL :
               newQuad(Q, EQUAL, t1, t2, t3);
               Q = Q->next;
               break;
            case DIF :
               newQuad(Q, DIFI, t1, t2, t3);
               Q = Q->next;
               break;
            default:
               break;
         } /* case op */
         break; /* OpK */

    default:
      break;
  }
} /* genExp */

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen( TreeNode * tree, int flag)
{ if (tree != NULL)
  { switch (tree->nodekind) {
      case StmtK:
        genStmt(tree, flag);
        break;
      case ExpK:
        genExp(tree, flag);
        break;
      default:
        break;
    }
    cGen(tree->sibling, flag);
  }
}

static void cGenWoS( TreeNode * tree, int flag)
{ if (tree != NULL)
  { switch (tree->nodekind) {
      case StmtK:
        genStmt(tree, flag);
        break;
      case ExpK:
        genExp(tree, flag);
        break;
      default:
        break;
    }
  }
}

int IncrementaReg(int flag){
   switch (flag)
   {
   case 0:
      if(Registercnt >= 7){
		   Registercnt = -1;
	   }
	   return ++Registercnt;
   case 1:
      if(Registercnt < 8)
         Registercnt = 7;
      else if(Registercnt >= 15){
		   Registercnt = 7;
	   }
	   return ++Registercnt;
   default:
      break;
   }
}

int RetornaUltReg(){
	return Registercnt;
}

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
QuadList * codeGen(TreeNode * syntaxTree, int flag)
{  
   Q = startQuad();
   cGen(syntaxTree, flag);
   newQuad(Q, HALT, "-", "-", "-");
   //printf("\n");
   return Q;
}

