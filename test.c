#include <stdio.h>
#include <string.h>

int isNumericExpressionValid(char expr[]) {
    int len = strcspn(expr, "\r\n");
    int brackets = 0;
    int op_count = 0;
    int num_count = 0;

    for (int i = 0; i < len; i++) {
        char ch = expr[i];

        // Verifica se o caractere é um número
        if (ch >= '0' && ch <= '9') {
            num_count++;
        }
        // Verifica se o caractere é um operador
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
            op_count++;
        }
        // Verifica se o caractere é um parêntese aberto
        else if (ch == '(') {
            brackets++;
        }
        // Verifica se o caractere é um parêntese fechado
        else if (ch == ')') {
            brackets--;
            // Verifica se há mais parênteses fechados do que abertos
            if (brackets < 0) {
                return 0;
            }
        }
        // Caractere inválido
        else {
            return 0;
        }
    }

    // Verifica se há mais parênteses abertos do que fechados
    if (brackets != 0) {
        return 0;
    }

    // Verifica se há pelo menos um número e um operador
    if (num_count < 1 || op_count < 1) {
        return 0;
    }

    return 1;
}

int main() {
    char expr[100];
    printf("Digite a expressao numerica: ");
    fgets(expr, 100, stdin);

    if (isNumericExpressionValid(expr)) {
        printf("Expressao numerica valida.\n");
    }
    else {
        printf("Expressao numerica invalida.\n");
    }

    return 0;
}