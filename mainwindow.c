#include "blackship.h"

typedef enum {
    EXIT   = -1,
    NONE   = 0,
    PARTS  = 1,
    DEPLOY = 2,
} ActionCommand;

SDL_Window *window;
SDL_Renderer *render;

SDL_Texture *mainBG;

SDL_bool firstDraw;

void mainWinEvent();
ActionCommand getMainAction();

int InitWindow(void)
{
    /* SDL_image初期化 */
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        return -1;
    }

    /** メインのウインドウ(表示画面)とレンダラーの作成 **/
    window = SDL_CreateWindow("shoot", 80, 50, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == NULL)
        return -1;

    render = SDL_CreateRenderer(window, -1, 0);
    if (render == NULL)
        return -1;

    return 0;
}

/* ウインドウの終了処理 */
void DestroyWindow(void)
{
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
}

int initMainWin()
{
    firstDraw = SDL_TRUE;

    SDL_Surface *s
        = IMG_Load("/home/vmware/git/makeship/mainWindow.png");
    if (s == NULL) {
        return -1;
    }
    mainBG = SDL_CreateTextureFromSurface(render, s);
    SDL_FreeSurface(s);
    if (mainBG == NULL) {
        return -1;
    }

    SDL_Rect mainBGSrc     = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect mainWinTarget = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(render, mainBG, &mainBGSrc, &mainWinTarget);

    char playerMoney[10];
    sprintf(playerMoney, "%d", playerInfo.money);
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font40, playerMoney, (SDL_Color){ 0, 0, 0, 255 });
    SDL_Rect moneyTarget = { 85, 10, surface->w, surface->h };
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_RenderCopy(render, texture, NULL, &moneyTarget);

    char playerExp[10];
    sprintf(playerExp, "%d", playerInfo.xp);
    surface            = TTF_RenderUTF8_Blended(font40, playerExp, (SDL_Color){ 0, 0, 0, 255 });
    SDL_Rect expTarget = { 295, 10, surface->w, surface->h };
    texture            = SDL_CreateTextureFromSurface(render, surface);
    SDL_RenderCopy(render, texture, NULL, &expTarget);

    if (isCreatedPlayer) {
        int atk = 0;
        for (int i = 0; i < playerInfo.gunNum; i++) {
            atk += playerInfo.gun[i].bullet.damage;
        }

        SDL_Rect playerSrc    = { 0, 0, 500, 182 };
        SDL_Rect playerTarget = { 454, 166, 150, 55 };
        texture               = SDL_CreateTextureFromSurface(render, playerInfo.surface);
        SDL_RenderCopy(render, texture, &playerSrc, &playerTarget);

        char playerHP[100];
        sprintf(playerHP, "耐久 : %d", playerInfo.hp);
        surface = TTF_RenderUTF8_Blended(font, playerHP, (SDL_Color){ 0, 0, 0, 255 });
        texture = SDL_CreateTextureFromSurface(render, surface);

        SDL_Rect textSrc = { 0, 0, surface->w, surface->h };
        SDL_Rect textTar = { 454, 110, surface->w, surface->h };
        SDL_RenderCopy(render, texture, &textSrc, &textTar);

        char playerAtk[100];
        sprintf(playerAtk, "攻撃 : %d", atk);
        surface = TTF_RenderUTF8_Blended(font, playerAtk, (SDL_Color){ 0, 0, 0, 255 });
        texture = SDL_CreateTextureFromSurface(render, surface);

        textTar.y = 140;
        SDL_RenderCopy(render, texture, &textSrc, &textTar);
    }

    SDL_RenderPresent(render);

    /* image利用終了(テクスチャに転送後はゲーム中に使わないので) */
    IMG_Quit();

    return 0;
}

void drawMainWin()
{
    mainWinEvent();

    return;
}

void mainWinEvent()
{
    if (inputInfo.mouseL) {
        switch (getMainAction()) {
        case DEPLOY:
            gameMode = ACTION_WINDOW;
            break;
        case PARTS:
            gameMode = PARTS_WINDOW;
            break;
        case EXIT:
            gameMode = EXIT_GAME;
            break;
        default:
            break;
        }
    }
}

ActionCommand getMainAction()
{
    int x = inputInfo.mouseX;
    int y = inputInfo.mouseY;

    ActionCommand actionCmd = NONE;

    if (20 < x && x < 425 && 70 < y && y < 240) {
        actionCmd = DEPLOY;
    } else if (455 < x && x < 605 && 230 < y && y < 290) {
        actionCmd = PARTS;
    } else if (515 < x && x < 615 && 8 < y && y < 55) {
        actionCmd = EXIT;
    }
    return actionCmd;
}

void destroyMainWin()
{
    SDL_DestroyTexture(mainBG);
    return;
}