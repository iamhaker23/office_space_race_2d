#include "FinishLoop.h"

FinishLoop::FinishLoop() : Loop() {

	this->backgroundPNG = 0;
	this->scene = {};


}

FinishLoop::~FinishLoop() {
	this->freeData();
}

void FinishLoop::freeData() {
	this->scene.clear();
	//utils::freeTexture(this->backgroundPNG);
	Loop::freeData();

}

void FinishLoop::resetData() {
	this->freeData();
	Loop::resetData(true);

	this->loopStarted = debugger->getTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->backgroundPNG = Loop::getTexture("resources/images/backgrounds/finished.png");
	this->scene = {};
}



void FinishLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());

	if (this->UI.front()->clicked) {
		Loop::requestedActiveLoop = 1;
	}
	else {

		if (this->globals != NULL && this->globals->updated) {
			this->globals->updated = false;
			RaceData* r = this->globals->player;
			
			//playerName is used to communicate intended input between scenes, and player->getName is the actual name of the player
			string stats = r->getName().append(" - ").append(utils::getPositionLabel(r->getPosition()));
			
			this->addUI(Vect4f(0.0f, 100.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), stats, UIType::LABEL, *Loop::fonts.back());
			/*
			POSITIONS AREN'T CALCULATED FOR OTHER RACERS, SO NOTHING TO DISPLAY
			int count = 0;
			for (RaceData* r : this->globals->others) {
				string stats = utils::intLabel(&r->getName()[0], r->getPosition(), "");
				this->addUI(new Vect4f(0.0f, 100.0f - (60.0f*(count++)), 0.0f), new Vect4f(200.0f, 60.0f, 0.0f), stats, UIType::LABEL, Loop::fonts.back());
			}*/
		}
	}

}

void FinishLoop::handleActivation() {
	//LoopManager has activated this scene
	this->resetData();
	
	Loop::addFont("resources/fonts/BKANT.TTF", 26);

	this->addUI(Vect4f(150.0f, 0.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Main Menu", UIType::BUTTON, *Loop::fonts.back());
}
