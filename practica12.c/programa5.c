#include <stdio.h>

#define RED "\033[31m"
int main (int argc, char **argv){
    FILE *origen,*destino;
    char linea[]="Esta linea se agrego inmediatamente la funcion fputs\nLEA" ;
    if (argc <2 ) {
      printf (RED"Error faltal: hacen falta argumentos");
      return 1;
    }
    
    origen = fopen(argv[1], "a");
    destino= fopen(argv[2], "w");
    if(origen!=NULL){
      destino= fopen(argv[2], "w");
      while(feof(origen)==0){
        fgets(linea,256,origen);
        fputs (linea,destino)
      }
        int res = fclose (origen);
        fclose(destino);

    }else{
        printf (RED"Error faltal: datos.txt no es un archivo o directorio");
    }
    return 0;
}
