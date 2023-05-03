#include <stdio.h>
#include <stdlib.h>

//#define ENTRADA "-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20"

int hasSyntaxError(char exp[]) {

    if (exp[0] == '*' || exp[0] == '/' || 
        exp[0] == '^' || exp[0] == ')' || 
        exp[0] == '\0') return 1;

    int pos = 1;
    int openedBrackets = 0;
    while (exp[pos] != '\0') {

        if (exp[pos] >= '0' && exp[pos] <= '9' &&
        (exp[pos-1] == ')' || exp[pos+1] == '(')
        ) return 1;
        
        else if (exp[pos] == '.'
        && (exp[pos+1] < '0' || exp[pos+1] > '9')
        ) return 1;

        else if (exp[pos] == '+' || exp[pos] == '-')
        {
            if ((exp[pos-1] < '0' || exp[pos-1] > '9')
            && (exp[pos-1] < '(' || exp[pos-1] > ')')
            || (exp[pos+1] < '0' || exp[pos+1] > '9')
            && (exp[pos+1] != '(')
            ) return 1;
        }

        else if (exp[pos] == '*' || exp[pos] == '/' || exp[pos] == '^')
        {
            if ((exp[pos-1] < '0' || exp[pos-1] > '9')
            && (exp[pos-1] != ')')
            || (exp[pos+1] < '0' || exp[pos+1] > '9')
            && (exp[pos+1] != '(')
            ) return 1;
        }

        else if (exp[pos] == '(')
        {
            openedBrackets++;
            if (exp[pos-1] != '+' && exp[pos-1] != '-'
            && exp[pos-1] != '*' && exp[pos-1] != '/'
            && exp[pos-1] != '^' && exp[pos-1] != '('
            || (exp[pos+1] < '0' || exp[pos+1] > '9')
            && exp[pos+1] != '+' && exp[pos+1] != '-'
            && exp[pos+1] != '('
            ) return 1;
        }

        else if (exp[pos] == ')')
        {
            openedBrackets--;
            if ((exp[pos-1] < '0' || exp[pos-1] > '9') && exp[pos-1] != ')'
            || exp[pos+1] != '+' && exp[pos+1] != '-'
            && exp[pos+1] != '*' && exp[pos+1] != '/'
            && exp[pos+1] != '^' && exp[pos+1] != ')'
            ) return 1;
        }

        if (exp[pos+1] == '\0'
            && (exp[pos] < '0' || exp[pos] > '9')
            && exp[pos] != ')')return 1;

        pos++;
    }

    return openedBrackets ? 1 : 0;
}

char * addZeroToSpecialCases(char exp[], char newExp[]) {
    printf("\n>>> tamanho do endereco de newExp: %i bytes", sizeof(newExp));
    printf("\n>>> tamanho do conteudo de newExp: %i bytes\n\n", sizeof((*newExp)));

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
    // if (hasSyntaxError(input)) exit(1);

    char input[] = "-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20";
    char newExp[] = "a";

    printf("\n>>> tem erro? %s\n", hasSyntaxError(input) ? "sim" : "nao");
    printf(">>> tamanho: %i bytes (com char null)\n", sizeof(input)*sizeof(char));

    printf(input);
    printf("\n");

    addZeroToSpecialCases(input, newExp);

    // printf(">>> tamanho do retorno: %i bytes\n", sizeof());

    // int pos = 0;
    // while (input[pos] != '\0') {
    //     printf("[%c] ", input[pos]);
    //     pos++;
    // }
    printf("\n");
    return 0;
}