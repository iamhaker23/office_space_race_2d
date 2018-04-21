#include "MenuLoop.h"

MenuLoop::MenuLoop() : Loop() {

	this->backgroundPNG = 0;
	this->scene = {};


}

MenuLoop::~MenuLoop() {
	this->freeData();
}

void MenuLoop::freeData() {
	this->scene.clear();
	utils::freeTexture(this->backgroundPNG);
	Loop::freeData();
}

void MenuLoop::resetData() {
	Loop::resetData(true);
	this->loopStarted = debugger->getTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->backgroundPNG = utils::initTexture(utils::loadPNG("resources/images/backgrounds/menu.png"));
	this->scene = {};
}

void MenuLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());

	if (((int)this->UI.size() > 0 && this->UI.back()->clicked)) {
		if (this->UI.front()->value != "") {
			this->globals->playerName = this->UI.front()->value;
		}
		Loop::requestedActiveLoop = 0;
	}

}

void MenuLoop::handleActivation() {
	//LoopManager has activated this scene
	this->resetData();

	font_data* font1 = new font_data();
	font1->init("resources/fonts/BKANT.TTF", 20);
	Loop::fonts.push_back(font1);

	string name = (this->globals->playerName != "") ? this->globals->playerName : "Player";

	this->addUI(Vect4f(0.0f, 80.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), name, UIType::TEXTBOX, *Loop::fonts.back());
	this->addUI(Vect4f(0.0f, -80.0f, 0.0f), Vect4f(200.0f, 60.0f, 0.0f), "Start Race", UIType::BUTTON, *Loop::fonts.back());


}
