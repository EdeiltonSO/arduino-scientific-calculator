# isso aqui é pra ser o readme, mas vamos com calma

## SOFTWARE

### Prioridade do scan
1. parênteses e funções
2. potencias
3. \* e /
4. \+ e -

### Etapas
- [ ] receber entrada
- [x] checar erros de sintaxe
- [x] add 0 antes dos +, - e . que precisarem
- [x] explodir a string
- [x] converter em vetor de structs (avaliar uso de atoi e atof)
- [ ] jogar structs numa pilha RPN
- [ ] resolver a pilha em RPN
- [ ] formatar dado de saída

```
string validada:
-5+35.9+42*56/(74-(5^2+9)*2)-20

string explodida:
['-', '5', '+', '35.9', '+', '42', '*', '56', '/', '(', '74', '-', '(', '5', '^', '2', '+', '9', ')', '*', '2', ')', '-', '20']
```

## HARDWARE

### Para implementação na versão atual

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

### Para implementação em versão futura

shift (pra inversa das funções abaixo)
- raiz
- sen
- cos
- tan
- e / ln
- log

`>>> 30 teclas até aqui`

## Pontos pra análise
- trocar o botão clear por segurar o backspace?
- ~~preciso adicionar 0 **depois** de +, - e .?~~ precisa, já foi tratado
- ~~number_int é suficientemente grande?~~ agora é
- como limitar a entrada de inteiros maiores que o permitido?
- o tamanho do lado esquerdo do float é um problema a ser resolvido?

## Issues
- tratar a entrada de números que parecem endereços IP
- verificar e tratar zeros à esquerda nos números
- limitar a entrada de mais de 6 casas decimais após a vírgula (?)
- tratar edge cases com indefinições
- tratar limitação do inteiro

## Outras decisões
- não haverá tecla pra constante pi (usuário digitará manualmente)
- não haverá tecla pra constante e (usuário fará e^1)
- por enquanto sem função pra integral definida

## Melhorias futuras
- permitir que o usuário calcule "1(1+1)"
- ~~permitir que o usuário calcule ".1" completando com zero~~ (done)
- ~~permitir que o usuário calcule "1+-+++---+-1"~~ (pensando bem, pra quê?)
- tratar sintax error pra press = com string vazia (ignorar tentativa)
- ler um float com "qualquer" precisão e calcular como notação científica
- mapear erros possíveis e separar em categorias (return 1, return 2, etc)
- alternar entre graus e radianos (quando tiver função trigonométrica)
- implementar raízes de índice N

## DOCUMENTAÇÃO

### explicar as flags desse char

```c
typedef struct EXPRESSION_ELEMENT {
    unsigned char flags; // [0 0000 000] => is_decimal[7] <not_used>[6:3] priority[2:0]
    union {
        char symbol_char;
        double number_double;
        int number_int;
    } content;
} EXPRESSION_ELEMENT;
```

### explicar essa estrutura

```c
typedef struct {
    int sizeOfList;
    EXPRESSION_ELEMENT* list;
} ELEMENT_LIST;
```

### explicar limitação de 6 casas decimais para float