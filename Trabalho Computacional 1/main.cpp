#include <iostream>
#include <vector>
#include <clocale>
#include "fileOperations.cpp"
#include "matrixOperations.cpp"

using namespace std;

int main() {

    setlocale(LC_ALL, "Portuguese");
    vector<vector<double>> matrix; 
    if (getMatrix(matrix) == 1) 
    {
        return 1;
    }
    int determinant = calcDeterminant(matrix, matrix.size());
    printMatrix(matrix);
    printf("\n");
    switchLines(matrix, 0, 3);
    printMatrix(matrix);
    printf("O determinante da matriz é: %d \n", determinant);
    // A leitura da matriz é feita normalmente matrix[linha][coluna]
    // Exemplo: matrix[0][0] retorna o primeiro elemento da matriz
    return 0;
}
