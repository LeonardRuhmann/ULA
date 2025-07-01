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
    uint8_t Temp_A = 0;     // Será O nosso resultado para os primeiros 8 bits do resultado de 16bits.
    uint8_t Temp_Q = (uint8_t)*Q;   // Nosso Multiplicador / Será nosso resultado para os últimos 8 bits do resultado de 16bits.
    uint8_t Temp_M = (uint8_t)*M;   // Nosso multiplicando.
    uint8_t Q_1 = 0;    // O bit mais à direita do bit menos significativo de Q.

    for (int i = 0; i < 8; i++) {
        uint8_t Q0 = Temp_Q & 1;    // Bit menos significativo de Q.

        // Se o bit menos significativo de Q for 1 e seu antecessor for 0 (Bit à sua direita), faça A = Q - M.
        if (Q0 == 1 && Q_1 == 0) {
            ULA_SUB((int8_t*)&Temp_A, (int8_t*)&Temp_M, overflow);
        
        // Se o bit menos significativo de Q for 0 e seu antecessor for 1 (Bit à sua direita), faça A = Q + M.
        } else if (Q0 == 0 && Q_1 == 1) {
            ULA_ADD((int8_t*)&Temp_A, (int8_t*)&Temp_M, overflow);
        }

        // Após isso, A, Q e Q-1 devem ser deslocados para a direita.
        // Exemplo: A = 1011 ; Q = 0110; Q-1 = 1 -> Deslocado para a direita fica: A = 1101; Q = 1011; Q-1 = 0.
        // Antes ~~~~~~~~~~^~~~~~~^~~~^~~~~~~~~^       Depois do deslocamento ~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~^
        //                (1)    (2) (3)      (4)                                               (1)         (3)

        // (1) Esse bit será empurrado para a posição (2).
        // (3) Esse bit será empurrado para a posição (4).

        uint8_t A_MSB = Temp_A & (1 << 7);    // Bit mais significativo de A.
        uint8_t A_LSB = Temp_A & 1;    // Bit menos significativo de A. Seria o (1) no exemplo acima
        uint8_t Q_LSB = Temp_Q & 1;    // Bit menos significativo de Q. Seria o (3) no exemplo acima

        // Como no exemplo anterior, vamos deslocar A, Q e Q-1.
        // Lembrando que se em A o MSB for 1, mesmo depois de deslocado, o MSB deve ser um.
        // Ou seja, normalmente 1011 >> 1 = 0101. Mas aqui, se o MSB é 1, 1011 >> 1 deve ser = 1101.
        //                                                       ^^^~~~~~~^~~~~~~~~~~~~~~~~~~~~^
        // Se o MSB for 0, então o deslocamento funciona normalmente: 0110 >> 1 = 0011.
        //      ^^^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~^
        Temp_A = (Temp_A >> 1) | A_MSB;    // Essa op diz: O bit MSB de A será mantido após o deslocamento. (Se for 1, será 1, se 0, será 0)
        Temp_Q = (Temp_Q >> 1) | (A_LSB << 7);    // Essa op diz: O bit MSB de Q será o LSB de A.
        Q_1 = Q_LSB;    // Essa op diz: O bit anterior ao LSB de Q será o LSB de Q (Que era LSb antes de Q ser deslocado).

        // As linhas 96 à 98 são na prática do exemplo da linha 79.
    }
    // No fim de todas as contas, a resposta está em A e Q. Pois na ULA, multiplicar duas palavras bi-
    // nárias significa que seu resultado será a concatenação de A e de Q. Logo, multiplicar duas pala-
    // vras de 8 bit nos resultaria em um resultado de 16 bits. multiplicar duas palavras de 4 bits
    // resultaria em um resultado de 8 bits. Ou seja, sempre o dobro.
    *A = (int8_t)Temp_A;
    *Q = (int8_t)Temp_Q;
}

//Divisão com sinal de Q(Dividendo de 8bits) por M(Divisor de 8bits) com Quociente em Q(8bits) e Resto em A(8bits)
void ULA_DIV(int8_t * A, int8_t * Q, int8_t * M, int8_t * overflow){
    //Implemente o código aqui
}
