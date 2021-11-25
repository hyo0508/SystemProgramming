#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define SPACE 32
#define ENTER 10

void DrawDinosaur(int);
void DrawCactus(int, int);

int _kbhit();
int GetKeyDown();
int IsDied(int, int, int);

void DrawDinosaur(int dinosaurHead) {
    move(dinosaurHead, 0);
    static bool legDraw = true;

    printw("        &&&&&&& \n");
    printw("       && &&&&&&\n");
    printw("       &&&&&&&&&\n");
    printw("&      &&&      \n");
    printw("&&     &&&&&&&  \n");
    printw("&&&   &&&&&     \n");
    printw(" &&  &&&&&&&&&& \n");
    printw(" &&&&&&&&&&&    \n");
    printw("   &&&&&&&&&    \n");
    printw("    &&&&&&&     \n");

    if (legDraw) {
        printw("     &    &&&&     \n");
        printw("     &&            ");
        legDraw = false;
    }
    else {
        printw("     &&&  &        \n");
        printw("          &&       ");
        legDraw = true;
    }

}

void DrawCactus(int cactusHead, int cactusPos) {
    move(cactusHead + 0, cactusPos);
    printw("$$$$");
    move(cactusHead + 1, cactusPos);
    printw(" $$ ");
    move(cactusHead + 2, cactusPos);
    printw(" $$ ");
    move(cactusHead + 3, cactusPos);
    printw(" $$ ");
    move(cactusHead + 4, cactusPos);
    printw(" $$ ");
}

int _kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(0, &fds);

    return select(1, &fds, NULL, NULL, &tv);
}

int GetKeyDown() {
    if (_kbhit()) {
        return getch();
    }
    return 1;
}

int isDied(int cp, int dh, int ch) {
    int std[17] = {10, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 11, 8, 7, 3};
    if (cp > 16) {
        return 0;
    }
    return dh > ch - std[cp];
}

int main(int argc, char* argv[]) {
    int winWidth = 0, winHeight = 0;
    int cactusHead = 0, cactusPos = 0;
    int dinosaurHead = 0;
    int getKey = 0;
    int score = 0, interval = 0, gap = 0;
    bool isJumping = false;
    bool isBottom = true;

    initscr();

    curs_set(0);

    getmaxyx(stdscr, winHeight, winWidth);

    dinosaurHead = winHeight - 13;
    cactusHead = winHeight - 5;
    cactusPos = winWidth - 20;
    interval = 100000;
    gap = 10000;

    while (true) {
        getKey = GetKeyDown();

        if ((getKey == SPACE) && isBottom) {
            isJumping = true;
            isBottom = false;
        }
        else if (getKey == ENTER) {
            break;
        }
        
        if (isJumping) {
            dinosaurHead -= 3;
        }
        else {
            dinosaurHead += 3;
        }

        if (dinosaurHead >= winHeight - 13) {
            dinosaurHead = winHeight - 13;
            isBottom = true;
        }
        if (dinosaurHead <= 2 || dinosaurHead <= winHeight - 28) {
            isJumping = false;
        }

        cactusPos -= 3;

        if (cactusPos <= 0) {
            cactusPos = winWidth - 20;
            score++;
            interval -= gap;
            if (gap) {
                gap -= 1000;
            }
        }

        printw("\n                         score: %d\n", score);

        DrawDinosaur(dinosaurHead);
        DrawCactus(cactusHead, cactusPos);

        if (isDied(cactusPos, dinosaurHead, cactusHead)) {
            int sec = 10000000;
            while (sec) {
                getKey = GetKeyDown();
                if (getKey == ENTER) {
                    break;
                }
                move (0, 0);
                printw("                         You Died\n");
                printw("                         score: %d\n", score);
                printw("                         This window closes in %02d seconds", sec / 1000000);
                DrawDinosaur(dinosaurHead);
                DrawCactus(cactusHead, cactusPos);
                usleep(100000);
                sec -= 100000;
                refresh();
            }
            break;
        }
        usleep(interval);

        refresh();
        clear();
    }

    endwin();
}