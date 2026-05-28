#include<stdio.h>
#include<stdbool.h>
int main(){
    int res;
    char h,p,H;
    //IGUAL A
        res = 'h' == 'p';
        printf("%d\n", res);
        res = 'h' == 'H';
        printf("%d\n", res);
        res = 'h' == 'h';
        printf("%d\n", res);
    //DIFERENTE A
        res = 'h' != 'p';
        printf("%d\n", res);
    //MENOR QUE
        res = 7 < 15;
        printf("%d\n", res);
    //MAYOR QUE
        res = 22 > 11;
        printf("%d\n", res);
    //MENOR O IGUAL QUE
        res = 15 <= 2;
        printf("%d\n", res);
    //MAYOR O IGUAL QUE
        res = 30 >= 100;
        printf("%d\n", res);
    return 0;
}