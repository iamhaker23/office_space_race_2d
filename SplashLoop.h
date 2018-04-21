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

	void display();
	void freeData() override;
	void resetData();
	void handleActivation();

};

#endif