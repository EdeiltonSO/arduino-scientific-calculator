#include <stdio.h>
#include <stdlib.h>

int hasSyntaxError(char *);

int main() {
    // char input[] = "-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20";

    printf(">>> %d", hasSyntaxError("1+1+1.1.1+1"));

    return 0;
}









































// FUNÇÕES PRONTAS

int hasSyntaxError(char exp[]) {

    if (exp[0] == '*' || exp[0] == '/' || 
        exp[0] == '^' || exp[0] == ')' || 
        exp[0] == '\0') return 1;

    char pos = 1;
    char openedBrackets = 0;
    char dotsOfCurrentNumber = 0;

    while (exp[pos] != '\0') {

        if (exp[pos] >= '0' && exp[pos] <= '9' &&
        (exp[pos-1] == ')' || exp[pos+1] == '(')
        ) return 1;
        
        else if (exp[pos] == '.'
        && (exp[pos+1] < '0' || exp[pos+1] > '9')
        ) return 1;

        else if (exp[pos] == '.') {

            dotsOfCurrentNumber++; // PAREI AQUI MAS NAO TA FUNCIONANDO AINDA

            // VEJA AÍ.

            if (exp[pos+1] < '0' || exp[pos+1] > '9') return 1;
        }

        else if (exp[pos] == '+' || exp[pos] == '-')
        {
            dotsOfCurrentNumber = 0;
            if ((exp[pos-1] < '0' || exp[pos-1] > '9')
            && (exp[pos-1] < '(' || exp[pos-1] > ')')
            || (exp[pos+1] < '0' || exp[pos+1] > '9')
            && (exp[pos+1] != '(')
            ) return 1;
        }

        else if (exp[pos] == '*' || exp[pos] == '/' || exp[pos] == '^')
        {
            dotsOfCurrentNumber = 0;
            if ((exp[pos-1] < '0' || exp[pos-1] > '9')
            && (exp[pos-1] != ')')
            || (exp[pos+1] < '0' || exp[pos+1] > '9')
            && (exp[pos+1] != '(')
            ) return 1;
        }

        else if (exp[pos] == '(')
        {
            openedBrackets++;
            dotsOfCurrentNumber = 0;
            if (exp[pos-1] != '+' && exp[pos-1] != '-'
            && exp[pos-1] != '*' && exp[pos-1] != '/'
            && exp[pos-1] != '^' && exp[pos-1] != '('
            || (exp[pos+1] < '0' || exp[pos+1] > '9')
            && exp[pos+1] != '+' && exp[pos+1] != '-'
            && exp[pos+1] != '('
            ) return 1;
        }

        else if (exp[pos] == ')')
        {
            openedBrackets--;
            dotsOfCurrentNumber = 0;
            if ((exp[pos-1] < '0' || exp[pos-1] > '9') && exp[pos-1] != ')'
            || exp[pos+1] != '+' && exp[pos+1] != '-'
            && exp[pos+1] != '*' && exp[pos+1] != '/'
            && exp[pos+1] != '^' && exp[pos+1] != ')'
            ) return 1;
        }

        if (exp[pos+1] == '\0'
            && (exp[pos] < '0' || exp[pos] > '9')
            && exp[pos] != ')') return 1;

        pos++;
    }

    printf("\ndotsOfCurrentNumber: %d\n", dotsOfCurrentNumber);
    return (openedBrackets && (dotsOfCurrentNumber > 1)) ? 1 : 0;
}