#pragma once

#ifndef KGE_VECT_H

#define KGE_VECT_H

#ifndef __KGE_VECT_H_DEFINED
	#define __KGE_VECT_H_DEFINED
	class Matrix3f;
	#include "kgeMatrix.h"
#endif

class Vect4f {
public:

	float x;
	float y;
	float z;
	float w;

	Vect4f(float x, float y, float z, float w);
	Vect4f(float x, float y, float z);
	Vect4f();
	~Vect4f();
	Vect4f(const Vect4f &copy);

	Vect4f add(Vect4f &other);
	Vect4f transform(Matrix3f &space);
	Vect4f scale(float x, float y, float z);
	float getXYMagnitude();
	float getX();
	float getY();
	float getZ();
	Vect4f subtract(Vect4f &other);


};

#endif