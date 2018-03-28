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
}