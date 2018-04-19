
#include "Camera.h"
#include "kgeGeometry.h"

Camera::Camera() {
	this->cameraTarget = NULL;
}


void Camera::freeData() {
	this->cameraTransform->freeData();

}

GameObject* Camera::getCameraTarget() {
	return this->cameraTarget;
}
void Camera::setCameraTarget(GameObject* obj) {
	this->cameraTarget = obj;
	setCameraPosition(obj->getWorldPosition()->x, obj->getWorldPosition()->y, obj->getWorldPosition()->z);
	this->setCameraZAngle(this->cameraTarget->getZAngle());
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

	if (this->cameraTarget != NULL) {
		Vect4f* follow = this->cameraTarget->getWorldPosition();
		Vect4f* camPos = getCameraPosition();

		float movDamp = this->getSlowFactorMov();
		this->setCameraPosition(camPos->x + ((follow->x - camPos->x) * movDamp), camPos->y + ((follow->y - camPos->y) * movDamp), 0.0f);
		this->setCameraZAngle(this->cameraTarget->getZAngle());

	}

	this->cameraTransform = (new Matrix3f(-zAngle, 0.0f, 0.0f, 0.0f, 1.0f));
	this->cameraTransform = this->cameraTransform->Multiply(new Matrix3f(0.0f, 0.0f, 0.0f, 0.0f, 0.5f));
	this->cameraTransform = this->cameraTransform->Multiply(new Matrix3f(0.0f, -cameraPos->x, -cameraPos->y, -cameraPos->z, 1.0f));

	return this->cameraTransform;
}