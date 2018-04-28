#include "MenuLoop.h"

MenuLoop::MenuLoop() : Loop() {

	this->backgroundPNG = 0;
	this->scene = {};
	this->startTime = 0.0;

}

MenuLoop::~MenuLoop() {
	this->freeData();
}

void MenuLoop::freeData() {
	for (GameObject* o : scene) {
		delete o;
	}
	this->scene.clear();
	//utils::freeTexture(this->backgroundPNG);
	Loop::resetData(true);
}

void MenuLoop::resetData() {

	Loop::resetData(true);
	freeData();
	this->loopStarted = debugger->getTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->scene = {};
	if (this->startTime == 0.0 && debugger != NULL) this->startTime = debugger->getTime();

}

void MenuLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());

	double timeSinceStarted = debugger->getTime() - startTime;

	if ((int)this->UI.size() > 0 && timeSinceStarted > 0.3) {

		double BUTTON_DELAY = 0.15;

		UIElement* startButton = this->UI.back();
		UIElement* nameInput = this->UI.at(1);
		UIElement* optionsButton = this->UI.at(2);
		UIElement* instructionsButton = this->UI.at(3);

		if (startButton->clicked) {
			this->globals->playerName = nameInput->value;
			Loop::requestedActiveLoop = 2;
		}
		else if (optionsButton->clicked) {
			this->globals->playerName = nameInput->value;
			Loop::requestedActiveLoop = 4;
		}
		else if (instructionsButton->clicked) {
			this->globals->playerName = nameInput->value;
			Loop::requestedActiveLoop = 5;
		}

	}

}

void MenuLoop::handleActivation() {
	//LoopManager has activated this scene
	this->resetData();

	this->backgroundPNG = Loop::getTexture("resources/images/backgrounds/presentation.png");
	

	Loop::addFont("resources/fonts/BKANT.TTF", 20);

	string name = (this->globals->playerName != "") ? this->globals->playerName : "Player";

	this->addUI(Vect4f(-165.0f, 45.0f, 0.0f), Vect4f(130.0f, 60.0f, 0.0f), "Name:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 45.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), name, UIType::TEXTBOX, *Loop::fonts.back());

	this->addUI(Vect4f(0.0f, -20.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Options  >", UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(0.0f, -85.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Instructions  >", UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(0.0f, -150.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Race  >", UIType::BUTTON, *Loop::fonts.back());


}
