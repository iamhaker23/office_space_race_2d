#pragma once

#include "GameObject.h"

#ifndef SCENE_H

#define SCENE_H

class Scene {

protected:
	vector<GameObject*> scene;
	inline void addObject(GameObject* obj) {
		scene.push_back(obj);
	}
	
public:
	Scene() {
		this->scene = {};
	}
	
	inline vector<GameObject*> getObjectList() {
		return this->scene;
	}

	inline void addObject(GameObject &obj) {
		this->addObject(new GameObject(obj));
	}

	inline virtual void freeData() = 0;
};


#endif