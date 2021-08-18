#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

/* ウインドウサイズ */
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

#define SHIP_WIDTH 256
#define SHIP_HEIGHT 128

#define MAX_ENEMY_NUM 10
#define BULLET_NUM 64
#define GUN_TYPE_NUM 3
#define MAX_GUN_NUM 30
#define BODY_TYPE_NUM 6
#define MAX_BODY_NUM 32

#define SAVE_TYPE_NUM 16

#define MAIN_WINDOW_BG "mainWindow.png"

typedef enum {
    EXIT_GAME     = -1,
    MAIN_WINDOW   = 0,
    PARTS_WINDOW  = 1,
    STAGE_WINDOW  = 2,
    ACTION_WINDOW = 3
} GameMode;

typedef enum {
    PAUSE      = -1,
    BATTLE     = 0,
    GAME_CLEAR = 1,
    GAME_OVER  = 2
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
    SDL_Texture* texture;
    int damage;
} BulletInfo;

typedef struct {
    BulletInfo info;
    SDL_bool onStage;
    int x;
    int y;
    float vx;
    float vy;
    int t;
} Bullet;

typedef struct {
    BulletInfo bullet;
    SDL_Surface* surface;
    int cost;
    int duration;
} GunInfo;

typedef struct {
    GunInfo info;
    int x;
    int y;
    float t;
} Gun;

typedef struct {
    int hp;
    SDL_Surface* surface;
    int cost;
} BodyInfo;

/*プレイヤー(初期)情報の構造体 */
typedef struct {
    SDL_Rect shape[256];
    SDL_Surface* surface;
    SDL_Texture* texture;
    int initMoney;
    int money;
    int level;
    int hp;
    int gunNum;
    Gun gun[MAX_GUN_NUM];
    int xp;
} PlayerInfo;

typedef struct {
    int hp;
    int x;
    int y;
    int vx;
    int vy;
} Player;

/* 敵(初期)情報の構造体 */
typedef struct {
    SDL_Surface* surface;
    SDL_Texture* texture;
    int hp;
    int speed;
    GunInfo gun;
} EnemyInfo;

typedef struct {
    EnemyInfo info;
    int hp;
    int x;
    int y;
    int vx;
    int vy;
} Enemy;

typedef struct {
    SDL_Texture* texture[5];
} EffectInfo;

typedef struct {
    int x;
    int y;
    int t;
    EffectInfo info;
} Effect;

typedef struct {
    Player player;
    int enemyNum;
    Enemy enemies[MAX_ENEMY_NUM];
    int bulletNum;
    Bullet bullets[BULLET_NUM];
    GameStatus phase;
    int score;
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

extern SDL_Window* window;
extern InputInfo inputInfo;

extern TTF_Font* font;
extern TTF_Font* font30;
extern TTF_Font* font40;

PlayerInfo initPlayer();
void exitGame(ExitCode exitStatus);

extern int initMainWin();
extern int initPartsWin();
extern int initActionWin();

extern void drawMainWin();
extern void drawPartsWin();
extern void drawActionWin(int millis);

extern void destroyMainWin();
extern void destroyPartsWin();
extern void destroyActionWin();

extern int InitWindow(void);
extern void DestroyWindow(void);
extern void RenderWindow(void);

extern SDL_bool collisionPlayer(Bullet b, Player p);
extern SDL_bool collisionEnemy(Bullet b, Enemy e);
