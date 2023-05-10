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

    if (exp[0] == '.' || exp[0] == '+' || exp[0] == '-')
    {
        newExp = (char *) realloc(newExp, newExpSize+2);
        newExp[0] = '0';
        newExp[1] = exp[0];
        newExp[2] = '\0';
        newExpSize += 2;
    }
    else {
        newExp = (char *) realloc(newExp, newExpSize+1);
        newExp[0] = exp[0];
        newExp[1] = '\0';
        newExpSize += 1;
    }

    int pos = 1;
    while (exp[pos] != '\0') {
        if (((exp[pos] == '+' || exp[pos] == '-') 
        && (exp[pos-1] < '0' || exp[pos-1] > '9') && exp[pos-1] != ')')
        || (exp[pos] == '.' && (exp[pos-1] < '0' || exp[pos-1] > '9')))
        {
            newExp = (char *) realloc(newExp, newExpSize+2);
            newExp[newExpSize-1] = '0';
            newExp[newExpSize] = exp[pos];
            newExp[newExpSize+1] = '\0';
            newExpSize += 2;
        }
        else 
        {
            newExp = (char *) realloc(newExp, newExpSize+1);
            newExp[newExpSize-1] = exp[pos];
            newExp[newExpSize] = '\0';
            newExpSize += 1;
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
    printf("\nENTRADA: %s", input);
    printf("\nSAIDA:   %s\n\n", addZeroToSpecialCases(input));

    return 0;
}