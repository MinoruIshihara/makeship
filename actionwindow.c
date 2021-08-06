#include "blackship.h"

typedef enum {
    EXIT         = -1,
    NONE         = 0,
    ACTION_PAUSE = 1,
    BACK         = 2
} ActionCommand;

SDL_Texture *actionBG;
Stage stage;

void drawActionWin(int millis)
{
    return;
}

int initActionWin()
{
    return 0;
}

extern void destroyActionWin()
{
    return;
}
