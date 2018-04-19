#pragma once

#include "Loop.h"
#include "RaceScene.h"

#ifndef GAMELOOP_H

	#define GAMELOOP_H

	class GameLoop : public Loop {
		

	private:
		RaceScene* scene;
		GLuint backgroundPNG;

		bool finished;
		double startTimeOutClock;

		static vector<CollisionRadii*> generateTrackBounds(char* filename);
		void initGame();

		static vector<GO_Racer*> generateRacers(int numAI); 
		static GO_Racer* generatePlayer();
		static vector<GameObject*> generateObjects();
		static vector<Vertex> generatePlaneMesh();
			
	public:
		
		GameLoop();
		~GameLoop();

		void init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs);
		void display();
		void freeData();
		void handleActivation();
		
	};

#endif
