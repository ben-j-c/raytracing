#include "RTCore.hpp"
//Only for this document
#define MAX_DEPTH ((float) 20000.0)
#define MAX_REFLECTIONS ((int) 3)

RTCore::RTCore(Camera* camera, Scene* scene, Lighting* lighting) {
	this->camera = camera;
	this->scene = scene;
	this->lighting = lighting;

	pixels = NULL;
}

RTCore::~RTCore() {
	if (pixels)
		free(pixels);
}


/*
	Takes pixel arguments with origin being the top left
	of the screen.  Returns an RGB vector.
*/
RTVec RTCore::traceRayFromScreen(int px, int py) {
	float w = (camera->right - camera->left)*0.5;
	float h = (camera->top - camera->bottom)*0.5;
	RTVec s(-w + w * 2 * px / camera->resx, h - h * 2 * py / camera->resx, -camera->near);
	RTVec c = s.unit();

#ifdef  DEBUG
	if (px == 300 && py == 300) {
		printf("Middle\n");
	}
#endif //  DEBUG

	RTVec colour = traceRay(RTRay(s, c), MAX_REFLECTIONS);

	return colour.clampToOne();
}


/*
	Checks if the intersection is within the range (0, 1).
*/
static int intersectionBeforeLight(RTQuadraticSolution intc) {
	if (intc.numSolutions) {
		if (intc.t1 > 0 && intc.t1 < 1)
			return 1;
		if (intc.t2 > 0 && intc.t2 < 1)
			return 1;
	}
	return 0;
}

/*
	Calculate the diffuse and specular component of the visible colour.
*/
RTVec RTCore::diffuseAndSpecular(const RTVec& pos, const RTVec& normal, const RTVec& reflected, int sphere) {
	RTVec colour(0, 0, 0);
	RTVec sphereColour(scene->sphere[sphere].r, scene->sphere[sphere].g, scene->sphere[sphere].b);

	//For every light check if the shadow ray can see the light
	for (int i = 0; i < lighting->numLight; i++) {
		int inShadow = 0;
		RTVec L = (RTVec(lighting->light[i].x, lighting->light[i].y, lighting->light[i].z) - pos);
		RTRay ray(pos, L);
		//Check if the shadow ray intersects ANY sphere in the scene
		for (int j = 0; !inShadow && (j < scene->numSphere); j++) {
			RTQuadraticSolution intersection = ray.intersectSphere(&(scene->sphere[j]));
			if(intersectionBeforeLight(intersection))
				inShadow = 1;	
		}
		if (!inShadow) { //If there is a clear line between the point on the original sphere to the light
			float cosineDiffuse = fmaxf(L.unit()*normal, 0);
			float cosineSpecular = fmaxf(L.unit()*reflected, 0);
			if(cosineSpecular > 0)
				cosineSpecular = powf (cosineSpecular, scene->sphere[sphere].n);

			float kd = scene->sphere[sphere].kd;
			float ks = scene->sphere[sphere].ks;
			RTVec lightIrradiance(lighting->light[i].ir, lighting->light[i].ig, lighting->light[i].ib);
			//Add diffuse and specular components from this light
			colour += lightIrradiance.elemMult(sphereColour)*(kd*cosineDiffuse);
			colour += lightIrradiance*(ks*cosineSpecular);
		}
	}

	return colour;
}

/*
	Recursive method for computing colour components of a ray.
*/
RTVec RTCore::traceRay(const RTRay& ray, int depth) {
	if (depth == 0) {
		RTVec returner(0,0,0);
		return returner;
	}

	//Find the closest intersection, default to -1 if no intersection
	float minT = MAX_DEPTH; int minIdx = -1;
	RTQuadraticSolution intersection(0, 0, 0);
	for (int i = 0 ; i < scene->numSphere; i++) {
		intersection = ((RTRay&)ray).intersectSphere(&(scene->sphere[i]));

		if (intersection.numSolutions) {
			if (intersection.t1 > ERROR_MARGIN && minT > intersection.t1) {
				minT = intersection.t1;
				minIdx = i;
			}
			else if(intersection.t2 > ERROR_MARGIN && minT > intersection.t2) {
				minT = intersection.t2;
				minIdx = i;
			}
		}
	}


	if (minIdx == -1) {
		RTVec returner(0,0,0);
		if(depth == MAX_REFLECTIONS)//If this ray is from the screen use background
			returner = RTVec(camera->backr, camera->backg, camera->backb);
		return returner;
	}
	else {
		RTSphere& minSphere = scene->sphere[minIdx];

		RTVec s = ((RTRay&)ray).atT(minT);
		RTVec normal = minSphere.normal(s);
		if (intersection.t1 < 0 && intersection.t2 > 0) //If the intersection is inside a sphere
			normal = normal.flip();

		RTVec cReflect = ((RTRay&)ray).c.flip().reflect(normal).unit();
		RTVec sReflect = ((RTRay&)ray).atT(minT) + (cReflect * ERROR_MARGIN);
		RTRay reflected(sReflect, cReflect);

		//Sum the ambient, relfective, diffuse, and specular
		RTVec colourReflected = traceRay(reflected, depth - 1); //Recursive/reflection step
		RTVec colourAmbient = RTVec(lighting->ar, lighting->ag, lighting->ab).elemMult(RTVec(minSphere.r, minSphere.g, minSphere.b));
		RTVec colourDiffAndSpec = diffuseAndSpecular(s + (normal*ERROR_MARGIN), normal, cReflect, minIdx); //Specular and diffuse terms
		RTVec returner = (colourAmbient*minSphere.ka) + (colourDiffAndSpec) + (colourReflected * minSphere.kr); //Summed components
		return returner;
	}
}

void RTCore::generateImage() {
	if (pixels)
		free(pixels);

	int resx = camera->resx;
	int resy = camera->resy;

	pixels = (unsigned char*) calloc(resx*resy*3, sizeof(unsigned char));
	for (int row = 0; row < resy; row++) {
		for (int col = 0; col < resx; col++) {
			RTVec colour = traceRayFromScreen(col, row);
			unsigned char* cPx = pixels + (col + resx * row)*3;
			*cPx = colourConversion(colour.x);
			*(cPx + 1) = colourConversion(colour.y);
			*(cPx + 2) = colourConversion(colour.z);
		}
	}
}

void RTCore::writeResultToFile(const char* fname) {
	if(pixels)
		save_imageP6(camera->resx, camera->resy, fname, pixels);
}


// Output in P6 format, a binary file containing:
// P6
// ncolumns nrows
// Max colour value
// colours in binary format thus unreadable
void RTCore::save_imageP6(int Width, int Height, const char* fname, unsigned char* pixels) {
	FILE *fp;
	const int maxVal = 255;
	printf("Saving image %s: %d x %d\n", fname, Width, Height);
	fp = fopen(fname, "wb");
	if (!fp) {
		printf("Unable to open file '%s'\n", fname);
		return;
	}
	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", Width, Height);
	fprintf(fp, "%d\n", maxVal);

	for (int j = 0; j < Height; j++) {
		fwrite(&pixels[j*Width * 3], 3, Width, fp);
	}

	fclose(fp);
}

// Output in P3 format, a text file containing:
// P3
// ncolumns nrows
// Max colour value (for us, and usually 255)
// r1 g1 b1 r2 g2 b2 .....
void RTCore::save_imageP3(int Width, int Height, const char* fname, unsigned char* pixels) {
	FILE *fp;
	const int maxVal = 255;

	printf("Saving image %s: %d x %d\n", fname, Width, Height);
	fp = fopen(fname, "w");
	if (!fp) {
		printf("Unable to open file '%s'\n", fname);
		return;
	}
	fprintf(fp, "P3\n");
	fprintf(fp, "%d %d\n", Width, Height);
	fprintf(fp, "%d\n", maxVal);

	int k = 0;
	for (int j = 0; j < Height; j++) {

		for (int i = 0; i < Width; i++)
		{
			fprintf(fp, " %d %d %d", pixels[k], pixels[k + 1], pixels[k + 2]);
			k = k + 3;
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}
#undef MAX_DEPTH
#undef MAX_REFLECTIONS
#undef ERROR_MARGIN