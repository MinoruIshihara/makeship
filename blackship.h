#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/* ウインドウサイズ */
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

#define SHIP_WIDTH 256
#define SHIP_HEIGHT 128

#define ENEMY_NUM 4
#define BULLET_NUM 64
#define GUN_TYPE_NUM 3
#define MAX_GUN_NUM 10
#define BODY_TYPE_NUM 6
#define MAX_BODY_NUM 32

#define SAVE_TYPE_NUM 16

#define MAIN_WINDOW_BG "./mainWindow.png"
#define PARTS_WINDOW_BG "./partsWindow.png"

extern int initMainWin();
extern int initPartsWin();
extern int initActionWin();

extern void drawMainWin();
extern void drawPartsWin();
extern void drawActionWin(int millis);

extern void destroyMainWin();
extern void destroyPartsWin();
extern void destroyActionWin();

typedef enum {
    EXIT_GAME     = -1,
    MAIN_WINDOW   = 0,
    PARTS_WINDOW  = 1,
    STAGE_WINDOW  = 2,
    ACTION_WINDOW = 3
} GameMode;

typedef enum {
    PAUSE      = -1,
    STAGE_1    = 1,
    STAGE_2    = 2,
    STAGE_3    = 3,
    GAME_CLEAR = 4,
    GAME_OVER  = 5
} GameStatus;

typedef enum {
    NONE_ERROR             = 0,
    FAILED_INIT_WINDOW     = -1,
    FAILED_INIT_SDL        = -2,
    FAIELD_INIT_THREAD     = -3,
    FAILED_INIT_MXLOCK     = -4,
    FAILED_INIT_MAIN_WIN   = -5,
    FAILED_INIT_PARTS_WIN  = -6,
    FAILED_INIT_ACTION_WIN = -7
} ExitCode;

/*弾丸情報の構造体*/
typedef struct {
    SDL_Surface* surface;
    int damage;
} BulletInfo;

typedef struct {
    BulletInfo info;
    SDL_bool onStage;
    int x;
    int y;
    int xv;
    int xy;
} Bullet;

typedef struct {
    BulletInfo bullet;
    SDL_Surface* surface;
} GunInfo;

typedef struct {
    GunInfo info;
    int x;
    int y;
} Gun;

typedef struct {
    int hp;
    SDL_Surface* surface;
} BodyInfo;

/*プレイヤー(初期)情報の構造体 */
typedef struct {
    SDL_Rect shape[256];
    SDL_Surface* surface;
    int money;
    int level;
    int hp;
    int speed;
    int sheild;
    GunInfo gun;
} PlayerInfo;

typedef struct {
    PlayerInfo info;
    int hp;
    int x;
    int y;
    int vx;
    int vy;
    int sheild;
    GunInfo gun;
} Player;

/* 敵(初期)情報の構造体 */
typedef struct {
    SDL_bool shape[256][128];
    int hp;
    int speed;
    int sheild;
    int gunPower;
    GunInfo gun;
} EnemyInfo;

typedef struct {
    EnemyInfo info;
    int hp;
    int x;
    int y;
    int vx;
    int vy;
    int sheild;
    GunInfo gun;
} Enemy;

typedef struct {
    Player player;
    Enemy enemies[ENEMY_NUM];
    Bullet bullets[BULLET_NUM];
    GameStatus phase;
} Stage;

typedef struct {
    SDL_bool left;
    SDL_bool right;
    SDL_bool fire;
    SDL_bool skill;
    SDL_bool escape;
    SDL_bool mouseL;
    int mouseX;
    int mouseY;
} InputInfo;

extern GameMode gameMode;
extern BulletInfo bulletInfos[SAVE_TYPE_NUM];
extern PlayerInfo playerInfo;
extern SDL_bool isCreatedPlayer;

extern SDL_Renderer* render;

extern void collisionPlayer(Bullet* ci, Player* cj);
extern void collisionEnemy(Bullet* ci, Enemy* cj);

/* window.c */
extern int InitWindow(void);
extern void DestroyWindow(void);
extern void RenderWindow(void);

extern SDL_Window* window;
extern InputInfo inputInfo;
