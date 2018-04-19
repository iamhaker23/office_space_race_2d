#pragma once

#include "GameObject.h"

#ifndef GO_RACER_H

#define GO_RACER_H

class GO_Racer : public GameObject {

private:

	bool steering;
	bool playerControl;
	bool aiControl;
	bool racer;

public:

	RaceData* raceData;

	~GO_Racer();
	GO_Racer(); 
	GO_Racer(string name, vector<GLuint> sprites, vector<Vertex> mesh, int activeSprite, Color4f* objectColor);
	GO_Racer(const GO_Racer &copy);

	void draw();

	bool isAI();
	bool isRacer();
	RaceData* getRaceData();

	vector<CollisionResult> GO_Racer::resolveCollision(vector <GameObject*> objs);

	void setAIControl(bool flag);
	void doAIControl(GameObject* track, int trackStep);

	void setPlayerControl(bool playerControl);
	void processInputs(InputStates* inputs);

};

#endif