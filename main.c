#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#define ENTRADA "-5+35.9+42*56/(74-(5^2+9)*2)-20"

int hasSyntaxError(char expression[]) {

    // MUDANCA DE PLANOS: NAO VAI TER REGEX
    // isso mesmo vai fazer tudo na unha
    // igual um australopiteco pois LIMITAÇÕES DE MEMORIA
    // que delicia cara

    // ajeitar essa última condição
    if (expression[0] == '.' || 
        expression[0] == '*' ||
        expression[0] == '/' ||
        expression[0] == '^' ||
        expression[0] == ')' ||
        expression[0] == '\0') return 1;

    int pos = 1;
    while (expression[pos] != '\0') {

        // 0123456789. +-*/^()
        if (expression[pos] >= '0' && expression[pos] <= '9')
            if (expression[pos-1] == ')' ||
                expression[pos+1] == '(') return 1; // exige asterisco
        
        else if (expression[pos] == '.')
            if (expression[pos-1] < '0' &&
                expression[pos+1] > '9') return 1;
        
        else if (expression[pos] == '+' || expression[pos] == '-') {}

        // ... * / ^ ( )

        // ULTIMO CARACTERE DA EXPRESSAO
        // ---------0123456789----------)----------
        else if (expression[pos+1] == '\0')
            if (!(expression[pos] < '0' && // 
                expression[pos] > '9' && // 
                expression[pos] == ')')) return 1; // 
            // dar return 1 se for < 0 e > 9 e nao for ) no ultimo caractere

            // deu sono aqui viu valeu fui até mais
        pos++;
    }

    return 0;
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
    char in[] = "5";
    // if (hasSyntaxError(input)) exit(1);

    printf("\n>>> tem erro? %s\n", hasSyntaxError(input) ? "sim" : "nao");
    printf("\nascii %i-%i-%i\n", '0', in[0], '9');

    // int pos = 0;
    // while (input[pos] != '\0') {
    //     printf("[%c] ", input[pos]);
    //     pos++;
    // }
    printf("\n");
    return 0;
}