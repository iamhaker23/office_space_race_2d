#pragma once

#include "GameLoop.h"
#include "MenuLoop.h"
#include "SplashLoop.h"
#include "FinishLoop.h"

#ifndef LOOPMAN_H

#define LOOPMAN_H

class LoopManager {

	protected:
		vector<Loop*> loops;
		int active;
		bool loopChanged;
	
	public:
		LoopManager();
		~LoopManager();
				
		int getActiveLoopIndex();
		Loop* getActiveLoop();
		void setActiveLoop(int loopIdx);

		bool hasLoopChanged();

		virtual void init(HDC &_hDC, DebugInfo &_debugger, InputStates &inputs) = 0;
		virtual bool isActivePausable();
		void initLoops(HDC &_hDC, DebugInfo &_debugger, InputStates &inputs);
		

		void freeData();
};

#endif