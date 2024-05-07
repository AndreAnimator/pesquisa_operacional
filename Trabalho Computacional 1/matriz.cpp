#include<iostream>
#include "matriz.h"

using namespace std;

int main()
{
    float matriz[3][3] = {{2.0, 1.0, 4.0}, {0.0, 2.0, 1.0}, {3.0, 0.0, 5.0}};
    int ordem = sizeof matriz[0] / sizeof(int);

    printf("O valor da ordem eh: %d \n",ordem);

    calcularDeterminante(matriz, ordem);
    return 0;
}