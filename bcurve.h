#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "common.h"

typedef struct bCurve{

	Vec p1,p2;
	Vec h1,h2;

}bCurve;

void drawBCurve(bCurve curve,SDL_Renderer *rend);
