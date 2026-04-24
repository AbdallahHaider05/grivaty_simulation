#ifndef WORLD_H
#define WORLD_H
#pragma once
#include <vector>
#include "point.h"
#include "body.h"

using namespace std;
class World {
public:
	vector<Body> bodies;
	float timefactor = 1;
	double FrameDuration;
	int width;
	Point scroll=Point(0.0,0.0);
	Point currentShift = Point(0, 0);
	bool isScrolling = false;
	Point lastPoisiton=Point(0,0);
	int BodyFollow = -1;
	float zoom = 1;
	int heigth;
	unsigned int VBOS[6];
	World(vector<Body> bodys, float tf,double fd);
	void updateBodies(float aspectRatio);
    vector<float> getBodiesPositions();
	vector<float> getBodiesColors();
	vector<float> getBodiesMasses();
	vector<float> getBodiesRadiuses();
	void sortBodies();
	void updateGpuVram();






};




#endif