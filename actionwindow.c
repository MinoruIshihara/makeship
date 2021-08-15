#include "blackship.h"

extern void destroyActionWin();

void calcStage(int millis);

void drawPlayer();
void drawEnemy();
void drawBullet();

EnemyInfo* makeEnemy();

Stage makeStage1();
Stage makeStage2();
Stage makeStage3();

typedef enum {
    EXIT         = -1,
    NONE         = 0,
    ACTION_PAUSE = 1,
    BACK         = 2
} ActionCommand;

SDL_Texture* actionBG;
Stage stage;

void drawActionWin(int millis)
{
    calcStage(millis);
    return;
}

int initActionWin(int stageNum)
{
    initStage(stageNum);

    SDL_Surface* actionBGSurface = IMG_Load("actionBG.png");
    actionBG                     = SDL_CreateTextureFromSurface(render, actionBGSurface);

    for (int i = 0; i < stage.enemyNum; i++) {
        stage.enemies[i].texture = SDL_CreateTextureFromSurface(render, stage.enemies[i].info.surface);
    }
}

int initStage(int stageNum)
{
    switch (stageNum) {
    case 1:
        stage = makeStage1();
        break;
    case 2:
        stage = makeStage2();
        break;
    case 3:
        stage = makeStage3();
        break;
    default:
        return -1;
    }
    return 0;
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

EnemyInfo* makeEnemy()
{
    EnemyInfo enemies[5];

    enemies[0].gun.bullet.damage  = 20;
    enemies[0].gun.bullet.surface = IMG_Lod("bullet1.png");
    enemies[0].gun.cost           = 0;
    enemies[0].gun.surface        = IMG_Load("gun1.png");
    enemies[0].hp                 = 150;
    enemies[0].speed              = 2;
    enemies[0].surface            = IMG_Load("enemy1.png");

    enemies[1].gun.bullet.damage  = 40;
    enemies[0].gun.bullet.surface = IMG_Lod("bullet2.png");
    enemies[1].gun.cost           = 0;
    enemies[1].gun.surface        = IMG_Load("gun2.png");
    enemies[1].hp                 = 200;
    enemies[1].speed              = 2;
    enemies[1].surface            = IMG_Load("enemy2.png");

    enemies[2].gun.bullet.damage  = 40;
    enemies[2].gun.bullet.surface = IMG_Lod("bullet2.png");
    enemies[2].gun.cost           = 0;
    enemies[2].gun.surface        = IMG_Load("gun2.png");
    enemies[2].hp                 = 300;
    enemies[2].speed              = 1;
    enemies[2].surface            = IMG_Load("enemy2.png");

    enemies[3].gun.bullet.damage  = 60;
    enemies[3].gun.bullet.surface = IMG_Lod("bullet2.png");
    enemies[3].gun.cost           = 0;
    enemies[3].gun.surface        = IMG_Load("gun3.png");
    enemies[3].hp                 = 500;
    enemies[3].speed              = 1;
    enemies[3].surface            = IMG_Load("enemy3.png");

    enemies[4].gun.bullet.damage  = 100;
    enemies[4].gun.bullet.surface = IMG_Lod("bullet3.png");
    enemies[4].gun.cost           = 0;
    enemies[4].gun.surface        = IMG_Load("gun3.png");
    enemies[4].hp                 = 200;
    enemies[4].speed              = 1;
    enemies[4].surface            = IMG_Load("enemy4.png");
}

Stage makeStage1()
{
    Stage stage1;

    EnemyInfo enemieInfos[5] = makeEnemy();

    stage1.player.info = playerInfo;
    stage1.player.hp   = playerInfo.hp;
    stage1.player.x    = 100;
    stage1.player.y    = 0;
    stage1.player.vx   = 0;
    stage1.player.vy   = 0;

    stage1.enemyNum        = 4;
    stage1.enemies[0].info = enemieInfos[0];
    stage1.enemies[0].hp   = enemieInfos[0].hp;
    stage1.enemies[0].x    = 530;
    stage1.enemies[0].y    = 0;
    stage1.enemies[0].vx   = 0;
    stage1.enemies[0].vx   = 0;

    stage1.enemies[1].info = enemieInfos[0];
    stage1.enemies[1].hp   = enemieInfos[0].hp;
    stage1.enemies[1].x    = WINDOW_WIDTH + 420;
    stage1.enemies[1].y    = 0;
    stage1.enemies[1].vx   = 0;
    stage1.enemies[1].vx   = 0;

    stage1.enemies[2].info = enemieInfos[0];
    stage1.enemies[2].hp   = enemieInfos[0].hp;
    stage1.enemies[2].x    = WINDOW_WIDTH + 530;
    stage1.enemies[2].y    = 0;
    stage1.enemies[2].vx   = 0;
    stage1.enemies[2].vx   = 0;

    stage1.enemies[3].info = enemieInfos[1];
    stage1.enemies[3].hp   = enemieInfos[1].hp;
    stage1.enemies[3].x    = WINDOW_WIDTH * 2 + 530;
    stage1.enemies[3].y    = 0;
    stage1.enemies[3].vx   = 0;
    stage1.enemies[3].vx   = 0;

    stage1.bulletNum = 0;

    stage1.phase = BATTLE;

    return stage1;
}

Stage makeStage2()
{
    Stage stage2;

    EnemyInfo enemieInfos[5] = makeEnemy();

    stage2.player.info = playerInfo;
    stage2.player.hp   = playerInfo.hp;
    stage2.player.x    = 100;
    stage2.player.y    = 0;
    stage2.player.vx   = 0;
    stage2.player.vy   = 0;

    stage2.enemyNum        = 6;
    stage2.enemies[0].info = enemieInfos[0];
    stage2.enemies[0].hp   = enemieInfos[0].hp;
    stage2.enemies[0].x    = 420;
    stage2.enemies[0].y    = 0;
    stage2.enemies[0].vx   = 0;
    stage2.enemies[0].vx   = 0;

    stage2.enemies[1].info = enemieInfos[0];
    stage2.enemies[1].hp   = enemieInfos[0].hp;
    stage2.enemies[1].x    = 530;
    stage2.enemies[1].y    = 0;
    stage2.enemies[1].vx   = 0;
    stage2.enemies[1].vx   = 0;

    stage2.enemies[2].info = enemieInfos[0];
    stage2.enemies[2].hp   = enemieInfos[0].hp;
    stage2.enemies[2].x    = WINDOW_WIDTH + 420;
    stage2.enemies[2].y    = 0;
    stage2.enemies[2].vx   = 0;
    stage2.enemies[2].vx   = 0;

    stage2.enemies[3].info = enemieInfos[1];
    stage2.enemies[3].hp   = enemieInfos[1].hp;
    stage2.enemies[3].x    = WINDOW_WIDTH + 530;
    stage2.enemies[3].y    = 0;
    stage2.enemies[3].vx   = 0;
    stage2.enemies[3].vx   = 0;

    stage2.enemies[4].info = enemieInfos[1];
    stage2.enemies[4].hp   = enemieInfos[1].hp;
    stage2.enemies[4].x    = WINDOW_WIDTH * 2 + 420;
    stage2.enemies[4].y    = 0;
    stage2.enemies[4].vx   = 0;
    stage2.enemies[4].vx   = 0;

    stage2.enemies[5].info = enemieInfos[2];
    stage2.enemies[5].hp   = enemieInfos[2].hp;
    stage2.enemies[5].x    = WINDOW_WIDTH * 2 + 530;
    stage2.enemies[5].y    = 0;
    stage2.enemies[5].vx   = 0;
    stage2.enemies[5].vx   = 0;

    stage2.bulletNum = 0;

    stage2.phase = BATTLE;

    return stage2;
}

Stage makeStage3()
{
    Stage stage3;

    EnemyInfo enemieInfos[5] = makeEnemy();

    stage3.player.info = playerInfo;
    stage3.player.hp   = playerInfo.hp;
    stage3.player.x    = 100;
    stage3.player.y    = 0;
    stage3.player.vx   = 0;
    stage3.player.vy   = 0;

    stage3.enemyNum        = 9;
    stage3.enemies[0].info = enemieInfos[0];
    stage3.enemies[0].hp   = enemieInfos[0].hp;
    stage3.enemies[0].x    = 310;
    stage3.enemies[0].y    = 0;
    stage3.enemies[0].vx   = 0;
    stage3.enemies[0].vx   = 0;

    stage3.enemies[1].info = enemieInfos[0];
    stage3.enemies[1].hp   = enemieInfos[0].hp;
    stage3.enemies[1].x    = 420;
    stage3.enemies[1].y    = 0;
    stage3.enemies[1].vx   = 0;
    stage3.enemies[1].vx   = 0;

    stage3.enemies[2].info = enemieInfos[1];
    stage3.enemies[2].hp   = enemieInfos[1].hp;
    stage3.enemies[2].x    = 530;
    stage3.enemies[2].y    = 0;
    stage3.enemies[2].vx   = 0;
    stage3.enemies[2].vx   = 0;

    stage3.enemies[3].info = enemieInfos[1];
    stage3.enemies[3].hp   = enemieInfos[1].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH + 310;
    stage3.enemies[3].y    = 0;
    stage3.enemies[3].vx   = 0;
    stage3.enemies[3].vx   = 0;

    stage3.enemies[3].info = enemieInfos[1];
    stage3.enemies[3].hp   = enemieInfos[1].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH + 420;
    stage3.enemies[3].y    = 0;
    stage3.enemies[3].vx   = 0;
    stage3.enemies[3].vx   = 0;

    stage3.enemies[3].info = enemieInfos[2];
    stage3.enemies[3].hp   = enemieInfos[2].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH + 530;
    stage3.enemies[3].y    = 0;
    stage3.enemies[3].vx   = 0;
    stage3.enemies[3].vx   = 0;

    stage3.enemies[3].info = enemieInfos[2];
    stage3.enemies[3].hp   = enemieInfos[2].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH * 2 + 310;
    stage3.enemies[3].y    = 0;
    stage3.enemies[3].vx   = 0;
    stage3.enemies[3].vx   = 0;
    stage3.bulletNum       = 0;

    stage3.enemies[3].info = enemieInfos[2];
    stage3.enemies[3].hp   = enemieInfos[2].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH * 2 + 420;
    stage3.enemies[3].y    = 0;
    stage3.enemies[3].vx   = 0;
    stage3.enemies[3].vx   = 0;

    stage3.enemies[3].info = enemieInfos[3];
    stage3.enemies[3].hp   = enemieInfos[3].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH * 2 + 530;
    stage3.enemies[3].y    = 0;
    stage3.enemies[3].vx   = 0;
    stage3.enemies[3].vx   = 0;

    stage3.bulletNum = 0;

    stage3.phase = BATTLE;

    return stage3;
}