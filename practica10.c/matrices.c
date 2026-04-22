#include <stdio.h>
#define TAM 1000
int main(){
void leer_matriz(int mat[][TAM], int m, int n){
for (int i =0; i < m; i++){
        for (int j=0; j<n; j++){
            printf ("Ingresa el valor para M[%d , %d]: ", i, j);
            scanf ("%d", &mat [i][j]);
        }
}
}
void mostrar_matriz (int mat[][TAM], int m, int n){
    for (int i= 0; i<m; i++){
        printf ("[ ");
        for (int j=0; j<n; j++){
            printf ("%d", mat[i][j]);
        }
        printf ("]\n");
    }
}
void  sumar_matrices ( int matA[][TAM],int matB[][TAM],int matC[][TAM], int m, int n){
  for (int i =0; i < m; i++){
    for (int j =0; j < n; j++){
    matC [i][j] = matA [i][j] + matB [i][j];
  }
}
  }
}
