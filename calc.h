#ifndef CALC_H
#define CALC_H

#include "defines.h"
#include "structures.h"

char* getErrorMessage(char);
char* getResultString(EXPRESSION_ELEMENT);
void calculate(char[], EXPRESSION_ELEMENT *);

// FUNÇÕES PRIVADAS:
// char hasSyntaxError(char[]);
// void addCharsToSpecialCases(char[], ARRAY *);
// ELEMENT_LIST transformCharToStruct(char[]);
// void createRPNStack(ELEMENT_LIST, EXPRESSION_ELEMENT*);
// char stackSolver(EXPRESSION_ELEMENT *, int, EXPRESSION_ELEMENT *);
// char countSizeAfterAddSpecialChars(char[]);
// char overflowError(EXPRESSION_ELEMENT, EXPRESSION_ELEMENT, char);
// EXPRESSION_ELEMENT operateTwoElements(EXPRESSION_ELEMENT, EXPRESSION_ELEMENT, EXPRESSION_ELEMENT);

#endif