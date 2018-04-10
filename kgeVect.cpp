#include "kgeVect.h"

Vect4f::Vect4f(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vect4f::Vect4f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 1.0f;
}
Vect4f::Vect4f() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 0.0f;
}
Vect4f* Vect4f::add(Vect4f* other) {
	return new Vect4f(other->x + this->x, other->y + this->y, other->z + this->z, other->w + this->w);
}
Vect4f* Vect4f::transform(Matrix3f* space) {
	return space->Multiply((new Matrix3f(0.0f, this->x, this->y, this->z, 1.0f)))->getPosition();
	
	/*Vect4f* output = new Vect4f(this->x, this->y, this->z, 1.0f);

	output->x = (output->x * space->values[0]) + (output->y * space->values[4]);
	output->y = (output->x * space->values[1]) + (output->y * space->values[5]);

	output->x += (output->x * space->scalex);
	output->y += (output->y * space->scaley);
	output->z += (output->z * space->scalez);

	output->x += (output->w * space->values[12]);
	output->y += (output->w * space->values[13]);
	output->z +=  (output->w * space->values[14]);
	
	
	return output;*/
}

float Vect4f::getXYMagnitude() {
	return sqrt((getX() * getX()) + (getY() * getY()));
}

Vect4f* Vect4f::scale(float x, float y, float z) {
	//return space->Multiply((new Matrix3f(0.0f, this->x, this->y, this->z, 1.0f)))->getPosition();
	Vect4f* output = new Vect4f(this->x, this->y, this->z, 1.0f);

	output->x = (output->x * x);
	output->y = (output->y * y);
	output->z = (output->z * z);

	return output;
}

float Vect4f::getX() {
	return this->x / this->w;
}

float Vect4f::getY() {
	return this->y / this->w;
}

float Vect4f::getZ() {
	return this->z / this->w;
}