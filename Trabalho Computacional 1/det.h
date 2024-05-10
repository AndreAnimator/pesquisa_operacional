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
