#pragma once

#include "Scene.h"
#include "GO_Racer.h"

#ifndef RACE_SCENE_H

#define RACE_SCENE_H

class RaceScene : public Scene {
private:
	GameObject* track;
	GO_Racer* player;
	vector<GO_Racer*> racers;

public:

	RaceScene() : Scene() {
		this->racers = {};
		this->player = NULL;
		this->track = NULL;
	}

	inline void setTrack(GameObject* track) {
		addObject(track);
		this->track = track;
	}

	inline void setPlayer(GO_Racer* player) {
		addCompetitor(player);
		this->player = player;
	}

	inline void addCompetitor(GO_Racer* competitor) {
		addObject(competitor);
		this->racers.push_back(competitor);
	}

	inline GameObject* getTrack() {
		return this->track;
	}

	inline GO_Racer* getPlayer() {
		return this->player;
	}

	inline vector<GO_Racer*> getRacers() {
		return this->racers;
	}


};

#endif