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
	int difficulty;
	int laps;
	int ai;
	string pusher;

	inline GameData() {
		this->player = NULL;
		this->others = {};
		this->playerName = "Player";
		this->updated = false;
		this->difficulty = -1;
		this->laps = -1;
		this->ai = -1;
		this->pusher = "";
	}

	inline void freeData() {
		this->others.clear();
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