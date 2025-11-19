#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IS_INT(flags)   !(flags | 0)
#define IS_FLOAT(flags) flags & 1 << 7
#define IS_CHAR(flags)  !(flags & 1 << 7) && flags | 0

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

        if(IS_FLOAT(flags)) printf("%f ", element.content.number_double);
        else if (IS_INT(flags)) printf("%d ", element.content.number_int);
        else printf("%c ", element.content.symbol_char);
    }
}

void printExpElementArray(EXPRESSION_ELEMENT* stack, int stackSize) {
    unsigned char flags;
    EXPRESSION_ELEMENT element;
    for (int i = 0; i < stackSize; i++)
    {
        element = stack[i];
        flags = element.flags;

        if (IS_FLOAT(flags)) printf("%f ", element.content.number_double);
        else if (IS_INT(flags)) printf("%i ", element.content.number_int);
        else printf("%c ", element.content.symbol_char);
    }
}

char countSizeAfterAddSpecialChars(char[]);
EXPRESSION_ELEMENT operateTwoElements(EXPRESSION_ELEMENT, EXPRESSION_ELEMENT, EXPRESSION_ELEMENT);
char isItAnOverflow(EXPRESSION_ELEMENT a, EXPRESSION_ELEMENT b, char op);

int hasSyntaxError(char[]);
void addCharsToSpecialCases(char[], ARRAY *);
ELEMENT_LIST transformCharToStruct(char[]);
void createRPNStack(ELEMENT_LIST, EXPRESSION_ELEMENT*);
int stackSolver(EXPRESSION_ELEMENT *, int, EXPRESSION_ELEMENT *);

int main() {

    // OVERFLOW TEST
    EXPRESSION_ELEMENT x;
    x.flags = 0b00000000;
    x.content.number_int = LLONG_MAX;

    EXPRESSION_ELEMENT y;
    y.flags = 0b00000000;
    y.content.number_int = -1;

    printf("\n%i\n", isItAnOverflow(x, y, '-'));

    return 0;
    // -------------

    char a[] = "0.5+35.9+42^56/((74-(5^2+9)*2.1))-20"; // ok
    char b[] = "(3.5*15/(3+0.2)^2-1.5)"; // ok 
    char c[] = "1+1"; // ok
    char d[] = "-1*3*(4-2)/5*(-1)"; // ok
    char e[] = "-3.5*15/(3+2)^2-1"; // ok
    char f[] = "(-.5*35.9+42^56/(-(-74-(+5^2+9)*2.123456789123456789))-20)"; // ok
    char g[] = "-3.5*15+(-2.5+.4)"; // ok
    char h[] = "-3.5*15+(-2.5*.4)"; // ok
    char i[] = "-3.5-15+(-2.5*.4)"; // ok
    char j[] = "5+((1+2)*4)-3"; // ok

    char k[] = "+"; // ok (syntax error)
    char l[] = "1/0"; // ok (divisao por 0)
    char m[] = "7.7777777777+1.1111111111"; // ok
    char n[] = "2147483647+1"; // tratar overflow ######################################
    
    ARRAY inputWithZeros;

    char* testeAtual = n;

    EXPRESSION_ELEMENT result;
    result.flags = 0b00000000;

    // SYNTAX ERROR
    printf("\n%s", testeAtual);
    if (hasSyntaxError(testeAtual)) { printf("\nsyntax error\n\n"); exit(1); }
    
    // ADD ZEROS
    addCharsToSpecialCases(testeAtual, &inputWithZeros);

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
    stackSolver(rpnStack, structuredExp.RPNExpSize, &result);

    printf("\n");
    printExpElementArray(&result, 1);
    printf("\n\n");

    return 0;
}

// FUNÇÕES PRINCIPAIS
int hasSyntaxError(char exp[]) {
    char pos = 1;
    char openedBrackets = 0;
    char dotsOfCurrentNumber = 0;

    if (exp[0] == '*' ||
        exp[0] == '/' || 
        exp[0] == '^' ||
        exp[0] == ')' || 
        exp[0] == '\0' ||
        ((exp[0] == '+' || exp[0] == '-') && exp[pos] == '\0')
    ) return 1;

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
            && exp[pos+1] != '(' && exp[pos+1] != '.'
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

void addCharsToSpecialCases(char input[], ARRAY *output) {
    char shift = 0;
    char pos = 0;

    output->size = countSizeAfterAddSpecialChars(input);
    output->values = (char*) malloc((output->size+1)*sizeof(char));
    output->values[output->size] = '\0';

    while (input[pos] != '\0') {
        if ((input[pos] == '-') && (input[pos-1] < '0' || input[pos-1] > '9') && input[pos-1] != ')')
        {
            char needAnotherBracketPair = 0;

            char i = pos;
            while ((input[i+1] >= '0' && input[i+1] <= '9') || input[i+1] == '.')
                i++;
            i++;

            if (input[i] != '+' && input[i] != '-' && input[i] != ')'  && input[i] != '(')
            {
                needAnotherBracketPair = 1;
            }

            if (needAnotherBracketPair) {
                output->values[pos+shift] = '(';
                output->size++;
                shift++;
            }
            output->values[pos+shift] = '0';
            shift++;
            output->values[pos+shift] = input[pos];
            pos++;
            output->size+=2;

            while ((input[pos] >= '0' && input[pos] <= '9') || input[pos] == '.')
            {
                output->values[pos+shift] = input[pos];
                output->size++;
                pos++;
            }

            if (needAnotherBracketPair)
            {
                output->values[pos+shift] = ')';
                output->size++;
                shift++;
            }
        }

        else if (input[pos] == '.' && (input[pos-1] < '0' || input[pos-1] > '9'))
        {
            output->values[pos+shift] = '0';
            shift++;
            output->values[pos+shift] = input[pos];
            pos++;
            output->size+=2;

            while ((input[pos] >= '0' && input[pos] <= '9') || input[pos] == '.')
            {
                output->values[pos+shift] = input[pos];
                output->size++;
                pos++;
            }
        }

        else if (input[pos] == '+' 
        && (input[pos-1] < '0' || input[pos-1] > '9') && input[pos-1] != ')')
        {
            output->values[pos+shift] = input[pos+1];
            output->size++;
            shift--;
            pos++;
        }
        
        else {
            output->values[pos+shift] = input[pos];
            output->size++;
            pos++;
        }
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

                if(IS_FLOAT(number.flags))
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

int stackSolver(EXPRESSION_ELEMENT *rpnStack, int stackSize, EXPRESSION_ELEMENT *result) {
    if (stackSize <3 && IS_CHAR(rpnStack[0].flags)) exit(1);
    
    printExpElementArray(rpnStack, stackSize);
    printf("\n");

    printf("stackSize: %i\n\n", stackSize);
    for (int i = 0; i < stackSize; i++)
    {
        if (IS_CHAR(rpnStack[i].flags))
        {
            EXPRESSION_ELEMENT firstElement = rpnStack[i-2];
            EXPRESSION_ELEMENT secondElement = rpnStack[i-1];
            printExpElementArray(&firstElement, 1);
            printExpElementArray(&rpnStack[i], 1);
            printExpElementArray(&secondElement, 1);
            
            EXPRESSION_ELEMENT intermediateResult = operateTwoElements(firstElement, secondElement, rpnStack[i]);
            rpnStack[i] = intermediateResult;
            printf("= ");
            printExpElementArray(&intermediateResult, 1);

            printf("\n---------------------------\n");

            for (int j = i; j < stackSize; j++)
            {
                rpnStack[j-2] = rpnStack[j];
            }

            stackSize -= 2;
            i = 0;

            if (stackSize == 1)
            {
                *result = intermediateResult;
                return 0;
            }
        }
    }
}

// FUNÇÕES AUXILIARES
void divZeroError() {
    // tratar divisão por zero
    printf("divisao por 0");
    exit(1);
}

char countSizeAfterAddSpecialChars(char input[]) {
    char inputCount = 0;
    char shift = 0;
    char pos = 0;
    char i;

    while (input[pos] != '\0') {
        inputCount++;

        if ((input[pos] == '-') && (input[pos-1] < '0' || input[pos-1] > '9') && input[pos-1] != ')')
        {
            i = pos;
            while ((input[i+1] >= '0' && input[i+1] <= '9') || input[i+1] == '.')
                i++;
            i++;

            if (input[i] != '+' && input[i] != '-' && input[i] != ')'  && input[i] != '(')
                shift+=2; // ()
            
            shift++; // 0
        }

        else if (input[pos] == '.' 
        && (input[pos-1] < '0' || input[pos-1] > '9'))
        {
            shift++; // 0
            i = pos;
            while (input[i] >= '0' && input[i] <= '9') i++;
        }

        else if (input[pos] == '+' 
        && (input[pos-1] < '0' || input[pos-1] > '9') 
        && input[pos-1] != ')')
            shift--; // +
        
        pos++;
    }

    return inputCount+shift;
}

char isItAnOverflow(EXPRESSION_ELEMENT a, EXPRESSION_ELEMENT b, char op) {
    char isAnOverflow = 0;

    if (IS_INT(a.flags) && IS_INT(b.flags)) {
        if (
            (op == '+')
            && ((b.content.number_int > 0 && a.content.number_int > LLONG_MAX - b.content.number_int)
            || (b.content.number_int < 0 && a.content.number_int < LLONG_MIN + llabs(b.content.number_int)))
        ) { printf("MAIS"); isAnOverflow = 1; }

        else if (
            (op == '-')
            && ((b.content.number_int > 0 && a.content.number_int < LLONG_MIN + b.content.number_int)
            || (b.content.number_int < 0 && a.content.number_int > LLONG_MAX - b.content.number_int))
        ) { printf("MENOS"); isAnOverflow = 1; }

        else if (
            (op == '*' && b.content.number_int != 0)
            && ((b.content.number_int > 0 && (a.content.number_int > LLONG_MAX / b.content.number_int || a.content.number_int < LLONG_MIN / b.content.number_int))
            || (b.content.number_int < 0 && (a.content.number_int < LLONG_MAX / b.content.number_int || a.content.number_int > LLONG_MIN / b.content.number_int)))
        ) { isAnOverflow = 1; }

        else if (
            op == '^'
            && ((double)a.content.number_int <= pow(LLONG_MAX, 1.0/(double)b.content.number_int) && (double)a.content.number_int >= pow(LLONG_MIN, 1.0/(double)b.content.number_int))
        ) { isAnOverflow = 1; }
    }
    else if (IS_INT(a.flags) && IS_FLOAT(b.flags)) {
        if (
            (op == '+' || op == '-')
            && ((b.content.number_double > 0 && a.content.number_int > LLONG_MAX - b.content.number_double)
            || (b.content.number_double < 0 && a.content.number_int < LLONG_MIN - b.content.number_double))
        ) { isAnOverflow = 1; }

        else if (
            (op == '*' && b.content.number_double != 0)
            && ((b.content.number_double > 0 && (a.content.number_int > LLONG_MAX / b.content.number_double || a.content.number_int < LLONG_MIN / b.content.number_double))
            || (b.content.number_double < 0 && (a.content.number_int < LLONG_MAX / b.content.number_double || a.content.number_int > LLONG_MIN / b.content.number_double)))
        ) { isAnOverflow = 1; }

        else if (
            op == '^'
            && ((double)a.content.number_int <= pow(LLONG_MAX, 1.0/b.content.number_double) && (double)a.content.number_int >= pow(LLONG_MIN, 1.0/b.content.number_double))
        ) { isAnOverflow = 1; }
    }
    else if (IS_FLOAT(a.flags) && IS_INT(b.flags)) {
        if (
            (op == '+' || op == '-')
            && ((b.content.number_int > 0 && a.content.number_double > LLONG_MAX - b.content.number_int)
            || (b.content.number_int < 0 && a.content.number_double < LLONG_MIN - b.content.number_int))
        ) { isAnOverflow = 1; }

        else if (
            (op == '*' && b.content.number_int != 0)
            && ((b.content.number_int > 0 && (a.content.number_double > LLONG_MAX / b.content.number_int || a.content.number_double < LLONG_MIN / b.content.number_int))
            || (b.content.number_int < 0 && (a.content.number_double < LLONG_MAX / b.content.number_int || a.content.number_double > LLONG_MIN / b.content.number_int)))
        ) { isAnOverflow = 1; }

        else if (
            op == '^'
            && (a.content.number_double <= pow(LLONG_MAX, 1.0/(double)b.content.number_int) && a.content.number_double >= pow(LLONG_MIN, 1.0/(double)b.content.number_int))
        ) { isAnOverflow = 1; }
    }
    else { // IS_FLOAT(a.flags) && IS_FLOAT(b.flags)
        if (
            (op == '+' || op == '-')
            && ((b.content.number_double > 0 && a.content.number_double > LLONG_MAX - b.content.number_double)
            || (b.content.number_double < 0 && a.content.number_double < LLONG_MIN - b.content.number_double))
        ) { isAnOverflow = 1; }

        else if (
            (op == '*' && b.content.number_double != 0)
            && ((b.content.number_double > 0 && (a.content.number_double > LLONG_MAX / b.content.number_double || a.content.number_double < LLONG_MIN / b.content.number_double))
            || (b.content.number_double < 0 && (a.content.number_double < LLONG_MAX / b.content.number_double || a.content.number_double > LLONG_MIN / b.content.number_double)))
        ) { isAnOverflow = 1; }

        else if (
            op == '^'
            && (a.content.number_double <= pow(LLONG_MAX, 1.0/b.content.number_double) && a.content.number_double >= pow(LLONG_MIN, 1.0/b.content.number_double))
        ) { isAnOverflow = 1; }
    }

    return isAnOverflow;
}

EXPRESSION_ELEMENT operateTwoElements(EXPRESSION_ELEMENT firstElement, EXPRESSION_ELEMENT secondElement, EXPRESSION_ELEMENT operator) {
    EXPRESSION_ELEMENT operateTwoElementsResult;

    operateTwoElementsResult.flags = 0b10000000;
    
    if (IS_INT(firstElement.flags) && IS_INT(secondElement.flags)) {
        operateTwoElementsResult.flags = 0b00000000;

        switch (operator.content.symbol_char) {
        case '+':
            isItAnOverflow(firstElement, secondElement, operator.content.symbol_char);
            operateTwoElementsResult.content.number_int = firstElement.content.number_int + secondElement.content.number_int;
            break;
        case '-':
            operateTwoElementsResult.content.number_int = firstElement.content.number_int - secondElement.content.number_int;
            break;
        case '*':
            operateTwoElementsResult.content.number_int = firstElement.content.number_int * secondElement.content.number_int;
            break;
        case '/':
            if (secondElement.content.number_int == 0)
                divZeroError();
            else{
                if (firstElement.content.number_int % secondElement.content.number_int == 0) {
                    operateTwoElementsResult.content.number_int = firstElement.content.number_int / secondElement.content.number_int;
                }
                else {
                    operateTwoElementsResult.content.number_double = firstElement.content.number_int / secondElement.content.number_int;
                    operateTwoElementsResult.flags = 0b10000000;
                }
            }
            break;
        case '^':
            operateTwoElementsResult.content.number_double = pow(firstElement.content.number_int, secondElement.content.number_int);
            operateTwoElementsResult.flags = 0b10000000;
            break;
        default:
            break;
        }
    }

    else if (IS_INT(firstElement.flags) && IS_FLOAT(secondElement.flags)) {
        switch (operator.content.symbol_char) {
        case '+':
            operateTwoElementsResult.content.number_double = firstElement.content.number_int + secondElement.content.number_double;
            break;
        case '-':
            operateTwoElementsResult.content.number_double = firstElement.content.number_int - secondElement.content.number_double;
            break;
        case '*':
            operateTwoElementsResult.content.number_double = firstElement.content.number_int * secondElement.content.number_double;
            break;
        case '/':
            if (secondElement.content.number_double == 0.0)
                divZeroError();
            operateTwoElementsResult.content.number_double = firstElement.content.number_int / secondElement.content.number_double;
            break;
        case '^':
            operateTwoElementsResult.content.number_double = powl(firstElement.content.number_int, secondElement.content.number_double);
            break;
        default:
            break;
        }
    }

    else if (IS_FLOAT(firstElement.flags) && IS_INT(secondElement.flags)) {
        switch (operator.content.symbol_char) {
        case '+':
            operateTwoElementsResult.content.number_double = firstElement.content.number_double + secondElement.content.number_int;
            break;
        case '-':
            operateTwoElementsResult.content.number_double = firstElement.content.number_double - secondElement.content.number_int;
            break;
        case '*':
            operateTwoElementsResult.content.number_double = firstElement.content.number_double * secondElement.content.number_int;
            break;
        case '/':
            if (secondElement.content.number_int == 0)
                divZeroError();
            operateTwoElementsResult.content.number_double = firstElement.content.number_double / secondElement.content.number_int;
            break;
        case '^':
            operateTwoElementsResult.content.number_double = powl(firstElement.content.number_double, secondElement.content.number_int);
            break;
        default:
            break;
        }
    }

    else { // IS_FLOAT(firstElement.flags) && IS_FLOAT(secondElement.flags)
        switch (operator.content.symbol_char) {
        case '+':
            operateTwoElementsResult.content.number_double = firstElement.content.number_double + secondElement.content.number_double;
            break;
        case '-':
            operateTwoElementsResult.content.number_double = firstElement.content.number_double - secondElement.content.number_double;
            break;
        case '*':
            operateTwoElementsResult.content.number_double = firstElement.content.number_double * secondElement.content.number_double;
            break;
        case '/':
            if (secondElement.content.number_double == 0.0)
                divZeroError();
            operateTwoElementsResult.content.number_double = firstElement.content.number_double / secondElement.content.number_double;
            break;
        case '^':
            operateTwoElementsResult.content.number_double = powl(firstElement.content.number_double, secondElement.content.number_double);
            break;
        default:
            break;
        }
    }

    return operateTwoElementsResult;
}
