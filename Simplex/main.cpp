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
    
    printf("\nComeça aqui o método simplex\n");

    // Variáveis auxiliares

    vector<double> newline;
    

    // --------------- pré FASE  1 ----------------

    // Dados do problema
    cout <<"Dados do problema: \n" << endl;
    vector<vector<double>> b;
    vector<vector<double>> A;
    vector<double> f;
    bool haveOneMax = false;
    cout << "ANTES DE LER\n";
    readInput(A, b, f, haveOneMax);

    cout << "DEPOIS DE LER\n";
    
    cout << "Matriz A: \n";
    printMatrix(A);
    cout << "\nMatriz b: \n";
    printMatrix(b);
    cout << "\nFunção f: \n";
    for(int i = 0; i < f.size(); i++){
        cout << f[i] << " ";
    }

    int iteracao = 0;
    // vetor de custos
    vector<vector<double>> x;
    // vetores de variáveis básicas
    vector<vector<double>> xtb;
    // vetores de variáveis não-básicas:
    vector<vector<double>> xtn;
    // vetor de indices não básicos
    vector<double> Nindices;
    // vetor de indices básicos
    vector<double> Bindices;

   // matriz de Básicos
    vector<vector<double>> B; 
    
    // matriz de Não Básicos
    vector<vector<double>> N;

    // vetor de custos:
    vector<vector<double>> c;
    for(int i = 0; i < f.size(); i++){
        newline.push_back(f[i]);
        c.push_back(newline);
        newline.clear();
    }
    cout << "Vetor de custos:\n";
    printMatrix(c);

    // custos básicos:
    vector<vector<double>> ctb;

    // custos não básicos:
    vector<vector<double>> ctn;

    // vetor multiplicador simplex

    vector<vector<double>> lambdaT;

    //----------------------------------------------------------------------------------

    // [FORMULAÇÃO DO PROBLEMA ARTIFICIAL]

    // m número de restrições do problema
    int m = A.size();
    cout << "\nNúmero de restrições: \n" << m << endl;
    // n número de variáveis do problema
    int n = f.size() - m;
    cout << "\nNúmero de variáveis: \n" << n << endl;
    // Função objetivo
    double minf = 0;
    for(int i = n; i < n + m; i++){
        minf += f[i];
    }
    cout << "\nFunção objetivo: " << minf << endl;
    // Restrições
    // Para cada restrição j = 1,..., m incluir uma variavel xn+j.
    for(int i = 0; i < n + m; i++){
        newline.push_back(f[i]);
        x.push_back(newline);
        newline.clear();
    }
    // Partição Básica inicial:
    for(int i = n + 1; i <= n + m; i++){
        Nindices.push_back(i);
    }
    for(int i = 1; i <= n; i++){
        Bindices.push_back(i);
    }
    // vetores de variáveis básicas e não-básicas:
    for(int i = 0; i < Bindices.size(); i++){
        newline.push_back(f[Bindices[i]-1]);
        xtb.push_back(newline);
        newline.clear();
    }
    for(int i = 0; i < Nindices.size(); i++){
        newline.push_back(f[Nindices[i]-1]);
        xtn.push_back(newline);
        newline.clear();
    }
    // custos básicos
    for(int i = 0; i < Bindices.size(); i++){
        newline.push_back(f[Bindices[i]-1]);
    }
    ctb.push_back(newline);
    newline.clear();
    // obtendo B a partir dos indices básicos
    cout << "\nBindices: \n" << endl;
    for(int i = 0; i < Bindices.size(); i++){
        cout << Bindices[i] << " ";
    }
    cout << "\nObtendo B: " << endl;
    for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < Bindices.size(); j++){
            newline.push_back(A[i][Bindices[j]-1]);
        }
        B.push_back(newline);
        newline.clear();
    }
    printMatrix(B);
    // Inverso da matriz B
    vector<vector<double>> BminusOne = invertMatrix(B);
    cout << "\nInverso de B:\n";
    printMatrix(BminusOne);
    cout << "\nObtendo N: " << endl;
    // obtendo N a partir dos indices não básicos
    for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < Nindices.size(); j++){
            newline.push_back(A[i][Nindices[j]-1]);
        }
        N.push_back(newline);
        newline.clear();
    }
    printMatrix(N);
    iteracao++;
    if (haveOneMax) {
        cout << "O problema tem uma maximização" << endl;
        // Vai para a fase 1

        //[INICIO DA ITERAÇÃO SIMPLEX - FASE 1]
        
        //Passo 1: cálculo da solução básica

        for(int i = 0; i < N.size(); i++){
            for(int j = 0; j < N[0].size(); j++){
                xtn[i][j] = 0;
            }
        }

        //vetor de solução básicas
        xtb = basicSolution(BminusOne, b);

        cout << "\nSolução básica: " << endl;
        printMatrix(xtb);

        //Passo 2: cálculo dos custos relativos
        // Passo 2.1 vetor multiplicador simplex
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

        // Passo 2.2 Custos relativos
        //Custos relativos:

        vector<vector<double>> a = obtainPartition(A, Nindices);

        
        cout << "\n Os valores de a: " << endl;
        printMatrix(a);

        //vector<vector<double>> lambdaT;
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

        //Passo 2.3 Determinação da variável a entrar na base:
        cout << "Determinação da variável a entrar na base:" << endl;

        //Passo 3
        //Passo 4
        //Passo 5
        //Passo 6
        //Fim da iteração simplex - Fase 1
    }
    // Fase 2
    cout << "O problema não tem uma max" << endl;
    // Vai para a fase 2
//    cout << "A matriz A é: \n";
//    printMatrix(A);
//    printMatrix(b);
    for (double iten : f) {
        cout << iten << " ";
    }

    cout << "\nPara aqui?" << endl;
    //Passo 1: cálculo da solução básica

    for(int i = 0; i < N.size(); i++){
        for(int j = 0; j < N[0].size(); j++){
            xtn[i][j] = 0;
        }
    }

    //vetor de solução básicas
    xtb = basicSolution(BminusOne, b);

    // cout << "\nInverso de B:\n";
    // printMatrix(BminusOne);
    // cout <<"\nMultiplicado por vetor b: \n";
    // printMatrix(b);

    cout << "\nSolução básica: " << endl;
    cout << "\nPara aqui?" << endl;
    printMatrix(xtb);
    cout << "\nPara aqui?" << endl;

    //Passo 2: cálculo dos custos relativos
    // Passo 2.1 vetor multiplicador simplex
    vector<vector<double>> ctb_row;
    // ctb_row = columnToRow(ctb, 0, 1); // pqq vira row?
    cout << "B: " << endl;
    printMatrix(BminusOne);
    cout << "Numero line size" << ctb_row.size() << endl;
    cout << "Numero col size" << BminusOne[0].size() << endl;
    vector<vector<double>> lambda = multiplyMatrices(ctb, BminusOne, ctb.size(), BminusOne[0].size());
    //resultado esperado: -3/2, 0, -1/2

    cout << "\n Vetor Multiplicador Simplex: " << endl;
    printMatrix(lambda);

    // Passo 2.2 Custos relativos
    //Custos relativos:

    vector<vector<double>> a = obtainPartition(A, Nindices);

    
    cout << "\n Os valores de a: " << endl;
    printMatrix(a);

    //vector<vector<double>> lambdaT;
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

    //Passo 2.3 Determinação da variável a entrar na base:
    cout << "Determinação da variável a entrar na base:" << endl;

    //Passo 3
    //Passo 4
    //Passo 5
    //Passo 6

// -------- A PARTIR DAQUI É SÓ ASNEIRA -------------------------

    // n + m tem q ser o tamanho da linha da tabela

    // com isso obtem-se as matrizes B e N
    // são obtidas a partir das colunas de A nos indíces de Bindices e Nindices respectivamente
    // depois eu faço isso kkk

    // Vetor das variáveis básicas
    // b é novamente a posição dos indices do vetor Bindices


    // vector<vector<double>> xn = {{0}, {0}};

    // ---PASSO 2---
    //cáculo dos custos relativos
 
    // for(int i = 0; i < Bindices.size(); i++){
    //     newline.push_back(c[Bindices[i] - 1][0]);
    //     ctb.push_back(newline);
    //     newline.clear();
    // }
    // cout << "\n Custos básicos: " << endl;
    // printMatrix(ctb);

    // cout << "\nAgora multiplica Custos Básicos: " << endl;
    // printMatrix(columnToRow(ctb, 0, 3));
    // cout << "\nMatrix B: " << endl;
    // B = obtainPartition(A, Bindices);
    // printMatrix(B);
    // cout << "\nPela matriz inversa de B: " << endl;
    // BminusOne = invertMatrix(B);
    // cout << "\n----------\n\n" << endl;
    // printMatrix(BminusOne);

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
    //printMatrix(xb);

    return 0;
}