#pragma once
#include "RaceData.h"
#include "kgeUtils.h"

#ifndef GAMEDATA_H


#define GAMEDATA_H
using std::string;

struct GameData {

public:
	string playerName;
	RaceData* player;
	vector<RaceData*> others;
	bool updated;

	inline GameData() {
		this->player = NULL;
		this->others = {};
		this->playerName = "Player";
		this->updated = false;
	}

	inline void freeData() {
		for (RaceData* o : others) {
			if (o != NULL) delete o;
		}
		this->others.clear();
		if (player != NULL) delete this->player;
	}

	inline ~GameData() {
		freeData();
	}

	inline GameData(const GameData &copy) {
		this->player = copy.player;
		this->others = copy.others;
		this->playerName = copy.playerName;
		this->updated = copy.updated;
	}
};


#endif