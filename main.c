#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isExpressionValid(char vetor[]) {
    int expressionLength = strlen(vetor);
    for (int i = 0; i < expressionLength; i++)
        printf("[%c] ", vetor[i]);
    return 1;
}

void addZeroToSpecialCases(char vetor[]) {
    //return
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
    char input[] = "-5+35.9+42*56/(74-(5^2+9)*2)-20";

    if (!isExpressionValid(input)) exit(1);

    printf("%s\n", input[4]);
    printf("%s\n\n", isExpressionValid(input));

    return 0;
}