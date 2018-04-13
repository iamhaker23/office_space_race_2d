#pragma once

#include "GameLoop.h"
#include "MenuLoop.h"

#ifndef LOOPMAN_H

#define LOOPMAN_H

class LoopManager {

	private:
		vector<Loop*> loops;
		int active;
		bool loopChanged;
	
	public:
		LoopManager();
		~LoopManager();

		void setLoops(vector<Loop*> loops);
		
		Loop* getActiveLoop();
		void setActiveLoop(int loopIdx);

		bool hasLoopChanged();

		virtual void init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs) = 0;
		void initLoops(HDC _hDC, DebugInfo* _debugger, InputStates* inputs);
		

		void freeData();
};

#endif