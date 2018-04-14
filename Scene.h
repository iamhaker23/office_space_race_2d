#pragma once

#include "GameObject.h"

#ifndef SCENE_H

#define SCENE_H

class Scene {

private:
	vector<GameObject*> scene;
	
public:
	Scene() {
		this->scene = {};
	}
	
	inline vector<GameObject*> getObjectList() {
		return this->scene;
	}

	inline void addObject(GameObject* obj) {
		scene.push_back(obj);
	}

	inline void freeData() {
		scene.clear();
	}
};


#endif