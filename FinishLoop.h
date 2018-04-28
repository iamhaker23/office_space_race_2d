#pragma once
#include "Loop.h"

#ifndef FINISHLOOP_H

#define FINISHLOOP_H

class FinishLoop : public Loop {


private:
	vector<GameObject*> scene;
	GLuint backgroundPNG;
	double startTime;

public:

	FinishLoop();
	~FinishLoop();

	void display();
	void freeData() override;
	void resetData();
	void handleActivation();

};

#endif
