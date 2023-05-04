#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    if (exp[0] == '.' || exp[0] == '+' || exp[0] == '-')
    {
        printf("0%c", exp[0]);
        // aloca em newExp o espaço de um char
        // adiciona '0' nesse espaço
        // aloca em newExp o espaço de um char
        // adiciona exp[0] nesse espaço
    }

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
    char input[] = "-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20";
    char newExp[] = "qwerty";

    addZeroToSpecialCases(input, newExp);

    printf("\n");
    return 0;
}