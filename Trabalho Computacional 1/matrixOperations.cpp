#include <iostream>
#include <vector>

using namespace std;
#include <vector>

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

void gauss(double **matrix, int tamanho_da_matrix, int linha_do_pivo)
{
	double multiplicador = 1;

	for (int i = linha_do_pivo + 1; i < tamanho_da_matrix; i++)
	{
		multiplicador = matrix[i][linha_do_pivo] / matrix[linha_do_pivo][linha_do_pivo];
		for (int j = 0; j <= tamanho_da_matrix; j++)
		{
			matrix[i][j] = matrix[i][j] - multiplicador * matrix[linha_do_pivo][j];
		}
	}
}