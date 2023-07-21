#include <stdio.h>
#include <stdlib.h>
#define MINUS(char1, char0) (char1 == '-' && (char0 == '\0' || char0 == '(' /*|| char0 == ')'*/ || !(char0 >= '0' && char0 <= '9')))

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

int hasSyntaxError(char *);
void addZeroToSpecialCases(char *, ARRAY *);
ELEMENT_LIST transformCharToStruct(char *);
void createRPNStack(ELEMENT_LIST, EXPRESSION_ELEMENT*);

void stackSolver(/* recebe ponteiro pra pilha */) {
    // retorna resultado
}

int main() {
    char a[] = "0.5+35.9+42^56/((74-(5^2+9)*2.1))-20";
    char b[] = "(3.5*15/(3+0.2)^2-1.5)";
    char c[] = "1+1";
    char d[] = "0-1+3*(4-2)/5*(0-1)";
    char e[] = "-3.5*15/(3+2)^2-1";
    char f[] = "(-.5+35.9+42^56/(-(-74-(+5^2+9)*2.123456789123456789))-20)";

    ELEMENT_LIST structuredExp = transformCharToStruct(d);
    printf("\n");
    for (int i = 0; i < structuredExp.size; i++)
    {
        EXPRESSION_ELEMENT element = structuredExp.list[i];
        unsigned char flags = element.flags;

        if(flags & 1 << 7) printf("%f ", element.content.number_double);
        else if (!(flags | 0)) printf("%d ", element.content.number_int);
        else printf("%c ", element.content.symbol_char);
    }

    // EXPRESSION_ELEMENT rpnStack[structuredExp.RPNExpSize];
    // createRPNStack(structuredExp, rpnStack);

    // printf("\n");
    // unsigned char flags;
    // EXPRESSION_ELEMENT element;
    // for (int i = 0; i < structuredExp.RPNExpSize; i++)
    // {
    //     element = rpnStack[i];
    //     flags = element.flags;

    //     if(flags & 1 << 7) printf("%f ", element.content.number_double);
    //     else if (!(flags | 0)) printf("%i ", element.content.number_int);
    //     else printf("%c ", element.content.symbol_char);
    // }
    // printf("\n\n");

    return 0;
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
    char before[2] = "\0\0";

    while (exp[pos] != '\0') {
        // se pos for numérico
        if (exp[pos] >= '0' && exp[pos] <= '9' || exp[pos] == '.') {

            if (!before[0] && pos-2 >= 0) before[0] = exp[pos-2];
            if (!before[1] && pos-1 >= 0) before[1] = exp[pos-1]; 

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

                if (number.flags & 1 << 7) {
                    if (MINUS(before[1], before[0]))
                    {
                        number.content.number_double = 0-atof(currentNumber);
                    }
                    else
                    {
                        number.content.number_double = atof(currentNumber);
                    }
                }
                else {
                    if (MINUS(before[1], before[0]))
                    {
                        number.content.number_int = 0-atoll(currentNumber);
                    }
                    else
                    {
                        number.content.number_int = atoll(currentNumber);
                    }
                    
                }

                before[0] = '\0';
                before[1] = '\0';

                elementList.size++;
                elementList.RPNExpSize++;

                if (elementList.size > 1) elementList.list = realloc(elementList.list, elementList.size * sizeof(EXPRESSION_ELEMENT));
                elementList.list[elementList.size-1] = number;

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
            // symbol.content.symbol_char = exp[pos];
            symbol.content.symbol_char = MINUS(exp[pos], exp[pos-1]) ? '+' : exp[pos];
            
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

// ----------------------------------------------------------------------

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
        if ((rpnStack[pos].flags & 0b00000111) == 0) {
            printf("\nNUMBER");
            numStack[++topStack] = rpnStack[pos];
        }
        else {
            // retire o penúltimo e o último operando da pilha
            // faça a operação que se pede entre eles
            // coloque o resultado de volta na pilha
            EXPRESSION_ELEMENT result;
            
            if (INT_FLOAT)
            {
                result.flags = 0x80;
                switch (rpnStack[pos].content.symbol_char)
                {
                    case '+':
                        result.content.number_double = numStack[topStack-1].content.number_int + numStack[topStack].content.number_double;
                        break;
                    case '-':
                        result.content.number_double = numStack[topStack-1].content.number_int - numStack[topStack].content.number_double;
                        break;
                    case '*':
                        result.content.number_double = numStack[topStack-1].content.number_int * numStack[topStack].content.number_double;
                        break;
                    case '/':
                        result.content.number_double = numStack[topStack-1].content.number_int / numStack[topStack].content.number_double;
                        break;
                    case '^':
                        result.content.number_double = pow(numStack[topStack-1].content.number_int, numStack[topStack].content.number_double);
                        break;
                    default:
                        break;
                }
                topStack -= 2;
                result.content.number_double = numStack[topStack-1].content.number_int + numStack[topStack].content.number_int;
            }
            else if (FLOAT_INT)
            {
                result.flags = 0x80;
                switch (rpnStack[pos].content.symbol_char)
                {
                    case '+':
                        result.content.number_double = numStack[topStack-1].content.number_double + numStack[topStack].content.number_int;
                        break;
                    case '-':
                        result.content.number_double = numStack[topStack-1].content.number_double - numStack[topStack].content.number_int;
                        break;
                    case '*':
                        result.content.number_double = numStack[topStack-1].content.number_double * numStack[topStack].content.number_int;
                        break;
                    case '/':
                        result.content.number_double = numStack[topStack-1].content.number_double / numStack[topStack].content.number_int;
                        break;
                    case '^':
                        result.content.number_double = pow(numStack[topStack-1].content.number_double, numStack[topStack].content.number_int);
                        break;
                    default:
                        break;
                }
                topStack -= 2;
                result.content.number_double = numStack[topStack-1].content.number_int + numStack[topStack].content.number_int;
            }
            else if (FLOAT_FLOAT)
            {
                result.flags = 0x80;
                switch (rpnStack[pos].content.symbol_char)
                {
                    case '+':
                        result.content.number_double = numStack[topStack-1].content.number_double + numStack[topStack].content.number_double;
                        break;
                    case '-':
                        result.content.number_double = numStack[topStack-1].content.number_double - numStack[topStack].content.number_double;
                        break;
                    case '*':
                        result.content.number_double = numStack[topStack-1].content.number_double * numStack[topStack].content.number_double;
                        break;
                    case '/':
                        result.content.number_double = numStack[topStack-1].content.number_double / numStack[topStack].content.number_double;
                        break;
                    case '^':
                        result.content.number_double = pow(numStack[topStack-1].content.number_double, numStack[topStack].content.number_double);
                        break;
                    default:
                        break;
                }
                topStack -= 2;
                result.content.number_double = numStack[topStack-1].content.number_double + numStack[topStack].content.number_double;
            }
            else {
                result.flags = 0x00;
                switch (rpnStack[pos].content.symbol_char)
                {
                    case '+':
                        result.content.number_int = numStack[topStack-1].content.number_int + numStack[topStack].content.number_int;
                        break;
                    case '-':
                        result.content.number_int = numStack[topStack-1].content.number_int - numStack[topStack].content.number_int;
                        break;
                    case '*':
                        result.content.number_int = numStack[topStack-1].content.number_int * numStack[topStack].content.number_int;
                        break;
                    case '/':
                        result.content.number_int = numStack[topStack-1].content.number_int / numStack[topStack].content.number_int;
                        break;
                    case '^':
                        result.content.number_int = pow(numStack[topStack-1].content.number_int, numStack[topStack].content.number_int);
                        break;
                    default:
                        break;
                }
                topStack -= 2;
                result.content.number_int = numStack[topStack-1].content.number_int + numStack[topStack].content.number_int;
            }
            

            // printf("\nINT_INT: %d%c%d=", 
            //     numStack[topStack-1].content.number_int,
            //     rpnStack[pos].content.symbol_char,
            //     numStack[topStack].content.number_int);


            // result.flags = 0;
            // switch (rpnStack[pos].content.symbol_char)
            // {
            //     case '+':
            //         result.content.number_int = numStack[topStack-1].content.number_int + numStack[topStack].content.number_int;
            //             topStack -= 2;
            //         break;
            //     case '-':
            //         result.content.number_int = numStack[topStack-1].content.number_int - numStack[topStack].content.number_int;
            //             topStack -= 2;
            //         break;
            //     case '*':
            //         result.content.number_int = numStack[topStack-1].content.number_int * numStack[topStack].content.number_int;
            //             topStack -= 2;
            //         break;
            //     case '/':
            //         result.content.number_int = numStack[topStack-1].content.number_int / numStack[topStack].content.number_int;
            //             topStack -= 2;
            //         break;
            //     case '^':
            //         result.content.number_int = pow(numStack[topStack-1].content.number_int, numStack[topStack].content.number_int);
            //             topStack -= 2;
            //         break;
            //     default:
            //         break;
            // }
            // printf("%d", result.content.number_int);

            numStack[++topStack] = result;
        }
    } while ((rpnStack[pos++].flags & 1 << 6) != 0b01000000);

    return numStack[0];
}