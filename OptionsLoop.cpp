#include "OptionsLoop.h"

OptionsLoop::OptionsLoop() : Loop() {

	this->backgroundPNG = 0;
	this->scene = {};
	this->startTime = 0.0;

}

OptionsLoop::~OptionsLoop() {
	this->freeData();
}

void OptionsLoop::freeData() {
	for (GameObject* o : scene) {
		delete o;
	}
	this->scene.clear();
	//utils::freeTexture(this->backgroundPNG);
	Loop::resetData(true);
}

void OptionsLoop::resetData() {

	
	Loop::resetData(true);
	freeData();
	this->loopStarted = debugger->getTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->scene = {};
	if (this->startTime == 0.0 && debugger != NULL) this->startTime = debugger->getTime();

}

void OptionsLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());
	double now = debugger->getTime();
	double timeSinceStarted = debugger->getTime() - startTime;

	if ((int)this->UI.size() > 0 && timeSinceStarted > 0.3) {

		double BUTTON_DELAY = 0.15;

		UIElement* menuButton = this->UI.back();
		UIElement* difficultySelector = this->UI.at(1);
		UIElement* lapSelector = this->UI.at(3);
		UIElement* aiSelector = this->UI.at(5);
		UIElement* pusherSelector = this->UI.at(7);
		UIElement* pusheeSelector = this->UI.at(9);
		UIElement* trackSelector = this->UI.at(11);
		UIElement* reverseSelector = this->UI.at(13);
		UIElement* collResSelector = this->UI.at(15);

		if (menuButton->clicked) {
			this->globals->difficulty = stoi(difficultySelector->value);
			this->globals->laps = stoi(lapSelector->value);
			this->globals->ai = stoi(aiSelector->value);
			this->globals->pusher = pusherSelector->value;
			this->globals->pushee = pusheeSelector->value;
			this->globals->track = stoi(trackSelector->value);
			this->globals->reverse = (reverseSelector->value == "Yes");
			this->globals->collisionResolution = stoi(collResSelector->value);
			Loop::requestedActiveLoop = 1;
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
				if (pusherSelector->value == "Barry[OLD]") pusherSelector->value = "Barry";
				else if (pusherSelector->value == "Barry") pusherSelector->value = "Suzanne";
				else pusherSelector->value = "Barry[OLD]";
			}
		}
		else if (pusheeSelector->clicked) {
			if ((now - pusheeSelector->changedAt) > BUTTON_DELAY) {
				pusheeSelector->changedAt = now;
				if (pusheeSelector->value == "Office Dog") pusheeSelector->value = "IT Guy";
				else if (pusheeSelector->value == "IT Guy") pusheeSelector->value = "Sally";
				else pusheeSelector->value = "Office Dog";
			}
		}
		else if (trackSelector->clicked) {
			if ((now - trackSelector->changedAt) > BUTTON_DELAY) {
				trackSelector->changedAt = now;
				trackSelector->value = (trackSelector->value == "1") ? "2" : "1";
			}
		}
		else if (reverseSelector->clicked) {
			if ((now - reverseSelector->changedAt) > BUTTON_DELAY) {
				reverseSelector->changedAt = now;
				reverseSelector->value = (reverseSelector->value == "Yes") ? "No" : "Yes";
			}
		}
		else if (collResSelector->clicked) {
			if ((now - collResSelector->changedAt) > BUTTON_DELAY) {
				collResSelector->changedAt = now;
				collResSelector->value = (collResSelector->value == "5") ? "1" : utils::intToString(stoi(collResSelector->value) + 1);
			}
		}

	}

}

void OptionsLoop::handleActivation() {
	//LoopManager has activated this scene
	this->resetData();

	this->backgroundPNG = Loop::getTexture("resources/images/backgrounds/presentation.png");
	

	Loop::addFont("resources/fonts/PIXEL.TTF", 25);

	int difficulty = (this->globals->difficulty != -1) ? this->globals->difficulty : 1;
	int laps = (this->globals->laps != -1) ? this->globals->laps : 2;
	int ai = (this->globals->ai != -1) ? this->globals->ai : 3;
	int collRes = this->globals->collisionResolution;
	string pusher = (this->globals->pusher != "") ? this->globals->pusher : "Barry[OLD]";
	string pushee = (this->globals->pushee != "") ? this->globals->pushee : "Office Dog";
	
	string track = (this->globals->track != -1) ? utils::intToString(this->globals->track) : "1";
	string reverse = (this->globals->reverse) ? "Yes" : "No";
	
	this->addUI(Vect4f(-165.0f, 260.0f, 0.0f), Vect4f(130.0f, 60.0f, 0.0f), "Difficulty:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 260.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), utils::intToString(difficulty), UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-165.0f, 195.0f, 0.0f), Vect4f(130.0f, 60.0f, 0.0f), "Laps:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 195.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), utils::intToString(laps), UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-165.0f, 130.0f, 0.0f), Vect4f(130.0f, 60.0f, 0.0f), "# CPU:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 130.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), utils::intToString(ai), UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-165.0f, 65.0f, 0.0f), Vect4f(130.0f, 60.0f, 0.0f), "Pusher:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 65.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), pusher, UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-165.0f, 0.0f, 0.0f), Vect4f(130.0f, 60.0f, 0.0f), "Pushee:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, 0.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), pushee, UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-165.0f, -65.0f, 0.0f), Vect4f(130.0f, 60.0f, 0.0f), "Track:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, -65.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), track, UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-165.0f, -130.0f, 0.0f), Vect4f(130.0f, 60.0f, 0.0f), "Reverse:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, -130.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), reverse, UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(-165.0f, -195.0f, 0.0f), Vect4f(130.0f, 60.0f, 0.0f), "Coll. Res.:", UIType::LABEL, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, -195.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), utils::intToString(collRes), UIType::BUTTON, *Loop::fonts.back());

	this->addUI(Vect4f(0.0f, -260.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Menu >", UIType::BUTTON, *Loop::fonts.back());


}
