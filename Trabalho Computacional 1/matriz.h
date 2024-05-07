#include <stdio.h>

template <size_t size_x, size_t size_y>
float calcularDeterminante(float (&matriz)[size_x][size_y], int ordem)
{
    float multiplicacao;
    float determinante=1;

    for(int i = 0; i < ordem; i++){
        for(int j = 0; j < ordem; j++){
            multiplicacao = matriz[j][i]/matriz[i][i];
            for(int k = 0; k < ordem; k++){
                if(i!=j)
                    matriz[j][k] = matriz[j][k]-matriz[i][k]*multiplicacao;
            }
        }
    }
    for(int i = 0; i < ordem; i++)
    {
        determinante=determinante*matriz[i][i];
    }

    printf("O valor da determinante eh: %.0f \n",determinante);
    return true;
}