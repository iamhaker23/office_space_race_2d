#pragma once

#include "Loop.h"

#ifndef GAMELOOP_H

	#define GAMELOOP_H

	class GameLoop : public Loop {
		

	private:
		vector<GameObject*> scene;
		nv::Image* backgroundPNG;

		bool finished;
		double startTimeOutClock;

		static vector<CollisionRadii*> generateTrackBounds(char* filename);
		void initGame();
			
	public:
		
		GameLoop();
		~GameLoop();

		void init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs);
		void display();
		void freeData();
		void handleActivation();
		
	};

#endif
