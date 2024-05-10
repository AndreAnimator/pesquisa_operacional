#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int getMatrix(vector<vector<double>> matrix)
{
    // Inicilizando duas matrizes vazias
    vector<vector<double>> NIL(0, vector<double>(0, 0)), matrix;
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

    // Determinando o número de linhas e colunas da matriz
    int num_lines = matrix.size();
    int num_coluns = matrix[0].size(); // Assumindo que todas as linhas tenham o mesmo número de colunas

    if (num_lines != num_coluns) {
        cerr << "A matriz não é quadrada!" << endl;
        return 1;
    }

    return 0;
}

