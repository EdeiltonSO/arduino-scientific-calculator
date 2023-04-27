#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENTRADA "-5+35.9+42*56/(74-(5^2+9)*2)-20"
// 0123456789.+-*/^()

int hasSyntaxError(char expression[]) {

    // MUDANCA DE PLANOS: NAO VAI TER REGEX
    // isso mesmo vai fazer tudo na unha
    // igual um australopiteco pois LIMITAÇÕES DE MEMORIA
    // que delicia cara

    int pos = 0;
    while (expression[pos] != '\0') {
        
        pos++;
    }

    return 0;
}

void addZeroToSpecialCases(char vector[]) {

}

void transformCharToStruct(char vector) {

}

void createRPNStack(/* recebe ponteiro pra pilha e array de structs */) {
    // retorna status positivo se a pilha foi preenchida
}

void stackSolver(/* recebe ponteiro pra pilha */) {
    // retorna resultado
}

typedef struct {
    // flags = [00000000]
    // flags[7]: isNumber
    // flags[1:0]: priority
    char flags;
    // content = "vetor de caracteres com tamanho alocado dinamicamente"
    char content[];
} STACK_ELEMENT;

int main() {
    char input[] = "-5+35.9+42*56/(74-(5^2+9)*2)-20";

    if (hasSyntaxError(input)) exit(1);

    int pos = 0;
    while (input[pos] != '\0') {
        printf("[%c] ", input[pos]);
        pos++;
    }
    printf("\n");
    return 0;
}