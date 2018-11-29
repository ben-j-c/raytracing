#ifndef __RAYT_MAIN
#define __RAYT_MAIN
#include "main.hpp"
#include "RTGeo.hpp"
#include <math.h>



typedef struct{
	char name[64];
	float x, y, z;
	float ir, ig, ib;
} Light;

//Camera parameters
typedef struct{
	float near, left, right, bottom, top;
	float backr, backg, backb;
	int resx, resy;
} Camera;

//Light parameters
typedef struct{
	float ar, ag, ab;
	Light light[10];
	int numLight = 0;
} Lighting;

//Scene parameters
typedef struct{
	RTSphere sphere[15];
	int numSphere = 0;
} Scene;


inline int validatePercent(float val){
	return (val >= 0)&&(val <= 1);
}

inline int validateColour(float r, float g, float b){
	return validatePercent(r) && validatePercent(g) && validatePercent(b);
}

inline int validateSphere(RTSphere* s){
	return
	validatePercent(s->r)&&
	validatePercent(s->g)&&
	validatePercent(s->b)&&
	validatePercent(s->ka)&&
	validatePercent(s->kd)&&
	validatePercent(s->ks)&&
	validatePercent(s->kr);
}

inline int validateLight(Light* l){
	return
	validatePercent(l->ir)&&
	validatePercent(l->ig)&&
	validatePercent(l->ib);
}

/*
	Converts a float within [0,1] and converts it to an unsigned char within [0,255]
*/
inline unsigned char colourConversion(float c){
	return (unsigned char) floorf(fmaxf(fminf(c, 1.0), 0.0)*255);
}

#endif