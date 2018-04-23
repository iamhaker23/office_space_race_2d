#pragma once

#include "Loop.h"
#include "RaceScene.h"

#ifndef GAMELOOP_H

	#define GAMELOOP_H

	class GameLoop : public Loop {
		

	private:
		RaceScene scene;
		vector<GLuint> backgrounds;
		vector<GLuint> sprites;

		bool finished;
		double startTimeOutClock;

		vector<CollisionRadii> GameLoop::generateTrackBounds(char* filename);
		void initGame();

		static vector<GO_Racer> generateRacers(int numAI); 
		static GO_Racer generatePlayer(string &name);
		static vector<GameObject> generateObjects();
		static vector<Vertex> generatePlaneMesh();
			
	public:
		
		GameLoop();
		~GameLoop();

		void display();
		void freeData() override;
		void resetData();
		void handleActivation();
		
	};

#endif
