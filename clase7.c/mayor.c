#include <stdio.h>
#define MAX 1000

int main(){
    int n;
    int array[MAX];
    int max;

    printf("Ingresar el número de datos:\n");
    if (scanf("%d", &n) != 1) {
        printf("Entrada inválida.\n");
        return 1;
    }

    if (n <= 0) {
        printf("No hay datos.\n");
        return 1;
    }

    for(int i = 0; i < n; i++){
        printf("Ingresar el dato %d:\n", i+1);
        if (scanf("%d", &array[i]) != 1) {
            printf("Entrada inválida.\n");
            return 1;
        }
    }

    max = array[0];
    for(int i = 1; i < n; i++){
        if (max < array[i])
            max = array[i];
    }

    printf("El valor máximo es %d\n", max);
    return 0;
}