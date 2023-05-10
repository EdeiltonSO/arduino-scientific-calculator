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

char * addZeroToSpecialCases(char exp[]) {
    int newExpSize = 1;
    char* newExp = (char*) calloc(1, sizeof(char));

    char* test = (char*) calloc(1, sizeof(char));
    test = (char *) realloc(test, 6);
    test[0] = 'a';
    test[1] = 'l';
    test[2] = 'o';
    test[3] = 'w';
    test[4] = '!';
    test[5] = '\0';

    printf("%s\n", exp);
    //printf("\ntest: %s\n", test);

    if (exp[0] == '.' || exp[0] == '+' || exp[0] == '-')
    {
        printf("%c%c", '0', exp[0]);
        // newExp = (char *) realloc(newExp, newExpSize+2);
        // newExp[0] = '0';
        // newExp[1] = exp[0];
        // newExp[2] = '\0';
        // newExpSize += 2;
        // printf("\n-: %c", exp[0]);
    }
    else {
        printf("%c", exp[0]);
        // newExp = (char *) realloc(newExp, newExpSize+1);
        // newExp[0] = exp[0];
        // newExp[1] = '\0';
        // newExpSize += 1;
        // printf("\n : %c", exp[0]);
    }

    int pos = 1;
    while (exp[pos] != '\0') {
        if (((exp[pos] == '+' || exp[pos] == '-') 
        && (exp[pos-1] < '0' || exp[pos-1] > '9') && exp[pos-1] != ')')
        || (exp[pos] == '.' && (exp[pos-1] < '0' || exp[pos-1] > '9')))
        {
            printf("%c%c", '0', exp[pos]);
            // aloca em newExp o espaço de um char
            // adiciona '0' nesse espaço
            // aloca em newExp o espaço de um char
            // adiciona exp[0] nesse espaço

            // newExp = (char *) realloc(newExp, newExpSize+2);
            // newExp[newExpSize] = '0';
            // newExp[newExpSize+1] = exp[pos];
            // newExp[newExpSize+2] = '\0';
            // newExpSize += 2;
            // printf("\n-: %c", exp[pos]);
        }
        else
        {
            printf("%c", exp[pos]);
            // aloca em newExp o espaço de um char
            // adiciona exp[0] nesse espaço

            // newExp = (char *) realloc(newExp, newExpSize+1);
            // newExp[newExpSize] = exp[pos];
            // newExp[newExpSize+1] = '\0';
            // newExpSize += 1;
            // printf("\n : %c", exp[pos]);
        }
        pos++;
    }

    return test;
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
    char * newExp = addZeroToSpecialCases(input);
    //printf("\nANTES:  %s\nDEPOIS: %s\n\n", input, newExp);

    // char * newExp = addZeroToSpecialCases(input);
    // printf("\nANTES:  %s\nDEPOIS: %s\n\n", input, newExp);

    return 0;
}