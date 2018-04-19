#pragma once
#include "Loop.h"

#ifndef MENULOOP_H

#define MENULOOP_H

class MenuLoop : public Loop {


private:
	vector<GameObject*> scene;
	GLuint backgroundPNG;

public:

	MenuLoop();
	~MenuLoop();

	void init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs);
	void display();
	void freeData();
	void handleActivation();

};

#endif
