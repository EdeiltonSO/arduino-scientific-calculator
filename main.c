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
    char size;
    char stackSize;
    EXPRESSION_ELEMENT* list;
} ELEMENT_LIST;

int hasSyntaxError(char *);
char * addZeroToSpecialCases(char *);
ELEMENT_LIST transformCharToStruct(char *);

void createRPNStack(ELEMENT_LIST elementList) {
    printf("tamanho da lista: %i\ntamanho da pilha: %i\n", elementList.size, elementList.stackSize);

    for (int i = 0; i < elementList.size; i++)
    {
        EXPRESSION_ELEMENT element = elementList.list[i];
        unsigned char flags = element.flags;

        if(flags & 1 << 7)
            printf("%lf\n", element.content.number_double);
        else if (!(flags | 0))
            printf("%d\n", element.content.number_int);
        else 
            printf("%c\n", element.content.symbol_char);

    }
}

void stackSolver(/* recebe ponteiro pra pilha */) {
    // retorna resultado
}

int main() {
    char input[] = "(-.5+35.9+42^56/(-(-74-(+5^2+9)*2.123456789123456789123456789))-20)";

    ELEMENT_LIST x = transformCharToStruct(input);
    createRPNStack(x);

    return 0;
}

// FUNÇÕES PRONTAS

int hasSyntaxError(char exp[]) {
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
    char* newExp = calloc(1, sizeof(char));

    if (exp[0] == '.' || exp[0] == '+' || exp[0] == '-')
    {
        newExp = realloc(newExp, newExpSize+2);
        newExp[0] = '0';
        newExp[1] = exp[0];
        newExp[2] = '\0';
        newExpSize += 2;
    }
    else {
        newExp = realloc(newExp, newExpSize+1);
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
            newExp = realloc(newExp, newExpSize+2);
            newExp[newExpSize-1] = '0';
            newExp[newExpSize] = exp[pos];
            newExp[newExpSize+1] = '\0';
            newExpSize += 2;
        }
        else 
        {
            newExp = realloc(newExp, newExpSize+1);
            newExp[newExpSize-1] = exp[pos];
            newExp[newExpSize] = '\0';
            newExpSize += 1;
        }
        pos++;
    }
    return newExp;
}

ELEMENT_LIST transformCharToStruct(char* exp) {
    ELEMENT_LIST elementList;
    elementList.list = malloc(sizeof(EXPRESSION_ELEMENT));
    elementList.stackSize = 0;
    elementList.size = 0;

    int pos = 0;
    int currentNumberLength = 1;
    char* currentNumber = calloc(1, sizeof(char));

    while (exp[pos] != '\0') {
        // .0123456789
        if (exp[pos] >= '0' && exp[pos] <= '9' || exp[pos] == '.') {
            currentNumberLength++;
            currentNumber = realloc(currentNumber, currentNumberLength);
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

                elementList.size++;
                elementList.stackSize++;

                if (elementList.size > 1) elementList.list = realloc(elementList.list, elementList.size * sizeof(EXPRESSION_ELEMENT));
                elementList.list[elementList.size-1] = number;

                // reseta currentNumber pra proxima iteração
                currentNumberLength = 1;
                currentNumber = realloc(currentNumber, currentNumberLength);
                currentNumber[0] = '\0';
            }

            if (exp[pos] == '(' || exp[pos] == ')') {
                EXPRESSION_ELEMENT symbol;
                symbol.flags = 1;
                symbol.content.symbol_char = exp[pos];

                elementList.size++;
                elementList.list = realloc(elementList.list, elementList.size * sizeof(EXPRESSION_ELEMENT));
                elementList.list[elementList.size-1] = symbol;
            }
            else // +-*/^
            {
                EXPRESSION_ELEMENT symbol;

                symbol.flags = 0;
                symbol.content.symbol_char = exp[pos];
                elementList.stackSize++;
                
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

                elementList.size++;
                elementList.list = realloc(elementList.list, elementList.size * sizeof(EXPRESSION_ELEMENT));
                elementList.list[elementList.size-1] = symbol;
            }
        }
        pos++;        
    }
    free(currentNumber);
    return elementList;
}