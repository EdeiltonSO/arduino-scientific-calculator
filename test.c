#include <stdio.h>
#include <stdlib.h>

int main() {

    // 2.1234567890000000112138423 quero apenas zeros depois do limite de precisão

    double a = atof("2.123456789123456789123456789");
    double b = atof("3.987654321");
    
    printf("\n>>> %.50lf\n", a+b);

    return 0;
}