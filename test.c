#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * addZeroToSpecialCases(char exp[], char newExp[]) {
    printf("\nNEWEXP:");
    printf("\n>>> conteudo: %s", newExp);
    printf("\n>>> tamanho em bytes do endereco: %i", sizeof(newExp));
    printf("\n>>> tamanho em bytes do conteudo em newExp[0]: %i", sizeof((*newExp)));
    printf("\n>>> comprimento da string iniciada em newExp[0]: %i\n\n", strlen(newExp));

    printf("\n>>> tamanho em bytes da string inteira (w/ null): %i", sizeof(newExp)*sizeof(char));

    if (exp[0] == '.' || exp[0] == '+' || exp[0] == '-')
        printf("0%c", exp[0]);

    int pos = 1;
    while (exp[pos] != '\0') {
        if (((exp[pos] == '+' || exp[pos] == '-') 
        && (exp[pos-1] < '0' || exp[pos-1] > '9') && exp[pos-1] != ')')
        || (exp[pos] == '.' && (exp[pos-1] < '0' || exp[pos-1] > '9')))
        {
            printf("0%c", exp[pos]);
            // aloca em newExp o espaço de um char
            // adiciona '0' nesse espaço
            // aloca em newExp o espaço de um char
            // adiciona exp[0] nesse espaço
        }
        else
        {
            printf("%c", exp[pos]);
            // aloca em newExp o espaço de um char
            // adiciona exp[0] nesse espaço
        }
        pos++;
    }

    return newExp;
}

int main() {
    // if (hasSyntaxError(input)) exit(1);

    char input[] = "-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20";
    char newExp[] = "qwerty";

    printf(">>> tamanho: %i bytes (com char null)\n", sizeof(input)*sizeof(char));

    printf(input);
    printf("\n");

    addZeroToSpecialCases(input, newExp);

    printf("\n");
    return 0;
}