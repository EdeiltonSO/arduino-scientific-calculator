#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define ENTRADA "-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20"

typedef struct EXPRESSION_ELEMENT {
    char flags; // [0 0000 000] => [is_decimal 0000 priority]
    union {
        char op;
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

    EXPRESSION_ELEMENT* elementList = (EXPRESSION_ELEMENT*) calloc(1, sizeof(EXPRESSION_ELEMENT));

    int pos = 0;
    int elementListQtd = 0;
    int sizeTemp = 1;
    char* temp = (char*) calloc(sizeTemp, sizeof(char));
    while (exp[pos] != '\0')
    {
        // .0123456789
        if (exp[pos] >= '0' && exp[pos] <= '9' || exp[pos] == '.') {
            sizeTemp++;
            temp = (char *) realloc(temp, sizeTemp);
            temp[sizeTemp-2] = exp[pos];
            temp[sizeTemp-1] = '\0';
        }
        // +-*/^()
        else
        {
            EXPRESSION_ELEMENT number;
            EXPRESSION_ELEMENT symbol;

            // NUMBER
            number.flags = 0b00000000;
            int i = 0;
            while (temp[i] != '\0')
            {
                if (temp[i] == '.')
                {
                    printf("aaaaaaaaaaa");
                    number.flags = 0b10000000;
                    break;
                }
                i++;
            }

            if(number.flags |= (1 << 7)) {
                printf("\neh decimal");
            }
            else {
                printf("\nnao eh decimal");
            }

            /*
            if (number.decimal == 0b1)
            {
                number.content.number_double = temp;
            }
            else {
                number.content.number_int = temp;
            }
            */
            
            
            // libera a memória de temp a partir do segundo elemento
            sizeTemp = 1;
            temp = (char *) realloc(temp, sizeTemp);
            temp[0] = '\0';

            // SYMBOL (daqui pra baio ta supostamente ok)

            // symbol.decimal = 0;
            // symbol.content.op = exp[pos];
            // mudar p/ switch case

            // if (exp[pos] == '(' || exp[pos] == ')')
            //     symbol.priority = 1;
            // else if (exp[pos] == '^')
            //     symbol.priority = 2;
            // else if (exp[pos] == '*' || exp[pos] == '/')
            //     symbol.priority = 3;
            // else
            //     symbol.priority = 4;

            // alocar memoria e add os dois EXPRESSION_ELEMENT na elementList

        }
        
        pos++;
    }
    
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
    transformCharToStruct("9+5");
    //printf("\n%d", zero-48);
    //printf(">>> %d", sizeof(EXPRESSION_ELEMENT));

    //printf("\nENTRADA: %s", input);
    //printf("\nSAIDA:   %s\n\n", addZeroToSpecialCases(input));

    return 0;
}