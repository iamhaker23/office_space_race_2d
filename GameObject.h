#pragma once

#include <vector>
#include <string>
#include "kgeUtils.h"
#include "InputStates.h"

#ifndef GAME_OBJECT_H

#define GAME_OBJECT_H

using std::string;
using std::vector;


struct Vertex {
public:
	float x;
	float y;
	float u;
	float v;
	Vertex(float u, float v, float x, float y) {
		this->u = u;
		this->v = v;
		this->x = x;
		this->y = y;
	}
};


enum AnimationLogic {
	PINGPONG = 1, LOOPEND = 2
};

class GameObject {
	private:
		int activeSpriteIndex;
		vector<string> sprites;
		float objectColor[4] = {
			0.0f, 0.0f, 0.0f, 0.0f 
		};

		// openGL Matrix indices
		//|0, 4, 8,  12|
		//|1, 5, 9,  13|
		//|2, 6, 10, 14|
		//|3, 7, 11, 15|
		GLfloat worldTransformMatrix[16];

		GLfloat forces[3];
		GLfloat oldForces[3];
		GLfloat zTorque;
		GLfloat scales[3];
		
		GLfloat friction;
		GLfloat angularDamping;
				
		vector<Vertex> mesh;

		int pingPongDirection;
		string name;

		bool playerControl;
		float topSpeed;

	public:
		GameObject();
		GameObject(const GameObject &copy);
		GameObject(string name, vector<string> sprites, vector<Vertex> mesh, int activeSprite, float objectColor[]);
		void setSprite(int index);
		void animate(AnimationLogic al);
		string toString();

		void draw();
		void processInputs(InputStates* inputs);

		void scale(float uScale);
		void nuScale(float x, float y, float z);
		void translate(float x, float y, float z);

		void setPhysicalAttributes(float friction, float angularDamping, float topSpeed);
		vector<float> getPosition();
		float getZAngle();
		void setPlayerControl(bool playerControl);
		string getName();
};


#endif