#include <stdio.h>
#define RED "\033[31m"
int main (int argc, char **argv){
    FILE *archivo;
    char linea[256];
    if (argc <2 ) {
      printf (RED"Error faltal: hacen falta argumentos");
      return 1;
    }
    
    archivo = fopen(argv[1], "r");
    if(archivo!=NULL){
        printf ("El archivo se abrio correctamente\n");
        while(feof(archivo)==0){
            fgets (linea, 256, archivo);
            printf()
        }
        int res = fclose (archivo);
        printf("fclose = %d\n", res);
    }else{
        printf (RED"Error faltal: datos.txt no es un archivo o directorio");
    }
    return 0;
}
