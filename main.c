#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define ENTRADA "-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20"

typedef struct EXPRESSION_ELEMENT {
    unsigned char flags; // [0 0000 000] => is_decimal[7] <not_used>[6:3] priority[2:0]
    union {
        unsigned char symbol_char;
        double number_double;
        int number_int;
    } content;
} EXPRESSION_ELEMENT;

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

void transformCharToStruct(char* exp) {

    int elementListSize = 0;
    EXPRESSION_ELEMENT* elementList = (EXPRESSION_ELEMENT*) malloc(sizeof(EXPRESSION_ELEMENT));

    int pos = 0;
    int currentNumberLength = 1;
    char* currentNumber = (char*) malloc(sizeof(char));

    while (exp[pos] != '\0') {
        // .0123456789
        if (exp[pos] >= '0' && exp[pos] <= '9' || exp[pos] == '.') {
            currentNumberLength++;
            currentNumber = (char *) realloc(currentNumber, currentNumberLength);
            currentNumber[currentNumberLength-2] = exp[pos];
            currentNumber[currentNumberLength-1] = '\0';
        }
        // ()
        else if (exp[pos] == '(' || exp[pos] == ')') {
            EXPRESSION_ELEMENT symbol;
            symbol.flags = 1;
            symbol.content.symbol_char = exp[pos];

            elementListSize++;
            elementList = (EXPRESSION_ELEMENT*) realloc(elementList, elementListSize * sizeof(EXPRESSION_ELEMENT));
            elementList[elementListSize-1] = symbol;

        }
        // +-*/^
        else if (exp[pos] == '(' || exp[pos] == ')')
        {
            printf("\n>>> %s", currentNumber);
            printf("\n>>> %c", exp[pos]);

            // NUMBER

            EXPRESSION_ELEMENT number;

            number.flags = 0;
            int i = 0;
            while (currentNumber[i] != '\0')
            {
                if (currentNumber[i] == '.')
                {
                    number.flags = 0b10000000;
                    break;
                }
                i++;
            }

            if(number.flags & 1 << 7) {
                printf("\neh decimal");
                // converter de string pra double
                number.content.number_double = 0.0;
            }
            else {
                printf("\nnao eh decimal");
                // converter de string pra int
                number.content.number_int = 0;
            }
            
            elementListSize++;

            if (elementListSize > 1) elementList = (EXPRESSION_ELEMENT*) 
                realloc(elementList, elementListSize * sizeof(EXPRESSION_ELEMENT));
            elementList[elementListSize-1] = number;

            // reseta currentNumber pra proxima iteração
            currentNumberLength = 1;
            currentNumber = (char *) realloc(currentNumber, currentNumberLength);
            currentNumber[0] = '\0';

            // SYMBOL

            EXPRESSION_ELEMENT symbol;

            symbol.flags = 0;
            symbol.content.symbol_char = exp[pos];
            
            switch (exp[pos])
            {
            case '^':
                symbol.flags |= 1 << 1;
                break;
            case '*':
            case '/':
                symbol.flags |= 1 << 1;
                symbol.flags |= 1 << 0;
                break;
            default:
                symbol.flags |= 1 << 2;
                break;
            }

            elementListSize++;
            elementList = (EXPRESSION_ELEMENT*) 
                realloc(elementList, elementListSize * sizeof(EXPRESSION_ELEMENT));
            elementList[elementListSize-1] = symbol;
        }
        pos++;        
    }

    // se sobrar
    if (currentNumber[0] != '\0')
    {
        printf("\n>>> %s", currentNumber);
        EXPRESSION_ELEMENT number;
        number.flags = 0;

        int i = 0;
        while (currentNumber[i] != '\0')
        {
            if (currentNumber[i] == '.')
            {
                number.flags = 0x80;
                break;
            }
            i++;
        }

        if(number.flags & 1 << 7) {
            printf("\neh decimal");
            // converter de string pra double
            number.content.number_double = 0.0;
        }
        else {
            printf("\nnao eh decimal");
            // converter de string pra int
            number.content.number_int = 0;
        }
        
        elementListSize++;
        if (elementListSize > 1) elementList = (EXPRESSION_ELEMENT*) 
            realloc(elementList, elementListSize * sizeof(EXPRESSION_ELEMENT));
        elementList[elementListSize-1] = number;

        // reseta currentNumber pra proxima iteração
        currentNumberLength = 1;
        currentNumber = (char *) realloc(currentNumber, currentNumberLength);
        currentNumber[0] = '\0';
    }


    free(currentNumber);

    printf("\n-----\n");
    for (int i = 0; i < elementListSize; i++)
    {
        printf("%d ", elementList[i].flags);
        
        // if (elementList[i].flags == 0)
        //     printf("[i:%d]", elementList[i].content.number_int);
        // else if (elementList[i].flags == 0x80)
        //     printf("[f:%f]", elementList[i].content.number_double);
        // else
        //     printf("[c:%c]", elementList[i].content.symbol_char);
    }
    printf("\n-----\n");
    
}

void createRPNStack(/* recebe ponteiro pra pilha e array de structs */) {
    // retorna status positivo se a pilha foi preenchida
}

void stackSolver(/* recebe ponteiro pra pilha */) {
    // retorna resultado
}


int main() {
    char input[] = "-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20";
    char test[] = "0.5";

    char zero = '7';
    transformCharToStruct("9345.67+124*(444.5)");
    //printf("\n%d", zero-48);
    //printf(">>> %d", sizeof(EXPRESSION_ELEMENT));

    //printf("\nENTRADA: %s", input);
    //printf("\nSAIDA:   %s\n\n", addZeroToSpecialCases(input));

    return 0;
}