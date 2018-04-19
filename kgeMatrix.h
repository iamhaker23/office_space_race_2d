#pragma once

#include "kgeUtils.h"

#ifndef KGE_MATRIX_H

#define KGE_MATRIX_H


#ifndef __KGE_MATRIX_H_DEFINED
	#define __KGE_MATRIX_H_DEFINED
	class Vect4f;
	#include "kgeVect.h"
#endif

class Matrix3f {

public:
	// openGL Matrix indices
	//|0, 4, 8,  12|
	//|1, 5, 9,  13|
	//|2, 6, 10, 14|
	//|3, 7, 11, 15|
	GLfloat values[16];
	float scalex = 1.0f;
	float scaley = 1.0f;
	float scalez = 1.0f;

	~Matrix3f();
	Matrix3f();
	Matrix3f(const Matrix3f &copy);

	void freeData();
	void initValues();
	Matrix3f(float radians, float x, float y, float z, float scale);
	Matrix3f(float radians, float x, float y, float z, float scalex, float scaley, float scalez);

	Vect4f* getPosition();
	Matrix3f* Add(Matrix3f* other);
	Matrix3f* Multiply(Matrix3f* other);
	Matrix3f* RotateRadians(float angle);
	Matrix3f* Translate(float x, float y, float z);
	Matrix3f* Scale(float x, float y, float z);
	Matrix3f* inverse();
	float getZAngle();
	void loadMatrix(GLfloat* _values);
};

#endif