#include "game.h"
#include "resource.h"

SDL_Renderer *rend;
int close;
int dragging = 0;
Vec *dragVec = 0;

Vec cross;
int crossing = 0;

int drawInfo = 1;

void r_drawCurves();
void scaleCoords(int* x,int* y);
int insideOf(Vec p,Vec r1,Vec r2);

SDL_Texture *r_loadTexture(char *name);
void r_loadTextures();

int cvsNum = 0;
bCurve cvs[MAX_CURVES];

void r_init(SDL_Renderer *r){
	close = 1;
    rend = r;
	gameState = Menu;

	SDL_SetRenderDrawBlendMode(rend,SDL_BLENDMODE_BLEND);

	r_loadTextures();

}

void r_renderToTexture(SDL_Texture *renderTexture){

	SDL_SetRenderDrawColor(rend,8,8,8,255);
	SDL_SetRenderTarget(rend,renderTexture);
	SDL_RenderClear(rend);


	SDL_Rect pos;
	pos = (SDL_Rect){0,0,SCREENWIDTH,SCREENHEIGHT};

	switch(gameState){

		case Menu:
			SDL_RenderCopy(rend,textures[T_Title],0,&pos);

		break;
		case Playing:
			r_drawCurves();
		break;
	}

	SDL_SetRenderTarget(rend,0);
}

void r_drawCurves(){

	SDL_SetRenderDrawColor(rend,255,255,255,255);
	
	//draw curves:
	for(int i = 0; i < MAX_CURVES; i++){

		if(cvs[i].p1.x == 0 && cvs[i].p1.y == 0) //empty curve
			continue;

		if((cvs[i].p1.x != cvs[i].h1.x && cvs[i].p1.y != cvs[i].h1.y) || (cvs[i].p2.x != cvs[i].h2.x && cvs[i].p2.y != cvs[i].h2.y))
			drawBCurve(cvs[i],rend);
		else{
			SDL_RenderDrawLine(rend,cvs[i].p1.x,cvs[i].p1.y,cvs[i].p2.x,cvs[i].p2.y);
		}

		if(!drawInfo)
			continue;

		//find out if i'm editing this curve

		if(dragging && dragVec != 0)
			if( ( ( (dragVec - (Vec*)(cvs)) * sizeof(Vec) / sizeof(bCurve) ) != i) )
				continue;

		//draw handles
		SDL_SetRenderDrawColor(rend,0,0,255,255);
		Vec *pvec = (Vec*)(&cvs[i]);
		for(int j = 0; j < sizeof(bCurve)/sizeof(Vec);pvec++, j++){

			SDL_Rect rect;
			rect.x = pvec->x-HANDLE_SIZE/2;
			rect.y = pvec->y-HANDLE_SIZE/2;
			rect.h = HANDLE_SIZE;
			rect.w = HANDLE_SIZE;

			SDL_RenderDrawRect(rend,&rect);

		}


		if(!drawInfo)
			continue;

		SDL_SetRenderDrawColor(rend,0,0,255,127);
		//draw lines
		SDL_RenderDrawLine(rend,cvs[i].p1.x,cvs[i].p1.y,cvs[i].h1.x,cvs[i].h1.y);
		SDL_RenderDrawLine(rend,cvs[i].h1.x,cvs[i].h1.y,cvs[i].h2.x,cvs[i].h2.y);
		SDL_RenderDrawLine(rend,cvs[i].h2.x,cvs[i].h2.y,cvs[i].p2.x,cvs[i].p2.y);
		SDL_RenderDrawLine(rend,cvs[i].p2.x,cvs[i].p2.y,cvs[i].p1.x,cvs[i].p1.y);
		SDL_SetRenderDrawColor(rend,255,255,255,255);
	}
	
	
	//draw cross
	if(crossing){
	SDL_SetRenderDrawColor(rend,255,0,0,255);
	SDL_RenderDrawLine(rend,cross.x - CROSS_LENGTH/2,cross.y - CROSS_LENGTH / 2,cross.x + CROSS_LENGTH/2,cross.y + CROSS_LENGTH / 2);
	SDL_RenderDrawLine(rend,cross.x + CROSS_LENGTH/2,cross.y - CROSS_LENGTH / 2,cross.x - CROSS_LENGTH/2,cross.y + CROSS_LENGTH / 2);
	}

	SDL_SetRenderDrawColor(rend,8,8,8,255);
}

void g_getInput(){

	int x,y;
	SDL_GetMouseState(&x,&y);
	scaleCoords(&x,&y);

	if(dragging && dragVec != 0){
		dragVec->x = x;
		dragVec->y = y;
	}


	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				close = 0;
			break;
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_RESIZED)
					close = 2;
			break;
			case SDL_MOUSEBUTTONDOWN:

				switch(gameState){

					case Menu:
						if(event.button.button == SDL_BUTTON_RIGHT){
							gameState = Playing;
							cross.x = x;
							cross.y = y;
							crossing =  1;
						}

					break;
					case Playing:

						if(event.button.button == SDL_BUTTON_LEFT){

							dragging = 1;

							//check if clicked on handle
							for(int i = 0; i < MAX_CURVES; i++){

								if(cvs[i].p1.x == 0 && cvs[i].p1.y == 0) //empty curve
									continue;

								Vec *pvec = (Vec*)(&cvs[i]);
								for(int j = 0; j < sizeof(bCurve)/sizeof(Vec); pvec++,j++){

									if(insideOf((Vec){x,y},(Vec){pvec->x-HANDLE_SIZE/2,pvec->y-HANDLE_SIZE/2},(Vec){pvec->x+HANDLE_SIZE/2,pvec->y+HANDLE_SIZE/2}))
										dragVec = pvec;

								}

							}

						}else{

							if(!crossing){
								cross.x = x;
								cross.y = y;
								crossing =  1;
							}else{

								if(insideOf((Vec){x,y},(Vec){cross.x - CROSS_LENGTH,cross.y - CROSS_LENGTH},(Vec){cross.x + CROSS_LENGTH,cross.y + CROSS_LENGTH} ))
									break;

								crossing = 0;
								//create new curve
								for(int i = 0; i < MAX_CURVES; i++){

									if(cvs[i].p1.x == 0 && cvs[i].p1.y == 0){ //empty curve

										cvs[i].p1 = cross;
										cvs[i].p2.x = x;
										cvs[i].p2.y = y;
										cvs[i].h1 = cvs[i].p1;
										cvs[i].h2 = cvs[i].p2;
										break;

									}else
										continue;
									
								}
							}
						}

					break;
				}
			break;
			case SDL_MOUSEBUTTONUP:

				switch(gameState){

					case Playing:

						if(event.button.button == SDL_BUTTON_LEFT){

							dragging = 0;
							dragVec = 0;

						}else{

						}
						
					break;

				}
			break;
			case SDL_KEYDOWN:
				if(event.key.keysym.scancode == SDL_SCANCODE_H){
					drawInfo = !drawInfo;
				}

			break;
			default:
				close = 1;
			break;
		}
	}
	
}

int g_logic(){

	return close;

}

int insideOf(Vec p,Vec r1,Vec r2){

	if(p.x > r1.x && p.x < r2.x && p.y > r1.y && p.y < r2.y)
		return 1;
	else
		return 0;

}

void scaleCoords(int* x,int* y){


	//scales coordinates
	int nx,ny;
	nx = texPos->w+texPos->x*2;
	ny = texPos->h+texPos->y*2;
	float winScale;
	float ratio = (float)nx / ny;

	if(ratio > (SCREENWIDTH/(float)SCREENHEIGHT))
	winScale = (float)ny/SCREENHEIGHT;
	else
	winScale = (float)nx / (float)SCREENWIDTH;

	*x -= texPos->x;
	*y -= texPos->y;
	*x = *x / winScale;
	*y = *y / winScale;


}

SDL_Texture *r_loadTexture(char *name){
	//actually loads it from constant memory
	SDL_Surface *surf;
	SDL_Texture *tex;
	SDL_RWops *ops;
	ops = SDL_RWFromConstMem(title,1048714);
	
	surf = SDL_LoadBMP_RW(ops,0);
	tex = SDL_CreateTextureFromSurface(rend,surf);

	SDL_FreeSurface(surf);
	free(ops);
	return tex;
}

void r_loadTextures(){

	textures[T_Title] = r_loadTexture("title.bmp");
}