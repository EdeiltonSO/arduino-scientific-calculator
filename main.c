#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char flags;
    union {
        char symbol_char;
        double number_double;
        unsigned long int number_int;
    } content;
} EXPRESSION_ELEMENT;

typedef struct {
    char sizeOfList;
    EXPRESSION_ELEMENT* list;
} ELEMENT_LIST;

int hasSyntaxError(char *);
char * addZeroToSpecialCases(char *);
ELEMENT_LIST transformCharToStruct(char *);

void createRPNStack(/* recebe array de structs e retorna ponteiro pra pilha */) {

    // retorna status positivo se a pilha foi preenchida
}

void stackSolver(/* recebe ponteiro pra pilha */) {
    // retorna resultado
}


int main() {
    char input[] = "(-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20)";

    ELEMENT_LIST x = transformCharToStruct("(1.9999999+456)-789*444.5/(1^2)");

    printf("\nNUMERO DE ELEMENTOS: %d\n", x.sizeOfList);

    printf(">>> %d", hasSyntaxError(input));

    return 0;
}

// FUNÇÕES PRONTAS

int hasSyntaxError(char exp[]) {
    printf("%s\n", exp);

    char pos = 1;
    char openedBrackets = 0;
    char dotsOfCurrentNumber = 0;

    if (exp[0] == '*' || exp[0] == '/' || 
        exp[0] == '^' || exp[0] == ')' || 
        exp[0] == '\0') return 1;

    if (exp[0] == '(') openedBrackets++;

    while (exp[pos] != '\0') {
        if (exp[pos] == '.') {
            dotsOfCurrentNumber++;
            if (dotsOfCurrentNumber > 1) return 1;
        }
        else
            if (exp[pos] < '0' || exp[pos] > '9') dotsOfCurrentNumber = 0;

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
            && exp[pos+1] != '(' && exp[pos+1] != '.'
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
            && exp[pos+1] != '(' && exp[pos+1] != '.'
            ) return 1;
        }

        else if (exp[pos] == ')')
        {
            openedBrackets--;
            if ((exp[pos-1] < '0' || exp[pos-1] > '9') && exp[pos-1] != ')'
            || exp[pos+1] != '+' && exp[pos+1] != '-'
            && exp[pos+1] != '*' && exp[pos+1] != '/'
            && exp[pos+1] != '^' && exp[pos+1] != ')'
            && exp[pos+1] != '\0'
            ) return 1;
        }

        if (exp[pos+1] == '\0'
            && (exp[pos] < '0' || exp[pos] > '9')
            && exp[pos] != ')') return 1;

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

ELEMENT_LIST transformCharToStruct(char* exp) {
    int elementListSize = 0;
    EXPRESSION_ELEMENT* elements = (EXPRESSION_ELEMENT*) malloc(sizeof(EXPRESSION_ELEMENT));
    ELEMENT_LIST elementList;

    int pos = 0;
    int currentNumberLength = 1;
    char* currentNumber = (char*) calloc(1, sizeof(char));

    while (exp[pos] != '\0') {
        // .0123456789
        if (exp[pos] >= '0' && exp[pos] <= '9' || exp[pos] == '.') {
            currentNumberLength++;
            currentNumber = (char *) realloc(currentNumber, currentNumberLength);
            currentNumber[currentNumberLength-2] = exp[pos];
            currentNumber[currentNumberLength-1] = '\0';
        }
        else {
            if (currentNumber[0] != '\0') {
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

                if(number.flags & 1 << 7)
                    number.content.number_double = atof(currentNumber);
                else
                    number.content.number_int = atol(currentNumber);
                -
                elementListSize++;
                if (elementListSize > 1) elements = (EXPRESSION_ELEMENT*) 
                    realloc(elements, elementListSize * sizeof(EXPRESSION_ELEMENT));
                elements[elementListSize-1] = number;

                // reseta currentNumber pra proxima iteração
                currentNumberLength = 1;
                currentNumber = (char *) realloc(currentNumber, currentNumberLength);
                currentNumber[0] = '\0';
            }

            if (exp[pos] == '(' || exp[pos] == ')') {
                EXPRESSION_ELEMENT symbol;
                symbol.flags = 1;
                symbol.content.symbol_char = exp[pos];

                elementListSize++;
                elements = (EXPRESSION_ELEMENT*) realloc(elements, elementListSize * sizeof(EXPRESSION_ELEMENT));
                elements[elementListSize-1] = symbol;
            }
            else // +-*/^
            {
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
                elements = (EXPRESSION_ELEMENT*) 
                    realloc(elements, elementListSize * sizeof(EXPRESSION_ELEMENT));
                elements[elementListSize-1] = symbol;
            }
        }
        pos++;        
    }
    free(currentNumber);
    
    elementList.list = elements;
    elementList.sizeOfList = elementListSize;
    return elementList;
}