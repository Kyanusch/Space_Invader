#pragma once
#include "raylib.h"
#include <vector>
#include "virtualCamera.h"
class Gridboarder{
private:
	//Vector3 plane;
	std::vector<std::vector<Vector3>> matrix;
	int lx;
	int ly;
	//float spacing;
	float distortion;
	virtualCamera::sVector3 origin;
	virtualCamera::sVector3 dx;
	virtualCamera::sVector3 dy;
	virtualCamera::sVector3 pDistortion;
public:
	Gridboarder(Vector3 Origin, Vector3 dX, Vector3 dY, int lx, int ly, float distortion);
	Gridboarder(Vector3 Origin, Vector3 dX, Vector3 dY, int lx, int ly);
	void draw() const;
};

