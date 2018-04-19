#include "FinishLoop.h"

FinishLoop::FinishLoop() : Loop() {

	//reset camera
	delete Loop::camera;
	Loop::camera = new Camera();

	this->backgroundPNG = utils::initTexture(utils::loadPNG("resources/images/backgrounds/finished.png"));
	this->scene = {};

}

FinishLoop::~FinishLoop() {
	this->freeData();
}

void FinishLoop::freeData() {
	Loop::freeData();
	Loop::freeStaticData();
}

void FinishLoop::init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs)
{
	Loop::init(_hDC, _debugger, inputs);
	GameObject::setDebugger(_debugger);

	glClearColor(0.0, 0.0, 0.0, 0.0);

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

		if (this->globals->updated) {
			this->globals->updated = false;
			RaceData* r = this->globals->player;
			string stats = r->getName().append(" - ").append(utils::getPositionLabel(r->getPosition()));
			this->addUI(new Vect4f(0.0f, 100.0f, 0.0f), new Vect4f(200.0f, 60.0f, 0.0f), stats, UIType::LABEL, Loop::fonts.back());
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
	//MenuLoop is simple and does not require any housekeeping
	fonts.clear();
	font_data* font1 = new font_data();
	font1->init("resources/fonts/BKANT.TTF", 20);
	Loop::fonts.push_back(font1);

	Loop::freeData();

	this->addUI(new Vect4f(150.0f, 0.0f, 0.0f), new Vect4f(200.0f, 60.0f, 0.0f), "Main Menu", UIType::BUTTON, Loop::fonts.back());
}
