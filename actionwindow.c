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

void shootGun(int gunNum);

void gameClear();
void gameOver();

SDL_bool collisionPlayer(Bullet b, Player p);
SDL_bool collisionEnemy(Bullet b, Enemy e);

EnemyInfo* makeEnemy();
Player makePlayer();
Stage makeStage1();
Stage makeStage2();
Stage makeStage3();

SDL_Texture* actionBG;
SDL_Surface* actionFront;
SDL_Texture* actionFrontTexture;
Stage stage;

int framePos;

int initActionWin(int stageNum)
{
    SDL_Surface* actionBGSurface = IMG_Load("actionBG.png");
    actionBG                     = SDL_CreateTextureFromSurface(render, actionBGSurface);
    framePos                     = 0;

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
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_Delay(10);
    SDL_RenderCopy(render, actionBG, NULL, NULL);
    drawBullet();
    drawEnemy();
    drawPlayer();

    actionWinEvent();
    SDL_RenderPresent(render);
    calcStage(millis);
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
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    int len = stage.player.hp * 100 / playerInfo.hp;
    SDL_RenderDrawLine(render, stage.player.x - framePos, 270 - stage.player.y - 45, stage.player.x + len - framePos, 270 - stage.player.y - 45);
}

void drawEnemy()
{
    for (int i = 0; i < stage.enemyNum; i++) {
        if (stage.enemies[i].hp > 0) {
            SDL_Rect tar = {
                stage.enemies[i].x - framePos,
                270 - stage.enemies[i].y - 45,
                125, 45
            };
            SDL_RenderCopy(render, stage.enemies[i].info.texture, NULL, &tar);
            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
            SDL_RenderDrawLine(render, stage.enemies[i].x - framePos, 270 - stage.enemies[i].y - 45, stage.enemies[i].x + stage.enemies[i].hp - framePos, 270 - stage.enemies[i].y - 45);
        }
    }
}

void drawBullet()
{
    for (int i = 0; i < BULLET_NUM; i++) {
        if (stage.bullets[i].onStage) {
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
}

void drawEffect()
{
}

void actionWinEvent()
{
    if (inputInfo.right) {
        stage.player.vx = 4;
    } else if (inputInfo.left) {
        stage.player.vx = -4;
    } else {
        stage.player.vx = 0;
    }

    if (inputInfo.mouseL) {
        for (int i = 0; i < playerInfo.gunNum; i++) {
            if (playerInfo.gun[i].t <= 0) {
                shootGun(i);
            }
        }
    }

    for (int i = 0; i < playerInfo.gunNum; i++) {
        if (0 < playerInfo.gun[i].t) {
            playerInfo.gun[i].t -= 0.1;
        }
    }
}

void shootGun(int gunNum)
{
    stage.bullets[stage.bulletNum].info = playerInfo.gun[gunNum].info.bullet;
    stage.bullets[stage.bulletNum].x    = stage.player.x + (playerInfo.gun[gunNum].x + playerInfo.gun[gunNum].info.surface->w) / 4;
    stage.bullets[stage.bulletNum].y    = stage.player.y + 10;
    double x                            = inputInfo.mouseX - (stage.player.x + 75 - framePos);
    double sita                         = 0.785;
    if (x <= 490 && 0 <= x) {
        sita = 0.5 * asin(x / 490.0);
    }
    stage.bullets[stage.bulletNum].vx = 10.0 * cos(sita);
    stage.bullets[stage.bulletNum].vy = 10.0 * sin(sita);

    stage.bullets[stage.bulletNum].onStage = SDL_TRUE;
    stage.bullets[stage.bulletNum].t       = 0;

    stage.bulletNum += 1;
    if (stage.bulletNum > BULLET_NUM) {
        stage.bulletNum = 0;
    }

    playerInfo.gun[gunNum].t = playerInfo.gun[gunNum].info.duration;
}

void actionWinEnemyEvent()
{
}

void calcStage(int millis)
{
    stage.player.x += stage.player.vx;
    if (320 < stage.player.x + 75 && stage.player.x < 1600) {
        framePos = stage.player.x + 75 - 320;
    }

    for (int i = 0; i < BULLET_NUM; i++) {
        if (stage.bullets[i].onStage) {
            stage.bullets[i].t++;

            stage.bullets[i].x += stage.bullets[i].vx;
            stage.bullets[i].y += stage.bullets[i].vy;

            stage.bullets[i].vy -= 0.2;

            if (stage.bullets[i].y < 0) {
                stage.bullets[i].onStage = SDL_FALSE;
            }

            for (int j = 0; j < stage.enemyNum; j++) {
                if (0 < stage.enemies[j].hp && 4 < stage.bullets[i].t && collisionEnemy(stage.bullets[i], stage.enemies[j])) {
                    stage.enemies[j].hp -= stage.bullets[i].info.damage;
                    if (stage.enemies[j].hp < 1) {
                        stage.score += stage.enemies[j].info.hp;
                        if (j == stage.enemyNum - 1) {
                            gameClear();
                        }
                    }
                    stage.bullets[i].onStage = SDL_FALSE;
                }
            }

            if (20 < stage.bullets[i].t && collisionPlayer(stage.bullets[i], stage.player)) {
                stage.player.hp -= stage.bullets[i].info.damage;
                if (stage.player.hp < 1) {
                    gameOver();
                }
                stage.bullets[i].onStage = SDL_FALSE;
            }
        }
    }
}

SDL_bool collisionEnemy(Bullet b, Enemy e)
{
    int pointX = (b.x + b.info.surface->w / 8 - e.x) * e.info.surface->w / 125;
    int pointY = (45 - (b.y - b.info.surface->h / 8)) * e.info.surface->h / 45;
    //unsigned char* pixels = (unsigned char*)e.info.surface->pixels;
    return 0 <= pointX && pointX < e.info.surface->w && 0 <= pointY && pointY < e.info.surface->h;
}

SDL_bool collisionPlayer(Bullet b, Player p)
{
    int pointX            = b.x + b.info.surface->w / 8 - p.x;
    int pointY            = 55 - (b.y - b.info.surface->h / 8);
    unsigned char* pixels = (unsigned char*)playerInfo.surface->pixels;
    if (0 <= pointX && pointX < 150 && 0 <= pointY && pointY < 55) {
        int a = pixels[4 * (pointY * playerInfo.surface->w) + pointX + 3];
        return (a == 255);
    } else {
        return SDL_FALSE;
    }
}

void gameClear()
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font30, "完了", (SDL_Color){ 0, 0, 0, 255 });
    SDL_Rect tar         = { 320 - surface->w / 2, 160 - surface->h / 2, surface->w, surface->h };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_RenderCopy(render, texture, NULL, &tar);
    SDL_RenderPresent(render);
    SDL_Delay(3000);
    playerInfo.money += stage.score;
    gameMode = MAIN_WINDOW;
    initMainWin();
}

void gameOver()
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font30, "完了", (SDL_Color){ 0, 0, 0, 255 });
    SDL_Rect tar         = { 320 - surface->w / 2, 160 - surface->h / 2, surface->w, surface->h };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_RenderCopy(render, texture, NULL, &tar);
    SDL_RenderPresent(render);
    SDL_Delay(3000);
    gameMode = MAIN_WINDOW;
    initMainWin();
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

    for (int i = 0; i < BULLET_NUM; i++) {
        stage1.bullets[i].onStage = SDL_FALSE;
    }

    stage1.score = 0;

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

    for (int i = 0; i < BULLET_NUM; i++) {
        stage2.bullets[i].onStage = SDL_FALSE;
    }

    stage2.score = 0;

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

    for (int i = 0; i < BULLET_NUM; i++) {
        stage3.bullets[i].onStage = SDL_FALSE;
    }

    stage3.score = 0;

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