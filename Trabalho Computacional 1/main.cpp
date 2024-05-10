#include <iostream>
#include <vector>
#include <clocale>
#include "det.h"
#include "fileOperations.cpp"

using namespace std;

int main() {

    setlocale(LC_ALL, "Portuguese");
    vector<vector<double>> matrix; 
    if (getMatrix(matrix) == 1) 
    {
        return 1;
    }
    int determinant = calcDeterminant(matrix, matrix.size());
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    printf("O determinante da matriz é: %d \n", determinant);
    // A leitura da matriz é feita normalmente matrix[linha][coluna]
    // Exemplo: matrix[0][0] retorna o primeiro elemento da matriz
    return 0;
}
