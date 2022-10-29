#include <SDL2/SDL.h>
#include <stdio.h>
#include "common.h"
#include "bcurve.h"

typedef enum GameState{

    Menu,
    Playing,

}GameState;

typedef enum TextureIndex{

    T_Title,

}TextureIndex;

SDL_Rect *texPos;
GameState gameState; //game state

SDL_Texture *textures[512];

void r_init(SDL_Renderer *rend);
void g_getInput();
int g_logic();
void r_renderToTexture(SDL_Texture *renderTexture);
