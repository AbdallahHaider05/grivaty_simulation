#ifndef BODY_H
#define BODY_H
#pragma once
#include <vector>
#include "point.h"
#include <iostream>
using namespace std;
class Body {
public:
	float mass;
	Point acc =Point(0,0);
	Point speed= Point(0,0);
	bool shown=true;
	bool forceCircles=false;
	Point postion;
	Body(double mass, Point postion);
	float redius;
	float color[4] = { 0.0,0.0,0.0,0.0 };
	void applyGF(Body body,double duration);
	unsigned int VBO, VAO;
	vector<float> path;
	void updateRedius();


	void updatePhysics(double duration);
};
#endif