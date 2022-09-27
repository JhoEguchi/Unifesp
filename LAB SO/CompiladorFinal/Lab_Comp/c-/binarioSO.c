#include "globals.h"
#include "assembly.h"
#include "symtab.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int EncontraLinhaSO(AssemblyList *A, char *label){ // Encontra a linha para pular em operacoes de JUMP e JAL
	AssemblyList *pointer;
	pointer = A;
	while(pointer != NULL){
		if(pointer->format == 0){
			if(strcmp(pointer->label, label) == 0)
				return pointer->lineNo;
		}
		pointer = pointer->next;
	}
	return 0;
}

int *RetornavetorSO(int b, int size){	// Retorna o vetor, sendo b o numero binario e size, a quantidade de bits utilizada por tal parte da instrucao
	int *vetor = (int*) malloc(size*sizeof(int));
	int cont;
	if(b == 0)
		cont = 0;
	else 
		cont = floor(log10(abs(b))) + 1;
	int i;
	for(i = 0; i < cont; i++){
		vetor[size-i-1] = b % 10;
		b = b/10;
	}
	for(i = size - cont - 1; i > 0; i--){
		vetor[i] = 0;
	}
	return vetor;
}


int *Retornavetor_numeroSO(int b, int isNegative, int size){	// Mesma funcao de cima, no entanto, tem a flag isNegative, para numeros negativos (incluir 1 no bit mais significativo)
	int *vetor = (int*) malloc(size*sizeof(int));
	int cont;
	if(b == 0)
		cont = 0;
	else 
		cont = floor(log10(abs(b))) + 1;
	int i;
	for(i = 0; i < cont; i++){
		vetor[size-i-1] = b % 10;
		b = b/10;
	}
	for(i = size - cont -1; i > 0; i--){
		vetor[i] = 0;
	}
	vetor[0] = isNegative;
	return vetor;
}

int InttoBinSO(int inteiro){	// Converte inteiro para binario (devolve um numero inteiro, mas como se fosse possivel ler em binario)
	if(inteiro == 0) return 0;
	if(inteiro == 1) return 1;
	return (inteiro % 2) + 10*InttoBinSO(inteiro/2);
}

int* RetornaRegistradorSO(char *reg){	// Retorna a posicao do registrador reg
	if (strcmp(reg, "$zero") == 0) return RetornavetorSO(0, 5); //$zero
	if (strcmp(reg, "$t0") == 0) return RetornavetorSO(1, 5); //$t0
	if (strcmp(reg, "$t1") == 0) return RetornavetorSO(10, 5); //$t1
	if (strcmp(reg, "$t2") == 0) return RetornavetorSO(11, 5); //$t2
	if (strcmp(reg, "$t3") == 0) return RetornavetorSO(100, 5); //$t3
	if (strcmp(reg, "$t4") == 0) return RetornavetorSO(101, 5); //$t4
	if (strcmp(reg, "$t5") == 0) return RetornavetorSO(110, 5); //$t5
	if (strcmp(reg, "$t6") == 0) return RetornavetorSO(111, 5); //$t6
	if (strcmp(reg, "$t7") == 0) return RetornavetorSO(1000, 5); //$t7
	if (strcmp(reg, "$t8") == 0) return RetornavetorSO(1001, 5); //$t8
	if (strcmp(reg, "$t9") == 0) return RetornavetorSO(1010, 5); //$t9
	if (strcmp(reg, "$t10") == 0) return RetornavetorSO(1011, 5); //$t10
	if (strcmp(reg, "$t11") == 0) return RetornavetorSO(1100, 5); //$t11
	if (strcmp(reg, "$t12") == 0) return RetornavetorSO(1101, 5); //$t12
	if (strcmp(reg, "$t13") == 0) return RetornavetorSO(1110, 5); //$t13
	if (strcmp(reg, "$t14") == 0) return RetornavetorSO(1111, 5); //$t14
	if (strcmp(reg, "$t15") == 0) return RetornavetorSO(10000, 5); //$t15
	if (strcmp(reg, "$a0") == 0) return RetornavetorSO(10001, 5); //$a0
	if (strcmp(reg, "$a1") == 0) return RetornavetorSO(10010, 5); //$a1
	if (strcmp(reg, "$a2") == 0) return RetornavetorSO(10011, 5); //$a2
	if (strcmp(reg, "$a3") == 0) return RetornavetorSO(10100, 5); //$a3
	if (strcmp(reg, "$a4") == 0) return RetornavetorSO(10101, 5); //$a4
	if (strcmp(reg, "$a5") == 0) return RetornavetorSO(10110, 5); //$a5
	if (strcmp(reg, "$PC") == 0) return RetornavetorSO(10111, 5); //$PC
    if (strcmp(reg, "$sp") == 0) return RetornavetorSO(11000, 5); //$sp para o SO
	if (strcmp(reg, "$gp") == 0) return RetornavetorSO(11001, 5); //$gp para o SO
	if (strcmp(reg, "$h0") == 0) return RetornavetorSO(11010, 5); //$h0
	if (strcmp(reg, "$h1") == 0) return RetornavetorSO(11011, 5); //$h1
	if (strcmp(reg, "$v0") == 0) return RetornavetorSO(11100, 5); //$v0
	//if (strcmp(reg, "$sp") == 0) return RetornavetorSO(11101, 5); //$sp para o programa
	//if (strcmp(reg, "$gp") == 0) return RetornavetorSO(11110, 5); //$gp para o programa
	if (strcmp(reg, "$ra") == 0) return RetornavetorSO(11111, 5); //$ra
}

void genBinarySO(AssemblyList *head, BinaryList *B){	// Funcao que gera o codigo binario
	AssemblyList *A;
	A = head;
	while(A != NULL)
	{
		switch(A->format)
		{
		case 1:
			if(strcmp(A->op, "ADD") == 0) InsereBinario(B,  RetornavetorSO(0, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			else if(strcmp(A->op, "SUB") == 0) InsereBinario(B,  RetornavetorSO(10, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			else if(strcmp(A->op, "SLT") == 0) InsereBinario(B,  RetornavetorSO(1001, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			else if(strcmp(A->op, "MULT") == 0) InsereBinario(B,  RetornavetorSO(11010, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			else if(strcmp(A->op, "DIV") == 0) InsereBinario(B,  RetornavetorSO(11011, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			else if(strcmp(A->op, "CpyHDtoIM") == 0) InsereBinario(B,  RetornavetorSO(11100, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			else if(strcmp(A->op, "CpyRegtoHD") == 0) InsereBinario(B,  RetornavetorSO(11101, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			else if(strcmp(A->op, "CpyHDtoReg") == 0) InsereBinario(B,  RetornavetorSO(11110, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			else if(strcmp(A->op, "CpyDMtoHD") == 0) InsereBinario(B,  RetornavetorSO(11111, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			else if(strcmp(A->op, "CpyHDtoDM") == 0) InsereBinario(B,  RetornavetorSO(100000, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), RetornaRegistradorSO(A->arg3), NULL, NULL, 1);
			break;
		case 2:
			if(strcmp(A->op, "ADDI") == 0) InsereBinario(B,  RetornavetorSO(1, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), NULL, Retornavetor_numeroSO(InttoBinSO(A->imed), A->imed < 0 ? 1 : 0, 16), NULL, 2);
			else if(strcmp(A->op, "SUBI") == 0) InsereBinario(B,  RetornavetorSO(11, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), NULL, Retornavetor_numeroSO(InttoBinSO(A->imed), A->imed < 0 ? 1 : 0, 16), NULL, 2);
			else if(strcmp(A->op, "SLTI") == 0) InsereBinario(B,  RetornavetorSO(1010, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), NULL, Retornavetor_numeroSO(InttoBinSO(A->imed), A->imed < 0 ? 1 : 0, 16), NULL, 2);
			else if(strcmp(A->op, "BEQ") == 0) InsereBinario(B,  RetornavetorSO(10000, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), NULL, Retornavetor_numeroSO(InttoBinSO(EncontraLinhaSO(head, A->label)), 0, 16), NULL, 2);
			else if(strcmp(A->op, "BNE") == 0) InsereBinario(B,  RetornavetorSO(10001, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), NULL, Retornavetor_numeroSO(InttoBinSO(EncontraLinhaSO(head, A->label)), 0, 16), NULL, 2);
			else if(strcmp(A->op, "LOAD") == 0) InsereBinario(B,  RetornavetorSO(10010, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), NULL, Retornavetor_numeroSO(InttoBinSO(A->imed), A->imed < 0 ? 1 : 0, 16), NULL, 2);
			else if(strcmp(A->op, "STORE") == 0) InsereBinario(B,  RetornavetorSO(10100, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), NULL, Retornavetor_numeroSO(InttoBinSO(A->imed), A->imed < 0 ? 1 : 0, 16), NULL, 2);
			break;
		case 3:
			if(strcmp(A->op, "MOVE") == 0) InsereBinario(B,  RetornavetorSO(10101, 6), RetornaRegistradorSO(A->arg1), RetornaRegistradorSO(A->arg2), NULL, NULL, NULL, 3);
			break;
		case 4:
			if(strcmp(A->op, "LOADI") == 0) InsereBinario(B,  RetornavetorSO(10011, 6), RetornaRegistradorSO(A->arg1), NULL, NULL, NULL, Retornavetor_numeroSO(InttoBinSO(A->imed), A->imed < 0 ? 1 : 0, 21), 4);
			break;
		case 5:
			if(strcmp(A->op, "IN") == 0) InsereBinario(B,  RetornavetorSO(10110, 6), RetornaRegistradorSO(A->arg1), NULL, NULL, NULL, NULL, 5);
			else if(strcmp(A->op, "JUMP") == 0) InsereBinario(B,  RetornavetorSO(1101, 6), RetornaRegistradorSO(A->arg1), NULL, NULL, NULL, NULL, 5);
			else if(strcmp(A->op, "OUT") == 0) InsereBinario(B,  RetornavetorSO(10111, 6), RetornaRegistradorSO(A->arg1), NULL, NULL, NULL, NULL, 5);
			else if(strcmp(A->op, "Display") == 0) InsereBinario(B,  RetornavetorSO(100010, 6), RetornaRegistradorSO(A->arg1), NULL, NULL, NULL, NULL, 5);
			else if(strcmp(A->op, "DisplayProgram") == 0) InsereBinario(B,  RetornavetorSO(100011, 6), RetornaRegistradorSO(A->arg1), NULL, NULL, NULL, NULL, 5);
			else if(strcmp(A->op, "Execproc") == 0) InsereBinario(B,  RetornavetorSO(1101, 6), RetornaRegistradorSO(A->arg1), NULL, NULL, NULL, NULL, 5);
			else if(strcmp(A->op, "jumpProc") == 0) InsereBinario(B,  RetornavetorSO(1101, 6), RetornaRegistradorSO(A->arg1), NULL, NULL, NULL, NULL, 5);
			break;
		case 6:
			if(strcmp(A->op, "JUMPI") == 0) InsereBinario(B,  RetornavetorSO(1110, 6), NULL, NULL, NULL, NULL, Retornavetor_numeroSO(InttoBinSO(EncontraLinhaSO(head, A->label)), 0, 21), 6);
			else if(strcmp(A->op, "JAL") == 0) InsereBinario(B,  RetornavetorSO(1111, 6), NULL, NULL, NULL, NULL, Retornavetor_numeroSO(InttoBinSO(EncontraLinhaSO(head, A->label)), 0, 21), 6);
			break;
		case 7:
			if(strcmp(A->op, "HLT") == 0) InsereBinario(B,  RetornavetorSO(11001, 6), NULL, NULL, NULL, NULL, NULL, 7);
			else if(strcmp(A->op, "setCS") == 0) InsereBinario(B,  RetornavetorSO(100100, 6), NULL, NULL, NULL, NULL, NULL, 7);
			else if(strcmp(A->op, "unSetCS") == 0) InsereBinario(B,  RetornavetorSO(100101, 6), NULL, NULL, NULL, NULL, NULL, 7);
			break;
		default:
			break;
		}
		A = A->next;
	}
}

BinaryList *BinaryGeneratorSO(AssemblyList *A){
	BinaryList *head;
	int i;
	head = InsereBinario(head,  NULL, NULL, NULL, NULL, NULL, NULL, -1);
	genBinarySO(A, head);
	return head;
}
