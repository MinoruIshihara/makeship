#include "blackship.h"

extern void destroyActionWin();

void calcStage(int millis);

void drawPlayer();
void drawEnemy();
void drawBullet();

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
    calcStage(millis);
    return;
}

int initActionWin(int stageNum)
{
    return 0;
}

int initStage(int stageNum)
{
}

void destroyActionWin()
{
    return;
}

void calcStage(int millis)
{
}

void drawPlayer()
{
}

void drawEnemy()
{
}

void drawBullet()
{
}

void drawEffect()
{
}

void makeStage()
{
    EnemyInfo enemies[10];

    enemies[0].surface = IMG_Load("");

    stage.enemyNum
        = 1;

    Player player;
    player.info = playerInfo;
    player.hp   = playerInfo.hp;
    player.x    = 100;
    player.y    = 0;
    player.vx   = 0;
    player.vy   = 0;

    stage.player = player;
    stage.enemies[0]
}