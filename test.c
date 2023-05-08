#include <stdio.h>
#include <stdlib.h>

int main() {
    int size = 1;
    char* exp = (char*) calloc(size, sizeof(char));

    printf("ANTES:");
    if (exp[0] == '\0')
        printf("\nta zerado: %c", exp[0]);
    else
        printf("\nnao ta zerado: %c", exp[0]);
    
    printf("\nDEPOIS:");
    exp = (char *) realloc(exp, size+1);
    exp[0] = '0';
    exp[1] = '\n';
    size++;

    printf("\n>>> exp[0]: %c\n>>> exp[1]: %c", exp[0], exp[1]);

    return 0;
}