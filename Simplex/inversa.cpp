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