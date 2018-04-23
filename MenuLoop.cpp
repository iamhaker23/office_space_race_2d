#include "MenuLoop.h"

MenuLoop::MenuLoop() : Loop() {

	this->backgroundPNG = 0;
	this->scene = {};


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
}

void MenuLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());
	double now = debugger->getTime();
	if ((int)this->UI.size() > 0) {
		UIElement* startButton = this->UI.back();
		UIElement* difficultySelector = this->UI.at(3);
		if (startButton->clicked) {
			UIElement* nameInput = this->UI.at(1);
			if (nameInput->value != "") {
				this->globals->playerName = nameInput->value;
				this->globals->difficulty = stoi(difficultySelector->value);
			}
			Loop::requestedActiveLoop = 2;
		}
		else if (difficultySelector->clicked){
			if ((now - difficultySelector->changedAt) > 0.1) {
				difficultySelector->changedAt = now;
				difficultySelector->value = (difficultySelector->value == "1") ? "2" : "1";
			}
		}

	}

}

void MenuLoop::handleActivation() {
	//LoopManager has activated this scene
	this->resetData();

	this->backgroundPNG = Loop::getTexture("resources/images/backgrounds/menu.png");
	

	Loop::addFont("resources/fonts/BKANT.TTF", 20);

	string name = (this->globals->playerName != "") ? this->globals->playerName : "Player";
	int difficulty = (this->globals->difficulty != -1) ? this->globals->difficulty : 1;

	this->addUI(Vect4f(-150.0f, 45.0f, 0.0f), Vect4f(100.0f, 60.0f, 0.0f), "Name:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 45.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), name, UIType::TEXTBOX, *Loop::fonts.back());

	this->addUI(Vect4f(-150.0f, -20.0f, 0.0f), Vect4f(100.0f, 60.0f, 0.0f), "Level:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, -20.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), utils::intToString(difficulty), UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(0.0f, -85.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Start Race", UIType::BUTTON, *Loop::fonts.back());


}
