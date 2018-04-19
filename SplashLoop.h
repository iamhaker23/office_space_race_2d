#pragma once
#include "Loop.h"

#ifndef SPLASHLOOP_H

#define SPLASHLOOP_H

class SplashLoop : public Loop {

private:
	vector<GameObject*> scene;
	GLuint backgroundPNG;

public:

	SplashLoop();
	~SplashLoop();

	void init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs);
	void display();
	void freeData();
	void handleActivation();

};

#endif