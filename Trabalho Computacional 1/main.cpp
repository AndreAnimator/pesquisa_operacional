#include <iostream>
#include <vector>
#include <clocale>
#include <fstream>
#include <sstream>

using namespace std;
int getMatrix(vector<vector<double>>& matrix);
double calcDeterminant(std::vector<std::vector<double>> matrix, int order);
void switchLines(vector<vector<double>>& matrix, int x_line, int y_line);
void printMatrix(vector<vector<double>> matrix);

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

int getMatrix(vector<vector<double>>& matrix)
{
    // Inicilizando duas matrizes vazias
    string line;
    ifstream file("matriz.txt");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    // Leitura do arquivo linha a linha
    while (getline(file, line)) {
        vector<double> matrix_line;
        stringstream ss(line);
        double valor;

        // Tratando todos os elementos como das linhas como double
        while (ss >> valor) {
            matrix_line.push_back(valor);
        }
        matrix.push_back(matrix_line);
    }
    file.close();
    return 0;
}

double calcDeterminant(std::vector<std::vector<double>> matrix, int order)
{
    double multiply;
    double determinant = 1;

    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            multiply = matrix[j][i] / matrix[i][i];
            for (int k = 0; k < order; k++) {
                if (i != j)
                    matrix[j][k] = matrix[j][k] - matrix[i][k] * multiply;
            }
        }
    }

    for (int i = 0; i < order; i++) {
        determinant *= matrix[i][i];
    }

    return determinant;
}

void switchLines(vector<vector<double>>& matrix, int x_line, int y_line)
{
    double aux;

    for (int i = 0; i < matrix.size(); i++)
    {
        aux = matrix[x_line][i];
		matrix[x_line][i] = matrix[y_line][i];
		matrix[y_line][i] = aux; 
    }
}

void printMatrix(vector<vector<double>> matrix)
{
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

