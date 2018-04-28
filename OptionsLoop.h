#pragma once
#include "Loop.h"

#ifndef OPTIONSLOOP_H

#define OPTIONSLOOP_H

class OptionsLoop : public Loop {


private:
	vector<GameObject*> scene;
	GLuint backgroundPNG;
	double startTime;

public:

	OptionsLoop();
	~OptionsLoop();

	void display();
	void freeData() override;
	void resetData();
	void handleActivation();

};

#endif
