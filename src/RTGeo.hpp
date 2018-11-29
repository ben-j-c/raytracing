#ifndef __RAYT_VEC
#define __RAYT_VEC
#include "math.h"
#define ERROR_MARGIN ((float) 0.001)

class RTVec{
public:
	float x; float y; float z;
	int index;
	RTVec();
	RTVec(float, float, float);
	RTVec(const RTVec&);

	~RTVec();
	RTVec operator+(const RTVec&);
	RTVec& operator+=(const RTVec&);
	RTVec operator-(const RTVec&);
	RTVec& operator-=(const RTVec&);
	float operator*(const RTVec&); //Dot product
	RTVec operator*(float);
	float operator[](int index);
	RTVec& operator=(const RTVec&);
	RTVec& set(const RTVec&);
	RTVec cross(const RTVec&);
	RTVec reflect(const RTVec&);
	RTVec& normalize(); //replace values
	RTVec norm(); //new vec
	RTVec unit();
	RTVec scale(float, float, float);
	RTVec scale(const RTVec&);
	RTVec flip();
	RTVec elemMult(const RTVec&);
	RTVec clampToOne();
	RTVec clampToOneKeepRatio();
	float magnitude();
	float magSquared();
};

class RTSphere{
	public:
	char name[64];
	float x, y, z;
	float sx, sy, sz;
	float r, g, b;
	float ka;
	float kd;
	float ks;
	float kr;
	int n;
	RTVec normal(RTVec&);
};

class RTQuadraticSolution{
public:
	RTQuadraticSolution(int, int, float, float);
	RTQuadraticSolution(int, float, float);
	int numSolutions;
	int rayInside;
	float t1;
	float t2;
};

class RTRay{
public:
	RTVec s;
	RTVec c;
	RTRay(const RTVec&, const RTVec&);
	RTRay(const RTRay&);
	RTQuadraticSolution intersectSphere(RTSphere*);
	RTRay& operator=(const RTRay&);
	RTVec atT(float);
};

#endif