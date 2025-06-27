#ifndef ULA_H
#define ULA_H

#include <stdint.h>

//Operações Aritméticas com Inteiros na ULA

void ULA_ADD(int8_t * regA, int8_t * regB, int8_t * overflow);
void ULA_SUB(int8_t * regA, int8_t * regB, int8_t * overflow);
void ULA_MUL(int8_t * A, int8_t * Q, int8_t * M, int8_t * overflow);
void ULA_DIV(int8_t * A, int8_t * Q, int8_t * M, int8_t * overflow);

#endif //ULA_H
