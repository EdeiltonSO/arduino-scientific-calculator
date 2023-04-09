### isso aqui é pra ser o README mas vamos com calma

3 + 4 * 5 / ( 74 - 71 ) - 1

                -
         +             1
   3           /
            *     -
           4 5   7 2

prioridade de busca:
1. parênteses e funções
2. potencias
3. * e /
4. + e -

validar expressao

separar a string de entrada em um vetor de strings:
-5+35.9+42*56/(74-(5^2+9)*2)-20
['-', '5', '+', '35.9', '+', '42', '*', '56', '/', '(', '74', '-', '(', '5', '^', '2', '+', '9', ')', '*', '2', ')', '-', '20']

converter vetor pra notação polonesa reversa (RPN)
ler a expressão em RPN e usar uma pilha pras operações
devolver o resultado

RESUMO

acho que por enquanto precisamos:

1. validar a expressão
2. separar a string em um vetor
3. converter pra notação polonesa reversa
4. usar uma pilha pra operar a string em RPN
5. retornar o que sobrar na pilha

