#include "Gridboarder.h"
#include <iostream>
Gridboarder::Gridboarder(Vector3 Origin, Vector3 dX, Vector3 dY, int lx, int ly, float distortion) : lx(lx), ly(ly), distortion(distortion){
	origin = { Origin };
	dx = { dX };
	dy = { dY };
	virtualCamera::sVector3 temppoint = origin;
	pDistortion = virtualCamera::normalize(virtualCamera::cross(dx, dy)) * distortion;
	for (int x = 0; x < lx+1; x++) {
		std::vector<Vector3> vx;
		temppoint = origin + (dx * x);
		for (int y = 0; y < ly+1; y++) {
			temppoint = temppoint + (dy * y);
			auto tempP = temppoint + (pDistortion * (GetRandomValue(-100, 100)/ 100.0f));
			vx.push_back(tempP.vec);
		}
		matrix.push_back(vx);
	}


	/*for (int l = 0; l < lenght +1; l++) {
		std::vector<Vector3> vDeep;
		for (int d = 0; d < depth +1; d++) {
			virtualCamera::sVector3 tempP({ l * spacing, 0 ,d * spacing });
			tempP = tempP+ virtualCamera::sVector3({0.0f, (float)GetRandomValue(-100,100), 0.0f }) / spacing * 100.0f;
			vDeep.push_back(tempP.vec);
		}
		matrix.push_back(vDeep);
	}*/
}
Gridboarder::Gridboarder(Vector3 Origin, Vector3 dX, Vector3 dY, int lx, int ly) : lx(lx), ly(ly), distortion(0.0f){
	origin = { Origin };
	dx = { dX };
	dy = { dY };
	virtualCamera::sVector3 temppoint = origin;
	pDistortion = virtualCamera::normalize(virtualCamera::cross(dx, dy)) * distortion;
	for (int x = 0; x < lx + 1; x++) {
		std::vector<Vector3> vy;
		temppoint = origin + (dx * x);
		for (int y = 0; y < ly + 1; y++) {
			auto tempP = temppoint + (pDistortion * (GetRandomValue(-100, 100) / 100.0f));
			vy.push_back(tempP.vec);
			temppoint + dy;
		}
		matrix.push_back(vy);
	}
}
void Gridboarder::draw() const{
	for (int x = 0; x < lx + 1; x++) {
		for (int y = 0; y < ly; y++) {
			auto resultsP1 = virtualCamera::projectPoint(matrix[x][y]);
			auto resultsP2 = virtualCamera::projectPoint(matrix[x][y+1]);
			if (resultsP1.inView && resultsP2.inView) {
				DrawLine(resultsP1.position2D.x, resultsP1.position2D.y, resultsP2.position2D.x, resultsP2.position2D.y, { 9, 82, 4 , 255 });
			}
		}
	}
	for (int y = 0; y < ly + 1; y++) {
		for (int x = 0; x < lx; x++) {
			auto resultsP1 = virtualCamera::projectPoint(matrix[x][y]);
			auto resultsP2 = virtualCamera::projectPoint(matrix[x+1][y]);
			if (resultsP1.inView && resultsP2.inView) {
				DrawLine(resultsP1.position2D.x, resultsP1.position2D.y, resultsP2.position2D.x, resultsP2.position2D.y, { 9, 82, 4 , 255 });
			}
		}
	}
}