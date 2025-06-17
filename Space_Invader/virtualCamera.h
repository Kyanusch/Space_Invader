#pragma once
#include "raylib.h"
#include <cmath>
#include <stdexcept>

namespace virtualCamera{
	const float worldWidth = 2000.0f;	//world width
	const float worldHight = 1000.0f;	//World hight

	struct sVector3 {
		Vector3 vec;
		sVector3();	//empty vector
		sVector3(Vector3 vector);
		sVector3 operator+(const sVector3& b) const; 		//vector addition
		sVector3 operator-(const sVector3& b) const;		//vector substraction
		sVector3 operator*(float s) const;					//vector multiplication
		sVector3 operator/(float s) const;					//vector division
	};
	float scalar(const sVector3& a, const sVector3& b);
	sVector3 cross(const sVector3& a, const sVector3& b);
	float lenght(const sVector3& v);
	sVector3 normalize(const sVector3& v);

	typedef struct camResults {		//datatype for the end results
		Vector2 position2D;
		float distance;
		bool inView;
	} camResults;

	extern sVector3 position;	//camera position in 3D world
	extern sVector3 forward;	//camera direction of forward 
	extern sVector3 up;			//camera direction of up
	extern sVector3 right;		//camera direction of right
	extern float focalLenght;	//defines the zoom

	void initialize(const Vector3& pPosition, const Vector3& pTarget, const Vector3& pWorldUp, float pFocalLenght);
	camResults projectPoint(const Vector3& point);
	sVector3 rotatePointAroundAxis(const sVector3& pPoint, const Vector3& axis, float angleRadians);
}

