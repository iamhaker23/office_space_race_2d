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
		double startTime;
		bool renderedFirstFrame;

		vector<CollisionRadii> GameLoop::generateTrackBounds(char* filename);
		void initGame();

		static vector<GO_Racer> generateRacers(int numAI, Vect4f &startPos); 
		static GO_Racer generatePlayer(string &name);
		static vector<GameObject> generateObjects();
		static vector<GameObject> generateDesks();
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
