#pragma once
#include "Loop.h"

#ifndef FINISHLOOP_H

#define FINISHLOOP_H

class FinishLoop : public Loop {


private:
	vector<GameObject*> scene;
	GLuint backgroundPNG;

public:

	FinishLoop();
	~FinishLoop();

	void init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs);
	void display();
	void freeData();
	void handleActivation();

};

#endif
