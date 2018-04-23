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

		double BUTTON_DELAY = 0.15;

		UIElement* startButton = this->UI.back();
		UIElement* difficultySelector = this->UI.at(3);
		UIElement* lapSelector = this->UI.at(5);
		UIElement* aiSelector = this->UI.at(7);
		UIElement* pusherSelector = this->UI.at(9);

		if (startButton->clicked) {
			UIElement* nameInput = this->UI.at(1);
			if (nameInput->value != "") {
				this->globals->playerName = nameInput->value;
				this->globals->difficulty = stoi(difficultySelector->value);
				this->globals->laps = stoi(lapSelector->value);
				this->globals->ai = stoi(aiSelector->value);
				this->globals->pusher = pusherSelector->value;
			}
			Loop::requestedActiveLoop = 2;
		}
		else if (difficultySelector->clicked) {
			if ((now - difficultySelector->changedAt) > BUTTON_DELAY) {
				difficultySelector->changedAt = now;
				difficultySelector->value = (difficultySelector->value == "2") ? "0" : utils::intToString(stoi(difficultySelector->value) + 1);
			}
		}
		else if (lapSelector->clicked) {
			if ((now - lapSelector->changedAt) > BUTTON_DELAY) {
				lapSelector->changedAt = now;
				lapSelector->value = (lapSelector->value == "5") ? "1" : utils::intToString(stoi(lapSelector->value) + 1);
			}
		}
		else if (aiSelector->clicked) {
			if ((now - aiSelector->changedAt) > BUTTON_DELAY) {
				aiSelector->changedAt = now;
				aiSelector->value = (aiSelector->value == "10") ? "1" : utils::intToString(stoi(aiSelector->value) + 1);
			}
		}
		else if (pusherSelector->clicked) {
			if ((now - pusherSelector->changedAt) > BUTTON_DELAY) {
				pusherSelector->changedAt = now;
				if (pusherSelector->value == "Carlos") pusherSelector->value = "Barry";
				else if (pusherSelector->value == "Barry") pusherSelector->value = "Margaret";
				else pusherSelector->value = "Carlos";
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
	int laps = (this->globals->laps != -1) ? this->globals->laps : 2;
	int ai = (this->globals->ai != -1) ? this->globals->ai : 3;
	string pusher = (this->globals->pusher != "") ? this->globals->pusher : "Carlos";

	this->addUI(Vect4f(-150.0f, 175.0f, 0.0f), Vect4f(100.0f, 60.0f, 0.0f), "Name:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 175.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), name, UIType::TEXTBOX, *Loop::fonts.back());

	this->addUI(Vect4f(-150.0f, 110.0f, 0.0f), Vect4f(100.0f, 60.0f, 0.0f), "Level:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 110.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), utils::intToString(difficulty), UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-150.0f, 45.0f, 0.0f), Vect4f(100.0f, 60.0f, 0.0f), "Laps:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 45.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), utils::intToString(laps), UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-150.0f, -20.0f, 0.0f), Vect4f(100.0f, 60.0f, 0.0f), "# CPU:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, -20.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), utils::intToString(ai), UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-150.0f, -85.0f, 0.0f), Vect4f(100.0f, 60.0f, 0.0f), "Pusher:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, -85.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), pusher, UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(0.0f, -150.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Race", UIType::BUTTON, *Loop::fonts.back());


}
