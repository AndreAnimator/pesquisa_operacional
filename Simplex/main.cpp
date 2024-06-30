#include <iostream>
#include <iomanip>
#include <vector>
#include <clocale>
#include <fstream>
#include <sstream>

#include "takeEntry.h"

using namespace std;

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
    // cout << "Quantidade de linhas: " << row_size << "Quantidade de linhas pegando da matriz: " << matrix_1[0].size() << endl;
    // cout << "Quantidade de colunas: " << col_size << "Quantidade de colunas pegando da matriz: " << matrix_2.size() << endl;
    if(matrix_1[0].size() == matrix_2.size()){
        for (int i = 0; i < matrix_1.size(); i++) {
            for (int j = 0; j < matrix_2[0].size(); j++) {
                aux = 0;
                for (int k = 0; k < matrix_2.size(); k++) {
                    // cout << "matrix_1 [" << i << " " << k << "] * [" << "matrix_2 " << k << " " << j << "]"<< endl;
                    // cout << matrix_1[i][k] << " * " << matrix_2[k][j] << " =" << endl;
                    aux += matrix_1[i][k] * matrix_2[k][j];
                    // cout << aux << endl;
                }
                new_line.push_back(aux);
            }
            result.push_back(new_line);
            new_line.clear();
        }
    }
    else
        cout << "Coluna e linha de tamanho diferente" << endl;
    return result;
}

vector<vector<double>> invertMatrix(vector<vector<double>> matrix){
    vector<vector<double>> original_matrix;
    vector<vector<double>> result;
    vector<vector<double>> matrix_checker;
    original_matrix = matrix;
    int determinant = calcDeterminant(matrix, matrix.size());
    // printf("Matriz original:");
    // printMatrix(original_matrix);
    // printf("\n");
    expandMatrix(matrix, matrix.size());
    gaussJordanElimination(matrix, matrix.size());
    // printf("O determinante da matriz é: %d \n", determinant);
    result = sliceMatrix(matrix, matrix.size());
    // printf("\nMatriz inversa: \n");
    // printMatrix(result);
    // printf("OIEEEEEEEEEE cade minha matriz\n");
    matrix_checker = multiplyMatrices(result, original_matrix, result.size(), original_matrix[0].size());
    // printf("Checando a matriz...");
    // printMatrix(matrix_checker);
    return result;
}

vector<vector<double>> basicSolution(vector<vector<double>> B, vector<vector<double>> b){
    // xB =(x3, x4, x5), ou seja Xb = (xn, ..., xm)
    // xB ← B^−1 * b (equivalentemente, resolva o sistema B ˆxB = b)
    // ˆxN ← 0
    // multiplyMatrices(B, xb, B.size(), xb.size());
    printf("\nEntrou no vetor\n");
    vector<vector<double>> result = multiplyMatrices(B, b, B.size(), b[0].size());
    return result;
}

vector<vector<double>> columnToRow(vector<vector<double>> column, int column_index, int column_size){
    vector<double> newline;
    vector<vector<double>> row;

    for(int i = 0; i < column_size; i++){
        newline.push_back(column[i][column_index]);
    }
    row.push_back(newline);
    
    return row;
}

vector<vector<double>> obtainPartition(vector<vector<double>> A, vector<double> indexes){
    vector<double> newline;
    vector<vector<double>> row;

    for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < indexes.size(); j++){
            newline.push_back(A[i][indexes[j] - 1]);
        }
        row.push_back(newline);
        newline.clear();
    }

    return row;
}

int main() {

    setlocale(LC_ALL, "Portuguese");
    
    printf("\nComeça aqui o método simplex");
    

    // --------------- pré FASE  1 ----------------

    // Dados do problema
    cout <<"Dados do problema: \n" << endl;
    vector<vector<double>> b;
    vector<vector<double>> A;
    vector<double> f;
    cout << "ANTES DE LER\n";
    readInput(A, b, f);
    cout << "A matriz A é: \n";
    printMatrix(A);
    printMatrix(b);
    for (double iten : f) {
        cout << iten << " ";
    }
    int n = 2; // tamanho dos Bindices
    int m = 4; // tamanho dos Nindices
    // n + m tem q ser o tamanho da linha da tabela

    // vetor de indices básicos
    vector<double> Bindices = {1, 4, 2};

    // vetor de indices não básicos
    vector<double> Nindices = {3, 5};

    // com isso obtem-se as matrizes B e N
    // são obtidas a partir das colunas de A nos indíces de Bindices e Nindices respectivamente
    // depois eu faço isso kkk
    vector<vector<double>> B = {{1, 0, 1}, {1, 1, -1}, {-1, 0, 1}}; 
    
    vector<vector<double>> N = {{1, 0}, {0, 0}, {0, 1}};

    // Vetor das variáveis básicas
    // b é novamente a posição dos indices do vetor Bindices
    vector<vector<double>> c = {{-1}, {-2}, {0}, {0}, {0}};

    // PERGUNTAR AO PROFESSOR SE É MELHOR FAZER SÓ UM VETOR
    // OU SE PODE SER SEPARADO EM CTB E CTN

    // --------------- FASE  1 ----------------
    // iteração

    // ---PASSO 1---
    //cáculo da solução básica

    //xb = x3, x4, x5
    //xb = B^-1*b
    //obtem-se:
    // vector<double> xb = {6, 4, 4};

    vector<vector<double>> BminusOne = invertMatrix(B);

    vector<vector<double>> xb = basicSolution(BminusOne, b);

    cout << "\nSolução básica: " << endl;
    printMatrix(xb);

    // vector<vector<double>> xn = {{0}, {0}};

    // ---PASSO 2---
    //cáculo dos custos relativos

    //vetor multiplicador simplex
    //c^tB = (cB1 , cB2 , cB3 ) = (c3, c4, c5) = (-1, 0, -2).
    vector<vector<double>> ctb;

    vector<double> newline;

    for(int i = 0; i < Bindices.size(); i++){
        newline.push_back(c[Bindices[i] - 1][0]);
        ctb.push_back(newline);
        newline.clear();
    }
    cout << "\n Custos básicos: " << endl;
    printMatrix(ctb);

    cout << "\nAgora multiplica Custos Básicos: " << endl;
    printMatrix(columnToRow(ctb, 0, 3));
    // cout << "\nMatrix B: " << endl;
    // B = obtainPartition(A, Bindices);
    // printMatrix(B);
    // cout << "\nPela matriz inversa de B: " << endl;
    // BminusOne = invertMatrix(B);
    // cout << "\n----------\n\n" << endl;
    // printMatrix(BminusOne);

    //Passo 2: {Cálculo dos custos relativos}
    //2.1 vetor multiplicador simplex
    vector<vector<double>> ctb_row;
    ctb_row = columnToRow(ctb, 0, 3);
    cout << "B: " << endl;
    printMatrix(BminusOne);
    cout << "Numero line size" << ctb_row.size() << endl;
    cout << "Numero col size" << BminusOne[0].size() << endl;
    vector<vector<double>> lambda = multiplyMatrices(ctb_row, BminusOne, ctb_row.size(), BminusOne[0].size());
    //resultado esperado: -3/2, 0, -1/2

    cout << "\n Vetor Multiplicador Simplex: " << endl;
    printMatrix(lambda);

    //2.2 Custos relativos
    //Custos relativos:

    vector<vector<double>> a = obtainPartition(A, Nindices);

    
    cout << "\n Os valores de a: " << endl;
    printMatrix(a);

    vector<vector<double>> lambdaT;
    for(int i = 0; i < lambda.size(); i++){
        newline.push_back(lambda[i][0]);
    }
    lambdaT.push_back(newline);
    newline.clear();
    cout << "\nValor multiplicado: " << endl;
    cout << "Multiplica: " << endl;
    printMatrix(lambda);
    cout << "por: ..." << endl;
    for(int i = 0; i < a[0].size(); i++){
        vector<vector<double>> a_aux;
        vector<vector<double>> multiply_aux;
        for(int j = 0; j < a.size(); j++){
            newline.push_back(a[j][i]);
            a_aux.push_back(newline);
            newline.clear();
        }
        cout << "O valor de a aux: " << endl;
        printMatrix(a_aux);
        multiply_aux = multiplyMatrices(lambda, a_aux, lambda.size(), a_aux[0].size());
        cout << "Quero saber o valor de multiply: " << multiply_aux[0][0] << endl;
        // printMatrix(c);
        // c[Nindices[i] - 1][0] = c[Nindices[i] - 1][0]; 
    }
    
    for(int i = 0; i < N.size(); i++){
        c[Nindices[i]-1][0] = c[Nindices[i]-1][0];
    }

    //2.3 Determinação da variável a entrar na base:

    bool hasSolution = false;
    bool min = N[0][0];

    for(int i; i < N[0].size(); i++){
        if(N[0][i] < min)
            min = N[0][i];
    }

    cout << "Valor mínimo é " << min << endl;
    if(min > 0){
        hasSolution = true;
    }
    printMatrix(xb);

    return 0;
}