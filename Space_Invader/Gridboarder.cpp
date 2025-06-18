#include "Gridboarder.h"
#include <iostream>
Gridboarder::Gridboarder(Vector3 Origin, Vector3 dX, Vector3 dY, int lx, int ly, float distortion) : lx(lx), ly(ly), distortion(distortion){
	origin = { Origin };    //convert Vector3 to virtualCamera::sVector3
	dx = { dX };
	dy = { dY };
	virtualCamera::sVector3 temppoint = origin;
	pDistortion = virtualCamera::normalize(virtualCamera::cross(dx, dy)) * distortion;  // randomizes the gird points in the Z plane
	for (int x = 0; x < lx + 1; x++) {    // create grid points
		std::vector<Vector3> vx;
		temppoint = origin + (dx * x);
		for (int y = 0; y < ly+1; y++) {
			temppoint = temppoint + (dy * y);
			auto tempP = temppoint + (pDistortion * (GetRandomValue(-100, 100)/ 100.0f));
			vx.push_back(tempP.vec);
		}
		matrix.push_back(vx);
	}
}

Gridboarder::Gridboarder(Vector3 Origin, Vector3 dX, Vector3 dY, int lx, int ly) : lx(lx), ly(ly), distortion(0.0f){
	origin = { Origin };    //convert Vector3 to virtualCamera::sVector3
	dx = { dX };
	dy = { dY };
	virtualCamera::sVector3 temppoint = origin;
	pDistortion = virtualCamera::normalize(virtualCamera::cross(dx, dy)) * distortion;
	for (int x = 0; x < lx + 1; x++) {      // create grid points
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
void Gridboarder::draw() const {
    const float maxDistance = 20000.0f; // Distance at which lines become completely transparent
    const unsigned char baseAlpha = 255;

    for (int x = 0; x < lx + 1; x++) {
        for (int y = 0; y < ly; y++) {
            auto resultsP1 = virtualCamera::projectPoint(matrix[x][y]);
            auto resultsP2 = virtualCamera::projectPoint(matrix[x][y+1]);
            if (resultsP1.inView && resultsP2.inView) {
                float avgDistance = (resultsP1.distance + resultsP2.distance) / 2.0f; // Calculate average distance for the line
                // Calculate alpha based on distance
                unsigned char alpha = avgDistance >= maxDistance ? 0 : static_cast<unsigned char>(baseAlpha * (1.0f - avgDistance / maxDistance));
				// Draw the line between resultsP1 and resultsP2
                DrawLine(resultsP1.position2D.x, resultsP1.position2D.y, resultsP2.position2D.x, resultsP2.position2D.y, { 9, 82, 4, alpha });
            }
        }
    }
    
    for (int y = 0; y < ly + 1; y++) {
        for (int x = 0; x < lx; x++) {
            auto resultsP1 = virtualCamera::projectPoint(matrix[x][y]);
            auto resultsP2 = virtualCamera::projectPoint(matrix[x+1][y]);
            if (resultsP1.inView && resultsP2.inView) {
                float avgDistance = (resultsP1.distance + resultsP2.distance) / 2.0f; // Calculate average distance for the line
                // Calculate alpha based on distance
                unsigned char alpha = avgDistance >= maxDistance ? 0 : static_cast<unsigned char>(baseAlpha * (1.0f - avgDistance / maxDistance));
				// Draw the line between resultsP1 and resultsP2
                DrawLine(resultsP1.position2D.x, resultsP1.position2D.y, resultsP2.position2D.x, resultsP2.position2D.y, { 9, 82, 4, alpha });
            }
        }
    }
}