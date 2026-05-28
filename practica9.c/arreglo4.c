#include <stdio.h>
/*
Este programa ilustra que todas las variables tiene una direccion
El nombre de un arreglo almacena la direccion de memoria del primer elemento
*/
int main (){
int array [10];
int *apuntador;

apuntador = &array[7]; // asignando la direccion del elemento central
for (int i=-7; i <8; i++){
    printf("Ingresa el valor %d: ",i+7);
scanf ("%d", (apuntador+i));
}
printf("[");
for (int i=-7; i <8; i++){
printf("%d ,",*(apuntador + i));
}
printf ("]\n");
return 0;

}