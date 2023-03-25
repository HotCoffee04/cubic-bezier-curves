#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


#include "common.h"
#include "game.h"

void resizedWindow();

SDL_Window *win;
SDL_Rect texturePos;

float winScale;

int main(int argc, char* argv[]){

	SDL_Init(SDL_INIT_VIDEO);

	win  = SDL_CreateWindow("Bezier Curves",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREENWIDTH,SCREENHEIGHT,SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	//cycle through all the renderers and find OpenGL
	SDL_Renderer* rend = 0;
	for( int i = 0; i < SDL_GetNumRenderDrivers(); ++i )
	{
		SDL_RendererInfo rendererInfo;
		SDL_GetRenderDriverInfo( i, &rendererInfo );
		if(strcmp(rendererInfo.name,"opengl") != 0)
		{
			continue;
		}

		rend = SDL_CreateRenderer( win, i, 0 );
		break;
	}

	SDL_Texture *renderTexture;
	renderTexture = SDL_CreateTexture(rend,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,SCREENWIDTH,SCREENHEIGHT);

	texturePos.h = SCREENHEIGHT;
	texturePos.w = SCREENWIDTH;
	texturePos.x = 0;
	texturePos.y = 0;

	texPos = &texturePos;

	resizedWindow();

	r_init(rend);

	int close = 0;
	while (!close)
	{
		SDL_SetRenderDrawColor(rend,0,0,0,255);
		SDL_RenderClear(rend);

		g_getInput();

		switch(g_logic()){
			
			case 0:
				close = 1;
				break;
		}
		resizedWindow(); //every frame update render texture coordinates

		r_renderToTexture(renderTexture);

		SDL_RenderCopy(rend,renderTexture,0,&texturePos);

		SDL_RenderPresent(rend);

		//60 fps
		SDL_Delay(1000/60);

	}

	SDL_DestroyTexture(renderTexture);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void resizedWindow(){

	//adjust render texture scale and position to 
	//properly fit in the window.
	int nx,ny;
	SDL_GetWindowSize(win,&nx,&ny);
	int sx,sy;

	float ratio = (float)nx / ny;

	texturePos.x = texturePos.y = 0;

	if(ratio > (SCREENWIDTH/(float)SCREENHEIGHT)){
		//X is bigger than Y
		winScale = (float)ny/SCREENHEIGHT;

		sx = (SCREENHEIGHT*(SCREENWIDTH/(float)SCREENHEIGHT)) * winScale;
		sy = SCREENHEIGHT * winScale;

		texturePos.x = (float)nx / 2 - ( (float)sx/2);
	}else{
		//Y is bigger than X
		winScale = (float)nx / (float)SCREENWIDTH;

		sx = SCREENWIDTH * winScale;
		sy = (SCREENWIDTH / (SCREENWIDTH/(float)SCREENHEIGHT)) * winScale;

		texturePos.y = (float)ny / 2 - ( (float)sy/2);
	}

	texturePos.h = sy;
	texturePos.w = sx;
}