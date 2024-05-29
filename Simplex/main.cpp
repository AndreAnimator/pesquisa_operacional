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
vector<vector<double>> invertMatrix(vector<vector<double>> matrix);
vector<vector<double>> basicSolution(vector<vector<double>> B, vector<vector<double>> xb, int n, int m);
double functionGoalAvaliation(vector<double> c, vector<vector<double>> B, vector<double> x); //auxiliar da solução básica
vector<vector<double>> simpleMultiplierVector(vector<vector<double>> B, vector<vector<double>> lambda);
vector<double> relativeCosts(vector<double> c, vector<double> a, vector<double> lambdaT);
vector<double> baseVariableDetermination(vector<double> c, int n, int m);
bool otimalTest(vector<double> c);
double simplesDirection(vector<vector<double>> B, vector<vector<double>> a);


int main() {

    setlocale(LC_ALL, "Portuguese");

    /*
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
    printf("OIEEEEEEEEEE cade minha matriz\n");
    matrix_checker = multiplyMatrices(result, original_matrix, result.size(), original_matrix[0].size());
    printMatrix(matrix_checker);
    */

    // A leitura da matriz é feita normalmente matrix[linha][coluna]
    // Exemplo: matrix[0][0] retorna o primeiro elemento da matriz
    printf("\nComeça aqui o método simplex");
    

    // --------------- FASE  1 ----------------

    // Dados do problema
    vector<double> x1 = {1, 1, 1, 0, 0};
    vector<double> x2 = {1, -1, 0, 1, 0};
    vector<double> x3 = {-1, 1, 0, 0, 1};
    vector<vector<double>> b = {{6, 4, 4}};
    // Função mínima
    vector<double> f = {-1, -2, 0, 0, 0};
    vector<vector<double>> A = {x1, x2, x3};

    // com esses valores
    vector<double> xn = {0, 0};
    int n = 2;
    int m = 4;

    // obtem-se esses valores das variáveis básicas
    vector<double> Blinha = {3, 4, 5};
    vector<vector<double>> B = {{3}, {4}, {5}};
    vector<double> N = {1, 2};

    // --------------- FASE  2 ----------------
    
    // iteração

    // ---PASSO 1---
    //cáculo da solução básica

    //xb = x3, x4, x5
    //xb = B^-1*b
    //obtem-se:
    // vector<double> xb = {6, 4, 4};

    vector<vector<double>> xb = basicSolution(B, b, 2, 4);
    printf("Resultado esperado: xb = {6, 4, 4};\n");
    printMatrix(xb);

    //avaliação da função objetivo
    //f(x) = cb1xb1 + cb2xb2 + cb3xb3 = 0*6 + 0*4 + 0*4

    // ---PASSO 2---
    //cáculo dos custos relativos

    //c^tB = (cB1 , cB2 , cB3 ) = (c3, c4, c5) = (0, 0, 0).

    vector<double> c = {0, 0, 0};


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

vector<vector<double>> invertMatrix(vector<vector<double>> matrix){
    vector<vector<double>> original_matrix;
    vector<vector<double>> result;
    vector<vector<double>> matrix_checker;
    original_matrix = matrix;
    int determinant = calcDeterminant(matrix, matrix.size());
    printf("Matriz original:");
    printMatrix(original_matrix);
    printf("\n");
    expandMatrix(matrix, matrix.size());
    gaussJordanElimination(matrix, matrix.size());
    printf("O determinante da matriz é: %d \n", determinant);
    result = sliceMatrix(matrix, matrix.size());
    printf("\nMatriz inversa: \n");
    printMatrix(result);
    printf("OIEEEEEEEEEE cade minha matriz\n");
    matrix_checker = multiplyMatrices(result, original_matrix, result.size(), original_matrix[0].size());
    printf("Checando a matriz...");
    printMatrix(matrix_checker);
    return result;
}

vector<vector<double>> basicSolution(vector<vector<double>> B, vector<vector<double>> b, int n, int m){
    // xB =(x3, x4, x5), ou seja Xb = (xn, ..., xm)
    // xB ← B^−1 * b (equivalentemente, resolva o sistema B ˆxB = b)
    // ˆxN ← 0
    // multiplyMatrices(B, xb, B.size(), xb.size());
    printf("\nEntrou no vetor\n");
    vector<vector<double>> xb = multiplyMatrices(B, b, B.size(), b.size());
    printf("Multiplicação de B por b:\n");
    printMatrix(xb);
    vector<vector<double>> result;
    vector<double> new_line;
    for(int i = 0; i < xb.size(); i++){
        for(int j = n; j <= m; j++){
            new_line.push_back(xb[i][j]);
        }
        result.push_back(new_line);
        new_line.clear();
    }
    printf("\nResultado: \n");
    printMatrix(result);
    return result;
}

vector<vector<double>> simpleMultiplierVector(vector<vector<double>> B, vector<vector<double>> lambda){
    //funcao pra calcular transposta
    // cb = Bt * lambda
    return multiplyMatrices(B, lambda, B.size(), lambda.size());
}

vector<double> relativeCosts(vector<double> c, vector<double> a, vector<double> lambdaT){
    //cNj ← cNj − λT aNj j = 1, 2, ..., n − m
    return a;
}
vector<double> baseVariableDetermination(vector<double> c, int n, int m){
    // cNk ← min. {ˆcNj , j = 1, 2, ..., n − m} (a variavel xNk entra na base)
    return c;
}
bool otimalTest(vector<double> c){
    // Se ˆcNk ≥ 0, ent˜ao: pare {solu¸c˜ao na itera¸c˜ao atual ´e ´otima}
    return true;
}
double simplesDirection(vector<vector<double>> B, vector<vector<double>> a){
    // y ← B−1aNk (equivalentemente, resolva o sistema: By = aNk )
    return 0.1;
}