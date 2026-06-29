#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define IS_INT(flags)   !(flags | 0)
#define IS_FLOAT(flags) flags & 1 << 7
#define IS_CHAR(flags)  !(flags & 1 << 7) && flags | 0
#define IS_ERROR(flags) flags & 1 << 6

#define NO_ERROR_CODE       0
#define SINTAX_ERROR_CODE   1
#define DIV_ZERO_ERROR_CODE 2
#define OVERFLOW_ERROR_CODE 3

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

// PRINCIPAIS
char hasSyntaxError(char[]);
void addCharsToSpecialCases(char[], ARRAY *);
ELEMENT_LIST transformCharToStruct(char[]);
void createRPNStack(ELEMENT_LIST, EXPRESSION_ELEMENT*);
char stackSolver(EXPRESSION_ELEMENT *, int, EXPRESSION_ELEMENT *);

// AUXILIARES
void calculate(char[], EXPRESSION_ELEMENT *);
char countSizeAfterAddSpecialChars(char[]);
char overflowError(EXPRESSION_ELEMENT, EXPRESSION_ELEMENT, char);
EXPRESSION_ELEMENT operateTwoElements(EXPRESSION_ELEMENT, EXPRESSION_ELEMENT, EXPRESSION_ELEMENT);
char* getErrorMessage(char);
char* getResultString(EXPRESSION_ELEMENT);

// TEMPORÁRIAS
void printElementList(ELEMENT_LIST);
void printExpElementArray(EXPRESSION_ELEMENT *, int);

int main() {
    char a[] = "0.5+35.9+42^5/((74-(5^2+9)*2.1))-20"; // ok: 50265874.861539
    char b[] = "(3.5*15/(3+0.2)^2-1.5)"; // ok: 3.626953
    char c[] = "1+1"; // ok: 2
    char d[] = "-1*3*(4-2)/5*(-1)"; // ok: 1.2
    char e[] = "-3.5*15/(3+2)^2-1"; // ok: -3.100000
    char f[] = "(-.5*35.9+42^56/(-(-74-(+5^2+9)*2.123456789123456789))-20)"; // ok: [overflow error]
    char g[] = "-3.5*15+(-2.5+.4)"; // ok: -54.600000
    char h[] = "-3.5*15+(-2.5*.4)"; // ok: -53.500000
    char i[] = "-3.5-15+(-2.5*.4)"; // ok: -19.500000
    char j[] = "5+((1+2)*4)-3"; // ok: 14
    char k[] = "+"; // ok: [sintax error]
    char l[] = "1/0"; // ok: [div/0 error]
    char m[] = "7.7777777777+1.1111111111"; // ok: 8.888889
    char n[] = "2147483647+1"; // ok: 2147483648
    char o[] = "+1"; // ok: [sintax error]
    char p[] = "1+"; // ok: [sintax error]
    char q[] = "(-.5*35.9+42^11/(-(-74-(+5^2+9)*2.123456789123456789))-20)"; // ok: 4906842933918337.000000
    
    char* testeAtual = a;
    char* outputString;
    EXPRESSION_ELEMENT output;

    calculate(testeAtual, &output);

    // --------------------------------------------------------
    outputString = IS_ERROR(output.flags)
        ? getErrorMessage(output.content.symbol_char)
        : getResultString(output);
    // --------------------------------------------------------

    printf("%s", outputString);

    return 0;
}

// FUNÇÕES PRINCIPAIS
char hasSyntaxError(char exp[]) {
    char pos = 1;
    char openedBrackets = 0;
    char dotsOfCurrentNumber = 0;

    if (exp[0] == '*' ||
        exp[0] == '/' || 
        exp[0] == '^' ||
        exp[0] == ')' || 
        exp[0] == '\0' ||
        ((exp[0] == '+' || exp[0] == '-') && exp[pos] == '\0')
    ) return SINTAX_ERROR_CODE;

    if (exp[0] == '(') openedBrackets++;

    while (exp[pos] != '\0') {
        if (exp[pos] == '.') {
            dotsOfCurrentNumber++;
            if (dotsOfCurrentNumber > 1) return SINTAX_ERROR_CODE;
        }
        else
            if (exp[pos] < '0' || exp[pos] > '9') dotsOfCurrentNumber = 0;

        if (exp[pos] >= '0' && exp[pos] <= '9' &&
        (exp[pos-1] == ')' || exp[pos+1] == '(')
        ) return SINTAX_ERROR_CODE;
        
        else if (exp[pos] == '.'
        && (exp[pos+1] < '0' || exp[pos+1] > '9')
        ) return SINTAX_ERROR_CODE;

        else if (exp[pos] == '+' || exp[pos] == '-')
        {
            if ((exp[pos-1] < '0' || exp[pos-1] > '9')
            && (exp[pos-1] < '(' || exp[pos-1] > ')')
            || (exp[pos+1] < '0' || exp[pos+1] > '9')
            && exp[pos+1] != '(' && exp[pos+1] != '.'
            ) return SINTAX_ERROR_CODE;
        }

        else if (exp[pos] == '*' || exp[pos] == '/' || exp[pos] == '^')
        {
            if ((exp[pos-1] < '0' || exp[pos-1] > '9')
            && (exp[pos-1] != ')')
            || (exp[pos+1] < '0' || exp[pos+1] > '9')
            && exp[pos+1] != '(' && exp[pos+1] != '.'
            ) return SINTAX_ERROR_CODE;
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
            ) return SINTAX_ERROR_CODE;
        }

        else if (exp[pos] == ')')
        {
            openedBrackets--;
            if ((exp[pos-1] < '0' || exp[pos-1] > '9') && exp[pos-1] != ')'
            || exp[pos+1] != '+' && exp[pos+1] != '-'
            && exp[pos+1] != '*' && exp[pos+1] != '/'
            && exp[pos+1] != '^' && exp[pos+1] != ')'
            && exp[pos+1] != '\0'
            ) return SINTAX_ERROR_CODE;
        }

        if (exp[pos+1] == '\0'
            && (exp[pos] < '0' || exp[pos] > '9')
            && exp[pos] != ')') return SINTAX_ERROR_CODE;

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
                        number.flags = 0b10000000;
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

char stackSolver(EXPRESSION_ELEMENT *rpnStack, int stackSize, EXPRESSION_ELEMENT *result) {
    if (stackSize <3 && IS_CHAR(rpnStack[0].flags)) 
        return SINTAX_ERROR_CODE;
    
    for (int i = 0; i < stackSize; i++)
    {
        if (IS_CHAR(rpnStack[i].flags))
        {
            EXPRESSION_ELEMENT firstElement = rpnStack[i-2];
            EXPRESSION_ELEMENT secondElement = rpnStack[i-1];
            EXPRESSION_ELEMENT intermediateResult = operateTwoElements(firstElement, secondElement, rpnStack[i]);

            if (IS_ERROR(intermediateResult.flags)) 
                return intermediateResult.content.symbol_char;

            rpnStack[i] = intermediateResult;

            for (int j = i; j < stackSize; j++)
                rpnStack[j-2] = rpnStack[j];

            stackSize -= 2;
            i = 0;

            if (stackSize == 1)
            {
                *result = intermediateResult;
                return NO_ERROR_CODE;
            }
        }
    }
}

// FUNÇÕES AUXILIARES
void calculate(char input[], EXPRESSION_ELEMENT *output) {
    ARRAY inputWithZeros;
    char hasError = 0;

    output->flags = 0b00000000;

    // SYNTAX ERROR
    hasError = hasSyntaxError(input);
    if (hasError) {
        output->flags = 0b01000000;
        output->content.symbol_char = hasError;
        return;
    }
    
    // ADD ZEROS
    addCharsToSpecialCases(input, &inputWithZeros);

    // TRANSFORM TO STRUCT
    ELEMENT_LIST structuredExp = transformCharToStruct(inputWithZeros.values);

    // CREATE RPN STACK
    EXPRESSION_ELEMENT rpnStack[structuredExp.RPNExpSize];
    createRPNStack(structuredExp, rpnStack);

    // STACK SOLVER    
    hasError = stackSolver(rpnStack, structuredExp.RPNExpSize, output);
    if (hasError) {
        output->flags = 0b01000000;
        output->content.symbol_char = hasError;
        return;
    }
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

char overflowError(EXPRESSION_ELEMENT a, EXPRESSION_ELEMENT b, char op) {
    char isAnOverflow = 0;

    if (IS_INT(a.flags) && IS_INT(b.flags)) {
        if (
            (op == '+')
            && ((b.content.number_int > 0 && a.content.number_int > LLONG_MAX - b.content.number_int)
            || (b.content.number_int < 0 && a.content.number_int < LLONG_MIN + llabs(b.content.number_int)))
        ) { isAnOverflow = 1; }

        else if (
            (op == '-')
            && ((b.content.number_int > 0 && a.content.number_int < LLONG_MIN + b.content.number_int)
            || (b.content.number_int < 0 && a.content.number_int > LLONG_MAX - b.content.number_int))
        ) { isAnOverflow = 1; }

        else if (
            (op == '*' && b.content.number_int != 0)
            && ((b.content.number_int > 0 && (a.content.number_int > LLONG_MAX / b.content.number_int || a.content.number_int < LLONG_MIN / b.content.number_int))
            || (b.content.number_int < 0 && (a.content.number_int < LLONG_MAX / b.content.number_int || a.content.number_int > LLONG_MIN / b.content.number_int)))
        ) { isAnOverflow = 1; }

        else if (op == '^') {

            long long result = 1;
            
            if (b.content.number_int < 0) {
                isAnOverflow = 1;
            }
            else {
                for (long long i = 0; i < b.content.number_int; i++) {
                    
                    if (a.content.number_int != 0 && llabs(result) > LLONG_MAX / llabs(a.content.number_int)) {
                        isAnOverflow = 1;
                        break;
                    }

                    result *= a.content.number_int;
                }
            }
        }
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

        else if (op == '^') {

            long long result = 1;

            if (b.content.number_double < 0) {
                isAnOverflow = 1;
            }
            else {
                for (long long i = 0; i < b.content.number_double; i++) {

                    if (a.content.number_int != 0 && llabs(result) > LLONG_MAX / llabs(a.content.number_int)) {
                        isAnOverflow = 1;
                        break;
                    }

                    result *= a.content.number_int;
                }
            }
        }
    }
    else if (IS_FLOAT(a.flags) && IS_INT(b.flags)) {
        if (
            (op == '+' || op == '-')
            && ((b.content.number_int > 0 && a.content.number_double > LLONG_MAX - b.content.number_int)
            || (b.content.number_int < 0 && a.content.number_double < LLONG_MIN - b.content.number_int))
        ) { isAnOverflow = 1; }

        else if (op == '*' && b.content.number_int != -1 && b.content.number_int != 0) {
            if (
                (b.content.number_int > 0 && (a.content.number_double > LLONG_MAX / b.content.number_int || a.content.number_double < LLONG_MIN / b.content.number_int))
                || (b.content.number_int < 0 && (a.content.number_double < LLONG_MAX / b.content.number_int || a.content.number_double > LLONG_MIN / b.content.number_int))
            ) { isAnOverflow = 1; }
        }

        else if (op == '^') {

            long long result = 1;

            if (b.content.number_int < 0) {
                isAnOverflow = 1;
            }
            else {
                for (long long i = 0; i < b.content.number_int; i++) {

                    if (a.content.number_double != 0 && llabs(result) > LLONG_MAX / llabs(a.content.number_double)) {
                        isAnOverflow = 1;
                        break;
                    }

                    result *= a.content.number_double;
                }
            }
        }
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

        else if (op == '^') {

            long long result = 1;

            if (b.content.number_double < 0) {
                isAnOverflow = 1;
            }
            else {
                for (long long i = 0; i < b.content.number_double; i++) {

                    if (a.content.number_double != 0 && llabs(result) > LLONG_MAX / llabs(a.content.number_double)) {
                        isAnOverflow = 1;
                        break;
                    }

                    result *= a.content.number_double;
                }
            }
        }
    }

    if (isAnOverflow)
        return OVERFLOW_ERROR_CODE;
    
    return NO_ERROR_CODE;
}

EXPRESSION_ELEMENT operateTwoElements(EXPRESSION_ELEMENT firstElement, EXPRESSION_ELEMENT secondElement, EXPRESSION_ELEMENT operator) {
    EXPRESSION_ELEMENT operateTwoElementsResult;
    char overflowCodeError = 0;

    operateTwoElementsResult.flags = 0b10000000;
    
    if (IS_INT(firstElement.flags) && IS_INT(secondElement.flags)) {
        operateTwoElementsResult.flags = 0b00000000;
        
        switch (operator.content.symbol_char) {

        case '+':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_int = firstElement.content.number_int + secondElement.content.number_int;
            break;

        case '-':
            operateTwoElementsResult.content.number_int = firstElement.content.number_int - secondElement.content.number_int;
            break;

        case '*':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_int = firstElement.content.number_int * secondElement.content.number_int;
            break;

        case '/':
            if (secondElement.content.number_int == 0) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = DIV_ZERO_ERROR_CODE;
                break;
            }
            else {
                if (firstElement.content.number_int % secondElement.content.number_int == 0) {
                    operateTwoElementsResult.content.number_int = firstElement.content.number_int / secondElement.content.number_int;
                }
                else {
                    operateTwoElementsResult.content.number_double = (double) firstElement.content.number_int / (double) secondElement.content.number_int;
                    operateTwoElementsResult.flags = 0b10000000;
                }
            }
            break;

        case '^':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_int = powl(firstElement.content.number_int, secondElement.content.number_int);
            break;

        default:
            break;
        }
    }

    else if (IS_INT(firstElement.flags) && IS_FLOAT(secondElement.flags)) {
        switch (operator.content.symbol_char) {
        case '+':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_double = firstElement.content.number_int + secondElement.content.number_double;
            break;
        case '-':
            operateTwoElementsResult.content.number_double = firstElement.content.number_int - secondElement.content.number_double;
            break;
        case '*':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_double = firstElement.content.number_int * secondElement.content.number_double;
            break;
        case '/':
            if (secondElement.content.number_double == 0.0) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = DIV_ZERO_ERROR_CODE;
                break;
            }
            operateTwoElementsResult.content.number_double = firstElement.content.number_int / secondElement.content.number_double;
            break;
        case '^':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_double = powl(firstElement.content.number_int, secondElement.content.number_double);
            break;
        default:
            break;
        }
    }

    else if (IS_FLOAT(firstElement.flags) && IS_INT(secondElement.flags)) {
        switch (operator.content.symbol_char) {
        case '+':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_double = firstElement.content.number_double + secondElement.content.number_int;
            break;
        case '-':
            operateTwoElementsResult.content.number_double = firstElement.content.number_double - secondElement.content.number_int;
            break;
        case '*':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_double = firstElement.content.number_double * (double) secondElement.content.number_int;
            break;
        case '/':
            if (secondElement.content.number_int == 0) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = DIV_ZERO_ERROR_CODE;
                break;
            }
            operateTwoElementsResult.content.number_double = firstElement.content.number_double / (double) secondElement.content.number_int;
            break;
        case '^':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_double = powl(firstElement.content.number_double, secondElement.content.number_int);
            break;
        default:
            break;
        }
    }

    else { // IS_FLOAT(firstElement.flags) && IS_FLOAT(secondElement.flags)
        switch (operator.content.symbol_char) {
        case '+':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_double = firstElement.content.number_double + secondElement.content.number_double;
            break;
        case '-':
            operateTwoElementsResult.content.number_double = firstElement.content.number_double - secondElement.content.number_double;
            break;
        case '*':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_double = firstElement.content.number_double * secondElement.content.number_double;
            break;
        case '/':
            if (secondElement.content.number_double == 0.0) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = DIV_ZERO_ERROR_CODE;
                break;
            }
            operateTwoElementsResult.content.number_double = firstElement.content.number_double / secondElement.content.number_double;
            break;
        case '^':
            overflowCodeError = overflowError(firstElement, secondElement, operator.content.symbol_char);
            if (overflowCodeError) {
                operateTwoElementsResult.flags = 0b01000000;
                operateTwoElementsResult.content.symbol_char = overflowCodeError;
                break;
            }
            operateTwoElementsResult.content.number_double = powl(firstElement.content.number_double, secondElement.content.number_double);
            break;
        default:
            break;
        }
    }

    return operateTwoElementsResult;
}

char* getErrorMessage(char errorCode) {
    switch (errorCode) {
        case 1:
            return "[syntax error]";
        case 2:
            return "[div/0 error]";
        case 3:
            return "[overflow error]";
        default:
            return "[error]";
    }
}

char* getResultString(EXPRESSION_ELEMENT result) {
    int length;

    if (IS_FLOAT(result.flags))
        length = snprintf(NULL, 0, "%f", result.content.number_double);
    else
        length = snprintf(NULL, 0, "%lli", result.content.number_int);

    char *resultString = malloc(length + 1);

    if (IS_FLOAT(result.flags))
        snprintf(resultString, length + 1, "%f", result.content.number_double);
    else
        snprintf(resultString, length + 1, "%lli", result.content.number_int);

    for (int i = 0; resultString[i] != '\0'; i++) {
        if (resultString[i] == '.')
            resultString[i] = ',';
    }
    
    return resultString;
}

// FUNÇÕES TEMPORÁRIAS
void printElementList(ELEMENT_LIST elementList) {
    for (int i = 0; i < elementList.size; i++)
    {
        EXPRESSION_ELEMENT element = elementList.list[i];
        unsigned char flags = element.flags;

        if(IS_FLOAT(flags)) printf("%f ", element.content.number_double);
        else if (IS_INT(flags)) printf("%lli ", element.content.number_int);
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
        else if (IS_INT(flags)) printf("%lli ", element.content.number_int);
        else printf("%c ", element.content.symbol_char);
    }
}