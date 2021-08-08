/*
 ゲーム名   : blackship
 制作者     : 石原 実
 制作日時   : 2021/07/01
 
 ファイル名 : blackship
 機能       : メインシステム、モジュール呼び出し元
 */

#include "blackship.h"
#include <stdio.h>

void exitGame(ExitCode exitStatus);
static int InputEvent(void* data);
void getInputFromEvent(SDL_Event event);

SDL_mutex* mxLock;
InputInfo inputInfo = { SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE, 0, 0 };
GameMode gameMode   = MAIN_WINDOW;

PlayerInfo playerInfo;
SDL_bool isCreatedPlayer = SDL_FALSE;

/* main */
int main(int argc, char* argv[])
{
    int drawingDrutation = 100;

    //初期化
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        exitGame(FAILED_INIT_SDL);
    }

    SDL_Thread* eventThread = SDL_CreateThread(InputEvent, "InputEvent", &inputInfo);
    if (eventThread == NULL) {
        exitGame(FAIELD_INIT_THREAD);
    }
    SDL_DetachThread(eventThread);

    mxLock = SDL_CreateMutex();
    if (mxLock == NULL) {
        exitGame(FAILED_INIT_MXLOCK);
    }

    if (InitWindow() < 0) {
        exitGame(FAILED_INIT_WINDOW);
    }

    if (initMainWin() < 0) {
        exitGame(FAILED_INIT_MAIN_WIN);
    }

    if (initPartsWin() < 0) {
        exitGame(FAILED_INIT_PARTS_WIN);
    }

    initActionWin();

    while (1) {
        if (gameMode == EXIT_GAME)
            break;

        SDL_PumpEvents();

        //各モード処理
        if (0 == SDL_LockMutex(mxLock)) {
            switch (gameMode) {
            case MAIN_WINDOW:
                drawMainWin();
                break;
            case PARTS_WINDOW:
                drawPartsWin();
                break;
            case STAGE_WINDOW:
                break;
            case ACTION_WINDOW:
                if (isCreatedPlayer) {
                    drawActionWin(drawingDrutation);
                } else {
                    gameMode = MAIN_WINDOW;
                }
                break;
            case EXIT_GAME:
                exitGame(NONE_ERROR);
            }
            SDL_RenderPresent(render);
            SDL_UnlockMutex(mxLock);
        }
        SDL_Delay(30); //動作が不安定になるため20->40に変更
    }

    /** 終了処理 **/

    return 0;
}

void exitGame(ExitCode exitCode)
{
    switch (exitCode) {
    case NONE_ERROR:
        DestroyWindow();
        SDL_DestroyMutex(mxLock);
        SDL_Quit();
        exit(0);

    case FAILED_INIT_ACTION_WIN:
        destroyActionWin();

    case FAILED_INIT_PARTS_WIN:
        destroyPartsWin();

    case FAILED_INIT_MAIN_WIN:
        destroyMainWin();

    case FAILED_INIT_WINDOW:
        DestroyWindow();

    case FAILED_INIT_MXLOCK:
        SDL_DestroyMutex(mxLock);

    case FAIELD_INIT_THREAD:

    case FAILED_INIT_SDL:
        SDL_Quit();
    }
    exit(1);
}

int InputEvent(void* arg)
{
    SDL_Event event;
    while (gameMode != EXIT_GAME) {
        if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) > 0) {
            getInputFromEvent(event);
        }
    }
    return 0;
}

void getInputFromEvent(SDL_Event event)
{
    inputInfo.mouseL = SDL_FALSE;
    switch (event.type) {
    case SDL_QUIT:
        /** スレッド終了指示 **/
        gameMode = EXIT_GAME;
        break;
    case SDL_KEYDOWN:
        if (event.key.repeat)
            break;
        /** キーが押された方向を設定 **/
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            inputInfo.escape = SDL_TRUE;
            gameMode         = EXIT_GAME;
            break;
        case SDLK_RIGHT:
            inputInfo.right = SDL_TRUE;
            break;
        case SDLK_LEFT:
            inputInfo.left = SDL_TRUE;
            break;
        case SDLK_a:
            inputInfo.left = SDL_TRUE;
            break;
        case SDLK_d:
            inputInfo.right = SDL_TRUE;
            break;
        case SDLK_SPACE:
            inputInfo.fire = SDL_TRUE;
            break;
        case SDLK_q:
            inputInfo.skill = SDL_TRUE;
            break;
        default:
            break;
        }
        break;
    case SDL_KEYUP:
        if (event.key.repeat)
            break;
        /* 離されたときは解除 */
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            break;
        case SDLK_RIGHT:
            inputInfo.right = SDL_FALSE;
            break;
        case SDLK_LEFT:
            inputInfo.left = SDL_FALSE;
            break;
        case SDLK_a:
            inputInfo.left = SDL_FALSE;
            break;
        case SDLK_d:
            inputInfo.right = SDL_FALSE;
            break;
        case SDLK_SPACE:
            inputInfo.fire = SDL_FALSE;
            break;
        case SDLK_q:
            inputInfo.skill = SDL_FALSE;
            break;
        default:
            break;
        }
        break;
    case SDL_MOUSEMOTION: // マウスが移動した時
        inputInfo.mouseX = event.motion.x;
        inputInfo.mouseY = event.motion.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
        inputInfo.mouseX = event.button.x;
        inputInfo.mouseY = event.button.y;
        switch (event.button.button) {
        case SDL_BUTTON_LEFT:
            inputInfo.mouseL = SDL_TRUE;
            break;
        case SDL_BUTTON_MIDDLE:
            break;
        case SDL_BUTTON_RIGHT:
            break;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        inputInfo.mouseX = event.button.x;
        inputInfo.mouseY = event.button.y;
        switch (event.button.button) {
        case SDL_BUTTON_LEFT:
            inputInfo.mouseL = SDL_FALSE;
            break;
        case SDL_BUTTON_MIDDLE:
            break;
        case SDL_BUTTON_RIGHT:
            break;
        }
        break;
    }
}