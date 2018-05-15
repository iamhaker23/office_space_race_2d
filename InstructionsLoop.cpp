#include "InstructionsLoop.h"

InstructionsLoop::InstructionsLoop() : Loop() {

	this->backgroundPNG = 0;
	this->scene = {};
	this->startTime = 0.0;

}

InstructionsLoop::~InstructionsLoop() {
	this->freeData();
}

void InstructionsLoop::freeData() {
	this->scene.clear();
	Loop::freeData();

}

void InstructionsLoop::resetData() {
	this->freeData();
	Loop::resetData(true);

	this->loopStarted = debugger->getTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->backgroundPNG = Loop::getTexture("resources/images/backgrounds/presentation.png");
	this->scene = {};
	this->startTime = debugger->getTime();
}



void InstructionsLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());
	
	double timeSinceStarted = debugger->getTime() - startTime;

	if (this->UI.front()->clicked && timeSinceStarted > 0.3) {
		Loop::requestedActiveLoop = 1;
	}
	else {
		string instructions = "WASD - Move\nTAB - Pause\nESC - Quit\nUP - View Debug Info";
		Loop::writeMessage(instructions, 300.0f, 300.0f, Color4f(), Color4f(0.3f, 0.3f, 0.3f, 0.5f));
		
	}

}

void InstructionsLoop::handleActivation() {
	//LoopManager has activated this scene
	this->resetData();

	Loop::addFont("resources/fonts/PIXEL.TTF", 25);

	this->addUI(Vect4f(0.0f, -100.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Main Menu >", UIType::BUTTON, *Loop::fonts.back());
}
