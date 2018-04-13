#include "kgeMatrix.h"

Matrix3f::~Matrix3f() {
	delete[](GLfloat*)values;
}

Matrix3f::Matrix3f() {
	initValues();
}

Matrix3f::Matrix3f(const Matrix3f &copy) {
	this->scalex = copy.scalex;
	this->scaley = copy.scaley;
	this->scalez = copy.scalez;
	for (int i = 0; i < 16; i++) {
		this->values[i] = copy.values[i];
	}
}

void Matrix3f::initValues() {
	for (int i = 0; i < 16; i++) {
		this->values[i] = 0.0f;
	}
	this->values[0] = 1.0f;
	this->values[5] = 1.0f;
	this->values[10] = 1.0f;
	this->values[15] = 1.0f;
	
}

Matrix3f::Matrix3f(float radians, float x, float y, float z, float scale) {
	initValues();
	float cosRads = cos(radians);
	float sinRads = sin(radians);
	this->values[0] = cosRads;
	this->values[4] = -1.0f * sinRads;
	this->values[1] = sinRads;
	this->values[5] = cosRads;
	this->values[12] = x;
	this->values[13] = y;
	this->values[14] = z;

	values[10] = 1.0f;
	this->values[0] *= scale;
	this->values[5] *= scale;
	this->values[10] *= scale;
	this->scalex = scale;
	this->scaley = scale;
	this->scalez = scale;

}
Matrix3f::Matrix3f(float radians, float x, float y, float z, float scalex, float scaley, float scalez) {
	initValues();
	float cosRads = cos(radians);
	float sinRads = sin(radians);
	this->values[0] = cosRads;
	this->values[4] = -1.0f * sinRads;
	this->values[1] = sinRads;
	this->values[5] = cosRads;
	this->values[12] = x;
	this->values[13] = y;
	this->values[14] = z;

	values[10] = 1.0f;
	this->values[0] = scalex;
	this->values[5] = scaley;
	this->values[10] = scalez;
	this->scalex = scalex;
	this->scaley = scaley;
	this->scalez = scalez;

}

Vect4f* Matrix3f::getPosition() {
	return new Vect4f(this->values[12], this->values[13], this->values[14], this->values[15]);
}

Matrix3f* Matrix3f::Add(Matrix3f* other) {
	Vect4f* myPos = this->getPosition();
	Vect4f* otherPos = other->getPosition();

	return 
		new Matrix3f(
			this->getZAngle() + other->getZAngle(),
			myPos->x + otherPos->x,
			myPos->y + otherPos->y,
			myPos->z + otherPos->z,
			this->scalex + other->scalex,
			this->scaley + other->scaley,
			this->scalez + other->scalez
		);
	/*
	return (new Matrix3f(0.0f, myPos->x + otherPos->x, myPos->y + otherPos->y, myPos->z + otherPos->z, 1.0f))->Multiply(
		((new Matrix3f(this->getZAngle() + other->getZAngle(), 0.0f, 0.0f, 0.0f, 1.0f))->Multiply(
			new Matrix3f(0.0f, 0.0f, 0.0f, 0.0f, this->scalex + other->scalex, this->scaley + other->scaley, this->scalez + other->scalez)
		)
	));*/

}

Matrix3f* Matrix3f::Multiply(Matrix3f* other) {
	Matrix3f* output = new Matrix3f();

	for (int i = 0; i < 4; i++) {
		//|0, 4, 8,  12|
		//|1, 5, 9,  13|
		//|2, 6, 10, 14|
		//|3, 7, 11, 15|
		output->values[i] =
			(this->values[i] * other->values[0]) +
			(this->values[i + 4] * other->values[1]) +
			(this->values[i + 8] * other->values[2]) +
			(this->values[i + 12] * other->values[3]);
		output->values[i + 4] =
			(this->values[i] * other->values[4]) +
			(this->values[i + 4] * other->values[5]) +
			(this->values[i + 8] * other->values[6]) +
			(this->values[i + 12] * other->values[7]);
		output->values[i + 8] =
			(this->values[i] * other->values[8]) +
			(this->values[i + 4] * other->values[9]) +
			(this->values[i + 8] * other->values[10]) +
			(this->values[i + 12] * other->values[11]);
		output->values[i + 12] =
			(this->values[i] * other->values[12]) +
			(this->values[i + 4] * other->values[13]) +
			(this->values[i + 8] * other->values[14]) +
			(this->values[i + 12] * other->values[15]);
	}

	return output;
}

Matrix3f* Matrix3f::RotateRadians(float angle) {
	Vect4f* myPos = this->getPosition();
	return
		(new Matrix3f(
			angle,
			0.0f,
			0.0f,
			0.0f,
			1.0f))->Multiply(this);
}
Matrix3f* Matrix3f::Translate(float x, float y, float z) {
	return
		(this)->Add(new Matrix3f(
			0.0f,
			x,
			y,
			z,
			0.0f));
}
Matrix3f* Matrix3f::Scale(float x, float y, float z) {
	return
		(this)->Add(new Matrix3f(
			0.0f, 0.0f, 0.0f, 0.0f,
			x,
			y,
			z));
}


Matrix3f* Matrix3f::inverse() {
	Vect4f* pos = this->getPosition();
	return new Matrix3f(this->getZAngle()*-1.0f, pos->x*-1.0f, pos->y*-1.0f, pos->z*-1.0f, 1.0f / this->scalex, 1.0f / this->scaley, 1.0f / this->scalez);

}

float Matrix3f::getZAngle() {
	//tanA = opp(y)/adj(x)
	return atan2f(this->values[1], this->values[0]);
}

void Matrix3f::loadMatrix(GLfloat* _values) {
	for (int i = 0; i < 16; i++) {
		this->values[i] = _values[i];
	}
}
