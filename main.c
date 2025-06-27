//Trabalho da Disciplina de RUS0005 - ARQUITETURA DE COMPUTADORES
//UFC - Campus Russas
//Professor Autor: Reuber Regis de Melo
//OBS: Esse arquivo não precisa ser editado, somente deve ser editado o arquivo ULA.c
#include <stdio.h>
#include "ULA.h"

#define MAX_LINHAS 100 // Defina um limite máximo de linhas de dados

void print8Bits( int8_t *A){
	printf("(%4d)_10 = (",*A);
    for (int8_t i = 7; i >= 0; i--) {
        printf("%d", (*A >> i) & 1);
    }
	printf(")_2");
}


int main() {
	/*Cabeçalho*/
	printf("\t\t\t\t==========================================\n");
    printf("\t\t\t\t SIMULADOR DE ULA - v0.2  by Prof. Reuber      \n");
    printf("\t\t\t\t==========================================\n");
	
	/*---------------------------------*/
	//Leitura de Arquivo com os Operandos
	FILE *arquivo = fopen("dados.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    int8_t matriz[MAX_LINHAS][2]; // Matriz estática
    int linhas = 0;                  // Contador de linhas
    // Lendo os pares de números do arquivo até EOF
    while (linhas < MAX_LINHAS && fscanf(arquivo, "%d %d", &matriz[linhas][0], &matriz[linhas][1]) == 2) {
        linhas++;
    }
    fclose(arquivo);
    //Fim da Leitura do Arquivo com Operandos
	/*----------------------------*/
	
	/*-----------------------------*/
	//Chama a ULA para realizar as 4 operações e imprime os resultados
	for(int i=0;i<linhas;i++){	
		int8_t OpA = matriz[i][0], OpB=matriz[i][1], ovf, regA; //Operandos de 8 bits com sinal (-128 a 127)
		printf("----------------------------------------------\n");
		/*--ADD--*/
		print8Bits(&OpA);
		printf("  ");
		print8Bits(&OpB);	
		ULA_ADD(&OpA,&OpB,&ovf);
        printf("  ADD:   REG A: %6d           \tOverflow: %d",OpA,ovf);
        printf("\n");
//
		/*--SUB--*/
		OpA = matriz[i][0], OpB=matriz[i][1];
		print8Bits(&OpA);
		printf("  ");
		print8Bits(&OpB);
		ULA_SUB(&OpA,&OpB,&ovf);
		printf("  SUB:   REG A: %6d           \tOverflow: %d",OpA,ovf);
		printf("\n");
    	//*--MUL--*//*
		OpA = matriz[i][0], OpB=matriz[i][1];
		print8Bits(&OpA);
		printf("  ");
		print8Bits(&OpB);
		ULA_MUL(&regA,&OpA,&OpB,&ovf);
        //printf("  MUL: REG A: %4d REG Q:%4d AQ: %d\tOverflow: %d",regA, OpA,((int16_t)regA << 8) | ((uint8_t)OpA),ovf);
        printf("  MUL: REGs AQ: %6d           \tOverflow: %d",(int16_t) (((regA & 0x00FF) << 8) | (OpA & 0x00FF)),ovf);
		printf("\n");
//		*//*--DIV--*//*
		OpA = matriz[i][0], OpB=matriz[i][1];
		print8Bits(&OpA);
		printf("  ");
		print8Bits(&OpB);
		ULA_DIV(&regA,&OpA,&OpB,&ovf);
		printf("  DIV:   REG A: %6d REG Q:%4d\tOverflow: %d",regA, OpA,ovf);
		printf("\n");

	}
	//Fim das Operações da ULA e impressão dos resultados
	/*-----------------------------*/

	return 0;
}