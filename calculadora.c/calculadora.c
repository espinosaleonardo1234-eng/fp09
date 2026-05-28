 #include <stdio.h>

int main()
{
    int a, b, c;
    float f, g, h;
    char op, op2;
    printf("Selecciona una opcion\n");
    printf("1. Operaciones con enteros\n");
    printf("2. Operaciones con reales\n");

    scanf("%c", &op);
    printf("Selecciona una opcion\n");
    printf("1. Suma\n");
    printf("2. Resta\n");
    printf("3. Multiplicacion\n");
    printf("4. Division\n");
    printf("5. Modulo\n");
    printf("6. Salir\n");
    scanf("%c", &op2);

    if (op == '1')
    {
        printf("Ingresa los dos numeros con los que quieres operar(num1 num2)\n");
        scanf("%d %d", &a, &b);
        switch (op2)
        {
        case 1:
            c=a+b;
            break;
        case 2:
            c=a-b;
            break;
        case 3:
            c=a*b;
            break;
        case 4:
            c=a/b;
            break;
        case 5:
            c= a%b;
            break;
        default:
            break;
                return 0;
        }
        printf("El resultado es %d\n", c);
    }else{
        printf("Ingresa los dos numeros con los que quieres operar(num1 num2)\n");
        scanf("%f %f", &f, &g);
        switch (op2)
        {
        case 1:
            h=f+g;
            break;
        case 2:
            h=f-g;
            break;
        case 3:
            h=f*g;
            break;
        case 4:
            h=f/g;
            break;
        case 5:
            h= (int)f%(int)g;
            break;
        default:
            break;
                return 0;
        }
    printf("El resultado es %f\n", h);
    }
}