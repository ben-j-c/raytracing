#include "RTGeo.hpp"

RTRay::RTRay(const RTVec& s, const RTVec& c) {
	this->s = s;
	this->c = c;
}

RTRay::RTRay(const RTRay & copy) {
	this->s = copy.s;
	this->c = copy.c;
}

RTQuadraticSolution::RTQuadraticSolution(int numSolutions, float t1, float t2){
	this->numSolutions = numSolutions;
	this->t1 = t1;
	this->t2 = t2;
}
RTQuadraticSolution::RTQuadraticSolution(int numSolutions, int rayInside, float t1, float t2) {
	this->numSolutions = numSolutions;
	this->t1 = t1;
	this->t2 = t2;
	this->rayInside = rayInside;
}

RTQuadraticSolution RTRay::intersectSphere(RTSphere* sph) {
	RTVec sphPos;
	sphPos.x = sph->x;
	sphPos.y = sph->y;
	sphPos.z = sph->z;
	RTVec spr = (s - sphPos).scale(1/(sph->sx), 1/ (sph->sy), 1/ (sph->sz));
	RTVec cpr = c.scale(1/sph->sx, 1/sph->sy, 1/sph->sz);

	float A = cpr.magSquared();
	float B = (spr*cpr);
	float C = spr.magSquared() - 1;

	float discriminant = B*B - A*C;

	if (discriminant < 0) {
		RTQuadraticSolution returner(0,0,0);
		return returner;
	}
	else if (discriminant == 0) {
		RTQuadraticSolution returner(1, -B / A, 0);
		return returner;
	}
	else {
		RTQuadraticSolution returner(
			2,
			-B / A - sqrt(discriminant) / A,
			-B / A + sqrt(discriminant) / A);
		return returner;
	}
	
}

RTRay& RTRay::operator=(const RTRay& copy) {
	this->s = copy.s;
	this->c = copy.c;
	return *this;
}

RTVec RTRay::atT(float t) {
	RTVec returner = s + (c * t);
	return returner;
}