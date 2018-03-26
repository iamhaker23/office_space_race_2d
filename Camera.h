#pragma once

#include "GameObject.h"

#ifndef CAMERA_H

#define CAMERA_H

class Camera {

private:
	GameObject* cameraTarget;
	float zAngle = 0.0f;
	float cameraPos[3] = { 0.0f, 0.0f, 0.0f };
	float slowFactorMov = 0.15f ;
	float slowFactorRot = 0.99f;
	Matrix3f* cameraTransform;

public:
	Camera();
	GameObject* getCameraTarget();
	void setCameraTarget(GameObject* obj);
	float getCameraZAngle();
	void setCameraZAngle(float zRads);
	float* getCameraPosition(); 
	void setCameraPosition(float x, float y, float z);
	float getSlowFactorMov();
	float getSlowFactorRot();
	void setCameraSlowParentFactors(float movement, float rotation);
	
	Matrix3f* getTransformation();

};

#endif;
