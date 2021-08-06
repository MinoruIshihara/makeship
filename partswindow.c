#include "blackship.h"

typedef enum {
    EXIT         = -1,
    NONE         = 0,
    BACK         = 1,
    SAVE         = 2,
    CLEAR        = 3,
    CLOSE_LIST   = 4,
    PLACE_OBJECT = 5,
    PICK_OBJECT  = 6,
    BODY         = 7,
    GUN          = 8
} ActionCommand;

typedef enum {
    NORMAL     = 0,
    BODY_LIST  = 1,
    BODY_PLACE = 2,
    GUN_LIST   = 4,
    GUN_PLACE  = 5
} EditMode;

SDL_Texture *partsBG;
SDL_Surface *bodyDraw;
SDL_Texture *gunListTex;
SDL_Texture *bodyListTex;

GunInfo gunList[MAX_GUN_NUM];
BodyInfo bodyList[MAX_BODY_NUM];

EditMode editMode = NORMAL;

SDL_bool dragging = SDL_FALSE;
int picked        = 0;
int mousePointer[2];

void partsWinEvent();
ActionCommand getPartsAction();

void placeBody(BodyInfo body, int x, int y);
void placeGun(GunInfo gun, int x, int y);
void getGunList();
void getBodyList();

int initPartsWin()
{
    partsBG     = SDL_CreateTextureFromSurface(render, IMG_Load(PARTS_WINDOW_BG));
    gunListTex  = SDL_CreateTextureFromSurface(render, IMG_Load("body_list.png"));
    bodyListTex = SDL_CreateTextureFromSurface(render, IMG_Load("gun_list.png"));

    bodyDraw = SDL_CreateRGBSurface(0, 500, 182, 8, 0, 0, 0, 0);

    getBodyList();
    getGunList();

    /** ウインドウへの描画 **/
    drawPartsWin();

    /* image利用終了(テクスチャに転送後はゲーム中に使わないので) */
    IMG_Quit();

    return 0;
}

void drawPartsWin()
{
    SDL_Texture *texture;

    SDL_Rect mainBGSrc     = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect mainWinTarget = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(render, partsBG, &mainBGSrc, &mainWinTarget);

    SDL_Texture *drawTexture = SDL_CreateTextureFromSurface(render, bodyDraw);
    SDL_Rect drawSrc         = { 0, 0, 500, 182 };
    SDL_Rect drawTarget      = { 50, 43, 550, 225 };
    SDL_RenderCopy(render, drawTexture, &drawSrc, &drawTarget);

    SDL_Rect listSrc    = { 366, 38, 554, 229 };
    SDL_Rect listTarget = { 366, 38, 554, 229 };

    SDL_Rect pickedSrc;
    SDL_Rect pickedTarget;

    switch (editMode) {
    case NORMAL:
        break;
    case BODY_LIST:
        SDL_RenderCopy(render, bodyListTex, &listSrc, &listTarget);
        break;
    case BODY_PLACE:
        texture        = SDL_CreateTextureFromSurface(render, bodyList[picked].surface);
        pickedSrc.w    = bodyList[picked].surface->w;
        pickedSrc.h    = bodyList[picked].surface->h;
        pickedTarget.x = mousePointer[0];
        pickedTarget.y = mousePointer[1];
        pickedTarget.w = bodyList[picked].surface->w;
        pickedTarget.h = bodyList[picked].surface->h;
        SDL_RenderCopy(render, texture, &pickedSrc, &pickedTarget);
        break;
    case GUN_LIST:
        SDL_RenderCopy(render, gunListTex, &listSrc, &listTarget);
        break;
    case GUN_PLACE:
        texture        = SDL_CreateTextureFromSurface(render, gunList[picked].surface);
        pickedSrc.w    = gunList[picked].surface->w;
        pickedSrc.h    = gunList[picked].surface->h;
        pickedTarget.x = mousePointer[0];
        pickedTarget.y = mousePointer[1];
        pickedTarget.w = gunList[picked].surface->w;
        pickedTarget.h = gunList[picked].surface->h;
        SDL_RenderCopy(render, texture, &pickedSrc, &pickedTarget);
        break;
    }

    SDL_RenderPresent(render);

    partsWinEvent();

    return;
}

void partsWinEvent()
{
    SDL_Rect rect = { 0, 0, 256, 128 };

    if (getPartsAction() == PLACE_OBJECT) {
        mousePointer[0] = inputInfo.mouseX;
        mousePointer[1] = inputInfo.mouseY;
    }

    if (inputInfo.mouseL) {
        switch (getPartsAction()) {
        case BACK:
            gameMode = MAIN_WINDOW;
            break;
        case SAVE:
            break;
        case CLEAR:
            SDL_FillRect(playerInfo.surface, &rect, 255);
            break;
        case CLOSE_LIST:
            editMode = BODY_LIST;
            break;
        case GUN:
            editMode = GUN_LIST;
            break;
        case BODY:
            editMode = BODY_LIST;
            break;
        case PLACE_OBJECT:
            if (editMode == GUN_PLACE) {
                placeGun(gunList[picked], mousePointer[0], mousePointer[1]);
            } else if (editMode == BODY_PLACE) {
                placeBody(bodyList[picked], mousePointer[0], mousePointer[1]);
            }
            editMode = NORMAL;

        case PICK_OBJECT:
            if (editMode == GUN_LIST) {
                editMode = GUN_PLACE;
            } else {
                editMode = BODY_PLACE;
            }
        case EXIT:
            gameMode = EXIT_GAME;
            break;
        case NONE:
            inputInfo.mouseL = SDL_FALSE;
            break;
        }
    }
}

ActionCommand getPartsAction()
{
    int x = inputInfo.mouseX;
    int y = inputInfo.mouseY;

    ActionCommand actionCmd = NONE;

    switch (editMode) {
    case BODY_PLACE:
    case GUN_PLACE:
        if (50 < x && x < 550 && 43 < y && y < 225) {
            actionCmd = PLACE_OBJECT;
        }
        break;
    case BODY_LIST:
    case GUN_LIST:
        if (370 < x && x < 550 && 40 < y && y < 190) {
            actionCmd = PICK_OBJECT;
        } else {
            actionCmd = CLOSE_LIST;
        }
        break;
    case NORMAL:
        if (50 < x && x < 210 && 250 < y && y < 310) {
            actionCmd = BACK;
        } else if (390 < x && x < 550 && 250 < y && y < 310) {
            actionCmd = SAVE;
        } else if (240 < x && x < 345 && 230 < y && y < 270) {
            actionCmd = CLEAR;
        } else if (50 < x && x < 126 && 5 < y && y < 37) {
            actionCmd = BODY;
        } else if (130 < x && x < 208 && 5 < y && y < 37) {
            actionCmd = GUN;
        }
        break;
    }
    return actionCmd;
}

void destroyPartsWin()
{
    return;
}

void setPixel(SDL_bool data, int x, int y)
{
}

void getBodyList()
{
    bodyList[0].surface = SDL_CreateRGBSurface(0, 55, 30, 8, 0, 0, 0, 0);
    bodyList[0].hp      = 50;

    bodyList[1].surface = SDL_CreateRGBSurface(0, 30, 30, 8, 0, 0, 0, 0);
    bodyList[1].hp      = 50;

    bodyList[2].surface = SDL_CreateRGBSurface(0, 30, 30, 8, 255, 255, 255, 0);
    SDL2_filledTrgonRGBA(bodyList[2].surface, 0, 0, 30, 0, 0, 30, 255, 255, 255, 0);
    bodyList[2].hp = 30;

    bodyList[3].surface = SDL_CreateRGBSurface(0, 23, 30, 8, 255, 255, 255, 0);
    SDL_filledTrgonRGBA(bodyList[3].surface, 0, 0, 23, 0, 0, 30, 255, 255, 255, 0);
    bodyList[3].hp = 25;

    bodyList[4].surface = SDL_CreateRGBSurface(0, 30, 3, 8, 0, 0, 0, 0);
    bodyList[4].hp      = 5;

    bodyList[5].surface = SDL_CreateRGBSurface(0, 3, 30, 8, 0, 0, 0, 0);
    bodyList[5].hp      = 5;
}

void getGunList()
{
    char imageName[3][8] = {
        "gun1.png",
        "gun2.png",
        "gun3.png"
    };
    char bulletImageName[3][11] = {
        "bullet1.png",
        "bullet2.png",
        "bullet3.png"
    };

    for (int i = 0; i < GUN_TYPE_NUM; i++) {
        gunList[i].surface = IMG_Load(imageName[i]);

        BulletInfo binfo;
        binfo.surface     = IMG_Load(bulletImageName[i]);
        binfo.damage      = 20 + i * i * 10;
        gunList[i].bullet = binfo;
    }
}

int getObjIDFromList(int x, int y)
{
    int row, col;
    int objID = 0;
    if (x < 460) {
        col = 0;
    } else {
        col = 1;
    }
    if (y <= 90) {
        row = 0;
    } else if (90 < y && y <= 140) {
        row = 1;
    } else {
        row = 2;
    }

    objID = row * 2 + col;

    return objID;
}

void placeBody(BodyInfo body, int x, int y)
{
}

void placeGun(BodyInfo body, int x, int y)
{
}
