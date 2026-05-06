#include <stdio.h>
#define RED "\033[31m"
int main (){
    FILE *archivo;
    archivo= fopen("datos.txt", "r");

    if( archivo!=NULL){
        printf ("El archivo se abrio correctamente\n");
        int res = fclose (archivo);
        printf("fclose = %d\n", res);
    }else{
        printf (RED"Error faltal: datos.txt no es un archivo o directorio");
    }
    return 0;
}
