#include "MenuLoop.h"

MenuLoop::MenuLoop() {

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

	fonts.clear();
	font_data* font1 = new font_data();
	font1->init("resources/fonts/HANDA.TTF", 20);
	Loop::fonts.push_back(font1);
	
	/*
	vector<Vertex> planeMesh = {
		Vertex(0.0, 0.0, -0.5, -0.5),
		Vertex(0.0, 1.0, -0.5, 0.5),
		Vertex(1.0, 1.0, 0.5, 0.5),
		Vertex(1.0, 0.0, 0.5, -0.5)
	};

	scene.push_back(ai);
	camera->setCameraTarget(car);
	camera->setCameraSlowParentFactors(0.15f, 0.5f);
	*/
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
}
