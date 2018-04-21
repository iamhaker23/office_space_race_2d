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

	void display();
	void freeData() override;
	void resetData();
	void handleActivation();

};

#endif
