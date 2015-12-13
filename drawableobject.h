#ifndef __DRAWABLEOBJECT_H__
#define __DRAWABLEOBJECT_H__

#include <cmath>

class DrawableObject {
public:
	DrawableObject() {}
	virtual void draw() = 0;
};

#endif