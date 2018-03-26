
#include "Camera.h"
#include "kgeGeometry.h"

Camera::Camera() {
	this->cameraTarget = NULL;
}
GameObject* Camera::getCameraTarget() {
	return this->cameraTarget;
}
void Camera::setCameraTarget(GameObject* obj) {
	this->cameraTarget = obj;
}
float Camera::getCameraZAngle() {
	return this->zAngle;
}
void Camera::setCameraZAngle(float zRads) {
	this->zAngle = zRads;
}
Vect4f* Camera::getCameraPosition() {
	return this->cameraPos;
}

void Camera::setCameraPosition(float x, float y, float z) {
	this->cameraPos->x = x;
	this->cameraPos->y = y;
	this->cameraPos->z = z;
}

float Camera::getSlowFactorMov() {
	return this->slowFactorMov;
}
float Camera::getSlowFactorRot() {
	return this->slowFactorRot;
}
void Camera::setCameraSlowParentFactors(float movement, float rotation) {
	this->slowFactorMov = movement;
	this->slowFactorRot = rotation;
}

Matrix3f* Camera::getTransformation() {
	
	//On second thought, don't FORCE camera transform on all objects because it destroys world-space
	//I.E. world space becomes locked to camera space!
	
	//glMatrixMode(GL_PROJECTION);
	//glTranslatef(-cameraPos[0], -cameraPos[1], -cameraPos[2]);
	//glRotatef(-zAngle, 0.0f, 0.0f, 1.0f);

	//this->cameraTransform = (new Matrix3f(0.0f, -cameraPos[0], -cameraPos[1], -cameraPos[2], 0.0f))->RotateRadians(-zAngle);
	
	this->cameraTransform = (new Matrix3f(-zAngle, 0.0f, 0.0f, 0.0f, 1.0f));
	//this->cameraTransform = (new Matrix3f(0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	this->cameraTransform = this->cameraTransform->Multiply(new Matrix3f(0.0f, -cameraPos->x, -cameraPos->y, -cameraPos->z, 1.0f));
	//this->cameraTransform = (new Matrix3f(0.0f, -cameraPos[0], -cameraPos[1], -cameraPos[2], 1.0f));
	//this->cameraTransform = this->cameraTransform->Multiply(new Matrix3f(-zAngle, 0.0f, 0.0f, 0.0f, 1.0f));
	//this->cameraTransform = (new Matrix3f(-zAngle, 0.0f, 0.0f, 0.0f, 1.0f))->Translate(-cameraPos[0], -cameraPos[1], -cameraPos[2]);
	//glMultMatrixf(this->cameraTransform->values);
	
	/*
	zAngle = 0.0f;
	cameraPos[0] = 0.0f;
	cameraPos[1] = 0.0f;
	cameraPos[2] = 0.0f;
	*/

	return this->cameraTransform;
}