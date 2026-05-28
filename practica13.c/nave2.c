#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 30
#define HEIGHT 15

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

// Posición de la parte principal de la nave
int shipX, shipY;

// Posición del objetivo
int targetX, targetY;

// Dirección actual
char direction = 'd';

int score = 0;

bool game_over = false;

void moveShip(int *x, int *y, char dir){

    switch(dir){

        case 'w':
        case 'W':
            (*y)--;
            break;

        case 's':
        case 'S':
            (*y)++;
            break;

        case 'a':
        case 'A':
            (*x)--;
            break;

        case 'd':
        case 'D':
            (*x)++;
            break;
    }
}

void clearScreen(){

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#ifdef _WIN32

char readInput(){

    if(_kbhit())
        return _getch();

    return direction;
}

#else

struct termios oldt;

void enableRawMode(){

    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);

    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void disableRawMode(){

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

char readInput(){

    int ch = getchar();

    if(ch != EOF)
        return ch;

    return direction;
}

#endif

void generateTarget(){

    targetX = (rand() % (WIDTH - 2)) + 1;
    targetY = (rand() % (HEIGHT - 2)) + 1;
}

void setup(){

    // Posición inicial de la nave
    shipX = WIDTH / 2;
    shipY = HEIGHT / 2;

    generateTarget();
}

void draw(){

    clearScreen();

    for(int i = 0; i < HEIGHT; i++){

        for(int j = 0; j < WIDTH; j++){

            // Bordes
            if(j == 0 || i == 0 || i == HEIGHT - 1 || j == WIDTH - 1){

                printf(YELLOW "▒");
                continue;
            }

            // Parte principal de la nave
            if(j == shipX && i == shipY){

                printf(BLUE "#");
                continue;
            }

            // Ala izquierda
            if(j == shipX - 1 && i == shipY + 1){

                printf(BLUE "#");
                continue;
            }

            // Ala derecha
            if(j == shipX + 1 && i == shipY + 1){

                printf(BLUE "#");
                continue;
            }

            // Objetivo
            if(j == targetX && i == targetY){

                printf(RED "Ѽ");
                continue;
            }

            // Espacio vacío
            printf(RESET " ");
        }

        printf("\n");
    }

    printf(RESET "Score: %d\n", score);
}

void logic(){

    // Colisión con bordes
    if(
        shipX <= 1 ||
        shipX >= WIDTH - 2 ||
        shipY <= 1 ||
        shipY >= HEIGHT - 3
    ){
        game_over = true;
    }

    // Colisión con objetivo
    if(shipX == targetX && shipY == targetY){

        score++;

        generateTarget();
    }
}

int main(){

    char newDirection;

    setup();

#ifndef _WIN32
    enableRawMode();
#endif

    while(!game_over){

        draw();

        newDirection = readInput();

        if(
            newDirection == 'w' || newDirection == 'W' ||
            newDirection == 's' || newDirection == 'S' ||
            newDirection == 'a' || newDirection == 'A' ||
            newDirection == 'd' || newDirection == 'D'
        ){
            direction = newDirection;
        }

        moveShip(&shipX, &shipY, direction);

        logic();

#ifdef _WIN32
        Sleep(150);
#else
        usleep(150000);
#endif
    }

#ifndef _WIN32
    disableRawMode();
#endif

    printf("\nGame Over\n");

    return 0;
}