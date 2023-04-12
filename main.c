#include <stdio.h>
#include <string.h>

int len(char[]);

// typedef struct {
//     int isNumber;
//     int priority;
//     char content[];
// } STACK_ELEMENT;

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

    printf("%d\n", len(input));

    return 0;
}

int len(char vetor[]) {
    return strlen(vetor);
}