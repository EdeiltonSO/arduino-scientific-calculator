#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define ENTRADA "-5+35.9+42*56/(74-(5^2+9)*2)-20"
// 0123456789.+-*/^()

int isExpressionValid(char expression[]) {
    regex_t rgx;
    char notFirst[] = "[.*\/\^)]";
    int statusFirstPos = regcomp(&rgx, notFirst, 0);
    printf("\nstatusFirstPos: %i\n", statusFirstPos);
    int regexecReturn = regexec(&regex, expression, 0, NULL, 0);
    printf("\nregexecReturn: %i\n", regexecReturn);

    // SUBSTITUIR PELA REGEX ACIMA
    // if (expression[0] == '.' ||
    //     expression[0] == '*' ||
    //     expression[0] == '/' ||
    //     expression[0] == '^' ||
    //     expression[0] == ')') return 0; 

    // int pos = 0;
    // while (expression[pos] != '\0') {
        
    //     pos++;
    // }

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
    // content = "vetor de caracteres com tamanho alocado dinamicamente"
    char content[];
} STACK_ELEMENT;

int main() {
    char input[] = "-5+35.9+42*56/(74-(5^2+9)*2)-20";

    if (!isExpressionValid(input)) exit(1);

    int pos = 0;
    while (input[pos] != '\0') {
        printf("[%c] ", input[pos]);
        pos++;
    }
    return 0;
}