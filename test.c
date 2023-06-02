#include <stdio.h>
#include <stdlib.h>

double charToDouble(char charNum[]) {

    double doubleNum = atof(charNum);

    return doubleNum;
}

int main() {
    char num[] = "3.141867564534232456210";
    double value = atof(num);
    printf("Function: atof(\"%s\") = %e\n", num, value);

    return 0;
}