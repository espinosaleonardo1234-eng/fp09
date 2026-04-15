#include <stdio.h>
/*
Este programa sirve para imprimir las direcciones de los elementos de un arreglo
*/
int main (){
    int array [7];
    float array2[10];
        printf ("El tamaño de un numero entero en bytes es: %ld\n",sizeof(int)); 
    for (int i=0; i <7; ++i){
        printf ("La direccion del elemento %d es %p\n" i, &array[i]);
    }
    for (int j=0; j <10; ++j){
        printf ("La direccion del elemento %d es %p\n" j, &array2[j]);

}
 return 0;
}