#include "blackship.h"

typedef enum {
    EXIT         = -1,
    NONE         = 0,
    ACTION_PAUSE = 1,
    BACK         = 2
} ActionCommand;

int initActionWin(int stageNum);
int initStage(int stageNum);
void destroyActionWin();

void actionWinEvent();
void actionWinEnemyEvent();
void calcStage(int millis);

void drawActionWin(int millis);
void drawPlayer();
void drawEnemy();
void drawBullet();

EnemyInfo* makeEnemy();
Player makePlayer();
Stage makeStage1();
Stage makeStage2();
Stage makeStage3();

SDL_Texture* actionBG;
SDL_Surface* actionFront;
SDL_Texture* actionFrontTexture;
Stage stage;

int framePos = 0;

int initActionWin(int stageNum)
{
    SDL_Surface* actionBGSurface = IMG_Load("actionBG.png");
    actionBG                     = SDL_CreateTextureFromSurface(render, actionBGSurface);
    actionFront                  = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    SDL_FillRect(actionFront, NULL, 0xff0000ff);

    return initStage(stageNum);
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

void drawActionWin(int millis)
{
    calcStage(millis);
    SDL_Rect src       = { stage.player.x - 100, 0, 640, 270 };
    actionFrontTexture = SDL_CreateTextureFromSurface(render, actionFront);
    SDL_RenderCopy(render, actionFrontTexture, &src, NULL);
    SDL_RenderCopy(render, actionBG, NULL, NULL);
    drawBullet();
    drawEnemy();
    drawPlayer();

    actionWinEvent();
    SDL_RenderPresent(render);
    return;
}

void drawPlayer()
{
    SDL_Rect tar = {
        stage.player.x - framePos,
        270 - stage.player.y - 55,
        150, 55
    };
    SDL_RenderCopy(render, playerInfo.texture, NULL, &tar);
}

void drawEnemy()
{
    for (int i = 0; i < stage.enemyNum; i++) {
        SDL_Rect tar = {
            stage.enemies[i].x - framePos,
            270 - stage.enemies[i].y - 55,
            150, 55
        };
        SDL_RenderCopy(render, stage.enemies[i].info.texture, NULL, &tar);
    }
}

void drawBullet()
{
    for (int i = 0; i < stage.bulletNum; i++) {
        SDL_Rect tar = {
            stage.bullets[i].x - framePos,
            270 - stage.bullets[i].y,
            stage.bullets[i].info.surface->w / 4,
            stage.bullets[i].info.surface->h / 4
        };
        if (stage.bullets[i].vx == 0) {
            stage.bullets[i].vx = 0.00001;
        }
        float angle = 90 - atan(stage.bullets[i].vy / stage.bullets[i].vx) * 180 / 3.14;
        SDL_RenderCopyEx(render, stage.bullets[i].info.texture, NULL, &tar, angle, NULL, SDL_FLIP_NONE);
    }
}

void drawEffect()
{
}

void actionWinEvent()
{
    if (inputInfo.right) {
        stage.player.vx = 1;
    } else if (inputInfo.left) {
        stage.player.vx = -1;
    } else {
        stage.player.vx = 0;
    }

    if (inputInfo.mouseL) {
        stage.bullets[stage.bulletNum].info = playerInfo.gun[0].bullet;
        stage.bullets[stage.bulletNum].x    = stage.player.x;
        stage.bullets[stage.bulletNum].y    = stage.player.y + 10;
        stage.bullets[stage.bulletNum].vx   = 10;
        stage.bullets[stage.bulletNum].vy   = 10;

        stage.bulletNum += 1;
    }
}

void actionWinEnemyEvent()
{
}

void calcStage(int millis)
{
    stage.player.x += stage.player.vx;

    for (int i = 0; i < stage.bulletNum; i++) {
        stage.bullets[i].x += stage.bullets[i].vx;
        stage.bullets[i].y += stage.bullets[i].vy;

        stage.bullets[i].vy -= 0.2;
    }
}

EnemyInfo* makeEnemy()
{
    EnemyInfo* enemies = malloc(sizeof(EnemyInfo) * 5);

    enemies[0].gun.bullet.damage  = 20;
    enemies[0].gun.bullet.surface = IMG_Load("bullet1.png");
    enemies[0].gun.cost           = 0;
    enemies[0].gun.surface        = IMG_Load("gun1.png");
    enemies[0].hp                 = 150;
    enemies[0].speed              = 2;
    enemies[0].surface            = IMG_Load("enemy1.png");

    enemies[1].gun.bullet.damage  = 40;
    enemies[1].gun.bullet.surface = IMG_Load("bullet2.png");
    enemies[1].gun.cost           = 0;
    enemies[1].gun.surface        = IMG_Load("gun2.png");
    enemies[1].hp                 = 200;
    enemies[1].speed              = 2;
    enemies[1].surface            = IMG_Load("enemy2.png");

    enemies[2].gun.bullet.damage  = 40;
    enemies[2].gun.bullet.surface = IMG_Load("bullet2.png");
    enemies[2].gun.cost           = 0;
    enemies[2].gun.surface        = IMG_Load("gun2.png");
    enemies[2].hp                 = 300;
    enemies[2].speed              = 1;
    enemies[2].surface            = IMG_Load("enemy2.png");

    enemies[3].gun.bullet.damage  = 60;
    enemies[3].gun.bullet.surface = IMG_Load("bullet2.png");
    enemies[3].gun.cost           = 0;
    enemies[3].gun.surface        = IMG_Load("gun3.png");
    enemies[3].hp                 = 500;
    enemies[3].speed              = 1;
    enemies[3].surface            = IMG_Load("enemy3.png");

    enemies[4].gun.bullet.damage  = 100;
    enemies[4].gun.bullet.surface = IMG_Load("bullet3.png");
    enemies[4].gun.cost           = 0;
    enemies[4].gun.surface        = IMG_Load("gun3.png");
    enemies[4].hp                 = 200;
    enemies[4].speed              = 1;
    enemies[4].surface            = IMG_Load("enemy4.png");

    for (int i = 0; i < 5; i++) {
        enemies[i].gun.bullet.texture = SDL_CreateTextureFromSurface(render, enemies[i].gun.bullet.surface);
    }

    return enemies;
}

Stage makeStage1()
{
    Stage stage1;

    EnemyInfo* enemieInfos = makeEnemy();

    stage1.player = makePlayer();

    stage1.enemyNum = 4;

    stage1.enemies[0].info = enemieInfos[0];
    stage1.enemies[0].hp   = enemieInfos[0].hp;
    stage1.enemies[0].x    = 530;

    stage1.enemies[1].info = enemieInfos[0];
    stage1.enemies[1].hp   = enemieInfos[0].hp;
    stage1.enemies[1].x    = WINDOW_WIDTH + 420;

    stage1.enemies[2].info = enemieInfos[0];
    stage1.enemies[2].hp   = enemieInfos[0].hp;
    stage1.enemies[2].x    = WINDOW_WIDTH + 530;

    stage1.enemies[3].info = enemieInfos[1];
    stage1.enemies[3].hp   = enemieInfos[1].hp;
    stage1.enemies[3].x    = WINDOW_WIDTH * 2 + 530;

    stage1.bulletNum = 0;

    stage1.phase = BATTLE;

    for (int i = 0; i < stage1.enemyNum; i++) {
        stage1.enemies[i].y            = 0;
        stage1.enemies[i].vx           = 0;
        stage1.enemies[i].vx           = 0;
        stage1.enemies[i].info.texture = SDL_CreateTextureFromSurface(render, stage1.enemies[i].info.surface);
    }

    return stage1;
}

Stage makeStage2()
{
    Stage stage2;

    EnemyInfo* enemieInfos = makeEnemy();

    stage2.player = makePlayer();

    stage2.enemyNum        = 6;
    stage2.enemies[0].info = enemieInfos[0];
    stage2.enemies[0].hp   = enemieInfos[0].hp;
    stage2.enemies[0].x    = 420;

    stage2.enemies[1].info = enemieInfos[0];
    stage2.enemies[1].hp   = enemieInfos[0].hp;
    stage2.enemies[1].x    = 530;

    stage2.enemies[2].info = enemieInfos[0];
    stage2.enemies[2].hp   = enemieInfos[0].hp;
    stage2.enemies[2].x    = WINDOW_WIDTH + 420;

    stage2.enemies[3].info = enemieInfos[1];
    stage2.enemies[3].hp   = enemieInfos[1].hp;
    stage2.enemies[3].x    = WINDOW_WIDTH + 530;

    stage2.enemies[4].info = enemieInfos[1];
    stage2.enemies[4].hp   = enemieInfos[1].hp;
    stage2.enemies[4].x    = WINDOW_WIDTH * 2 + 420;

    stage2.enemies[5].info = enemieInfos[2];
    stage2.enemies[5].hp   = enemieInfos[2].hp;
    stage2.enemies[5].x    = WINDOW_WIDTH * 2 + 530;

    stage2.bulletNum = 0;

    stage2.phase = BATTLE;

    for (int i = 0; i < stage2.enemyNum; i++) {
        stage2.enemies[i].y            = 0;
        stage2.enemies[i].vx           = 0;
        stage2.enemies[i].vx           = 0;
        stage2.enemies[i].info.texture = SDL_CreateTextureFromSurface(render, stage2.enemies[i].info.surface);
    }

    return stage2;
}

Stage makeStage3()
{
    Stage stage3;

    EnemyInfo* enemieInfos = makeEnemy();

    stage3.player = makePlayer();

    stage3.enemyNum        = 9;
    stage3.enemies[0].info = enemieInfos[0];
    stage3.enemies[0].hp   = enemieInfos[0].hp;
    stage3.enemies[0].x    = 310;

    stage3.enemies[1].info = enemieInfos[0];
    stage3.enemies[1].hp   = enemieInfos[0].hp;
    stage3.enemies[1].x    = 420;

    stage3.enemies[2].info = enemieInfos[1];
    stage3.enemies[2].hp   = enemieInfos[1].hp;
    stage3.enemies[2].x    = 530;

    stage3.enemies[3].info = enemieInfos[1];
    stage3.enemies[3].hp   = enemieInfos[1].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH + 310;

    stage3.enemies[3].info = enemieInfos[1];
    stage3.enemies[3].hp   = enemieInfos[1].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH + 420;

    stage3.enemies[3].info = enemieInfos[2];
    stage3.enemies[3].hp   = enemieInfos[2].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH + 530;

    stage3.enemies[3].info = enemieInfos[2];
    stage3.enemies[3].hp   = enemieInfos[2].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH * 2 + 310;

    stage3.enemies[3].info = enemieInfos[2];
    stage3.enemies[3].hp   = enemieInfos[2].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH * 2 + 420;

    stage3.enemies[3].info = enemieInfos[3];
    stage3.enemies[3].hp   = enemieInfos[3].hp;
    stage3.enemies[3].x    = WINDOW_WIDTH * 2 + 530;

    stage3.bulletNum = 0;

    stage3.phase = BATTLE;

    for (int i = 0; i < stage3.enemyNum; i++) {
        stage3.enemies[i].y            = 0;
        stage3.enemies[i].vx           = 0;
        stage3.enemies[i].vx           = 0;
        stage3.enemies[i].info.texture = SDL_CreateTextureFromSurface(render, stage3.enemies[i].info.surface);
    }

    return stage3;
}

Player makePlayer()
{
    Player player;
    player.hp = playerInfo.hp;
    player.vx = 0;
    player.vy = 0;
    player.x  = 100;
    player.y  = 0;

    return player;
}