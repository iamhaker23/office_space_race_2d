#include "GO_Racer.h"

GO_Racer::~GO_Racer() {
	this->freeData();
}

void GO_Racer::freeData() {
	GameObject::freeData();
	delete this->raceData;
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
	this->raceData = new RaceData(*copy.raceData);
	this->racer = copy.racer;
	this->steering = copy.steering;
	this->loseSteeringOnCollision = copy.loseSteeringOnCollision;
}

GO_Racer::GO_Racer(string name, vector<GLuint> &sprites, vector<Vertex> &mesh, int activeSprite, Color4f &objectColor, vector<GLuint> chairSprites, bool loseSteeringOnCollision) : GameObject(name, sprites, mesh, activeSprite, objectColor) {

	this->loseSteeringOnCollision = loseSteeringOnCollision;
	steering = true;
	playerControl = false;
	aiControl = false;
	racer = true;
	raceData = new RaceData();
	raceData->setName(name);

	vector<Vertex> planeMesh = {
		Vertex(0.0, 0.0, -0.5, -0.5),
		Vertex(0.0, 1.0, -0.5, 0.5),
		Vertex(1.0, 1.0, 0.5, 0.5),
		Vertex(1.0, 0.0, 0.5, -0.5)
	};

	
	GameObject chair = GameObject(name.append("_CHAIR"), chairSprites, planeMesh, 0, Color4f(1.0f, 1.0f, 1.0f, 1.0f));
	chair.translate(0.0f, 0.7f, 0.0f);
	this->addChild(chair);

}

void GO_Racer::setAIControl(bool flag) {
	this->aiControl = flag;
}

void GO_Racer::doAIControl(GameObject* track, int trackStep) {

	if (!this->aiControl) return;

	Vect4f closestTo = this->localToWorldSpace(Vect4f(0.0f, 0.5f, 0.0f));

	if (GameObject::drawDebug) {
		drawLine(this->getWorldPosition().getX(), this->getWorldPosition().getY(), closestTo.getX(), closestTo.getY());
	}

	//NOTE: current segment is absolute (does not account for track direction) 
	//so does not map to track CollisionRadii indices without accounting for track direction

	int currentSegment = this->raceData->getCurrentSegment();
	if (trackStep < 0) currentSegment *= -1;

	int targetIdx = track->getNextIndexStep(currentSegment, trackStep);

	CollisionRadii* target = track->getRadiiAt(targetIdx);

	Vect4f targetWoCo = track->boundSpaceToObjectSpace(Vect4f(target->centreX, target->centreY, 0.0f));

	float distX = (targetWoCo.x - this->getWorldPosition().x);
	float distY = (targetWoCo.y - this->getWorldPosition().y);
	float distSqrd = (distX*distX) + (distY*distY);
	float angle = (this->getAngleToPosition(Vect4f(targetWoCo.x, targetWoCo.y, 0.0f)) - (this->getAngleFromX() + 90.0f));

	if (angle > 180.0f) angle -= 360.0f;
	if (angle < -180.0f) angle += 360.0f;

	if (abs(angle) > 0.02f || (distSqrd) > 0.001f) {
		this->zTorque += ((this->steering)?0.02f:0.005f)*angle;

		int sprite = (angle > 0.0f) ? 1 : 2;
		sprite = (angle == 0.0f) ? 0 : sprite;
		if ((int)this->children.size() > 0) this->children.at(0).setSprite(sprite);

		this->addForce(0.00f, ((this->steering)?0.09f:0.05f), 0.0f);
		this->animate(AnimationLogic::LOOPEND);
	}
	
}

bool GO_Racer::isAI() {
	return this->aiControl;
}

bool GO_Racer::isRacer() {
	return (this->aiControl || this->playerControl);
}

void GO_Racer::draw() {

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

		if (inputs->keys[0x53]) {
			//S Key
			if (this->forces[1] > -0.5f*this->topSpeed) {
				this->animate(AnimationLogic::LOOPEND);
				/*if ((this->oldForces[1] - (this->forces[1] - 0.02f)) > 0.05f) {
				this->oldForces[1] = this->forces[1];
				}*/
				this->forces[1] -= 0.02f * factor;
			}
		}
		if (inputs->keys[0x57]) {
			//W Key
			if (this->forces[1] < this->topSpeed) {
				this->animate(AnimationLogic::LOOPEND);
				/*if ((this->oldForces[1] - (this->forces[1] - 0.02f)) > 0.05f) {
				this->oldForces[1] = this->forces[1];
				}*/
				this->forces[1] += 0.03f * factor;
			}

		}
		bool backwards = this->forces[1] < 0.0f;
		if (this->steering) {
			

			if (inputs->keys[0x41]) {
				//A Key
				if (this->forces[1] != 0.0f) {
					sprite = 1;
					this->zTorque -= ((backwards) ? 1.0f : -1.0f) * 1.1f * factor;
				}
			}

			if (inputs->keys[0x44]) {
				//D Key
				if (this->forces[1] != 0.0f) {
					sprite = 2;
					this->zTorque += ((backwards) ? -1.0f : 1.0f) * -1.1f * factor;
				}
			}
		}

		if ((int)this->children.size() > 0) this->children.at(0).setSprite(sprite);
	}
}

vector<CollisionResult> GO_Racer::resolveCollisions(const vector<GameObject*> &objs) {
	
	vector<CollisionResult> results = GameObject::resolveCollisions(objs);

	//as long as we are far away enough from collision, allow steering again
	if (!this->collided) {
		bool steering = true;
		for (CollisionResult r : results) {
			if (r.distanceSqrd < 0.05f) {
				steering = false;
				GO_Racer* other = dynamic_cast<GO_Racer*>(r.other);
				if (other != NULL && other->loseSteeringOnCollision) {
					other->steering = false;
				}
			}
		}
		if (this->loseSteeringOnCollision) this->steering = steering;
	}
	else {
		if (this->loseSteeringOnCollision) this->steering = false;
	}

	return results;
}