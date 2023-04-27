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

        // se for 0~9
        if (expression[pos] >= '0' 
            && expression[pos] <= '9' 
            && (expression[pos-1] == ')' 
                || expression[pos+1] == '('))  // exige asterisco
                return 1;
        
        // se for .
        else if (expression[pos] == '.' 
            && (expression[pos-1] < '0' || expression[pos-1] > '9' 
            || expression[pos+1] < '0' || expression[pos+1] > '9'))
                return 1;


        // se for + ou -
        else if (expression[pos] == '+' || expression[pos] == '-')
            if ((expression[pos-1] < '0' || expression[pos-1] > '9') // anterior != do range 0~9
                && (expression[pos-1] < '(' || expression[pos-1] > ')') // anterior != de ( e )
                && expression[pos-1] != '+' // devo admitir essa bizarrice?
                && expression[pos-1] != '-' // devo admitir essa bizarrice?
                || (expression[pos+1] < '0' || expression[pos+1] > '9') // posterior != do range 0~9
                && (expression[pos+1] < '(' || expression[pos+1] > ')') // posterior != de ( e )
                && expression[pos+1] != '+' // devo admitir essa bizarrice?
                && expression[pos+1] != '-' // devo admitir essa bizarrice?
            )
            return 1;

        // já foi: 0123456789.+-
        // falta: */^()

        // ultimo caractere
        if (expression[pos+1] == '\0'
            && (expression[pos] < '0' || expression[pos] > '9')
            && expression[pos] != ')')return 1;

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
    char input[] = "-5+-+--7--+-+-+++35.9+42*56/(74-(5^2+9)*2)-20";
    //char in[] = "5";
    // if (hasSyntaxError(input)) exit(1);

    printf("\n>>> tem erro? %s\n", hasSyntaxError(input) ? "sim" : "nao");
    //printf("\nascii %i-%i-%i\n", '0', in[0], '9');

    // int pos = 0;
    // while (input[pos] != '\0') {
    //     printf("[%c] ", input[pos]);
    //     pos++;
    // }
    printf("\n");
    return 0;
}