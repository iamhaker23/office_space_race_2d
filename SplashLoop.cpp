#include "SplashLoop.h"

SplashLoop::SplashLoop() : Loop() {

	this->backgroundPNG = 0;
	this->scene = {};
	
}

SplashLoop::~SplashLoop() {
	this->freeData();
}

void SplashLoop::freeData() {
	for (GameObject* o : scene) {
		delete o;
	}
	this->scene.clear();
	//utils::freeTexture(this->backgroundPNG);
	Loop::resetData(false);
}

void SplashLoop::resetData() {
	Loop::resetData(false);
	freeData();
	this->loopStarted = debugger->getTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->scene = {};

}


void SplashLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());

	if (debugger->getTime() - this->loopStarted >= 0.95) {
		if (inputs->keys[VK_RETURN]) {
			Loop::requestedActiveLoop = 1;
		}
		else {
			writeMessage("Press Enter", 50.0f, 160.0f);
		}
	}

}

void SplashLoop::handleActivation() {
	//LoopManager has activated this scene
	this->resetData();

	this->backgroundPNG = Loop::getTexture("resources/images/backgrounds/splash.png");;

	Loop::addFont("resources/fonts/HANDC.TTF", 20);

}
