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
	this->w = 1.0f;
}
Vect4f::Vect4f(const Vect4f &copy) {
	this->x = copy.x;
	this->y = copy.y;
	this->z = copy.z;
	this->w = copy.w;
}

Vect4f Vect4f::add(Vect4f &other) {
	return Vect4f(other.x + this->x, other.y + this->y, other.z + this->z, other.w + this->w);
}
Vect4f Vect4f::transform(Matrix3f &space) {
	return space.Multiply(Matrix3f(0.0f, this->x, this->y, this->z, 1.0f)).getPosition();

}

float Vect4f::getXYMagnitude() {
	return sqrt((getX() * getX()) + (getY() * getY()));
}

Vect4f Vect4f::scale(float x, float y, float z) {
	Vect4f output = Vect4f(this->x, this->y, this->z, 1.0f);

	output.x = (output.x * x);
	output.y = (output.y * y);
	output.z = (output.z * z);

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

Vect4f Vect4f::subtract(Vect4f &other) {
	Vect4f output = Vect4f();
	output.x = this->getX() - other.getX();
	output.y = this->getY() - other.getY();
	output.z = this->getZ() - other.getZ();
	//w is left as 1.0f

	return output;
}

Vect4f::~Vect4f() {

}
