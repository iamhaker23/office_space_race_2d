#pragma once

#include <vector>
#include <string>
#include "kgeUtils.h"

#ifndef GAME_OBJECT_H

#define GAME_OBJECT_H

using std::string;
using std::vector;

class GameObject {
	private:
		int activeSpriteIndex;
		vector<string> sprites;
		float objectColor[4] = {
			0.0f, 0.0f, 0.0f, 0.0f 
		};
		float transformMatrix[16] = { 
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		};
		int pingPongDirection;

	public:
		GameObject();
		GameObject(vector<string> sprites, int activeSprite, float objectColor[]);
		virtual void draw();
		void setSprite(int index);
		void pingPongSprite();
		//void scale(float x, float y, float z);
		//void rotate(float x, float y, float z);
		//void translate(float x, float y, float z);

};

#endif