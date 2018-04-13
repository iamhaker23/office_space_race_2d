#pragma once

#include "Loop.h"

#ifndef GAMELOOP_H

	#define GAMELOOP_H

	class GameLoop : public Loop {
		

	private:
		vector<GameObject*> scene;
		static vector<CollisionRadii*> generateTrackBounds(char* filename);
		nv::Image* backgroundPNG;
			
	public:
		
		GameLoop();
		~GameLoop();

		void init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs);
		void display();
		void freeData();


	};

#endif
