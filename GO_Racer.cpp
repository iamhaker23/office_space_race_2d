#include "GO_Racer.h"

GO_Racer::~GO_Racer() {

}

GO_Racer::GO_Racer() : GameObject() {

	steering = true;
	playerControl = false;
	aiControl = false;
	racer = true;
	raceData = new RaceData();

}

GO_Racer::GO_Racer(const GO_Racer &copy) : GameObject(copy) {
	
	this->aiControl = copy.aiControl;
	this->playerControl = copy.playerControl;
	this->raceData = copy.raceData;
	this->racer = copy.racer;
	this->steering = copy.steering;

}

GO_Racer::GO_Racer(string name, vector<nv::Image*> sprites, vector<Vertex> mesh, int activeSprite, Color4f* objectColor) : GameObject(name, sprites, mesh, activeSprite, objectColor) {

	steering = true;
	playerControl = false;
	aiControl = false;
	racer = true;
	raceData = new RaceData();

}

void GO_Racer::setAIControl(bool flag) {
	this->aiControl = flag;
}

void GO_Racer::doAIControl(GameObject* track, int trackStep) {

	if (!this->aiControl) return;

	Vect4f* closestTo = this->localToWorldSpace(Vect4f(0.0f, 0.5f, 0.0f));

	if (GameObject::drawDebug) {
		drawLine(this->getWorldPosition()->getX(), this->getWorldPosition()->getY(), closestTo->getX(), closestTo->getY());
	}

	CollisionRadii* target = track->getNextCollisionRadiiFor(closestTo, trackStep);
	Vect4f* targetWoCo = (track->boundSpaceToObjectSpace(Vect4f(target->centreX, target->centreY, 0.0f)));

	float distX = (targetWoCo->x - this->getWorldPosition()->x);
	float distY = (targetWoCo->y - this->getWorldPosition()->y);
	float distSqrd = (distX*distX) + (distY*distY);
	float angle = (this->getAngleToPosition(new Vect4f(targetWoCo->x, targetWoCo->y, 0.0f)) - (this->getAngleFromX() + 90.0f));

	if (angle > 180.0f) angle -= 360.0f;
	if (angle < -180.0f) angle += 360.0f;

	if (abs(angle) > 0.02f || (distSqrd) > 0.001f) {

		this->zTorque += 0.02f*angle;
		this->addForce(0.00f, 0.09f, 0.0f);
	}

}

bool GO_Racer::isAI() {
	return this->aiControl;
}

bool GO_Racer::isRacer() {
	return (this->aiControl || this->playerControl);
}

void GO_Racer::draw()  {

	//Why does this syntactically look like a static call? :(
	GameObject::draw();

	if (this->playerControl) {
		float speed = sqrtf((this->forces[0] * this->forces[0]) + (this->forces[1] * this->forces[1])) * 10.0f;
		GameObject::debugger->addMessage(utils::floatLabel("Speed: ", speed, "m/s"));

	}
}

RaceData* GO_Racer::getRaceData() {
	return this->raceData;
}

void GO_Racer::setPlayerControl(bool playerControl) {
	this->playerControl = playerControl;
}


void GO_Racer::processInputs(InputStates* inputs) {

	if (playerControl) {

		GameObject::setDebugMode(inputs->keys[VK_UP]);

		//if the framerate is over 25, scale the force down
		//TODO: implement fixed-frequency tics
		//float factor = (GameObject::debugger != NULL && GameObject::debugger->getAverageFrameRate() > 25.0f) ? ((float)(1.0 / (GameObject::debugger->getAverageFrameRate()/100.0f))) : 1.0f;
		float factor = 1.0f;



		int sprite = 0;

		if (this->steering) {
			if (inputs->keys[0x53]) {
				//S Key
				if (this->forces[1] > -0.5f*this->topSpeed) {

					/*if ((this->oldForces[1] - (this->forces[1] - 0.02f)) > 0.05f) {
					this->oldForces[1] = this->forces[1];
					}*/
					this->forces[1] -= 0.02f * factor;
				}
			}
			if (inputs->keys[0x57]) {
				//W Key
				if (this->forces[1] < this->topSpeed) {

					/*if ((this->oldForces[1] - (this->forces[1] - 0.02f)) > 0.05f) {
					this->oldForces[1] = this->forces[1];
					}*/
					this->forces[1] += 0.03f * factor;
				}

			}
			bool backwards = this->forces[1] < 0.0f;

			if (inputs->keys[0x41]) {
				//A Key
				sprite = 1;
				if (this->forces[1] != 0.0f) {
					this->zTorque -= ((backwards) ? 1.0f : -1.0f) * 1.1f * factor;
				}
			}

			if (inputs->keys[0x44]) {
				//D Key
				sprite = 2;
				if (this->forces[1] != 0.0f) {
					this->zTorque += ((backwards) ? -1.0f : 1.0f) * -1.1f * factor;
				}
			}
		}

		//Animations after auto-animation
		if (inputs->LeftPressed) {
			this->animate(AnimationLogic::PINGPONG);
		}
		else {
			this->setSprite(sprite);
		}
	}
}

vector<CollisionResult> GO_Racer::resolveCollision(vector<GameObject*> objs) {
	vector<CollisionResult> results = GameObject::resolveCollisions(objs);

	//as long as we are far away enough from collision, allow steering again
	if (!this->collided) {
		bool steering = true;
		for (CollisionResult r : results) {
			if (r.distanceSqrd < 0.5f) {
				steering = false;
			}
		}
		this->steering = steering;
	}
	else {
		this->steering = false;
	}

	return results;
}