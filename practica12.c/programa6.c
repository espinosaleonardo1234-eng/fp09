#include <stdio.h>
#define RED "\033[31m"
int main (int argc, char **argv){
    FILE *archivo;
    if (argc <2 ) {
      printf (RED"Error faltal: hacen falta argumentos");
      return 1;
    }
    
    archivo = fopen(argv[1], "a");
    if(archivo!=NULL){
        printf ("El archivo se abrio correctamente\n");
        char cosa []= "tele";
        float precio= 7000.00;
       // while (feof(archivo)==0){
            fprintf(archivo, "\n%s\t%f", cosa, precio);
            //printf ("%s\t%f\n",cosa, precio);
      //  }
        int res = fclose (archivo);
        printf("fclose = %d\n", res);
    }else{
        printf (RED"Error faltal: %s no es un archivo o directorio\n",argv[1]);
    }
    return 0;
}
