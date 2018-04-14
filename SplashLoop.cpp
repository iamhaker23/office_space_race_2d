#include "SplashLoop.h"

SplashLoop::SplashLoop() : Loop() {

	//reset camera
	delete Loop::camera;
	Loop::camera = new Camera();

	this->backgroundPNG = utils::loadPNG("resources/images/backgrounds/splash.png");
	this->scene = {};

}

SplashLoop::~SplashLoop() {
	this->freeData();
}

void SplashLoop::freeData() {
	Loop::freeData();
	Loop::freeStaticData();
}

void SplashLoop::init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs)
{
	Loop::init(_hDC, _debugger, inputs);
	GameObject::setDebugger(_debugger);

	glClearColor(0.0, 0.0, 0.0, 0.0);

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
	
	fonts.clear();
	font_data* font1 = new font_data();
	font1->init("resources/fonts/BKANT.TTF", 20);
	Loop::fonts.push_back(font1);

	Loop::freeData();

	//less annoying to only use the delay once...
	//this->loopStarted = debugger->getTime();

}
