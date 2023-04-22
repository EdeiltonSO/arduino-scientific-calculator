#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isExpressionValid(char vector[]) {
    return 1;
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
    // content = "vetor de caracteres alocado dinamicamente"
    char content[];
} STACK_ELEMENT;

int main() {
    /*
    char input[] = "-5+35.9+42*56/(74-(5^2+9)*2)-20";

    if (!isExpressionValid(input)) exit(1);

    int pos = 0;
    while (input[pos] != '\0') {
        printf("[%c] ", input[pos]);
        pos++;
    }
    */

    char name[9];
    for (int i = 0; i < 10; i++)
        name[i] = '\0';
    for (int i = 0; i < 10; i++)
        switch (i)
        {
            case 0: name[i] |= (1 << 6) | (1 << 5) | (1 << 2) | (1 << 0);                       break;
            case 1: name[i] |= (1 << 6) | (1 << 5) | (1 << 2);                                  break;
            case 2: name[i] |= (1 << 6) | (1 << 5) | (1 << 2) | (1 << 0);                       break;
            case 3: name[i] |= (1 << 6) | (1 << 5) | (1 << 3) | (1 << 0);                       break;
            case 4: name[i] |= (1 << 6) | (1 << 5) | (1 << 3) | (1 << 2);                       break;
            case 5: name[i] |= (1 << 6) | (1 << 5) | (1 << 4) | (1 << 2);                       break;
            case 6: name[i] |= (1 << 6) | (1 << 5) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0); break;
            case 7: name[i] |= (1 << 6) | (1 << 5) | (1 << 3) | (1 << 2) | (1 << 1);            break;        
            default:                                                                            break;
        }
    int pos = 0;
    while (name[pos] != '\0') {
        printf("%c", name[pos]);
        pos++;
    }

    return 0;
}