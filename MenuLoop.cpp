#include "MenuLoop.h"

MenuLoop::MenuLoop() : Loop() {

	//reset camera
	delete Loop::camera;
	Loop::camera = new Camera();

	this->backgroundPNG = utils::initTexture(utils::loadPNG("resources/images/backgrounds/menu.png"));
	this->scene = {};

}

MenuLoop::~MenuLoop() {
	this->freeData();
}

void MenuLoop::freeData() {
	Loop::freeData();
	Loop::freeStaticData();
}

void MenuLoop::init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs)
{
	Loop::init(_hDC, _debugger, inputs);
	GameObject::setDebugger(_debugger);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	
}

void MenuLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());

	if (this->UI.back()->clicked) {
		if (this->UI.front()->value != "") {
			this->globals->playerName = this->UI.front()->value;
		}
		Loop::requestedActiveLoop = 2;
	}

}

void MenuLoop::handleActivation() {
	//LoopManager has activated this scene
	//MenuLoop is simple and does not require any housekeeping

	Loop::freeData();

	fonts.clear();
	font_data* font1 = new font_data();
	font1->init("resources/fonts/BKANT.TTF", 20);
	Loop::fonts.push_back(font1);

	this->addUI(new Vect4f(0.0f, 80.0f, 0.0f), new Vect4f(200.0f, 60.0f, 0.0f), "", UIType::TEXTBOX, Loop::fonts.back());
	this->addUI(new Vect4f(0.0f, -80.0f, 0.0f), new Vect4f(200.0f, 60.0f, 0.0f), "Start Race", UIType::BUTTON, Loop::fonts.back());
}
