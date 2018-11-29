#include <iostream>
#include <stdlib.h>
#include <fstream>

#include <stdio.h>
#include <string.h>

#include "main.hpp"
#include "RTGeo.hpp"
#include "RTCore.hpp"

#define BUFFER_SIZE ((int) 256)

Camera camera;
Scene scene;
Lighting lighting;

//Save location
char outputFile[256];

/*
Takes a single line as a command.
*/
static int parseLine(char *line){
	char command[64] = {0};
	sscanf(line,"%s", command);

	if(!strcmp(command, "NEAR")){
		sscanf(line,"%s %f", command, &camera.near);
	}
	else if(!strcmp(command, "LEFT")){
		sscanf(line,"%s %f", command, &camera.left);
	}
	else if(!strcmp(command, "RIGHT")){
		sscanf(line,"%s %f", command, &camera.right);
	}
	else if(!strcmp(command, "BOTTOM")){
		sscanf(line,"%s %f", command, &camera.bottom);
	}
	else if(!strcmp(command, "TOP")) {
		sscanf(line,"%s %f", command, &camera.top);
	}
	else if(!strcmp(command, "RES")) {
		sscanf(line,"%s %d %d", command, &camera.resx, &camera.resy);
	}
	else if(!strcmp(command, "SPHERE") && scene.numSphere < 16) {
		//          com nm x  y  z  sx sy sz r  g  b  ka kd ks kr n
		sscanf(line,"%s %s %f %f %f %f %f %f %f %f %f %f %f %f %f %d", command, 
			scene.sphere[scene.numSphere].name,
			&(scene.sphere[scene.numSphere].x),
			&(scene.sphere[scene.numSphere].y),
			&(scene.sphere[scene.numSphere].z),
			&(scene.sphere[scene.numSphere].sx),
			&(scene.sphere[scene.numSphere].sy),
			&(scene.sphere[scene.numSphere].sz),
			&(scene.sphere[scene.numSphere].r),
			&(scene.sphere[scene.numSphere].g),
			&(scene.sphere[scene.numSphere].b),
			&(scene.sphere[scene.numSphere].ka),
			&(scene.sphere[scene.numSphere].kd),
			&(scene.sphere[scene.numSphere].ks),
			&(scene.sphere[scene.numSphere].kr),
			&(scene.sphere[scene.numSphere].n)
			);

		if(!validateSphere(&scene.sphere[scene.numSphere])){
			std::cerr << "Invalid sphere #: " << scene.numSphere << std::endl;
			return 3;
		}

		scene.numSphere++;
	}
	else if(!strcmp(command, "LIGHT") && lighting.numLight < 11) {
		//           com nm x  y  z  r  g  b
		sscanf(line, "%s %s %f %f %f %f %f %f", command,
			lighting.light[lighting.numLight].name,
			&(lighting.light[lighting.numLight].x),
			&(lighting.light[lighting.numLight].y),
			&(lighting.light[lighting.numLight].z),
			&(lighting.light[lighting.numLight].ir),
			&(lighting.light[lighting.numLight].ig),
			&(lighting.light[lighting.numLight].ib)
		);

		if(!validateLight(&lighting.light[lighting.numLight])){
			std::cerr << "Invalid light #: " << lighting.numLight << std::endl;
			return 4;
		}

		lighting.numLight++;
	}
	else if(!strcmp(command, "BACK")) {
		sscanf(line,"%s %f %f %f", command, &camera.backr, &camera.backg, &camera.backb);

		if(!validateColour(camera.backr, camera.backg, camera.backb)){
			std::cerr << "Invalid BACK command" << std::endl;
			return 5;
		}
	}
	else if(!strcmp(command, "AMBIENT")) {
		sscanf(line,"%s %f %f %f", command, &lighting.ar, &lighting.ag, &lighting.ab);

		if(!validateColour(lighting.ar, lighting.ag, lighting.ab)){
			std::cerr << "Invalid AMBIENT command" << std::endl;
			return 5;
		}
	}
	else if(!strcmp(command, "OUTPUT")) {
		sscanf(line,"%s %s", command, &outputFile);
	}
	else if(strlen(command) != 0){
		std::cerr << "Invalid command" << std::endl;
	}

	return 0;
}

int main(int argc, char *argv[]){
	std::ifstream scriptFile;

	if(argc > 1)
		scriptFile.open(argv[1]);
	else {
		std::cerr << "Not enough input arguments." << std::endl;
		return 1;	
	}

	if(!scriptFile.is_open()) {
		std::cerr << "Unable to open file." << std::endl;
		return 2;
	}

	//printf("%d\n", scriptFile.eof());
	while(!scriptFile.eof()){
		char line[256] = {0};
		scriptFile.getline(line, BUFFER_SIZE);

		{
			int error = parseLine(line);
			if(error)
				return error;
		}
	}
	scriptFile.close();

	printf("TOP: %f\n", camera.top);
	printf("BOTTOM: %f\n", camera.bottom);
	printf("LEFT: %f\n", camera.left);
	printf("RIGHT: %f\n", camera.right);
	printf("NEAR: %f\n", camera.near);
	printf("RES: %d %d\n", camera.resx, camera.resy);
	printf("BACK: %f %f %f\n", camera.backr, camera.backg, camera.backb);
	printf("AMBIENT: %f %f %f\n", lighting.ar, lighting.ag, lighting.ab);
	printf("OUTPUT: %s\n", outputFile);

	for(int i = 0 ; i < scene.numSphere ; i++){
		RTSphere sphere = scene.sphere[i];
		printf("SPHERE: %s\n", sphere.name);
		printf("\tPos: %f %f %f\n", sphere.x, sphere.y, sphere.z);
		printf("\tScale: %f %f %f\n", sphere.sx, sphere.sy, sphere.sz);
		printf("\tColour: %f %f %f\n", sphere.r, sphere.g, sphere.b);
		printf("\tKx: %f %f %f %f\n", sphere.ka, sphere.kd, sphere.ks, sphere.kr);
		printf("\tN: %d\n", sphere.n);
	}

	for(int i = 0 ; i < lighting.numLight ; i++){
		Light light = lighting.light[i];
		printf("LIGHT: %s\n", light.name);
		printf("\tPos: %f %f %f\n", light.x, light.y, light.z);
		printf("\tColour: %f %f %f\n", light.ir, light.ig, light.ib);
	}
	
	RTCore core(&camera, &scene, &lighting);
	core.generateImage();
	core.writeResultToFile(outputFile);
}