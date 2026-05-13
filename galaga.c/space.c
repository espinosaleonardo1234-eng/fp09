#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WIDTH 35
#define HEIGHT 20
#define MAX_ENEMIES 6
#define MAX_STARS 3
#define MAX_ENEMY_BULLETS 3
#define COLOR_BORDER "\033[34m"
#define COLOR_RESET "\033[0m"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

typedef struct { int x, y; bool active; } Entity;

Entity player;
Entity bullet;
Entity enemies[MAX_ENEMIES];
Entity enemyBullets[MAX_ENEMY_BULLETS];
Entity stars[MAX_STARS];

int score = 0;
bool game_over = false;
int spawnTimer = 0;
int starTimer = 0;
int enemyDirection = 1;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}

void setup() {
    srand(time(NULL));
    player.x = WIDTH / 2;
    player.y = HEIGHT - 3;
    bullet.active = false;
    spawnTimer = 0;
    starTimer = 0;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
    }
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemyBullets[i].active = false;
    }
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].active = false;
    }
}

void draw() {
    clearScreen();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                printf(COLOR_BORDER "#" COLOR_RESET);
                continue;
            }

            bool drawn = false;

            if (bullet.active && bullet.x == j && bullet.y == i) {
                printf("|");
                drawn = true;
            }

            if (!drawn) {
                if (i == player.y && j == player.x) {
                    printf("#");
                    drawn = true;
                } else if (i == player.y + 1 && (j == player.x - 1 || j == player.x + 1)) {
                    printf("#");
                    drawn = true;
                }
            }

            if (!drawn) {
                for (int k = 0; k < MAX_STARS; k++) {
                    if (stars[k].active && stars[k].x == j && stars[k].y == i) {
                        printf("*");
                        drawn = true;
                        break;
                    }
                }
            }
            if (!drawn) {
                for (int k = 0; k < MAX_ENEMY_BULLETS; k++) {
                    if (enemyBullets[k].active && enemyBullets[k].x == j && enemyBullets[k].y == i) {
                        printf("!");
                        drawn = true;
                        break;
                    }
                }
            }
            if (!drawn) {
                for (int k = 0; k < MAX_ENEMIES; k++) {
                    if (enemies[k].active && enemies[k].x == j && enemies[k].y == i) {
                        printf("V");
                        drawn = true;
                        break;
                    }
                }
            }

            if (!drawn) printf(" ");
        }
        printf("\n");
    }
    printf("Puntos: %d | Teclas: A, D | DISPARAR: L | Q: Salir\n", score);
}

void logic() {
    if (bullet.active) {
        for (int step = 0; step < 3 && bullet.active; step++) {
            bullet.y--;
            if (bullet.y <= 1) {
                bullet.active = false;
                break;
            }
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (!enemies[i].active) continue;
                if (enemies[i].x == bullet.x && enemies[i].y == bullet.y) {
                    enemies[i].active = false;
                    bullet.active = false;
                    score += 10;
                    break;
                }
            }
            if (!bullet.active) break;
            for (int i = 0; i < MAX_STARS; i++) {
                if (!stars[i].active) continue;
                if (stars[i].x == bullet.x && stars[i].y == bullet.y) {
                    stars[i].active = false;
                    bullet.active = false;
                    score += 5;
                    break;
                }
            }
        }
    }

    spawnTimer++;
    if (spawnTimer > 24) {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) {
                enemies[i].active = true;
                enemies[i].x = (rand() % (WIDTH - 4)) + 2;
                enemies[i].y = 1;
                break;
            }
        }
        spawnTimer = 0;
    }

    starTimer++;
    if (starTimer > 40) {
        for (int i = 0; i < MAX_STARS; i++) {
            if (!stars[i].active) {
                int x = (rand() % (WIDTH - 4)) + 2;
                int y = (rand() % (HEIGHT - 6)) + 2;
                bool collision = false;
                for (int j = 0; j < MAX_ENEMIES; j++) {
                    if (enemies[j].active && enemies[j].x == x && enemies[j].y == y) {
                        collision = true;
                        break;
                    }
                }
                if (!collision) {
                    stars[i].active = true;
                    stars[i].x = x;
                    stars[i].y = y;
                    break;
                }
            }
        }
        starTimer = 0;
    }

    static int enemyBulletTimer = 0;
    if (++enemyBulletTimer > 6) {
        for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
            if (!enemyBullets[i].active) continue;
            enemyBullets[i].y++;
            if (enemyBullets[i].y >= HEIGHT - 1) {
                enemyBullets[i].active = false;
            } else if (enemyBullets[i].y == player.y &&
                (enemyBullets[i].x == player.x || enemyBullets[i].x == player.x - 1 || enemyBullets[i].x == player.x + 1)) {
                game_over = true;
            }
        }
        enemyBulletTimer = 0;
    }

    if (rand() % 40 == 0) {
        for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
            if (!enemyBullets[i].active) {
                int shooter = rand() % MAX_ENEMIES;
                if (enemies[shooter].active) {
                    enemyBullets[i].active = true;
                    enemyBullets[i].x = enemies[shooter].x;
                    enemyBullets[i].y = enemies[shooter].y + 1;
                }
                break;
            }
        }
    }

    static int moveCounter = 0;
    if (++moveCounter > 30) {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) continue;
            int dx = (rand() % 3) - 1;
            int nextX = enemies[i].x + dx;
            if (nextX > 1 && nextX < WIDTH - 1) {
                enemies[i].x = nextX;
            }
            enemies[i].y++;
            if (enemies[i].y >= player.y || enemies[i].y >= HEIGHT - 2) {
                game_over = true;
            }
        }
        moveCounter = 0;
    }

    for (int i = 0; i < MAX_STARS; i++) {
        if (!stars[i].active) continue;
        if ((stars[i].x == player.x && stars[i].y == player.y) ||
            (stars[i].x == player.x - 1 && stars[i].y == player.y + 1) ||
            (stars[i].x == player.x + 1 && stars[i].y == player.y + 1)) {
            stars[i].active = false;
            score += 5;
        }
    }
}

int main() {
    setup();
#ifndef _WIN32
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
#endif

    while (!game_over) {
        draw();
        
        char key = 0;
#ifdef _WIN32
        if (_kbhit()) key = _getch();
#else
        read(STDIN_FILENO, &key, 1);
#endif

        if (key == 'a' || key == 'A') player.x--;
        if (key == 'd' || key == 'D') player.x++;
        if ((key == 'l' || key == 'L') && !bullet.active) {
            bullet.active = true;
            bullet.x = player.x;
            bullet.y = player.y - 1;
        }
        if (key == 'q') break;

        logic();

        if (player.x < 2) player.x = 2;
        if (player.x > WIDTH - 3) player.x = WIDTH - 3;

#ifdef _WIN32
        Sleep(30);
#else
        usleep(30000);
#endif
    }

#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    printf("\n--- FIN DEL JUEGO ---\n");
    return 0;
}