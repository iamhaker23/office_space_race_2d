#include "LoopManager.h"

LoopManager::LoopManager() {
	this->loops = {};
	this->active = 0;
	this->loopChanged = true;
}
LoopManager::~LoopManager() {
	this->freeData();
}


bool LoopManager::isActivePausable() {
	return true;
}

Loop* LoopManager::getActiveLoop() {

	if (this->active >= (int)this->loops.size() || this->active < 0) {
		return NULL;
	}
	else {
		if (this->loopChanged) {
			int counter = 0;
			for (Loop* loop : this->loops) {
				if (counter++ == this->active) {
					loop->handleActivation();
				}
				else {
					loop->freeData();
				}
			}
		}
		this->loopChanged = false;
		return this->loops.at(this->active);
	}
	
}

int LoopManager::getActiveLoopIndex() {
	return this->active;
}

void LoopManager::setActiveLoop(int loopIdx) {
	if (loopIdx < (int)this->loops.size() && loopIdx >= 0) {
		this->active = loopIdx;
		this->loopChanged = true;
	}
}

bool LoopManager::hasLoopChanged() {

	if (this->active >= 0 && this->active < (int) this->loops.size()){
		Loop* loop = this->loops.at(this->active);
		int requestedActiveLoop = loop->requestedActiveLoop;
		if (requestedActiveLoop != -1) {
			setActiveLoop(requestedActiveLoop);

			//reset old loop request in case we return to it
			//prevent instantaneous forwarding to another loop
			loop->requestedActiveLoop = -1;
		}
	}

	return this->loopChanged;
}

void LoopManager::initLoops(HDC &_hDC, DebugInfo &_debugger, InputStates &inputs) {
	Loop::init(_hDC, _debugger, inputs);
}

void LoopManager::freeData() {
	for (Loop* l : loops) {
		delete l;
	}
	this->loops.clear();
	Loop::freeAllStaticData();
	
}