#include <iostream>
#include <iomanip>
#include <vector>
#include <clocale>
#include <fstream>
#include <sstream>

using namespace std;

int getMatrix(vector<vector<double>>& matrix);
double calcDeterminant(vector<vector<double>> matrix, int order);
void expandMatrix(vector<vector<double>>& matrix, int order);
void switchLines(vector<vector<double>>& matrix, int x_line, int y_line);
void switchColumns(vector<vector<double>>& matrix, int x_column, int y_column);
void printMatrix(vector<vector<double>> matrix);
int gaussJordanElimination(vector<vector<double>>& matrix, int order);
vector<vector<double>> sliceMatrix(vector<vector<double>>& matrix, int order);
vector<vector<double>> multiplyMatrices(vector<vector<double>>& matrix_1, vector<vector<double>>& matrix_2, int line_size, int col_size);


int main() {

    setlocale(LC_ALL, "Portuguese");
    vector<vector<double>> original_matrix;
    vector<vector<double>> matrix;
    vector<vector<double>> result;
    vector<vector<double>> matrix_checker;
    if (getMatrix(matrix) == 1) 
    {
        return 1;
    }
    original_matrix = matrix;
    int determinant = calcDeterminant(matrix, matrix.size());
    printMatrix(matrix);
    printf("\n");
    expandMatrix(matrix, matrix.size());
    gaussJordanElimination(matrix, matrix.size());
    printMatrix(matrix);
    printf("O determinante da matriz é: %d \n", determinant);
    result = sliceMatrix(matrix, matrix.size());
    printMatrix(result);
    matrix_checker = multiplyMatrices(result, original_matrix, result.size(), original_matrix[0].size());
    printMatrix(matrix_checker);
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
    for (int i = 0; i < matrix.size(); i++)
    {
        if (matrix.size() != matrix[i].size())
        {
            cerr << "Matriz não é quadrada!" << endl;
            return 1;
        }
    }
    file.close();
    return 0;
}

double calcDeterminant(vector<vector<double>> matrix, int order) {
    double determinant = 1;

    for (int i = 0; i < order; i++) {
        if (matrix[i][i] == 0) {
            bool swapped = false;
            for (int k = i + 1; k < order; k++) {
                if (matrix[k][i] != 0) {
                    switchLines(matrix, i, k);
                    determinant *= -1; // Muda o sinal do determinante
                    swapped = true;
                    break;
                }
            }
            if (!swapped) {
                // Se não é possível trocar a linha, a matriz é singular
                return 0;
            }
        }

        for (int j = i + 1; j < order; j++) {
            double multiplier = matrix[j][i] / matrix[i][i];
            for (int k = i; k < order; k++) {
                matrix[j][k] -= matrix[i][k] * multiplier;
            }
        }
    }

    // Calculate the determinant by multiplying the diagonal elements
    for (int i = 0; i < order; i++) {
        determinant *= matrix[i][i];
    }

    cout << endl << "DETERMINANTEEEE " << determinant << endl;
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
    // for (int i = 0; i < 2*order; i++)
    // {
    //     new_line.push_back(i+1);
    // }
    //new_matrix.push_back(new_line);
    matrix = new_matrix;

}

void switchLines(vector<vector<double>>& matrix, int x_line, int y_line)
{
    double aux;

    cout << endl << "Tamanho da linha a ser trocada: " << matrix[0].size() << endl;

    for (int i = 0; i < matrix[0].size(); i++)
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
    for (int i = 0; i < order; i++) {
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
        // if (matrix[i][i] == 0) {
        //     for (int j = 0; j < 2*order; j++) {
        //         if (matrix[i][j] != 0) {
        //             switchColumns(matrix, i, j);    
        //             break;
        //         }
        //     }
        // }
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
        printMatrix(matrix);
        cout << endl;
    }

    for (int i = 0; i < order; i++) {
        multiply = matrix[i][i];
        for (int j = 0; j < 2*order; j++) {
            matrix[i][j] = matrix[i][j] / multiply;
        }
    }
    return 0;
}

vector<vector<double>> sliceMatrix(vector<vector<double>>& matrix, int order)
{
    vector<vector<double>> new_matrix;
    vector<double> new_line;
    for (int i = 0; i < order; i++) {
        for (int j = order; j < order*2; j++) {
            new_line.push_back(matrix[i][j]);
        }
        new_matrix.push_back(new_line);
        new_line.clear();
    }

    return new_matrix;
}

vector<vector<double>> multiplyMatrices(vector<vector<double>>& matrix_1, vector<vector<double>>& matrix_2, int row_size, int col_size)
{
    vector<vector<double>> result;
    double aux;
    vector<double> new_line;
    cout << "Quantidade de linhas: " << row_size << "\nQuantidade de colunas" << col_size << endl;
    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            aux = 0;
            for (int k = 0; k < row_size; k++) {
                aux += matrix_1[i][k] * matrix_2[k][j];
            }
            new_line.push_back(aux);
        }
        result.push_back(new_line);
        new_line.clear();
    }
    return result;
}