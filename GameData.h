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
	string pushee;
	bool reverse;
	int track;
	int collisionResolution;

	inline GameData() {
		this->player = NULL;
		this->others = {};
		this->playerName = "Player";
		this->updated = false;
		this->difficulty = 0;
		this->laps = 2;
		this->ai = 3;
		this->pusher = "Suzanne";
		this->pushee = "Office Dog";
		this->track = 1;
		this->reverse = false;
		this->collisionResolution = 2;
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
		this->pusher = copy.pusher;
		this->pushee = copy.pusher;
		this->track = copy.track;
		this->reverse = copy.reverse;
	}
};


#endif