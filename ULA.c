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
void ULA_MUL(int8_t *A, int8_t *Q, int8_t *M, int8_t *overflow) {
    uint8_t Temp_A = 0;     
    uint8_t Temp_Q = (uint8_t)*Q;   
    uint8_t Temp_M = (uint8_t)*M;   
    uint8_t Q_1 = 0;  

    for (int i = 0; i < 8; i++) {
        uint8_t Q0 = Temp_Q & 1;   

        if (Q0 == 1 && Q_1 == 0) {
            ULA_SUB((int8_t*)&Temp_A, (int8_t*)&Temp_M, overflow);
        
        } else if (Q0 == 0 && Q_1 == 1) {
            ULA_ADD((int8_t*)&Temp_A, (int8_t*)&Temp_M, overflow);
        }

        uint8_t A_MSB = Temp_A & (1 << 7);    
        uint8_t A_LSB = Temp_A & 1;    
        uint8_t Q_LSB = Temp_Q & 1;    

        Temp_A = (Temp_A >> 1) | A_MSB;    
        Temp_Q = (Temp_Q >> 1) | (A_LSB << 7);    
        Q_1 = Q_LSB;    
    }
    *A = (int8_t)Temp_A;
    *Q = (int8_t)Temp_Q;
}

//Divisão com sinal de Q(Dividendo de 8bits) por M(Divisor de 8bits) com Quociente em Q(8bits) e Resto em A(8bits)
void ULA_DIV(int8_t * A, int8_t * Q, int8_t * M, int8_t * overflow){
    if (*M == 0 || (*Q == -128 && *M == -1)) {
        *overflow = 1;
        *A = 0;
        *Q = 0;
        return;
    }
    int sinal_de_Q = *Q < 0;
    int sinal_de_M = *M < 0;
    int sinal_do_quotiente = sinal_de_Q ^ sinal_de_M;

    uint8_t abs_Q = sinal_de_Q ? -(*Q) : *Q;
    uint8_t abs_M = sinal_de_M ? -(*M) : *M;

    uint8_t temp_A = 0;
    uint8_t temp_Q = abs_Q;

    for (int i = 0; i < 8; i++) {
        uint8_t bit_mais_significativo_de_Q = temp_Q & 0x80;
        temp_A = (temp_A << 1) | (bit_mais_significativo_de_Q >> 7);
        temp_Q <<= 1;

        uint8_t original_A = temp_A;
        temp_A -= abs_M;

        if (temp_A & 0x80) {
            temp_A = original_A;
        } else {
            temp_Q |= 1;
        }
    }

    int8_t resto = (int8_t)temp_A;
    int8_t quociente = (int8_t)temp_Q;

    if (sinal_de_Q) resto = -resto;
    if (sinal_do_quotiente) quociente = -quociente;

    *A = resto;
    *Q = quociente;
    *overflow = 0;
}
