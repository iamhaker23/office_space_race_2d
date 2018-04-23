#pragma once

#include "LoopManager.h"

#ifndef OFFICE_SPACE_RACE_LOOPMAN_H

#define OFFICE_SPACE_RACE_LOOPMAN_H

class LoopManagerOSR : public LoopManager {

	public:
		
		int MAX_LOOP_INDEX = 3;

		inline void loadLoop(int index) override {
			
			if (this->activeLoop != NULL) delete this->activeLoop;

			switch (index) {
				case(0):
				{
					this->activeLoop = new SplashLoop();
					break;
				}
				case(1):
				{
					this->activeLoop = new MenuLoop();
					break;
				}
				case(2):
				{
					this->activeLoop = new GameLoop();
					break;
				}
				case(3):
				{
					this->activeLoop = new FinishLoop();
					break;
				}
				default:
				{
					this->activeLoop = NULL;
					break;
				}

			}
			
		}

		inline bool isValidLoopIndex(int index) override {
			return (index >= 0 && index <= MAX_LOOP_INDEX);
		}

		inline bool isActivePausable() override{
			//In Office Space Racer, only Loop 2, GameLoop, is pausable.
			return this->getActiveLoopIndex() == 2;
		}
		
};

#endif