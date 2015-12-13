#ifndef __BOX_H__
#define __BOX_H__

class Box {
public:
	float right;
	float left;
	float top;
	float bottom;
	float front;
	float back;

	Box() {}
	Box(float right, float left, float top,  float bottom, float front, float back) : right(right), left(left), top(top), bottom(bottom), front(front), back(back) {}
};

#endif