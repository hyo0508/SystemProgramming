#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EXIT 27
#define PAUSE 112

#define MAP_INIT_POS_X 3
#define MAP_INIT_POS_Y 2
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

#define INIT_SPEED 200000

/* GLOBAL */
int snakePosX[100];
int snakePosY[100];
int snakeLen;

int speed;
int score;
int bestScore;
int lastScore;
int key;
int direction;

/* MEHTOD */
void Start();
void Reset();
void DrawMap();
void Move(int);
void Pause();
void GameOver();
void CreateFood();
void Debug();
int _kbhit();                         // Modify a Windows Func
void gotoxy(int, int, const char *);  // Modify a Windows Func
void SetColor();

int main(int argc, char *argv[]) {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    SetColor();
    Start();
    
    while (1) {
        if (_kbhit()) key = getch();
        usleep(speed);
        switch (key) {
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_DOWN:
            case KEY_UP:
                if ((direction == KEY_LEFT && key != KEY_RIGHT) ||
                    (direction == KEY_RIGHT && key != KEY_LEFT) ||
                    (direction == KEY_UP && key != KEY_DOWN) ||
                    (direction == KEY_DOWN && key != KEY_UP)) {
                    direction = key;
                }
                key = 0;
                break;
            case PAUSE:
                // Pause();
                break;
            case EXIT:
                endwin();
                exit(0);
        }

        Move(direction);
    }

    attron(COLOR_PAIR(5));
    DrawMap();
    getch();
    endwin();

    return 0;
}

void Start() {
    int colorFlag = 0;
    while (_kbhit()) getch();
    DrawMap();
    attron(COLOR_PAIR(3));

    gotoxy(MAP_INIT_POS_Y + 5, MAP_INIT_POS_X + (MAP_WIDTH / 2) - 7, "+--------------------------+");
    gotoxy(MAP_INIT_POS_Y + 6, MAP_INIT_POS_X + (MAP_WIDTH / 2) - 7, "         SNAKE GAME         ");
    gotoxy(MAP_INIT_POS_Y + 7, MAP_INIT_POS_X + (MAP_WIDTH / 2) - 7, "+--------------------------+");
    gotoxy(MAP_INIT_POS_Y + 11, MAP_INIT_POS_X + (MAP_WIDTH / 2) - 11, "PRESS THE ARROW KEYS TO PROCEED WITH THE GAME");
    gotoxy(MAP_INIT_POS_Y + 12, MAP_INIT_POS_X + (MAP_WIDTH / 2) - 7, "P : PAUSE");
    gotoxy(MAP_INIT_POS_Y + 13, MAP_INIT_POS_X + (MAP_WIDTH / 2) - 7, "ESC : EXIT");
    refresh();

    while (_kbhit()) {
        key = getch();
        if (key = EXIT) {
            endwin(), exit(0);
        } else break;
        attron(COLOR_PAIR(colorFlag));
        gotoxy(MAP_INIT_POS_Y + 9, MAP_INIT_POS_X + (MAP_WIDTH / 2) - 7, "PRESS ANY KEY TO START");
        usleep(800);
        refresh();
        colorFlag = colorFlag % 7 + 1;
    }

    Reset();
}

void Reset() {
    int idx = 0;
    clear();
    DrawMap();
    while (_kbhit()) getch();
    direction = KEY_LEFT;
    speed = INIT_SPEED;
    snakeLen = 5;
    score = 0;
    for (; idx < snakeLen; idx++) {
        snakePosX[idx] = MAP_WIDTH / 2 + idx;
        snakePosY[idx] = MAP_HEIGHT / 2;
        gotoxy(MAP_INIT_POS_Y + snakePosY[idx], MAP_INIT_POS_X + snakePosX[idx], "O");
    }
    gotoxy(MAP_INIT_POS_Y + snakePosY[0], MAP_INIT_POS_X + snakePosX[0], "H");
}

void DrawMap() {
    int idx;
    for (idx = 0; idx < MAP_WIDTH; idx++) {
        gotoxy(MAP_INIT_POS_Y, MAP_INIT_POS_X + idx, "#");
        gotoxy(MAP_INIT_POS_Y + MAP_HEIGHT - 1, MAP_INIT_POS_X + idx, "#");
    }

    for (idx = MAP_INIT_POS_Y; idx < MAP_INIT_POS_Y + MAP_HEIGHT - 1; idx++) {
        gotoxy(idx, MAP_INIT_POS_X, "#");
        gotoxy(idx, MAP_INIT_POS_X + MAP_WIDTH - 1, "#");
    }
}

void Move(int dir) {
    int idx;

    gotoxy(MAP_INIT_POS_Y + snakePosY[snakeLen - 1], MAP_INIT_POS_X + snakePosX[snakeLen - 1], " ");
    for (idx = snakeLen - 1; idx > 0; idx--) {
        snakePosX[idx] = snakePosX[idx - 1];
        snakePosY[idx] = snakePosY[idx - 1];
    }
    gotoxy(MAP_INIT_POS_Y + snakePosY[0], MAP_INIT_POS_X + snakePosX[0], "O");

    if (direction == KEY_LEFT) --(snakePosX[0]);
    if (direction == KEY_RIGHT) ++(snakePosX[0]);
    if (direction == KEY_UP) --(snakePosY[0]);
    if (direction == KEY_DOWN) ++(snakePosY[0]);
    gotoxy(MAP_INIT_POS_Y + snakePosY[idx], MAP_INIT_POS_X + snakePosX[idx], "H");

    refresh();
}

int _kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(0, &fds);

    return select(1, &fds, NULL, NULL, &tv);
}

void gotoxy(int y, int x, const char *msg) {
    move(y, x * 2);
    printw("%s", msg);
}

void SetColor() {
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
}