#include <iostream>
#include <iomanip>
#include <vector>
#include <clocale>
#include <fstream>
#include <sstream>

#include "takeEntry.h"

using namespace std;

int getMatrix(vector<vector<double>> &matrix)
{
    // Inicilizando duas matrizes vazias
    string line;
    ifstream file("matriz.txt");
    if (!file.is_open())
    {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    // Leitura do arquivo linha a linha
    while (getline(file, line))
    {
        vector<double> matrix_line;
        stringstream ss(line);
        double valor;

        // Tratando todos os elementos como das linhas como double
        while (ss >> valor)
        {
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

void switchLines(vector<vector<double>> &matrix, int x_line, int y_line)
{
    double aux;

    for (int i = 0; i < matrix[0].size(); i++)
    {
        aux = matrix[x_line][i];
        matrix[x_line][i] = matrix[y_line][i];
        matrix[y_line][i] = aux;
    }
}

double calcDeterminant(vector<vector<double>> matrix, int order)
{
    double determinant = 1;

    for (int i = 0; i < order; i++)
    {
        if (matrix[i][i] == 0)
        {
            bool swapped = false;
            for (int k = i + 1; k < order; k++)
            {
                if (matrix[k][i] != 0)
                {
                    switchLines(matrix, i, k);
                    determinant *= -1; // Muda o sinal do determinante
                    swapped = true;
                    break;
                }
            }
            if (!swapped)
            {
                // Se não é possível trocar a linha, a matriz é singular
                return 0;
            }
        }

        for (int j = i + 1; j < order; j++)
        {
            double multiplier = matrix[j][i] / matrix[i][i];
            for (int k = i; k < order; k++)
            {
                matrix[j][k] -= matrix[i][k] * multiplier;
            }
        }
    }

    // Calculate the determinant by multiplying the diagonal elements
    for (int i = 0; i < order; i++)
    {
        determinant *= matrix[i][i];
    }

    // cout << endl << "DETERMINANTE:  " << determinant << endl;
    return determinant;
}

void expandMatrix(vector<vector<double>> &matrix, int order)
{
    vector<vector<double>> new_matrix;
    vector<double> new_line;
    for (int i = 0; i < order; i++)
    {
        for (int j = 0; j < order; j++)
        {
            new_line.push_back(matrix[i][j]);
        }
        for (int j = order; j < 2 * order; j++)
        {
            if (j == i + order)
            {
                new_line.push_back(1);
            }
            else
            {
                new_line.push_back(0);
            }
        }
        new_matrix.push_back(new_line);
        new_line.clear();
    }

    // está fazendo com que a última linha identifique as colunas
    //  for (int i = 0; i < 2*order; i++)
    //  {
    //      new_line.push_back(i+1);
    //  }
    // new_matrix.push_back(new_line);
    matrix = new_matrix;
}
void switchColumns(vector<vector<double>> &matrix, int x_column, int y_column)
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
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            int digits = to_string(matrix[i][j]).length();
            if (digits > maxDigits)
            {
                maxDigits = digits;
            }
        }
    }

    // Imprimindo com espaços iguais
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            cout << setw(maxDigits) << fixed << setprecision(5) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int gaussJordanElimination(vector<vector<double>> &matrix, int order)
{
    double multiply;
    /*
        No loop das linhas está order-1 porque não queremos mexer na última linha
        já que ela é a linha que identifica as colunas
    */
    for (int i = 0; i < order; i++)
    {
        // Troca linhas se pivo é zero
        if (matrix[i][i] == 0)
        {
            for (int j = i + 1; j < order; j++)
            {
                if (matrix[j][i] != 0)
                {
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
        if (matrix[i][i] == 0)
        {
            cout << "Matriz singular" << endl;
            return 1;
        }
        for (int j = 0; j < order; j++)
        {
            if (i != j)
            {
                multiply = matrix[j][i] / matrix[i][i];
                for (int k = 0; k < 2 * order; k++)
                {
                    matrix[j][k] = matrix[j][k] - matrix[i][k] * multiply;
                }
            }
        }
        cout << endl;
    }

    for (int i = 0; i < order; i++)
    {
        multiply = matrix[i][i];
        for (int j = 0; j < 2 * order; j++)
        {
            matrix[i][j] = matrix[i][j] / multiply;
        }
    }
    return 0;
}

vector<vector<double>> sliceMatrix(vector<vector<double>> &matrix, int order)
{
    vector<vector<double>> new_matrix;
    vector<double> new_line;
    for (int i = 0; i < order; i++)
    {
        for (int j = order; j < order * 2; j++)
        {
            new_line.push_back(matrix[i][j]);
        }
        new_matrix.push_back(new_line);
        new_line.clear();
    }

    return new_matrix;
}

vector<vector<double>> multiplyMatrices(vector<vector<double>> &matrix_1, vector<vector<double>> &matrix_2, int row_size, int col_size)
{
    vector<vector<double>> result;
    double aux;
    vector<double> new_line;
    // cout << "Quantidade de linhas: " << row_size << "Quantidade de linhas pegando da matriz: " << matrix_1[0].size() << endl;
    // cout << "Quantidade de colunas: " << col_size << "Quantidade de colunas pegando da matriz: " << matrix_2.size() << endl;
    if (matrix_1[0].size() == matrix_2.size())
    {
        for (int i = 0; i < matrix_1.size(); i++)
        {
            for (int j = 0; j < matrix_2[0].size(); j++)
            {
                aux = 0;
                for (int k = 0; k < matrix_2.size(); k++)
                {
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

vector<vector<double>> invertMatrix(vector<vector<double>> matrix)
{
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

vector<vector<double>> basicSolution(vector<vector<double>> B, vector<vector<double>> b)
{
    // xB =(x3, x4, x5), ou seja Xb = (xn, ..., xm)
    // xB ← B^−1 * b (equivalentemente, resolva o sistema B ˆxB = b)
    // ˆxN ← 0
    // multiplyMatrices(B, xb, B.size(), xb.size());
    printf("\nEntrou no vetor\n");
    vector<vector<double>> result = multiplyMatrices(B, b, B.size(), b[0].size());
    return result;
}

vector<vector<double>> columnToRow(vector<vector<double>> column, int column_index, int column_size)
{
    vector<double> newline;
    vector<vector<double>> row;

    for (int i = 0; i < column_size; i++)
    {
        newline.push_back(column[i][column_index]);
    }
    row.push_back(newline);

    return row;
}

vector<vector<double>> obtainPartition(vector<vector<double>> A, vector<int> indexes)
{
    vector<double> newline;
    vector<vector<double>> row;

    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < indexes.size(); j++)
        {
            newline.push_back(A[i][indexes[j] - 1]);
        }
        row.push_back(newline);
        newline.clear();
    }

    return row;
}

int main()
{
    vector<double> newline;
    // --------------- pré FASE  1 ----------------
    cout << "Dados do problema: " << endl;
    vector<vector<double>> A; // matriz de restrições
    vector<vector<double>> B; // matriz de Básicos
    vector<vector<double>> N; // matriz de Não Básicos
    vector<int> Bindices;
    vector<int> Nindices;
    vector<vector<double>> b; // Matriz de restrições
    vector<double> f;         // Função objetivo
    bool haveOneMax = false;
    readInput(A, N, B, Bindices, Nindices, b, f, haveOneMax);
    cout << "Matriz A:" << endl;
    printMatrix(A);
    cout << "Matriz B: \n";
    printMatrix(B);
    cout << "Matriz N: \n";
    printMatrix(N);
    cout << "Indices de B: \n";
    for (int i = 0; i < Bindices.size(); i++)
    {
        cout << Bindices[i] << " ";
    }
    cout << "\nIndices de N: \n";
    for (int i = 0; i < Nindices.size(); i++)
    {
        cout << Nindices[i] << " ";
    }
    cout << "\nMatriz b: \n";
    printMatrix(b);
    cout << "\nFunção f: \n";
    for (int i = 0; i < f.size(); i++)
    {
        cout << f[i] << " ";
    }

    // ----------------- FASE 1 -------------------
    int iteracao = 0;
    vector<vector<double>> xtb; // vetores de variáveis básicas
    vector<vector<double>> xtn; // vetores de variáveis não-básicas
    vector<vector<double>> c;   // vetor de custos
    for (int i = 0; i < f.size(); i++)
    {
        newline.push_back(f[i]);
        c.push_back(newline);
        newline.clear();
    }
    cout << "\nVetor de custos:\n";
    printMatrix(c);
    vector<vector<double>> x;       // vetor de variáveis
    vector<vector<double>> ctb;     // custos básicos:
    vector<vector<double>> ctn;     // custos não básicos:
    vector<vector<double>> lambdaT; // vetor multiplicador simplex

    // [FORMULAÇÃO DO PROBLEMA ARTIFICIAL]
    int m = B.size();
    cout << "Número de restrições: \n"
         << m << endl;
    int n = f.size();
    for (int i = 0; i < n + m; i++)
    {
        newline.push_back(f[i]);
        x.push_back(newline);
        newline.clear();
    }

    // vetores de variáveis básicas e não-básicas:
    for (int i = 0; i < Bindices.size(); i++)
    {
        newline.push_back(f[Bindices[i] - 1]);
        xtb.push_back(newline);
        newline.clear();
    }
    for (int i = 0; i < Nindices.size(); i++)
    {
        newline.push_back(f[Nindices[i] - 1]);
        xtn.push_back(newline);
        newline.clear();
    }
    // custos básicos
    for (int i = 0; i < Nindices.size(); i++)
    {
        newline.push_back(f[Nindices[i] - 1]);
    }
    ctb.push_back(newline);
    newline.clear();
    // custos não básicos
    for (int i = 0; i < Bindices.size(); i++)
    {
        newline.push_back(f[Bindices[i] - 1]);
    }
    ctn.push_back(newline);
    newline.clear();

    printf("\nCustos básicos:\n");
    printMatrix(ctb);
    printf("\nCustos não básicos:\n");
    printMatrix(ctn);

    vector<vector<double>> BminusOne = invertMatrix(B); // Inverso da matriz B
    cout << "\nInverso de B:\n";
    printMatrix(BminusOne);
    iteracao++;
    // while (true)
    // {
    vector<vector<double>> lambda = multiplyMatrices(ctb, BminusOne, ctb.size(), BminusOne[0].size());
    cout << "\n Vetor Multiplicador Simplex: " << endl;
    printMatrix(lambda);

    vector<vector<double>> a = obtainPartition(A, Nindices);

    cout << "\n Os valores de a: " << endl;
    printMatrix(a);

    vector<vector<double>> reduced_costs = c;
    for (int i = 0; i < a[0].size(); i++)
    {
        vector<vector<double>> a_aux;
        vector<vector<double>> multiply_aux;
        for (int j = 0; j < a.size(); j++)
        {
            newline.push_back(a[j][i]);
            a_aux.push_back(newline);
            newline.clear();
        }

        cout << "O valor de a aux: " << endl;
        printMatrix(a_aux);
        multiply_aux = multiplyMatrices(lambda, a_aux, lambda.size(), a_aux[0].size());
        cout << "Quero saber o valor de multiply: " << multiply_aux[0][0] << endl;
        reduced_costs[Nindices[i] - 1][0] = c[Nindices[i] - 1][0] - multiply_aux[0][0];
    }
    cout << "Custos reduzidos: " << endl;
    printMatrix(reduced_costs);

    //IN PROGRESS: TESTE DE OTIMALIDADE FASE I

    bool hasNegative = false;
    for (int i = 0; i < reduced_costs.size(); i++)
    {
        if (reduced_costs[i][0] < 0)
        {
            hasNegative = true;
        }
    }
    vector<vector<double>> solution(xtb.size(), vector<double>(xtb[0].size(), 0.0));
    for(int i = 0; i < xtb.size(); i++){
        for(int j = 0; j < xtb[0].size(); j++)
            solution[i][j] = xtb[i][j];
    }
    vector<vector<double>> optimal(1, vector<double>(1, 0.0));
    for(int i = 0; i < xtb.size(); i++){
        for(int j = 0; j < xtb[0].size(); j++)
            optimal[0][0] += c[i][j] * xtb[i][j];
    }

    bool hasSolution = false;
    double min = N[0][0];
    int min_indice = 0;

    for (int i = 0; i < N[0].size(); i++)
    {
            if (N[0][i] < min){
                min = N[0][i];
                min_indice = i;
            }
    }

    cout << "Valor mínimo é " << min << endl;
    if (min > 0)
    {
        hasSolution = true;
    }

    vector<vector<double>> y = multiplyMatrices(BminusOne, a, BminusOne.size(), a[0].size()); // Fase 2

    printf("\nSolucao basica: \n");
    printMatrix(solution);
    printf("\nOptimal: \n");
    printMatrix(optimal);
    printf("y:\n");
    printMatrix(y);
    double e_chapeu = 10000;
    int e_chapeu_indice = 0;
    for (int i = 0; i < m; i++){
        double divisor = y[0][i];
        double dividendo = xtb[0][i];
        if(divisor > 0 && dividendo/divisor < e_chapeu){
            e_chapeu_indice = i;
            e_chapeu= dividendo/divisor;
        }
    }
    // troca a coluna e_chapeu_indice pela coluna min_indice
    // e_chapeu_indice = l
    // min_indice = k
    for(int i = 0; i < Nindices.size(); i++){
        if(Nindices[i] == min_indice + 1){
            Nindices[i] = e_chapeu_indice + 1;
            for (int dd : Nindices){
                cout << dd << " ";
            }
                cout << endl;
            cout << "chapeu indices:" << e_chapeu_indice << endl;
        }
    }
    for(int i = 0; i < Bindices.size(); i++){
        if(Bindices[i] == e_chapeu_indice + 1){
            Bindices[i] = min_indice + 1;
            for (int dd : Nindices){
                cout << dd << " ";
            }
                cout << endl;
            cout << "min_ indices:" << min_indice << endl;
        }
    }
    for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < Nindices.size(); j++){
            N[i][j] = A[i][Nindices[i] - 1];
        }
    }
    for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < Bindices.size(); j++){
            B[i][j] = A[i][Bindices[i] - 1];
        }
    }
    printf("\nAlguma coisa alguma coisa troca troca B\n");
    printMatrix(B);
    printf("\nAlguma coisa alguma coisa troca troca N\n");
    printMatrix(N);
    // Fase 2

    // Vai para a fase 2
    //    cout << "A matriz A é: \n";
    //    printMatrix(A);
    //    printMatrix(b);
    // for (double iten : f)
    // {
    //         cout << iten << " ";
    // }

    cout << "\nPara aqui?" << endl;
    // Passo 1: cálculo da solução básica

    for (int i = 0; i < N.size(); i++)
    {
            for (int j = 0; j < N[0].size(); j++)
            {
                xtn[i][j] = 0;
            }
    }

    // vetor de solução básicas
    xtb = basicSolution(BminusOne, b);

    // cout << "\nInverso de B:\n";
    // printMatrix(BminusOne);
    // cout <<"\nMultiplicado por vetor b: \n";
    // printMatrix(b);

    cout << "\nSolução básica: " << endl;
    cout << "\nPara aqui?" << endl;
    printMatrix(xtb);
    cout << "\nPara aqui?" << endl;

    // Passo 2: cálculo dos custos relativos
    //  Passo 2.1 vetor multiplicador simplex
    vector<vector<double>> ctb_row;
    // ctb_row = columnToRow(ctb, 0, 1); // pqq vira row?
    cout << "B: " << endl;
    printMatrix(BminusOne);
    cout << "Numero line size" << ctb_row.size() << endl;
    cout << "Numero col size" << BminusOne[0].size() << endl;

    // Passo 2.2 Custos relativos
    // Custos relativos:

    // vector<vector<double>> lambdaT;
    for (int i = 0; i < lambda.size(); i++)
    {
            newline.push_back(lambda[i][0]);
    }
    lambdaT.push_back(newline);
    newline.clear();
    cout << "\nValor multiplicado: " << endl;
    cout << "Multiplica: " << endl;
    printMatrix(lambda);
    cout << "por: ..." << endl;

    // Passo 2.3 Determinação da variável a entrar na base:
    cout << "Determinação da variável a entrar na base:" << endl;

    // Passo 3: teste de otimalidade
    // Se cnk >= 0
    // Se ainda há variáveis artificiais na base => pare
    // Senão, Fase II
    // Passo 4: cálculo da direção simplex
    //  y <- B-1ank
    // Passo 5: Determinação do passo e variável a sair da base
    // se y <= 0, então: pare {problema não tem solução ótima finita}
    // caso contrário, determine a variável a sair da base pela razão mínima:
    // ê <- XBl/yl = min{xbi/yi, tal que yi > 0, i = 1, 2, ..., m} (a variável xbl sai da base)
    // Passo 6: atualização nova partição básica, troque a l-éesima coluna de B pela k-ésima coluna de N
    // matriz básica nova: B <- [aB1 ... aBl-1 aNk abl+1 ... aBm]
    // matriz não básica nova: N <- [aN1 ... aNk-1 aBl aNk+1 ...aNn]
    // se ainda há variáveis artificiais na base => Retorne ao passo 1
    // senão => Fase II
    // iteração++

    // Fase II

    // Passo 1 cálculo da solução básica
    // Passo 2 cálculo dos custos relativos
    // 2.1 vetor multiplicador simplex
    // 2.2 custos relativos
    // 2.3 determinação da variável a entrar na base
    // Passo 3 teste de otimalidade
    // se cnk >= 0 então, pare, solução ótima
    // Passo 4 calculo da direção simplex
    // Passo 5 determinação do passo e variável a sair  da base
    // Passo 6 atualização: nova partição básica, troque a l-ésima coluna de B pela k-ésima coluna de N
    // iteração++
    // Solução ótima: S = para todo i = 1, ..., n, cixi

    return 0;
}