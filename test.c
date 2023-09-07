#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#define  INT_FLOAT   ((numStack[topStack-1].flags & 1 << 7) == 0x00) && ((numStack[topStack].flags & 1 << 7) == 0x80)
#define  FLOAT_INT   ((numStack[topStack-1].flags & 1 << 7) == 0x80) && ((numStack[topStack].flags & 1 << 7) == 0x00)
#define  FLOAT_FLOAT ((numStack[topStack-1].flags & 1 << 7) == 0x80) && ((numStack[topStack].flags & 1 << 7) == 0x80)
#define  IS_DIVISION rpnStack[pos].content.symbol_char == '/'

typedef struct {
    unsigned char flags;
    union {
        char symbol_char;
        double number_double;
        long long int number_int;
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

void printElementList(ELEMENT_LIST elementList) {
    for (int i = 0; i < elementList.size; i++)
    {
        EXPRESSION_ELEMENT element = elementList.list[i];
        unsigned char flags = element.flags;

        if(flags & 1 << 7) printf("%f ", element.content.number_double);
        else if (!(flags | 0)) printf("%d ", element.content.number_int);
        else printf("%c ", element.content.symbol_char);
    }
}
void printExpElementArray(EXPRESSION_ELEMENT* rpnStack, int rpnSize) {
    unsigned char flags;
    EXPRESSION_ELEMENT element;
    for (int i = 0; i < rpnSize; i++)
    {
        element = rpnStack[i];
        flags = element.flags;

        if(flags & 1 << 7) printf("%f ", element.content.number_double);
        else if (!(flags | 0)) printf("%i ", element.content.number_int);
        else printf("%c ", element.content.symbol_char);
    }
}
void printExpElement(EXPRESSION_ELEMENT element) {
    unsigned char flags;
    flags = element.flags;
    if(flags & 1 << 7) printf("%f", element.content.number_double);
    else if (!(flags | 0)) printf("%i", element.content.number_int);
    else printf("%c", element.content.symbol_char);
}

EXPRESSION_ELEMENT sum(EXPRESSION_ELEMENT a, EXPRESSION_ELEMENT b) {
    // validar limite de representação
    // verificar tipo
    // operar
    // retornar
    EXPRESSION_ELEMENT r;
    r.flags = 0;
    r.content.number_int = a.content.number_int + b.content.number_int;
    return r;
}
EXPRESSION_ELEMENT subtraction(EXPRESSION_ELEMENT a, EXPRESSION_ELEMENT b) {
    EXPRESSION_ELEMENT r;
    r.flags = 0;
    r.content.number_int = a.content.number_int - b.content.number_int;
    return r;
}
EXPRESSION_ELEMENT multiplication(EXPRESSION_ELEMENT a, EXPRESSION_ELEMENT b) {
    EXPRESSION_ELEMENT r;
    r.flags = 0;
    r.content.number_int = a.content.number_int * b.content.number_int;
    return r;
}
// EXPRESSION_ELEMENT division(EXPRESSION_ELEMENT num, EXPRESSION_ELEMENT den);
// EXPRESSION_ELEMENT power(EXPRESSION_ELEMENT base, EXPRESSION_ELEMENT exponent);

int hasSyntaxError(char *);
void addZeroToSpecialCases(char[], ARRAY *);
ELEMENT_LIST transformCharToStruct(char[]);
void createRPNStack(ELEMENT_LIST, EXPRESSION_ELEMENT*);

EXPRESSION_ELEMENT RPNStackSolver(EXPRESSION_ELEMENT rpnStack[]) {
    char pos = 0;
    char topStack = -1;

    // dimensionando e criando a pilha de operandos
    do { if ((rpnStack[pos].flags & 0b00000111) == 0) topStack++;
    } while ((rpnStack[pos++].flags & 1 << 6) != 0b01000000);
    EXPRESSION_ELEMENT numStack[++topStack];

    // usando a pilha e resolvendo a RPN
    pos = 0;
    topStack = -1;
    do {
        // se for numero, joga na pilha
        if ((rpnStack[pos].flags & 0b00000111) == 0) {
            printf("\nnumero ");
            printExpElement(rpnStack[pos]);
            printf(" foi pra pilha");
            numStack[++topStack] = rpnStack[pos];
        }
        // se for operador
        else {
            // retire o penúltimo e o último operando da pilha
            // faça a operação que se pede entre eles
            // coloque o resultado de volta na pilha
            printf("\ncalcular ");
            printExpElement(numStack[topStack-1]);
            printExpElement(rpnStack[pos]);
            printExpElement(numStack[topStack]);
            EXPRESSION_ELEMENT result;

            switch (rpnStack[pos].content.symbol_char) {
                case '+':
                    result = sum(numStack[topStack-1], numStack[topStack]);
                    break;
                case '-':
                    result = subtraction(numStack[topStack-1], numStack[topStack]);
                    break;
                case '*':
                    result = multiplication(numStack[topStack-1], numStack[topStack]);
                    break;
                case '/':
                    // result = division(numStack[topStack-1], numStack[topStack]);
                    break;
                case '^':
                    // result = power(numStack[topStack-1], numStack[topStack]);
                    break;       
                default:
                    break;
            }            
            result.flags = 0;

            topStack-=2;
            numStack[++topStack] = result;
        }
    } while ((rpnStack[pos++].flags & 1 << 6) != 0b01000000);

    return numStack[0];
}

int main() {
    char a[] = "0.5+35.9+42^56/((74-(5^2+9)*2.1))-20";
    char b[] = "(3.5*15/(3+0.2)^2-1.5)";
    char c[] = "1+1";
    char d[] = "-1+3*(4-2)*5*(-1)"; // OK
    char e[] = "-3.5*15/(3+2)^2-1";
    char f[] = "(-.5+35.9+42^56/(-(-74-(+5^2+9)*2.123456789123456789))-20)";
    char g[] = "5+((1+2)*4)-3"; // ESSE CASO TA QUEBRANDO NA FUNÇÃO DE ADD ZEROS
    ARRAY inputWithZeros;

    // SYNTAX ERROR
    printf("\n%s", d);
    if (hasSyntaxError(d)) { printf("\nsyntax error\n\n"); return 1; }

    printf("aaaaaaaaaaaaaaaaaaa");
    // ADD ZEROS
    addZeroToSpecialCases(d, &inputWithZeros);
    // TRANSFORM TO STRUCT
    printf("\n");
    ELEMENT_LIST structuredExp = transformCharToStruct(inputWithZeros.values);
    printElementList(structuredExp);

    // CREATE RPN STACK
    printf("\n");
    EXPRESSION_ELEMENT rpnStack[structuredExp.RPNExpSize];
    createRPNStack(structuredExp, rpnStack);
    printExpElementArray(rpnStack, structuredExp.RPNExpSize);
    
    // STACK SOLVER
    printf("\n");
    EXPRESSION_ELEMENT result = RPNStackSolver(rpnStack);

    if (result.flags == 0)
        printf("\nRESULTADO = %d", result.content.number_int);
    else
        printf("\nRESULTADO = %f", result.content.number_double);
    

    printf("\n\n");

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

void addZeroToSpecialCases(char input[], ARRAY *output) {

    char zerosToAdd = 0;
    char inputSize = 1;
    char pos = 1;
    (*output).size = 0;

    if (input[0] == '.' || input[0] == '+' || input[0] == '-') 
        zerosToAdd++;

    while (input[pos] != '\0') {
        if ((input[pos] == '.' || input[pos] == '+' || input[pos] == '-') && (input[pos-1] < '0' || input[pos-1] > '9')) {
            zerosToAdd++;
        }
        inputSize++;
        pos++;
    }

    (*output).values[inputSize+zerosToAdd];
    (*output).values[inputSize+zerosToAdd] = '\0';

    if (input[0] == '.' || input[0] == '+' || input[0] == '-')
    {
        (*output).values[0] = '0';
        (*output).values[1] = input[0];
        (*output).size += 2;
    }
    else (*output).values[(*output).size++] = input[0];

    pos = 1;

    while (input[pos] != '\0') {
        if (((input[pos] == '+' || input[pos] == '-') 
        && (input[pos-1] < '0' || input[pos-1] > '9') && input[pos-1] != ')')
        || (input[pos] == '.' && (input[pos-1] < '0' || input[pos-1] > '9')))
        {
            (*output).values[(*output).size++] = '0';
            (*output).values[(*output).size++] = input[pos];
        }
        else (*output).values[(*output).size++] = input[pos];
        pos++;
    }
}

ELEMENT_LIST transformCharToStruct(char exp[]) {
    ELEMENT_LIST elementList;
    elementList.list = malloc(sizeof(EXPRESSION_ELEMENT));
    elementList.symbolStackSize = 0;
    elementList.RPNExpSize = 0;
    elementList.size = 0;

    int pos = 0;
    int currentNumberLength = 0;
    char* currentNumber = calloc(1, sizeof(char));

    while (exp[pos] != '\0') {
        // se pos for numérico
        if (exp[pos] >= '0' && exp[pos] <= '9' || exp[pos] == '.') {
            currentNumberLength++;
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
                    number.content.number_int = atoll(currentNumber);

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
        else if (exp[pos] == '(' || exp[pos] == ')') {
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
        pos++;        
    }
    free(currentNumber);
    return elementList;
}

void createRPNStack(ELEMENT_LIST input, EXPRESSION_ELEMENT* output) {
    EXPRESSION_ELEMENT symbolStack[input.symbolStackSize];
    char symbolStackSize = 0;
    char outputSize = 0;

    for (int pos = 0; pos < input.size; pos++)
    {
        EXPRESSION_ELEMENT element = input.list[pos];
        unsigned char flags = element.flags;

        if (!(flags & 0b00000111)) // se for número
            output[outputSize++] = input.list[pos];
        else if (symbolStackSize == 0)
            symbolStack[symbolStackSize++] = input.list[pos];
        else
        {
            if (input.list[pos].content.symbol_char == '(')
                symbolStack[symbolStackSize++] = input.list[pos];
            else if (input.list[pos].content.symbol_char == ')')
            {
                while (symbolStack[symbolStackSize-1].content.symbol_char != '(')
                    output[outputSize++] = symbolStack[--symbolStackSize];
                symbolStackSize--;
            }
            else { // se input for +-*/^
                while ((input.list[pos].flags & 0b00000111) <= (symbolStack[symbolStackSize-1].flags & 0b00000111) && symbolStackSize > 0)
                    if (symbolStack[symbolStackSize-1].content.symbol_char != '(')
                        output[outputSize++] = symbolStack[--symbolStackSize];
                    else break;
                symbolStack[symbolStackSize++] = input.list[pos];
            }            
        }
    }

    while (symbolStackSize != 0)
        output[outputSize++] = symbolStack[--symbolStackSize];

    output[outputSize-1].flags |= 0b01000000;
}
