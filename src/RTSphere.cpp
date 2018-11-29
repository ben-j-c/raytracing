#include "RTGeo.hpp"

/*
	Returns the unit direction pointing from the center of the sphere, and through
	the point given; at the point where the direction passes though the surface of
	the sphere, it is normal to the surface.  I.e., for any point on the sphere,
	this returns the normal.  If the point given is at the center of the sphere,
	an error is thrown.
*/
RTVec RTSphere::normal(RTVec& pos) {
	float px = (pos.x - x) / (sx*sx);
	float py = (pos.y - y) / (sy*sy);
	float pz = (pos.z - z) / (sz*sz);
	RTVec returner(px, py, pz);
	return returner.unit();;
}
