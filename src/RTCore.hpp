#ifndef __RAYT_CORE
#define __RAYT_CORE

#include "main.hpp"
#include "RTGeo.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class RTCore{
public:
	RTCore(Camera*, Scene*, Lighting*);
	~RTCore();
	void generateImage();
	void writeResultToFile(const char*);
private:
	Camera* camera;
	Scene* scene;
	Lighting* lighting;
	unsigned char* pixels;
	RTVec traceRayFromScreen(int, int); //returns a colour
	RTVec traceRay(const RTRay&, int);
	RTVec diffuseAndSpecular(const RTVec& ,const RTVec&, const RTVec&, int);
	void save_imageP3(int, int, const char*, unsigned char*);
	void save_imageP6(int, int, const char*, unsigned char*);
};

#endif