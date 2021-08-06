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
    SDL_Surface *s = IMG_Load(MAIN_WINDOW_BG);
    if (s == NULL) {
        return -1;
    }
    mainBG = SDL_CreateTextureFromSurface(render, s);
    SDL_FreeSurface(s);
    if (mainBG == NULL) {
        return -1;
    }

    /** ウインドウへの描画 **/
    drawMainWin();

    /* image利用終了(テクスチャに転送後はゲーム中に使わないので) */
    IMG_Quit();

    return 0;
}

void drawMainWin()
{
    SDL_Rect mainBGSrc     = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect mainWinTarget = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(render, mainBG, &mainBGSrc, &mainWinTarget);
    SDL_RenderPresent(render);

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