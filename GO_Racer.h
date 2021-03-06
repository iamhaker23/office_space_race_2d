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
	bool loseSteeringOnCollision;

public:

	RaceData* raceData;

	~GO_Racer();
	GO_Racer(); 
	GO_Racer(string name, vector<GLuint> &sprites, vector<Vertex> &mesh, int activeSprite, Color4f &objectColor, vector<GLuint> chairSprites, bool loseSteeringOnCollision);
	GO_Racer(const GO_Racer &copy);
	void freeData() override;

	void draw() override;
	vector<CollisionResult> GO_Racer::resolveCollisions(const vector <GameObject*> &objs) override;

	bool isAI();
	bool isRacer();
	RaceData* getRaceData();


	void setAIControl(bool flag);
	void doAIControl(GameObject* track, int trackStep);

	void setPlayerControl(bool playerControl);
	void processInputs(InputStates* inputs);

};

#endif