# isso aqui é pra ser o README mas vamos com calma

## Software

### Prioridade do scan
1. parênteses e funções
2. potencias
3. \* e /
4. \+ e -

### Etapas
- receber entrada
- checar erros de sintaxe
- add zero antes e depois de + e - se necessário
- explodir a string
- converter em vetor de structs
- jogar structs numa pilha RPN
- resolver a pilha em RPN
- formatar dado de saída

```
string validada:
-5+35.9+42*56/(74-(5^2+9)*2)-20

string explodida:
['-', '5', '+', '35.9', '+', '42', '*', '56', '/', '(', '74', '-', '(', '5', '^', '2', '+', '9', ')', '*', '2', ')', '-', '20']
```

## HARDWARE

### Teclas

- on/off: chave (direto no vcc)

- clear
- backspace
- direcional E
- direcional D

```
1 2 3 4 5 6 7 8 9 0
. + - * / ^ ( ) =
```

`>>> 23 teclas até aqui`

shift (pra inversa das funções abaixo)
- raiz
- sen
- cos
- tan
- e / ln
- log

`>>> 30 teclas até aqui`

## Pontos pra análise
- como responder entradas vazias?
- como alternar entre graus e radianos?
- como implementar raízes de índice N?
- trocar o botão clear por segurar o backspace?

## Outras decisões
- não haverá tecla pra constante pi (usuário digitará manualmente)
- não haverá tecla pra constante e (usuário fará e^1)
- por enquanto sem função pra integral definida

## Melhorias futuras
- permitir que o usuário calcule "1(1+1)"