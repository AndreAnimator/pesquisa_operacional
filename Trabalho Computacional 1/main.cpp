#include <iostream>
#include <iomanip>
#include <vector>
#include <clocale>
#include <fstream>
#include <sstream>

using namespace std;
int getMatrix(vector<vector<double>>& matrix);
double calcDeterminant(std::vector<std::vector<double>> matrix, int order);
void expandMatrix(vector<vector<double>>& matrix, int order);
void switchLines(vector<vector<double>>& matrix, int x_line, int y_line);
void switchColumns(vector<vector<double>>& matrix, int x_column, int y_column);
void printMatrix(vector<vector<double>> matrix);
int gaussJordanElimination(vector<vector<double>>& matrix, int order);

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
    expandMatrix(matrix, matrix.size());
    gaussJordanElimination(matrix, matrix.size());
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

void expandMatrix(vector<vector<double>>& matrix, int order)
{
    vector<vector<double>> new_matrix;
    vector<double> new_line;
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            new_line.push_back(matrix[i][j]);
        }
        for (int j = order; j < 2*order; j++) {
            if (j == i + order) {
                new_line.push_back(1);
            }
            else {
                new_line.push_back(0);
            }
        }
        new_matrix.push_back(new_line);
        new_line.clear();
    }

    //está fazendo com que a última linha identifique as colunas
    for (int i = 0; i < 2*order; i++)
    {
        new_line.push_back(i+1);
    }
    new_matrix.push_back(new_line);
    matrix = new_matrix;

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

void switchColumns(vector<vector<double>>& matrix, int x_column, int y_column)
{
    double aux;

    for (int i = 0; i < matrix.size(); i++)
    {
        aux = matrix[i][x_column];
        matrix[i][x_column] = matrix[i][y_column];
        matrix[i][y_column] = aux; 
    }
}

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

int gaussJordanElimination(vector<vector<double>>& matrix, int order)
{
    double multiply;
    /* 
        No loop das linhas está order-1 porque não queremos mexer na última linha 
        já que ela é a linha que identifica as colunas
    */
    for (int i = 0; i < order-1; i++) {
        // Troca linhas se pivo é zero
        if (matrix[i][i] == 0) {
            for (int j = i+1; j < order; j++) {
                if (matrix[j][i] != 0) {
                    switchLines(matrix, i, j);
                    break;
                }
            }
        }
        // Troca colunas se pivo continua sendo zero
        if (matrix[i][i] == 0) {
            for (int j = 0; j < 2*order; j++) {
                if (matrix[i][j] != 0) {
                    switchColumns(matrix, i, j);    
                    break;
                }
            }
        }
        if (matrix[i][i] == 0) {
            cout << "Matriz singular" << endl;
            return 1;
        }
        for (int j = 0; j < order; j++) {
            if (i != j) {
                multiply = matrix[j][i] / matrix[i][i];
                for (int k = 0; k < 2*order; k++) {
                    matrix[j][k] = matrix[j][k] - matrix[i][k] * multiply;
                }
            }
        }
    }

    for (int i = 0; i < order; i++) {
        multiply = matrix[i][i];
        for (int j = 0; j < 2*order; j++) {
            matrix[i][j] = matrix[i][j] / multiply;
        }
    }
    return 0;
}