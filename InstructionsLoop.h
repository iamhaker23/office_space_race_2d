#pragma once
#include "Loop.h"

#ifndef INSTRUCTIONSLOOP_Hs

#define INSTRUCTIONSLOOP_H

class InstructionsLoop : public Loop {


private:
	vector<GameObject*> scene;
	GLuint backgroundPNG;
	double startTime;

public:

	InstructionsLoop();
	~InstructionsLoop();

	void display();
	void freeData() override;
	void resetData();
	void handleActivation();

};

#endif
