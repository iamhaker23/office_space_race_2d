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

	GameData() {
		this->player = NULL;
		this->others = {};
		this->playerName = "Player";
		this->updated = false;
	}
};


#endif