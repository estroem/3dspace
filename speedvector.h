#ifndef __SPEEDVECTOR_H__
#define __SPEEDVECTOR_H__

class SpeedVector {
public:
	float x;
	float y;
	float z;

	SpeedVector() : x(0), y(0), z(0) {}
	SpeedVector(float x, float y, float z);
};

#endif