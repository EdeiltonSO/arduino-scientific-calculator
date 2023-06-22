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
ARRAY addZeroToSpecialCases(char *);
ELEMENT_LIST transformCharToStruct(char *);

void createRPNStack(ELEMENT_LIST input, EXPRESSION_ELEMENT* output) {
    printf("\ntamanho da lista: %i\ntamanho da pilha: %i\n", input.size, input.RPNExpSize);

    EXPRESSION_ELEMENT symbolStack[input.symbolStackSize];
    char symbolStackSize = 0;
    char outputSize = 0;

    for (int pos = 0; pos < input.size; pos++)
    {
        EXPRESSION_ELEMENT element = input.list[pos];
        unsigned char flags = element.flags;

        // trocar essa e outras condicionais por diretivas define
        if(!(flags & 0b00000111)) // se for número
        {
            printf("# ");
            // printf("\n# > stackSize: %i", symbolStackSize);
            // printf("\n# > outputSize: %i\n", outputSize);
            
            output[outputSize] = input.list[pos];
            outputSize++;
        }
        else if (symbolStackSize == 0)
        {
            printf(". ");
            symbolStack[symbolStackSize] = input.list[pos];
            symbolStackSize++;
        }
        else
        {
            if (input.list[pos].content.symbol_char == '(')
            {
                printf("( ");
                // printf("\n( > stackSize: %i", symbolStackSize);
                // printf("\n( > outputSize: %i\n", outputSize);
                
                // symbolStack[symbolStackSize] = input.list[pos];
                symbolStackSize++;
            }
            else if (input.list[pos].content.symbol_char == ')')
            {
                printf(") ");
                // printf("\n) > stackSize: %i", symbolStackSize);
                // printf("\n) > outputSize: %i\n", outputSize);
                
                // while (symbolStack[symbolStackSize].content.symbol_char != '(') {
                //     output[outputSize] = symbolStack[symbolStackSize];
                //     outputSize++;
                //     symbolStackSize--;
                // }
                symbolStackSize--;
            }
            else { // se input for +-*/^
                //printf("[%d ", input.list[pos].flags & 0b00000111);
                printf("[0x%x] ", symbolStack[symbolStackSize].flags & 0b00000111);
                // printf("\n%c > stackSize: %i", input.list[pos].content.symbol_char, symbolStackSize);
                // printf("\n%c > outputSize: %i\n", input.list[pos].content.symbol_char, outputSize);

                // while (input.list[pos].flags & 0b00000111 < symbolStack[symbolStackSize].flags & 0b00000111)
                // {
                //     output[outputSize] = symbolStack[symbolStackSize];
                //     outputSize++;
                //     symbolStackSize--;
                // }
                // symbolStack[symbolStackSize] = input.list[pos];
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
    //char input[] = "-.5+35.9+42^56/(-(-74-(+5^2+9)*2.123456789123456789123456789))-20";
    char exp[] = "-(3.5*15/(-3+.2)^2-1.5)";
    //char e[] = "1+1";

    ARRAY a = addZeroToSpecialCases(exp);

    printf("\n%s", exp);
    printf("\n%s", a.values);
    printf("\nexp: %d", strlen(exp));
    printf("\na:   %d", strlen(a.values));

    return 0;
}

