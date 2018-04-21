#pragma once

#include "LoopManager.h"

#ifndef OFFICE_SPACE_RACE_LOOPMAN_H

#define OFFICE_SPACE_RACE_LOOPMAN_H

class LoopManagerOSR : public LoopManager {

	public:
		
		inline void init(HDC &_hDC, DebugInfo &_debugger, InputStates &inputs) {

			this->loops = {
				new SplashLoop(),
				new MenuLoop(),
				new GameLoop(),
				new FinishLoop()
			};

			initLoops(_hDC, _debugger, inputs);
			
		}

		inline bool isActivePausable() override{
			//In Office Space Racer, only Loop 2, GameLoop, is pausable.
			return this->getActiveLoopIndex() == 2;
		}
		
};

#endif