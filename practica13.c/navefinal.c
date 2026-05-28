#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WIDTH 35
#define HEIGHT 18
#define MAX_ENTITIES 25

// Colores ANSI
#define YELLOW "\033[33m"
#define BLUE   "\033[34m"
#define RED    "\033[31m"
#define GREEN  "\033[32m"
#define CYAN   "\033[36m"
#define RESET  "\033[0m"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

// --- Estructuras ---
typedef struct { 
    int x, y; 
    bool active; 
    int dir; 
    int type; // 0: Asteroide, 1: Estrella, 2: Nave Enemiga
} Entity;

typedef struct { 
    int x, y; 
    bool active; 
} Shot;

// --- Globales ---
int shipX, shipY, score = 0;
bool bulletActive = false, game_over = false, paused = false;
int bulletX, bulletY;

Entity pool[MAX_ENTITIES];
Shot enemyShots[MAX_ENTITIES];
time_t lastAstTime = 0, lastStarTime = 0, lastEnemyTime = 0;

// --- Sistema de Input (Windows/Linux) ---
#ifndef _WIN32
struct termios oldt;
void enableRawMode() {
    struct termios newt; tcgetattr(STDIN_FILENO, &oldt); newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
}
void disableRawMode() { tcsetattr(STDIN_FILENO, TCSANOW, &oldt); }
#endif

char readInput() {
#ifdef _WIN32
    return _kbhit() ? _getch() : 0;
#else
    int ch = getchar(); return (ch != EOF) ? ch : 0;
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}

// --- Lógica del Juego ---
void setup() {
    srand(time(NULL));
    shipX = WIDTH / 2; shipY = HEIGHT - 3;
    for(int i=0; i<MAX_ENTITIES; i++) {
        pool[i].active = false;
        enemyShots[i].active = false;
    }
}

void spawn(int type) {
    for(int i=0; i<MAX_ENTITIES; i++) {
        if(!pool[i].active) {
            pool[i].x = rand() % (WIDTH - 4) + 2;
            pool[i].y = (type == 1) ? (rand() % (HEIGHT - 8) + 1) : 1;
            pool[i].active = true;
            pool[i].dir = (rand() % 2 == 0) ? 1 : -1;
            pool[i].type = type;
            break;
        }
    }
}

void update() {
    time_t now = time(NULL);

    // Tiempos solicitados
    if(now - lastAstTime >= 2) { spawn(0); lastAstTime = now; }    // Asteroide 2s
    if(now - lastStarTime >= 4) { spawn(1); lastStarTime = now; }   // Estrella 4s
    if(now - lastEnemyTime >= 5) { spawn(2); lastEnemyTime = now; } // Nave Enemiga 5s

    // Disparo jugador (Velocidad alta)
    if(bulletActive) {
        bulletY -= 2; 
        if(bulletY <= 0) bulletActive = false;
    }

    for(int i=0; i<MAX_ENTITIES; i++) {
        // Movimiento de objetos
        if(pool[i].active) {
            if(pool[i].type != 1) { // No estrella
                pool[i].y++;
                pool[i].x += pool[i].dir;
                if(pool[i].x <= 1 || pool[i].x >= WIDTH-2) pool[i].dir *= -1;
                if(pool[i].y >= HEIGHT-2) pool[i].active = false;

                // Disparo enemigo (Naves disparan más)
                int prob = (pool[i].type == 2) ? 8 : 15;
                if(pool[i].active && rand() % prob == 0 && !enemyShots[i].active) {
                    enemyShots[i].x = pool[i].x;
                    enemyShots[i].y = pool[i].y + 1;
                    enemyShots[i].active = true;
                }
            }
        }
        // Movimiento de balas enemigas
        if(enemyShots[i].active) {
            enemyShots[i].y++;
            if(enemyShots[i].y >= HEIGHT-1) enemyShots[i].active = false;
        }
    }
}

void checkCollisions() {
    for(int i=0; i<MAX_ENTITIES; i++) {
        // 1. Muerte por bala enemiga
        if(enemyShots[i].active && enemyShots[i].x == shipX && enemyShots[i].y == shipY) game_over = true;

        if(pool[i].active) {
            // 2. Colisión física
            bool hit = (abs(shipX - pool[i].x) <= 1 && (shipY == pool[i].y || shipY+1 == pool[i].y));

            if(hit) {
                if(pool[i].type == 1) { // Estrella
                    score += 5; pool[i].active = false;
                } else { // Asteroide o Nave enemiga
                    game_over = true;
                }
            }

            // 3. Colisión de tu disparo con objeto
            if(bulletActive && bulletX == pool[i].x && (bulletY == pool[i].y || bulletY+1 == pool[i].y)) {
                if(pool[i].type == 0) score += 1;      // Asteroide 1
                else if(pool[i].type == 1) score += 5; // Estrella 5
                else if(pool[i].type == 2) score += 2; // Nave Enemiga 2
                
                pool[i].active = false;
                bulletActive = false;
            }
        }
    }
}

void draw() {
    clearScreen();
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            if(j == 0 || i == 0 || i == HEIGHT-1 || j == WIDTH-1) printf(YELLOW "▒");
            else if(i == shipY && j == shipX) printf(BLUE "▲");
            else if(i == shipY+1 && (j == shipX-1 || j == shipX+1)) printf(BLUE "m");
            else if(bulletActive && j == bulletX && (i == bulletY || i == bulletY+1)) printf(CYAN "¦");
            else {
                bool drawn = false;
                for(int k=0; k<MAX_ENTITIES; k++) {
                    if(pool[k].active && pool[k].x == j && pool[k].y == i) {
                        if(pool[k].type == 0) printf(GREEN "O");      
                        else if(pool[k].type == 1) printf(YELLOW "*"); 
                        else if(pool[k].type == 2) printf(RED "W");    
                        drawn = true; break;
                    }
                    if(enemyShots[k].active && enemyShots[k].x == j && enemyShots[k].y == i) {
                        printf(RED "v"); drawn = true; break; 
                    }
                }
                if(!drawn) printf(" ");
            }
        }
        printf("\n");
    }
    printf(RESET "Puntos: %d | P: Pausa | ESPACIO: Disparar\n", score);
    if(paused) printf(YELLOW "   === JUEGO PAUSADO ===\n" RESET);
}

int main() {
    setup();
#ifndef _WIN32
    enableRawMode();
#endif

    while(!game_over) {
        char in = readInput();
        if(in == 'p' || in == 'P') paused = !paused;

        if(!paused) {
            if(in == 'w' || in == 'W') if(shipY > 1) shipY--;
            if(in == 's' || in == 'S') if(shipY < HEIGHT-3) shipY++;
            if(in == 'a' || in == 'A') if(shipX > 2) shipX--;
            if(in == 'd' || in == 'D') if(shipX < WIDTH-3) shipX++;
            if(in == ' ' && !bulletActive) { bulletActive = true; bulletX = shipX; bulletY = shipY-1; }
            
            update();
            checkCollisions();
        }
        draw();

#ifdef _WIN32
        Sleep(60);
#else
        usleep(60000);
#endif
    }

#ifndef _WIN32
    disableRawMode();
#endif
    clearScreen();
    printf(RED "\n¡HAS MUERTO!\n" RESET);
    printf("Puntaje Final: %d\n\n", score);
    return 0;
}