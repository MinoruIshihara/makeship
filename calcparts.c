#include "blackship.h"

int* runLength(SDL_bool** bitData);
SDL_bool** revRunLength(int* runL);
void saveShip(int shipID, int hp, SDL_bool** shipBin);

int* runLength(SDL_bool** bitData)
{
    int* runL        = malloc(sizeof(char) * 64 * 16);
    SDL_bool current = SDL_FALSE;
    int incert       = 0;
    for (int y = 0; y < SHIP_HEIGHT; y++) {
        for (int x = 0; x < SHIP_WIDTH; x++) {
            if (current && bitData[y][x]) {
                runL[incert] = current;
            } else {
                current = !current;
                incert++;
            }
        }
    }
    return runL;
}

SDL_bool** revRunLength(int* runL)
{
    SDL_bool** bitData;
    bitData = malloc(sizeof(SDL_bool) * 256 * 128);
    return bitData;
}

void saveShip(int shipID, int hp, SDL_bool** shipBin)
{
}