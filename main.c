#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char RPNExpSize;
    char symbolStackSize;
    EXPRESSION_ELEMENT* list;
} ELEMENT_LIST;

typedef struct {
    char size;
    char* values;
} ARRAY;

int hasSyntaxError(char *);
void addZeroToSpecialCases(char *, ARRAY *);
ELEMENT_LIST transformCharToStruct(char *);

void createRPNStack(ELEMENT_LIST input, EXPRESSION_ELEMENT* output) {
    printf("tamanho da lista: %i\ntamanho da pilha: %i\n", input.size, input.RPNExpSize);

    EXPRESSION_ELEMENT symbolStack[input.symbolStackSize];
    char symbolStackSize = 0;
    char outputSize = 0;

    for (int pos = 0; pos < input.size; pos++)
    {
        EXPRESSION_ELEMENT element = input.list[pos];
        unsigned char flags = element.flags;

        // trocar essa e outras condicionais por diretivas define
        if(!(flags | 0 << 2) && !(flags | 0 << 1) && !(flags | 0 << 0))
        {
            // processa numeros conforme lógica da RPN
        }
        else
        {
            // processa simbolo conforme lógica da RPN usando symbolStack
        }
    }

    // pegar o último elemento do vetor output e add uma flag
    // informando que aquele é o último elemento da lista
    // a próxima função usa esse vetor e itera nele
    // até detectar essa flag
}

void stackSolver(/* recebe ponteiro pra pilha */) {
    // retorna resultado
}

int main() {
    char input2[] = "(-.5+35.9+42^56/(-(-74-(+5^2+9)*2.123456789123456789))-20)";
    char input[] = "-3.5*15/(3+2)^2-1";
    ARRAY expWithZeros;

    printf("\ninput:         %s", input);
    addZeroToSpecialCases(input, &expWithZeros);
    printf("\nstrlen: %d, size: %d", strlen(expWithZeros.values), expWithZeros.size);
    printf("\nwith zeros:    %s", expWithZeros.values);
    
    ELEMENT_LIST structuredExp = transformCharToStruct(expWithZeros.values);
    printf("\nstructuredExp: ");
    for (int i = 0; i < structuredExp.size; i++)
    {
        EXPRESSION_ELEMENT element = structuredExp.list[i];
        unsigned char flags = element.flags;

        if(flags & 1 << 7) printf("%f ", element.content.number_double);
        else if (!(flags | 0)) printf("%d ", element.content.number_int);
        else printf("%c ", element.content.symbol_char);
    }
    
    // EXPRESSION_ELEMENT stack[structuredExp.RPNExpSize];
    // createRPNStack(structuredExp, stack);

    // printf("expressao original: %s\n", input);
    // printf("tamanho da expressao original: %d\n", structuredExp.size);
    // printf("tamanho da expressao em RPN:   %d\n", structuredExp.RPNExpSize);
    // printf("tamanho da pilha de simbolos:  %d\n", structuredExp.symbolStackSize);

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

void addZeroToSpecialCases(char exp[], ARRAY *expWithZeros) {

    char zerosToAdd = 0;
    char inputSize = 1;
    char pos = 1;
    (*expWithZeros).size = 0;

    if (exp[0] == '.' || exp[0] == '+' || exp[0] == '-') 
        zerosToAdd++;

    while (exp[pos] != '\0') {
        if ((exp[pos] == '.' || exp[pos] == '+' || exp[pos] == '-') && (exp[pos-1] < '0' || exp[pos-1] > '9')) {
            zerosToAdd++;
        }
        inputSize++;
        pos++;
    }

    (*expWithZeros).values[inputSize+zerosToAdd];
    (*expWithZeros).values[inputSize+zerosToAdd] = '\0';

    if (exp[0] == '.' || exp[0] == '+' || exp[0] == '-')
    {
        (*expWithZeros).values[0] = '0';
        (*expWithZeros).values[1] = exp[0];
        (*expWithZeros).size += 2;
    }
    else (*expWithZeros).values[(*expWithZeros).size++] = exp[0];

    pos = 1;

    while (exp[pos] != '\0') {
        if (((exp[pos] == '+' || exp[pos] == '-') 
        && (exp[pos-1] < '0' || exp[pos-1] > '9') && exp[pos-1] != ')')
        || (exp[pos] == '.' && (exp[pos-1] < '0' || exp[pos-1] > '9')))
        {
            (*expWithZeros).values[(*expWithZeros).size++] = '0';
            (*expWithZeros).values[(*expWithZeros).size++] = exp[pos];
        }
        else (*expWithZeros).values[(*expWithZeros).size++] = exp[pos];
        pos++;
    }
}

ELEMENT_LIST transformCharToStruct(char* exp) {
    ELEMENT_LIST elementList;
    elementList.list = malloc(sizeof(EXPRESSION_ELEMENT));
    elementList.symbolStackSize = 0;
    elementList.RPNExpSize = 0;
    elementList.size = 0;

    int pos = 0;
    int currentNumberLength = 0;
    char* currentNumber = calloc(1, sizeof(char));

    while (exp[pos] != '\0') {
        // .0123456789
        if (exp[pos] >= '0' && exp[pos] <= '9' || exp[pos] == '.') {
            currentNumberLength++; // 2
            currentNumber = realloc(currentNumber, currentNumberLength+1);
            currentNumber[currentNumberLength-1] = exp[pos];
            currentNumber[currentNumberLength] = '\0';

            // se pos+1 não for numérico
            if(!(exp[pos+1] >= '0' && exp[pos+1] <= '9' || exp[pos+1] == '.')) {
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
                elementList.RPNExpSize++;

                if (elementList.size > 1) elementList.list = realloc(elementList.list, elementList.size * sizeof(EXPRESSION_ELEMENT));
                elementList.list[elementList.size-1] = number;

                // reseta currentNumber pra proxima iteração
                currentNumberLength = 0;
                currentNumber = realloc(currentNumber, currentNumberLength+1);
                currentNumber[0] = '\0';
            }
        }
        else {
            if (exp[pos] == '(' || exp[pos] == ')') {
                EXPRESSION_ELEMENT symbol;
                symbol.flags = 0b00000100;
                symbol.content.symbol_char = exp[pos];

                elementList.size++;
                elementList.list = realloc(elementList.list, elementList.size * sizeof(EXPRESSION_ELEMENT));
                elementList.list[elementList.size-1] = symbol;

                if(exp[pos] == '(') elementList.symbolStackSize++;
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
                    symbol.flags |= 1 << 0;
                    break;
                case '*':
                case '/':
                    symbol.flags |= 1 << 1;
                    break;
                default:
                    symbol.flags |= 1 << 0;
                    break;
                }

                elementList.size++;
                elementList.RPNExpSize++;
                elementList.symbolStackSize++;
                elementList.list = realloc(elementList.list, elementList.size * sizeof(EXPRESSION_ELEMENT));
                elementList.list[elementList.size-1] = symbol;
            }
        }
        pos++;        
    }
    free(currentNumber);
    return elementList;
}