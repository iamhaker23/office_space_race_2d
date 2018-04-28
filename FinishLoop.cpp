#include "FinishLoop.h"

FinishLoop::FinishLoop() : Loop() {

	this->backgroundPNG = 0;
	this->scene = {};
	this->startTime = 0.0;

}

FinishLoop::~FinishLoop() {
	this->freeData();
}

void FinishLoop::freeData() {
	this->scene.clear();
	Loop::freeData();

}

void FinishLoop::resetData() {
	this->freeData();
	Loop::resetData(true);

	this->loopStarted = debugger->getTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->backgroundPNG = Loop::getTexture("resources/images/backgrounds/finished.png");
	this->scene = {};
	if (this->startTime == 0.0 && debugger != NULL) this->startTime = debugger->getTime();

}



void FinishLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());

	double timeSinceStarted = debugger->getTime() - startTime;

	if (this->UI.front()->clicked && timeSinceStarted > 0.3) {
		Loop::requestedActiveLoop = 1;
	}
	else {

		if (this->globals != NULL && this->globals->updated) {
			this->globals->updated = false;
			RaceData* r = this->globals->player;
			
			string pos = (r->didNotFinish()) ? "DNF" : utils::getPositionLabel(r->getPosition());

			string stats = r->getName().append(" - ").append(pos);
			
			this->addUI(Vect4f(0.0f, 100.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), stats, UIType::LABEL, *Loop::fonts.back());
			
		}
	}

}

void FinishLoop::handleActivation() {
	//LoopManager has activated this scene
	this->resetData();
	
	Loop::addFont("resources/fonts/BKANT.TTF", 26);

	this->addUI(Vect4f(0.0f, 0.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Main Menu >", UIType::BUTTON, *Loop::fonts.back());
}
