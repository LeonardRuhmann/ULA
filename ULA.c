#include "ULA.h"
#include <stdio.h>

//Uma função static só pode ser usada dentro do arquivo .c onde foi declarada

//Somador Baseado no livro Willian Stallings 10ed, capítulo 10, pag. 334
static void somadorCompleto(uint8_t A, uint8_t B, uint8_t cin, uint8_t * s, uint8_t * cout){
   //Implemente o código aqui
   *s = (~A & ~B & cin) | (~A & B & ~cin) | (A & B & cin) | (A & ~B & ~cin);
   *cout = (A & B) | (A & cin) | (B & cin);
}

static void somador8bits(uint8_t * regA, uint8_t * regB, uint8_t * Cin, uint8_t * soma, uint8_t * Cout, uint8_t * overflow){
    //Implemente o código aqui
    for (int i = 0; i <= 7; i++){
        *Cin = *Cout;

        somadorCompleto((*regA >> i) & 1, (*regB >> i) & 1, *Cin, soma, Cout);

        *regA = (*soma) ? (*regA | (1 << i)) : (*regA & ~(1 << i));
    }  
    *overflow = ((*Cin) ^ (*Cout));
}

static void complementador(uint8_t * A){
    //Implemente o código aqui
    *A = ~(*A);    
}

//Adição de A e B e grava resultado em A
void ULA_ADD(int8_t * regA, int8_t * regB, int8_t * overflow){
    //Implemente o código aqui
    uint8_t Temp_regA = (uint8_t)*regA;
    uint8_t Temp_regB = (uint8_t) *regB;
    uint8_t Carry_in = 0;
    uint8_t Carry_out = 0;
    uint8_t Soma = 0;

    somador8bits(&Temp_regA, &Temp_regB, &Carry_in, &Soma, &Carry_out, overflow);

    *regA = Temp_regA;
}

//Subtração de A e B e grava resultado em A
void ULA_SUB(int8_t * regA, int8_t * regB, int8_t * overflow){
    //Implemente o código aqui
    uint8_t Temp_regA = (uint8_t)*regA;
    uint8_t Temp_regB = (uint8_t)*regB;
    complementador(&Temp_regB);
    uint8_t Carry_in = 0;
    uint8_t Carry_out = 1;
    uint8_t Soma = 0;

    somador8bits(&Temp_regA, &Temp_regB, &Carry_in, &Soma, &Carry_out, overflow);

    *regA = Temp_regA;
}

//Multiplicação de Q(8bits) com M(8bits) gera resultado de 16bits que está em A(8bits) e Q(8bits)
void ULA_MUL(int8_t * A, int8_t * Q, int8_t * M,  int8_t * overflow){
//Implemente o código aqui
}

//Divisão com sinal de Q(Dividendo de 8bits) por M(Divisor de 8bits) com Quociente em Q(8bits) e Resto em A(8bits)
void ULA_DIV(int8_t * A, int8_t * Q, int8_t * M, int8_t * overflow){
    //Implemente o código aqui
}
