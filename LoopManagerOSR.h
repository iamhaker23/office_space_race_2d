#pragma once

#include "LoopManager.h"

#ifndef OFFICE_SPACE_RACE_LOOPMAN_H

#define OFFICE_SPACE_RACE_LOOPMAN_H

class LoopManagerOSR : public LoopManager {

	public:

		inline void init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs) {

			setLoops({
				new MenuLoop(),
				new GameLoop()
			});

			initLoops(_hDC, _debugger, inputs);
			
		}
		
};

#endif