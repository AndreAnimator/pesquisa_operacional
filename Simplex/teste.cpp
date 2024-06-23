#include <iostream>
#include <vector>
#include <iomanip>
#include "takeEntry.h"

using namespace std;

void printMatrix(vector<vector<double>> matrix)
{
    int maxDigits = 0;
    // Procurando número máximo de dígitos na matriz
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            int digits = to_string(matrix[i][j]).length();
            if (digits > maxDigits) {
                maxDigits = digits;
            }
        }
    }

    // Imprimindo com espaços iguais
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << setw(maxDigits) << fixed << setprecision(5) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    vector<double> goal;
    vector<vector<double>> expressions;
    vector<double> b;
    solverInput(goal, expressions, b);
    // printMatrix(goal)
    printMatrix(expressions);
    // printMatrix(b);
    
}