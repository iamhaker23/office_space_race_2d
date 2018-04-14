#include "MenuLoop.h"

MenuLoop::MenuLoop() : Loop() {

	//reset camera
	delete Loop::camera;
	Loop::camera = new Camera();

	this->backgroundPNG = utils::loadPNG("resources/images/backgrounds/osr.png");
	this->scene = {};

}

MenuLoop::~MenuLoop() {
	this->freeData();
}

void MenuLoop::freeData() {

	Loop::freeStaticData();
	delete this->backgroundPNG;
}

void MenuLoop::init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs)
{
	hDC = _hDC;
	debugger = _debugger;

	GameObject::setDebugger(_debugger);

	Loop::inputs = inputs;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	
}

void MenuLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 1, Color4f());

	if (inputs->keys[VK_RETURN]) {
		Loop::requestedActiveLoop = 1;
	}
	else {
		writeMessage("Press enter to start.", 50.0f, 250.0f);
	}

}

void MenuLoop::handleActivation() {
	//LoopManager has activated this scene
	//MenuLoop is simple and does not require any housekeeping


	fonts.clear();
	font_data* font1 = new font_data();
	font1->init("resources/fonts/BKANT.TTF", 20);
	Loop::fonts.push_back(font1);

}
