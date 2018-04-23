#pragma once

#include "GameLoop.h"
#include "MenuLoop.h"
#include "SplashLoop.h"
#include "FinishLoop.h"

#ifndef LOOPMAN_H

#define LOOPMAN_H

class LoopManager {

	protected:
		Loop* activeLoop;
		int activeIndex;
	
	public:
		LoopManager();
		~LoopManager();
				
		int getActiveLoopIndex();
		Loop* getActiveLoop();
		bool setActiveLoop(int loopIdx);

		bool hasLoopChanged();

		virtual bool isActivePausable();
		virtual void loadLoop(int index) = 0;
		virtual bool isValidLoopIndex(int index) = 0;
		void init(HDC &_hDC, DebugInfo &_debugger, InputStates &inputs);
		

		void freeData();
};

#endif