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
vector<vector<double>> basicSolution(vector<vector<double>> B, vector<vector<double>> xb);
double functionGoalAvaliation(vector<double> c, vector<vector<double>> B, vector<double> x); //auxiliar da solução básica
vector<vector<double>> simpleMultiplierVector(vector<vector<double>> ctb, vector<vector<double>> B);
vector<double> relativeCosts(vector<double> c, vector<double> a, vector<double> lambdaT);
vector<double> baseVariableDetermination(vector<double> c, int n, int m);
bool otimalTest(vector<double> c);
double simplesDirection(vector<vector<double>> B, vector<vector<double>> a);
vector<vector<double>> columnToRow(vector<vector<double>> column, int column_index, int column_size);


int main() {

    setlocale(LC_ALL, "Portuguese");
    
    printf("\nComeça aqui o método simplex");
    

    // --------------- pré FASE  1 ----------------

    // Dados do problema
    vector<vector<double>> b = {{6}, {4}, {4}};
    // Função mínima
    vector<double> f = {-1, -2, 0, 0, 0};

    // com esses valores
    vector<double> xn = {0, 0};
    int n = 2;
    int m = 4;

    // obtem-se esses valores das variáveis básicas
    vector<double> Blinha = {3, 4, 5};

    // Eu não lembro o que eu tava tentando fazer acima dessa linha. LOL
    // x1 + x2 + x3 = 4
    // x1 + x4 = 3
    // x2 + x5 = 7/2
    vector<vector<double>> A = {{1, 0, 1, 1, 0}, {1, 1, -1, 0, 0}, {-1, 0, 1, 0, 1}};

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
    cout << "\nMatrix B: " << endl;
    printMatrix(B);
    cout << "\nPela matriz inversa de B: " << endl;
    printMatrix(BminusOne);

    //Passo 2: {Cálculo dos custos relativos}
    //2.1 vetor multiplicador simplex

    vector<vector<double>> lambda = simpleMultiplierVector(BminusOne, columnToRow(ctb, 0, 3));
    //resultado esperado: -3/2, 0, -1/2

    cout << "\n Vetor Multiplicador Simplex: " << endl;
    printMatrix(lambda);

    //2.2 Custos relativos
    //Custos relativos:

    vector<vector<double>> a;

    for(int i = 0; i < N.size(); i++){
        for(int j = 0; j < Nindices.size(); j++){
            newline.push_back(A[i][Nindices[j] - 1]);
        }
        a.push_back(newline);
        newline.clear();
    }
    cout << "\n Os valores de a: " << endl;
    printMatrix(a);

    for(int i = 0; i < N.size(); i++){
        c[Nindices[i]-1][0] = c[Nindices[i]-1][0];
    }
    vector<vector<double>> lambdaT;
    for(int i = 0; i < lambda.size(); i++){
        newline.push_back(lambda[i][0]);
    }
    lambdaT.push_back(newline);
    newline.clear();
    cout << "\nValor multiplicado: " << endl;
    printMatrix(multiplyMatrices(lambdaT, a, lambdaT.size(), a[0].size()));

    //2.3 Determinação da variável a entrar na base:

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
    cout << "Quantidade de linhas: " << row_size << "\nQuantidade de colunas: " << col_size << endl;
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

vector<vector<double>> basicSolution(vector<vector<double>> B, vector<vector<double>> b){
    // xB =(x3, x4, x5), ou seja Xb = (xn, ..., xm)
    // xB ← B^−1 * b (equivalentemente, resolva o sistema B ˆxB = b)
    // ˆxN ← 0
    // multiplyMatrices(B, xb, B.size(), xb.size());
    printf("\nEntrou no vetor\n");
    vector<vector<double>> result = multiplyMatrices(B, b, B.size(), b[0].size());
    return result;
}

vector<vector<double>> simpleMultiplierVector(vector<vector<double>> ctb, vector<vector<double>> B){
    //funcao pra calcular transposta
    // cb = Bt * lambda
    return multiplyMatrices(B, ctb, B.size(), ctb[0].size());
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

vector<vector<double>> columnToRow(vector<vector<double>> column, int column_index, int column_size){
    vector<double> newline;
    vector<vector<double>> row;

    for(int i = 0; i < column_size; i++){
        newline.push_back(column[i][column_index]);
    }
    row.push_back(newline);
    
    return row;
}