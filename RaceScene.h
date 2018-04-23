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

	inline void addCompetitor(GO_Racer* competitor) {
		addObject(competitor);
		racers.push_back(competitor);
	}

public:

	RaceScene() : Scene() {
		this->racers = {};
		this->player = NULL;
		this->track = NULL;
	}

	inline void setTrack(GameObject &track) {
		GameObject* ntrack = new GameObject(track);
		addObject(ntrack);
		this->track = ntrack;
	}

	inline void setPlayer(GO_Racer &player) {
		GO_Racer* nplayer = new GO_Racer(player);
		addCompetitor(nplayer);
		this->player = nplayer;
	}

	inline void addCompetitor(GO_Racer &competitor) {
		GO_Racer* ncompetitor = new GO_Racer(competitor);
		addObject(ncompetitor);
		this->racers.push_back(ncompetitor);
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

	inline void freeData() override {
		for (GameObject* o : this->scene) {
			if (o != NULL) {
				delete o;
			}
		}
		this->scene.clear();
		this->racers.clear();
	}

	~RaceScene() {
		this->freeData();
	}

};

#endif