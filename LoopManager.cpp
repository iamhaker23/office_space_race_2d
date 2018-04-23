#include "LoopManager.h"

LoopManager::LoopManager() {
	this->activeIndex = 0;
	Loop* activeLoop = NULL;
}

LoopManager::~LoopManager() {
	this->freeData();
}


bool LoopManager::isActivePausable() {
	return true;
}

Loop* LoopManager::getActiveLoop() {

	if (!this->isValidLoopIndex(this->activeIndex)) {
		return NULL;
	}
	else {
		this->loadLoop(this->activeIndex);
		if (this->activeLoop != NULL) this->activeLoop->handleActivation();
		return this->activeLoop;
	}
	
}

int LoopManager::getActiveLoopIndex() {
	return this->activeIndex;
}

bool LoopManager::setActiveLoop(int loopIdx) {
	if (isValidLoopIndex(loopIdx)) {
		this->activeIndex = loopIdx;
		return true;
	}
	else return false;
}

bool LoopManager::hasLoopChanged() {

	bool loopChanged = false;
	if (activeLoop != NULL) {
		if (Loop::requestedActiveLoop != -1) {
			loopChanged = setActiveLoop(Loop::requestedActiveLoop);
		}
	}

	return (this->activeLoop == NULL || loopChanged);
}

void LoopManager::init(HDC &_hDC, DebugInfo &_debugger, InputStates &inputs) {
	
	Loop::init(_hDC, _debugger, inputs);
}

void LoopManager::freeData() {
	if (this->activeLoop != NULL) delete this->activeLoop;
	Loop::freeAllStaticData();
	
}