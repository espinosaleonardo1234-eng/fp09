#include <stdio.h>
#define RED "\033[31m"
int main (int argc, char **argv){
    FILE *archivo;
    if (argc <2 ) {
      printf (RED"Error faltal: hacen falta argumentos");
      return 1;
    }
    
    archivo = fopen(argv[1], "r");
    if(archivo!=NULL){
        printf ("El archivo se abrio correctamente\n");
        int res = fclose (archivo);
        printf("fclose = %d\n", res);
    }else{
        printf (RED"Error faltal: datos.txt no es un archivo o directorio");
    }
    return 0;
}
