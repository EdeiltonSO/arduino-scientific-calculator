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
- como limitar a entrada de inteiros maiores que o permitido?
- o tamanho do lado esquerdo do float é um problema a ser resolvido?
- limitar a entrada de mais de 6 casas decimais após a vírgula?
- o que acontece com a ordem de precedência ao adicionar o zero em `-3.5*15`?

## Outras decisões
- não haverá tecla pra constante pi (usuário digitará manualmente)
- não haverá tecla pra constante e (usuário fará e^1)
- por enquanto sem função pra integral definida

## Melhorias futuras
- tratar sintax error pra press = com string vazia (ignorar tentativa)
- ler um float com "qualquer" precisão e calcular como notação científica
- mapear erros possíveis e separar em categorias (return 1, return 2, etc)
- alternar entre graus e radianos (quando tiver função trigonométrica)
- permitir que o usuário calcule "1(1+1)"
- implementar raízes de índice N

## Documentação
- explicar limitação de 6 casas decimais para float
- fazer tabela com prioridades dos elementos
- explicar flags do `EXPRESSION_ELEMENT`
- explicar estrutura `ELEMENT_LIST`

## Notas

Bit  | Finalidade
---: | ------------------------
7    | número decimal
6    | último elemento da pilha RPN
5:3  | (não utilizado)
2:0  | prioridade de símbolo

Símbolo  | Flag | Prioridade
-------: | :--: | ------------
\+ -     | 001  | 1
\* /     | 010  | 2
^        | 011  | 3
( )      | 100  | 4

A prioridade de qualquer número é sempre **zero**.