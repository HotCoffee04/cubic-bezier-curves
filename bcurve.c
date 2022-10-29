#include "bcurve.h"


double lerp(double a, double b, double f);
Vec vlerp(Vec a, Vec b, double t);
double vdistance(Vec a,Vec b);

void drawBCurve(bCurve c,SDL_Renderer *rend){


	int amount;
	amount = vdistance(c.p1,c.h1) + vdistance(c.h1,c.h2) + vdistance(c.h2,c.p2);
	amount *= 2;


	double q;
	Vec l1,l2,l3;
	
	for(int i = 0; i < amount; i++){

		q = (double)i/amount;


		l1 = vlerp(c.p1,c.h1,q);
		l2 = vlerp(c.h1,c.h2,q);
		l3 = vlerp(c.h2,c.p2,q);

		l1 = vlerp(l1,l2,q);
		l2 = vlerp(l2,l3,q);

		l1 = vlerp(l1,l2,q);

		SDL_RenderDrawPoint(rend,round(l1.x),round(l1.y));

	}

}

Vec vlerp(Vec a, Vec b, double t){

	Vec r;
	r.x = lerp(a.x,b.x,t);
	r.y = lerp(a.y,b.y,t);
	return r;
}

double lerp(double a, double b, double f) 
{
	return (a * (1.0 - f)) + (b * f);
}

double vdistance(Vec a,Vec b){

	return sqrt(pow((a.y-b.y),2)+pow(b.x-a.x,2));

}