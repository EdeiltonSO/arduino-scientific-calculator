#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// char * addZeroToSpecialCases(char exp[], char newExp[]) {


//     int tamanho_vetor = sizeof(newExp);
//     int tamanho_elemento = sizeof(int);
//     int num_elementos = tamanho_vetor / tamanho_elemento;
//     int tamanho_total = tamanho_vetor * num_elementos;

//     printf("### tamanho de newExp em bytes: %d\n", tamanho_total);

//     printf("\nNEWEXP:");
//     printf("\n>>> conteudo: %s", newExp);
//     printf("\n>>> tamanho em bytes do ENDERECO inicial: %i", sizeof(newExp));
//     printf("\n>>> tamanho em bytes do CONTEUDO em newExp[0]: %i", sizeof((*newExp)));
//     printf("\n>>> comprimento da string iniciada em newExp[0]: %i\n\n", strlen(newExp));

//     printf("\n>>> tamanho em bytes da string inteira (w/ null): %i", sizeof(newExp)*sizeof(char));

//     if (exp[0] == '.' || exp[0] == '+' || exp[0] == '-')
//         printf("0%c", exp[0]);

//     int pos = 1;
//     while (exp[pos] != '\0') {
//         if (((exp[pos] == '+' || exp[pos] == '-') 
//         && (exp[pos-1] < '0' || exp[pos-1] > '9') && exp[pos-1] != ')')
//         || (exp[pos] == '.' && (exp[pos-1] < '0' || exp[pos-1] > '9')))
//         {
//             printf("0%c", exp[pos]);
//             // aloca em newExp o espaço de um char
//             // adiciona '0' nesse espaço
//             // aloca em newExp o espaço de um char
//             // adiciona exp[0] nesse espaço
//         }
//         else
//         {
//             printf("%c", exp[pos]);
//             // aloca em newExp o espaço de um char
//             // adiciona exp[0] nesse espaço
//         }
//         pos++;
//     }

//     return newExp;
// }

int main() {
    char vetor[] = "abc";
    int novo_tamanho = 5;

    char* novo_vetor = (char*) realloc(vetor, strlen(vetor) + 2 * sizeof(char));

    strcpy(novo_vetor, vetor); // WHY IS NOT WORKING??????????

    printf("length vetor = %i", strlen(vetor));
    printf("\n");
    // printf("vetor = %i", strlen(vetor)*sizeof(char));
    // printf("\n");
    printf("length novo_vetor = %i", novo_tamanho * sizeof(char));
    printf("\n");

    printf("vetor[0] = %c", vetor[0]);
    printf("\n");

    return 0;
}

/*

// if (hasSyntaxError(input)) exit(1);

    printf("sizeof(char*) = %i", sizeof(char));
    printf("\n");
    //char input[] = "-.5+35.9+42^56/(-(-74-(+5^2+9)*2))-20";
    //char newExp[] = "abcd";
    char *n;
    int qtdChars = 1;
    n = (char *) realloc(n, qtdChars*sizeof(char));

    //printf(">>> tamanho: %i bytes (com char null)\n", sizeof(input));

    printf("sizeof(n)   = %i", sizeof(n));
    printf("\n");

    //*newExp = (char*) realloc((*newExp), 6);
    n = (char *) realloc(n, 2*qtdChars*sizeof(char));

    printf("sizeof(n)+1 = %i", sizeof(n));
    printf("\n");

    //scanf("%4s", newExp);
    //printf("%s", newExp);

    // addZeroToSpecialCases(input, newExp);

*/