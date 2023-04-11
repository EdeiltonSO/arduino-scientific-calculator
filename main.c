#include <stdio.h>
#include <string.h>

int len(char[]);

typedef struct {
    int isNumber;
    int priority;
    char content[];
} STACK_ELEMENT;

int main() {
    char input[] = "(5+4)*6-8/2";

    printf("%d\n", len(input));

    return 0;
}

int len(char vetor[]) {
    return strlen(vetor);
}