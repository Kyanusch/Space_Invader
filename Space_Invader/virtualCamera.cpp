#include "virtualCamera.h"

//#include <iostream>

namespace virtualCamera {
	sVector3 position;	//camera position in 3D world
	sVector3 forward;	//camera direction of forward 
	sVector3 up;		//camera direction of up
	sVector3 right;		//camera direction of right
	float focalLenght;	//defines the zoom
	

	sVector3::sVector3() : vec({ 0,0,0 }) {}	//empty vector
	sVector3::sVector3(Vector3 vector) : vec(vector){}
	sVector3 sVector3::operator+(const sVector3& b) const { return sVector3({ vec.x + b.vec.x, vec.y + b.vec.y, vec.z + b.vec.z }); }		//vector addition
	sVector3 sVector3::operator-(const sVector3& b) const { return sVector3({ vec.x - b.vec.x, vec.y - b.vec.y, vec.z - b.vec.z }); }		//vector substraction
	sVector3 sVector3::operator*(float s) const { return sVector3({ vec.x * s, vec.y * s, vec.z * s }); }		//vector multiplication
	sVector3 sVector3::operator/(float s) const { return sVector3({ vec.x / s, vec.y / s, vec.z / s }); }		//vector division

	float scalar(const sVector3& a, const sVector3& b) {		//scalar product
		return a.vec.x * b.vec.x + a.vec.y * b.vec.y + a.vec.z * b.vec.z;
	}
	sVector3 cross(const sVector3& a, const sVector3& b) {   //cross product
		return sVector3({
			a.vec.y * b.vec.z - a.vec.z * b.vec.y,
			a.vec.z * b.vec.x - a.vec.x * b.vec.z,
			a.vec.x * b.vec.y - a.vec.y * b.vec.x
			});
	}
	float lenght(const sVector3& v) {		//absolute value of a sVector3
		return std::sqrtf(scalar(v, v));
	}
	sVector3 normalize(const sVector3& v) {		//Hesse normal form of a vector
		float len = lenght(v);
		if (len == 0.0f) throw std::runtime_error("virtualCamera: Cannot normalize zero-lenght vector");
		return v / len;
	}

	void initialize(const Vector3& pPosition, const Vector3& pTarget, const Vector3& pWorldUp, float pFocalLenght) {
		position = sVector3(pPosition);
		focalLenght = pFocalLenght;
		forward = normalize(sVector3(pTarget) - position);
		right = normalize(cross(forward, sVector3(pWorldUp)));
		up = cross(right, forward);
	}
	camResults projectPoint(const Vector3& point) {
		sVector3 Point(point);
		sVector3 local = Point - position;
		float distance = lenght(local);
		bool inView = true;
		float z = scalar(local, forward);
		if (z <= 0.0f) {
			inView = false;
			//throw std::runtime_error("Point is behind the camera");
			//std::cout << "Point is behind the camera\n";
		}
		float x = -scalar(local, right) ;
		float y = scalar(local, up) ;
		Vector2 position2D = { (x / z) * focalLenght+ GetScreenWidth() / 2, (y / z) * focalLenght + GetScreenHeight() / 2};
		return { position2D,distance, inView};
	}

	sVector3 rotatePointAroundAxis(const sVector3& pPoint, const Vector3& axis, float angleRadians) {
		sVector3 point(pPoint);
		sVector3 normAxis = normalize(sVector3(axis));
		float halfAngle = angleRadians * 0.5f;
		float sinHalf = std::sin(halfAngle);
		float cosHalf = std::cos(halfAngle);

		float w = cosHalf;
		float x = normAxis.vec.x * sinHalf;
		float y = normAxis.vec.y * sinHalf;
		float z = normAxis.vec.z * sinHalf;

		// Quaternion rotation formula: v' = q * v * q^-1
		// where v = (0, x, y, z) and q^-1 = conjugate if q is normalized

		// Quaternion * vector
		float vw = -x * point.vec.x - y * point.vec.y - z * point.vec.z;
		float vx = w * point.vec.x + y * point.vec.z - z * point.vec.y;
		float vy = w * point.vec.y + z * point.vec.x - x * point.vec.z;
		float vz = w * point.vec.z + x * point.vec.y - y * point.vec.x;

		// Result * conjugate quaternion
		float rx = vw * -x + vx * w - vy * z + vz * y;
		float ry = vw * -y + vy * w - vz * x + vx * z;
		float rz = vw * -z + vz * w - vx * y + vy * x;

		return sVector3({ rx, ry, rz });
	}
};