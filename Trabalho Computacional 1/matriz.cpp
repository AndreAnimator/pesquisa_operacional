#include<iostream>
#include "det.h"

using namespace std;

int main()
{
    float matriz[4][4] ={
    {-2.0, -3.0, -1.0, -2.0}, 
    {-1.0, 0.0, 1.0, -2.0}, 
    {-3.0, -1.0, -4.0, 1.0}, 
    {-2.0, 2.0, -3.0, -1.0}
    };
    int ordem = sizeof matriz[0] / sizeof(int);

    printf("O valor da ordem eh: %d \n",ordem);

    calcDeterminant(matriz, ordem);
    return 0;
}