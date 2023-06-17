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
    char RPNExpSize;
    char symbolStackSize;
    EXPRESSION_ELEMENT* list;
} ELEMENT_LIST;

int hasSyntaxError(char *);
char * addZeroToSpecialCases(char *);
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

        // printf("%d\n", flags);
        // trocar essa e outras condicionais por diretivas define
        if(!(flags & 0b00000111)) // se for número
        {
            printf("# ");
            output[outputSize] = input.list[pos];
            outputSize++;
        }
        else
        {
            if (symbolStackSize == 0 || input.list[pos].content.symbol_char == '(')
            {
                printf("( ");
                symbolStack[symbolStackSize] = input.list[pos];
                symbolStackSize++;
            }
            else if (input.list[pos].content.symbol_char == ')')
            {
                printf(") ");
                while (symbolStack[symbolStackSize].content.symbol_char != '(') {
                    output[outputSize] = symbolStack[symbolStackSize];
                    outputSize++;
                    symbolStackSize--;
                }
                symbolStackSize--;
            }
            else { // se input nao for ')'
                printf(". ");
                while (input.list[pos].flags & 0b00000111 < symbolStack[symbolStackSize].flags & 0b00000111)
                {
                    output[outputSize] = symbolStack[symbolStackSize];
                    outputSize++;
                    symbolStackSize--;
                }
                symbolStack[symbolStackSize] = input.list[pos];
                symbolStackSize++;
            }
            
            // // processa simbolo conforme lógica da RPN usando symbolStack
            // /*
            // se topo == 0 ou se novo == '('
            //     bota na pilha
            //     incrementa topo da pilha
            // se novo == ')'
            //     se topo é '('
            //         remove da pilha
            //         decrementa topo da pilha
            //     se não
            //         bota topo na saída
            //         incrementa tamanho da saída
            //         remove topo da pilha
            //         decrementa topo da pilha
            //         repete até topo ser '('
            // se não
            //     se prioridade do novo < prioridade do topo e topo >= 0
            //         tira topo e bota na saída
            //         repete até ser false
            //         bota novo na pilha
            //     se não
            //         bota novo na pilha
            // */
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
    char input[] = "(-.5+35.9+42^56/(-(-74-(+5^2+9)*2.123456789123456789123456789))-20)";
    char exp[] = "3.5*15/(3+2)^2-1";

    ELEMENT_LIST structuredExp = transformCharToStruct(input);
    EXPRESSION_ELEMENT stack[structuredExp.RPNExpSize];

    createRPNStack(structuredExp, stack);

    // ---------------------------------
    printf("\n");
    printf("expressao original: %s\n", input);
    // printf("caracteres da expressao original: %d\n", strlen(input));
    printf("tamanho da expressao original: %d\n", structuredExp.size);
    printf("tamanho da expressao em RPN:   %d\n", structuredExp.RPNExpSize);
    printf("tamanho da pilha de simbolos:  %d\n", structuredExp.symbolStackSize);

    for (int i = 0; i < structuredExp.size; i++)
    {
        EXPRESSION_ELEMENT element = structuredExp.list[i];
        unsigned char flags = element.flags;

        if(flags & 1 << 7) printf("%f ", element.content.number_double);
        else if (!(flags | 0)) printf("%d ", element.content.number_int);
        else printf("%c ", element.content.symbol_char);
    }

    printf("\nRPN: \n");

    for (int i = 0; i < structuredExp.RPNExpSize; i++)
    {
        EXPRESSION_ELEMENT element = stack[i];
        unsigned char flags = element.flags;

        if(flags & 1 << 7) printf("%f ", element.content.number_double);
        else if (!(flags | 0)) printf("%d ", element.content.number_int);
        else printf("%c ", element.content.symbol_char);
    }

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
    elementList.symbolStackSize = 0;
    elementList.RPNExpSize = 0;
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
                elementList.RPNExpSize++;

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

                if(exp[pos] == '(') elementList.symbolStackSize++;
            }
            else // +-*/^
            {
                EXPRESSION_ELEMENT symbol;

                symbol.flags = 0;
                symbol.content.symbol_char = exp[pos];
                elementList.symbolStackSize++;
                elementList.RPNExpSize++;
                
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