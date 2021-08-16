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

int partsWinEvent();
ActionCommand getPartsAction();
int getObjIDFromList(int x, int y);

void placeBody(BodyInfo body, int x, int y);
void placeGun(GunInfo gun, int x, int y);

void getGunList();
void getBodyList();

int initPartsWin()
{
    partsBG     = SDL_CreateTextureFromSurface(render, IMG_Load("partsWindow.png"));
    gunListTex  = SDL_CreateTextureFromSurface(render, IMG_Load("gun_list.png"));
    bodyListTex = SDL_CreateTextureFromSurface(render, IMG_Load("body_list.png"));

    bodyDraw      = SDL_CreateRGBSurface(0, 499, 182, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    SDL_Rect rect = { 0, 0, 499, 128 };
    SDL_FillRect(bodyDraw, &rect, 0xffffff00);

    getBodyList();
    getGunList();

    IMG_Quit();

    return 0;
}

void drawPartsWin()
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    SDL_Rect mainBGSrc     = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect mainWinTarget = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(render, partsBG, &mainBGSrc, &mainWinTarget);

    SDL_Rect drawSrc         = { 0, 0, 499, 182 };
    SDL_Rect drawTarget      = { 50, 43, 499, 182 };
    SDL_Texture *drawTexture = SDL_CreateTextureFromSurface(render, bodyDraw);
    SDL_RenderCopy(render, drawTexture, &drawSrc, &drawTarget);

    char playerMoney[10];
    sprintf(playerMoney, "%d", playerInfo.money);
    surface              = TTF_RenderUTF8_Blended(font30, playerMoney, (SDL_Color){ 0, 0, 0, 255 });
    SDL_Rect moneyTarget = { 300, 5, surface->w, surface->h };
    texture              = SDL_CreateTextureFromSurface(render, surface);
    SDL_RenderCopy(render, texture, NULL, &moneyTarget);

    SDL_Rect listSrc    = { 366, 38, 499, 192 };
    SDL_Rect listTarget = { 366, 38, 274, 192 };

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
        pickedSrc.x    = 0;
        pickedSrc.y    = 0;
        pickedSrc.w    = bodyList[picked].surface->w;
        pickedSrc.h    = bodyList[picked].surface->h;
        pickedTarget.x = mousePointer[0] - bodyList[picked].surface->w / 2;
        pickedTarget.y = mousePointer[1] - bodyList[picked].surface->h;
        pickedTarget.w = bodyList[picked].surface->w;
        pickedTarget.h = bodyList[picked].surface->h;
        SDL_RenderCopy(render, texture, &pickedSrc, &pickedTarget);
        break;
    case GUN_LIST:
        SDL_RenderCopy(render, gunListTex, &listSrc, &listTarget);
        break;
    case GUN_PLACE:
        texture        = SDL_CreateTextureFromSurface(render, gunList[picked].surface);
        pickedSrc.x    = 0;
        pickedSrc.y    = 0;
        pickedSrc.w    = gunList[picked].surface->w;
        pickedSrc.h    = gunList[picked].surface->h;
        pickedTarget.x = mousePointer[0] - gunList[picked].surface->w / 2;
        pickedTarget.y = mousePointer[1] - gunList[picked].surface->h;
        pickedTarget.w = gunList[picked].surface->w;
        pickedTarget.h = gunList[picked].surface->h;
        SDL_RenderCopy(render, texture, &pickedSrc, &pickedTarget);
        break;
    }

    SDL_RenderPresent(render);

    partsWinEvent();

    return;
}

int partsWinEvent()
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
            initMainWin();
            break;
        case SAVE:
            memcpy(playerInfo.surface, bodyDraw, sizeof(SDL_Surface));
            playerInfo.texture = SDL_CreateTextureFromSurface(render, playerInfo.surface);
            isCreatedPlayer    = SDL_TRUE;
            break;
        case CLEAR:
            rect.w = bodyDraw->w;
            rect.h = bodyDraw->h;
            SDL_FillRect(bodyDraw, &rect, 0xffffff00);
            playerInfo.money = playerInfo.initMoney;
            break;
        case CLOSE_LIST:
            editMode = NORMAL;
            break;
        case GUN:
            editMode = GUN_LIST;
            break;
        case BODY:
            editMode = BODY_LIST;
            break;
        case PLACE_OBJECT:
            if (editMode == GUN_PLACE) {
                placeGun(
                    gunList[picked],
                    mousePointer[0] - gunList[picked].surface->w / 2,
                    mousePointer[1] - gunList[picked].surface->h);
            } else if (editMode == BODY_PLACE) {
                placeBody(
                    bodyList[picked],
                    mousePointer[0] - bodyList[picked].surface->w / 2,
                    mousePointer[1] - bodyList[picked].surface->h);
            }
            editMode = NORMAL;
            break;

        case PICK_OBJECT:
            if (editMode == GUN_LIST) {
                editMode = GUN_PLACE;
            } else {
                editMode = BODY_PLACE;
            }
            break;
        case EXIT:
            gameMode = EXIT_GAME;
            break;
        case NONE:
            break;
        }
        inputInfo.mouseL = SDL_FALSE;
    }
    return 0;
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
            picked    = getObjIDFromList(x, y);
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
    bodyList[0].surface = IMG_Load("body1.png");
    bodyList[0].hp      = 50;
    bodyList[0].cost    = 50;

    bodyList[1].surface = IMG_Load("body2.png");
    bodyList[1].hp      = 35;
    bodyList[1].cost    = 35;

    bodyList[2].surface = IMG_Load("body3.png");
    bodyList[2].hp      = 17;
    bodyList[2].cost    = 17;

    bodyList[3].surface = IMG_Load("body4.png");
    bodyList[3].hp      = 13;
    bodyList[3].cost    = 13;

    bodyList[4].surface = IMG_Load("body5.png");
    bodyList[4].hp      = 5;
    bodyList[4].cost    = 5;

    bodyList[5].surface = IMG_Load("body6.png");
    bodyList[5].hp      = 5;
    bodyList[5].cost    = 5;
}

void getGunList()
{
    char imageName[3][9] = {
        "gun1.png\0",
        "gun2.png\0",
        "gun3.png\0"
    };
    char bulletImageName[3][12] = {
        "bullet1.png\0",
        "bullet2.png\0",
        "bullet3.png\0"
    };

    for (int i = 0; i < GUN_TYPE_NUM; i++) {
        gunList[i].surface = IMG_Load(imageName[i]);
        gunList[i].cost    = 20 + i * i * 10;

        BulletInfo binfo;
        binfo.surface     = IMG_Load(bulletImageName[i]);
        binfo.texture     = SDL_CreateTextureFromSurface(render, binfo.surface);
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
    if (playerInfo.money < body.cost) {
        editMode = NORMAL;
    } else {
        SDL_Rect tar = { x - 50, y - 43, body.surface->w, body.surface->h };
        SDL_BlitSurface(body.surface, NULL, bodyDraw, &tar);
        playerInfo.hp += body.hp;
        playerInfo.money -= body.cost;
    }
}

void placeGun(GunInfo gun, int x, int y)
{
    if (playerInfo.money < gun.cost) {
        editMode = NORMAL;
    } else {
        SDL_Rect tar = { x - 50, y - 43, gun.surface->w, gun.surface->h };
        SDL_BlitSurface(gun.surface, NULL, bodyDraw, &tar);
        playerInfo.gun[playerInfo.gunNum] = gun;
        playerInfo.gunNum++;
        playerInfo.money -= gun.cost;
    }
}
