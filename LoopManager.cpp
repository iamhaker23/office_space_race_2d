#include "LoopManager.h"

LoopManager::LoopManager() {
	this->loops = {};
	this->active = 0;
	this->loopChanged = true;
}
LoopManager::~LoopManager() {
	this->loops.clear();
}

void LoopManager::setLoops(vector<Loop*> loops) {
	this->loops = loops;
}

bool LoopManager::isActivePausable() {
	return true;
}

Loop* LoopManager::getActiveLoop() {
	//handle activation in the call that will get the
	if (this->loopChanged) this->loops.at(this->active)->handleActivation();
	
	this->loopChanged = false;
	if (this->active >= (int)this->loops.size()) {
		return NULL;
	}
	return this->loops.at(this->active);
}

int LoopManager::getActiveLoopIndex() {
	return this->active;
}

void LoopManager::setActiveLoop(int loopIdx) {
	if (loopIdx < (int)this->loops.size()) {
		this->active = loopIdx;
		this->loopChanged = true;
	}
}

bool LoopManager::hasLoopChanged() {
	return this->loopChanged;
}

void LoopManager::initLoops(HDC _hDC, DebugInfo* _debugger, InputStates* inputs) {
	for (Loop* loop : this->loops) {
		loop->init(_hDC, _debugger, inputs);
	}
}

void LoopManager::freeData() {
	for (Loop* l : this->loops) {
		l->freeData();
	}
	this->loops.clear();
}