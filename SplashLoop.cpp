#include "SplashLoop.h"

SplashLoop::SplashLoop() : Loop() {

	this->backgroundPNG = 0;
	this->scene = {};
	
}

SplashLoop::~SplashLoop() {
	this->freeData();
}

void SplashLoop::freeData() {
	this->scene.clear();
	utils::freeTexture(this->backgroundPNG);
	Loop::freeData();
}

void SplashLoop::resetData() {
	Loop::resetData(false);

	this->loopStarted = debugger->getTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->backgroundPNG = utils::initTexture(utils::loadPNG("resources/images/backgrounds/splash.png"));
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

	font_data* font1 = new font_data();
	font1->init("resources/fonts/BKANT.TTF", 20);
	Loop::fonts.push_back(font1);

}
