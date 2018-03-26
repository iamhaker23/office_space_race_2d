#include "GameObject.h"


DebugInfo* GameObject::debugger = NULL;
Matrix3f* GameObject::worldToCamera = NULL;
bool GameObject::drawDebug = false;

GameObject::~GameObject() {
	for (CollisionRadii* c : this->collisionBounds) {
		c->angles.clear();
		c->radii.clear();
	}
	this->collisionBounds.clear();
}

GameObject::GameObject() {
	this->sprites.push_back("default-sprite.png");
	this->activeSpriteIndex = 0;
	this->pingPongDirection = 1;
	this->name = "DEFAULT_OBJECT";
	
	this->worldSpaceTransform = new Matrix3f();

	this->forces[0] = 0.0f;
	this->forces[1] = 0.0f;
	this->forces[2] = 0.0f;
	this->zTorque = 0.0f;

	this->friction = 0.0f;
	this->angularDamping = 0.0f;
	this->playerControl = false;

	this->scales[0] = 1.0f;
	this->scales[1] = 1.0f;
	this->scales[2] = 1.0f;

	this->topSpeed = 0.0f;

	this->colliderFlag = false;

	
	for (int i = 0; i < 16; i++) {
		this->alsoWorldSpaceTransform[i] = 0.0f;
	}
	this->alsoWorldSpaceTransform[0] = 1.0f;
	this->alsoWorldSpaceTransform[5] = 1.0f;
	this->alsoWorldSpaceTransform[10] = 1.0f;
	this->alsoWorldSpaceTransform[15] = 1.0f;
	
}


GameObject::GameObject(const GameObject &copy) {
	
	this->worldSpaceTransform = copy.worldSpaceTransform;

	for (int i = 0; i < 3; i++) {
		this->forces[i] = copy.forces[i];
	}
	for (int i = 0; i < 4; i++) {
		this->objectColor[i] = copy.objectColor[i];
	}
	this->friction = copy.friction;
	this->angularDamping = copy.angularDamping;

	this->activeSpriteIndex = copy.activeSpriteIndex;
	this->sprites = copy.sprites;
	this->name = copy.name;
	this->mesh = copy.mesh;

	this->playerControl = false;

	this->scales[0] = 1.0f;
	this->scales[1] = 1.0f;
	this->scales[2] = 1.0f;

	this->topSpeed = 0.0f;

	this->colliderFlag = false;

	for (int i = 0; i < 16; i++) {
		this->alsoWorldSpaceTransform[i] = 0.0f;
	}
	this->alsoWorldSpaceTransform[0] = 1.0f;
	this->alsoWorldSpaceTransform[5] = 1.0f;
	this->alsoWorldSpaceTransform[10] = 1.0f;
	this->alsoWorldSpaceTransform[15] = 1.0f;

}

GameObject::GameObject(string name, vector<string> sprites, vector<Vertex> mesh, int activeSprite, float objectColor[]) {
	
	this->name = name;
	
	this->mesh = mesh;
	this->sprites = sprites;

	this->activeSpriteIndex = 0;

	this->objectColor[0] = objectColor[0];
	this->objectColor[1] = objectColor[1];
	this->objectColor[2] = objectColor[2];
	this->objectColor[3] = objectColor[3];


	this->scales[0] = 1.0f;
	this->scales[1] = 1.0f;
	this->scales[2] = 1.0f;

	this->pingPongDirection = 1;

	this->worldSpaceTransform = new Matrix3f();

	this->forces[0] = 0.0f;
	this->forces[1] = 0.0f;
	this->forces[2] = 0.0f;
	this->zTorque = 0.0f;

	this->friction = 0.0f;
	this->angularDamping = 0.0f;
	this->playerControl = false;
	this->colliderFlag = false;

	for (int i = 0; i < 16; i++) {
		this->alsoWorldSpaceTransform[i] = 0.0f;
	}
	this->alsoWorldSpaceTransform[0] = 1.0f;
	this->alsoWorldSpaceTransform[5] = 1.0f;
	this->alsoWorldSpaceTransform[10] = 1.0f;
	this->alsoWorldSpaceTransform[15] = 1.0f;

}

void GameObject::draw() {


	//Problem: updates texture every frame!
	GLuint myTexture = utils::loadPNG(this->sprites[this->activeSpriteIndex]);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);

	glColor4f(this->objectColor[0], this->objectColor[1], this->objectColor[2], this->objectColor[3]);
	

	glPushMatrix();
		//TODO: World transform separate from screen transform

		/*this->worldSpaceTransform = this->worldSpaceTransform->Translate(this->forces[0], this->forces[1], this->forces[2]);
		this->worldSpaceTransform = this->worldSpaceTransform->RotateRadians(this->zTorque);// *(3.1415926f / 180.0f));
		this->worldSpaceTransform = this->worldSpaceTransform->Scale(scales[0], scales[1], scales[2]);
		*/
	

		//glMultMatrixf(alsoWorldSpaceTransform);

		glMultMatrixf(GameObject::worldToCamera->values);
		//glMultMatrixf(this->worldSpaceTransform->values);

		//glMultMatrixf((new Matrix3f(0.0f, 0.0f, 0.0f, 0.0f, this->scales[0], this->scales[1], this->scales[2]))->values);
		//glMultMatrixf((new Matrix3f(this->zTorque*(3.1415926f / 180.0f), 0.0f, 0.0f, 0.0f, 1.0f))->values);
		//glMultMatrixf((new Matrix3f(0.0f, this->forces[0], this->forces[1], this->forces[2], 1.0f))->values);

		Matrix3f* transform = (new Matrix3f(0.0f, this->forces[0], this->forces[1], this->forces[2], 1.0f))->Multiply(
		((new Matrix3f(this->zTorque*(3.1415926f / 180.0f), 0.0f, 0.0f, 0.0f, 1.0f))->Multiply(
		new Matrix3f(0.0f, 0.0f, 0.0f, 0.0f, this->scales[0], this->scales[1], this->scales[2])
		)
		));

		this->worldSpaceTransform = this->worldSpaceTransform->Multiply(transform);
		glMultMatrixf(this->worldSpaceTransform->values);

		


		//glScalef(scales[0], scales[1], scales[2]);
		//glRotatef(this->zTorque, 0.0f, 0.0f, 1.0f);
		//glTranslatef(this->forces[0], this->forces[1], this->forces[2]);

		//GLfloat tmpValues[16];
		//glGetFloatv(GL_MODELVIEW_MATRIX, tmpValues);
		//glGetFloatv(GL_MODELVIEW_MATRIX, alsoWorldSpaceTransform);
		
		// after world space
		//this->worldSpaceTransform = new Matrix3f();
		//this->worldSpaceTransform->loadMatrix(tmpValues);
		
		//this->worldSpaceTransform = this->worldSpaceTransform
			//->Add(GameObject::worldToCamera->inverse());
			//->Translate(GameObject::worldToCamera->getPosition()->x, GameObject::worldToCamera->getPosition()->y, GameObject::worldToCamera->getPosition()->z)
			//->Add(new Matrix3f(this->zTorque*(3.1415926f / 180.0f), 0.0f, 0.0f, 0.0f, 1.0f));
		

		resetModifiers();

	glBegin(GL_QUADS);
	
	for (Vertex v : this->mesh) {
		glTexCoord2f(v.u, v.v); 
		glVertex2f(v.x, v.y);
	}

	glEnd();

	if (GameObject::drawDebug) {
		for (int i = 0; i < (int)this->collisionBounds.size(); i++) {

			CollisionRadii* tmp = this->collisionBounds.at(i);
			for (int j = 0; j < (int)tmp->radii.size(); j++) {

				//draw all radii, white unless last selected
				drawCircle(0.0f, 0.0f, tmp->radii.at(j), (j != tmp->getLastSelected()[0] && j != tmp->getLastSelected()[1]) ? tmp->getDrawColor() : new Color4f(0.0f, 1.0f, 0.0f, 1.0f));

			}
		}
	}

	//glPopMatrix();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	
	
}

void GameObject::scale(float uniformScaleFactor) {
	this->scales[0] = uniformScaleFactor;
	this->scales[1] = uniformScaleFactor;
	this->scales[2] = uniformScaleFactor;
}

void GameObject::nuScale(float x, float y, float z) {
	this->scales[0] = x;
	this->scales[1] = y;
	this->scales[2] = z;
}


void GameObject::setPhysicalAttributes(float friction, float angularDamping, float topSpeed) {
	this->friction = friction;
	this->angularDamping = angularDamping;
	this->topSpeed = topSpeed;
}


void GameObject::setPlayerControl(bool playerControl) {
	this->playerControl = playerControl;
}

void GameObject::animate(AnimationLogic al) {

	if (al == AnimationLogic::PINGPONG) {
		if ((this->activeSpriteIndex <= 0 && this->pingPongDirection == -1) || (this->activeSpriteIndex >= ((int)this->sprites.size()-1) && this->pingPongDirection == 1)) {
			this->pingPongDirection = this->pingPongDirection * -1;
		}
		this->activeSpriteIndex = this->activeSpriteIndex + this->pingPongDirection;
	}
	else if (al == AnimationLogic::LOOPEND){
		this->activeSpriteIndex = (this->activeSpriteIndex >= ((int)this->sprites.size()-1)) ? 0 : this->activeSpriteIndex + 1;
	}
	
}

void GameObject::setSprite(int index) {
	if (index >= 0 && index <= (int)this->sprites.size()-1 ) {
		this->activeSpriteIndex = index;
	}
}

void GameObject::resetModifiers() {
	//Reset all latches otherwise they will apply additively
	//Because we accumulate this frame's tranforms in worldTransformMatrix
	this->scales[0] = 1.0f;
	this->scales[1] = 1.0f;
	this->scales[2] = 1.0f;

	if (this->angularDamping == 0.0f || abs(this->zTorque) < 0.01f) {
		//angular damping disabled
		this->zTorque = 0.0f;
	}
	else {
		this->zTorque *= 1.0f / this->angularDamping;
	}

	if (this->friction == 0.0f || (abs(this->forces[0]) < 0.01f && abs(this->forces[1]) < 0.01f && abs(this->forces[2]) < 0.01f)) {
		//friction disabled
		this->forces[0] = 0.0f;
		this->forces[1] = 0.0f;
		this->forces[2] = 0.0f;
	}
	else {
		this->forces[0] *= 1.0f / this->friction;
		this->forces[1] *= 1.0f / this->friction;
		this->forces[2] *= 1.0f / this->friction;
	}
}


void GameObject::processInputs(InputStates* inputs) {
	
	if (!playerControl) return;

	if (inputs->keys[0x53]) {
		//S Key
		if (this->forces[1] > -1*this->topSpeed) {

			/*if ((this->oldForces[1] - (this->forces[1] - 0.02f)) > 0.05f) {
				this->oldForces[1] = this->forces[1];
			}*/
			this->forces[1] -= 0.02f;
		}
	}
	if (inputs->keys[0x57]) {
		//W Key
		if (this->forces[1] < this->topSpeed) {

			/*if ((this->oldForces[1] - (this->forces[1] - 0.02f)) > 0.05f) {
				this->oldForces[1] = this->forces[1];
			}*/
			this->forces[1] += 0.03f;
		}
		
	}

	bool backwards = this->forces[1] < 0.0f;

	int sprite = 0;

	if (inputs->keys[0x41]) {
		//A Key
		sprite = 1;
		if (this->forces[1] != 0.0f) {
			this->zTorque -= ((backwards) ? 1.0f : -1.0f) * 1.1f;
		}
	}

	if (inputs->keys[0x44]) {
		//D Key
		sprite = 2;
		if (this->forces[1] != 0.0f) {
			this->zTorque += ((backwards)?-1.0f:1.0f) * -1.1f;
		}
	}
	
	this->setSprite(sprite);
	//Animations after auto-animation
	if (inputs->LeftPressed) {
		this->animate(AnimationLogic::LOOPEND);
	}

}

Vect4f* GameObject::getScreenPosition() {
	
	if (GameObject::worldToCamera != NULL) {
		return this->worldSpaceTransform->Add(GameObject::worldToCamera)->getPosition();
	}
	else {
		return this->worldSpaceTransform->getPosition();
	}
}

Vect4f* GameObject::getWorldPosition() {
	if (GameObject::worldToCamera != NULL) {
		//return (this->worldSpaceTransform->Multiply(GameObject::worldToCamera))->getPosition();
		//return (this->worldSpaceTransform)->Add(GameObject::worldToCamera)->getPosition();
		
		return this->worldSpaceTransform->getPosition();

		//return GameObject::worldToCamera->getPosition();
	}
	else {
		return this->worldSpaceTransform->getPosition();
	}
	
}

void GameObject::setWorldToCameraTransform(Matrix3f* wtc){
	//STATIC
	GameObject::worldToCamera = wtc;
}

void GameObject::setDebugger(DebugInfo* _debugger) {
	GameObject::debugger = _debugger;
}

void GameObject::setDebugMode(bool flag) {
	GameObject::drawDebug = flag;
}

float GameObject::getZAngle() {
	//0 rotation is at [1,0] 
	//0..PI = anticlockwise 180 (facing left)
	//0..-PI = clockwise 180 (facing right)
	return this->worldSpaceTransform->getZAngle();
}

void GameObject::translate(float x, float y, float z) {
	//todo
	//Non-force related translate
	//this->forces[0] = x;
	//this->forces[1] = y;
	//this->forces[2] = z;
	this->worldSpaceTransform->values[12] += x;
	this->worldSpaceTransform->values[13] += y;
	this->worldSpaceTransform->values[14] += z;
	
}

string GameObject::toString() {
	string output = "";

	output.append("Name:"); output.append(this->name); output.append("\n");
	output.append("Sprite:"); output.append(utils::intToString(this->activeSpriteIndex)); output.append("\n");
	output.append("Total Sprites:"); output.append(utils::intToString(this->sprites.size())); output.append("\n");
	output.append("pingPongDirection:"); output.append(string(1, (char)48 + this->pingPongDirection)); output.append("\n");
	return output;
}

string GameObject::getName() {
	return this->name;
}

void GameObject::setCollisionBounds(vector<CollisionRadii*> bounds) {
	this->collisionBounds = bounds;
}

bool GameObject::isCollider() {
	return this->colliderFlag;
}

void GameObject::setCollider(bool flag) {
	this->colliderFlag = flag;
}

void GameObject::resolveCollisions(vector<GameObject*> others) {
	
	if (this->isCollider()) {
		for (GameObject* other : others) {
			if (other != NULL && other != this) {
				if (other->isCollider() && !other->hasCollidedWith(this->name)) {
					float distX = this->getWorldPosition()->x - other->getWorldPosition()->x;
					float distY = this->getWorldPosition()->y - other->getWorldPosition()->y;

					float distanceSqrd = (distX * distX) + (distY * distY);

					//float myRad = this->getRadiusToObj(other);
					//float theirRad = other->getRadiusToObj(this);

					float angleToOther = this->getAngleToOther(other);

					//Confirmed: x-axis is 0|360
					//GameObject::debugger->addMessage(this->name);
					//GameObject::debugger->addMessage(utils::doubleToString((double)angleToOther));

					float myRad = this->getRadiusAtAngle(angleToOther, other->getWorldPosition());
					float theirRad = other->getRadiusAtAngle(360.0f - angleToOther, this->getWorldPosition());

					float combRad = theirRad + myRad;

					if (GameObject::drawDebug) {
						drawCircle(this->getWorldPosition()->x, this->getWorldPosition()->y, combRad*combRad, new Color4f(1.0f, 0.0f, 0.0f, 1.0f));
						drawCircle(this->getWorldPosition()->x, this->getWorldPosition()->y, distanceSqrd, new Color4f(0.0f, 0.0f, 1.0f, 1.0f));
						/*GameObject::debugger->addMessage(this->name);
						GameObject::debugger->addMessage(utils::doubleToString((double)myRad));
						GameObject::debugger->addMessage(utils::doubleToString((double)angleToOther));
						GameObject::debugger->addMessage(utils::doubleToString((double)theirRad));
						GameObject::debugger->addMessage(utils::doubleToString((double)360.0f - angleToOther));
						*/
					}

					if (combRad*combRad > distanceSqrd) {
						this->objectColor[0] = 0.0f;
						this->objectColor[1] = 0.0f;
						this->objectColor[2] = 1.0f;
						this->objectColor[3] = 1.0f;
						/*float xF = (other->getWorldPosition()->x*100.0f) - (this->getWorldPosition()->x*100.0f);
						float yF = (other->getWorldPosition()->y*100.0f) - (this->getWorldPosition()->y*100.0f);
						//GameObject::debugger->addMessage(utils::doubleToString((double)xF));
						//GameObject::debugger->addMessage(utils::doubleToString((double)yF));
						if (other->hasPhysics()) other->addForce(xF*0.001f, yF*0.001f, 0.0f);*/
						float distance = (distanceSqrd - (combRad*combRad))/2.0f;
						distance = (distance >= 1.0f) ? 1.0f : distance;
						if (other->hasPhysics()) other->translate(cosf((angleToOther-180.0f)*(3.1415926f / 180.0f))*distance, sinf((angleToOther - 180.0f)*(3.1415926f / 180.0f)*distance), 0.0f);
						//if (other->hasPhysics()) other->addForce(cosf((angleToOther - 180.0f)*(3.1415926f / 180.0f))*distance, sinf((angleToOther - 180.0f)*(3.1415926f / 180.0f)*distance), 0.0f);
					}
					else {
						this->objectColor[0] = 1.0f;
						this->objectColor[1] = 1.0f;
						this->objectColor[2] = 1.0f;
						this->objectColor[3] = 1.0f;
					}
					other->setHasCollidedWith(this->name);
					this->setHasCollidedWith(other->name);
				}
			}
		}
	}
}


void GameObject::addForce(float x, float y, float z) {
	this->forces[0] = x;
	this->forces[1] = y;
	this->forces[2] = z;
}

bool GameObject::hasPhysics() {
	return this->physics;
}

void GameObject::setPhysics(bool flag) {
	this->physics = flag;
}

float GameObject::getAngleToOther(GameObject* other) {

	Vect4f* myPos = this->getWorldPosition();
	float myX = myPos->x;
	float myY = myPos->y;

	Vect4f* otherPos = other->getWorldPosition();
	float otherX = otherPos->x;
	float otherY = otherPos->y;

	return (atan2f(myY - otherY, myX - otherX) * (180.0f / 3.1415926f)) + 180.0f;
}

float GameObject::getRadiusAtAngle(float angleToOther, Vect4f* otherPosition) {
	int closestCollisionRadii = 0;

	float smallestDistSqrd = FLT_MAX;

	for (int i = 0; i < (int)this->collisionBounds.size(); i++) {

		CollisionRadii* tmp = this->collisionBounds.at(i);

		float distSqrd = ((tmp->centreX - otherPosition->x)*(tmp->centreX - otherPosition->x)) + ((tmp->centreY - otherPosition->y)*(tmp->centreY - otherPosition->y));
		if (distSqrd < smallestDistSqrd) {
			//closest centre so far
			//Because an object may have multiple centres
			closestCollisionRadii = i;
		}
	}
	CollisionRadii* closest = this->collisionBounds.at(closestCollisionRadii);
	return closest->getInterpolatedRadiusAt(angleToOther);
}

float GameObject::getRadiusToObj(GameObject* other){

	Vect4f* myPos = this->getWorldPosition();
	float myX = myPos->x;
	float myY = myPos->y;

	Vect4f* otherPos = other->getWorldPosition();
	float otherX = otherPos->x;
	float otherY = otherPos->y;

	float radius = 1.0f;
	
	if ((int)this->collisionBounds.size() == 0) {
		//Default radius
		//Vect4f* myPos = this->getWorldPosition();
		//GameObject::drawCircle(myPos->x, myPos->y, radius);
		return radius;
	}
	
	int closestCollisionRadii = 0;

	float smallestDistSqrd = FLT_MAX;

	for (int i = 0; i < (int)this->collisionBounds.size(); i++) {

		CollisionRadii* tmp = this->collisionBounds.at(i);

		float distSqrd = ((tmp->centreX - otherX)*(tmp->centreX - otherX)) + ((tmp->centreY - otherY)*(tmp->centreY - otherY));
		if (distSqrd < smallestDistSqrd) {
			//closest centre so far
			//Because an object may have multiple centres
			closestCollisionRadii = i;
		}
	}
	

	//float angleToOther = (atan2f(myX*otherY - myY*otherY, myX*otherX+ myY*otherY) * (180.0f / 3.1415926f)) + 180.0f;
	
	//angle between localX and object
	//float angleToOther = (atan2f(otherY, otherX + otherY) * (180.0f / 3.1415926f)) + 180.0f;
	
	//otherToMe is 360-angleToOther
	float angleToOther = (atan2f(myY - otherY, myX - otherX) * (180.0f / 3.1415926f)) + 180.0f;

	//if (GameObject::screenToWorld != NULL) {
	//	angleToOther -= GameObject::screenToWorld->getZAngle()/(180.0f / 3.1415926f);
	//}

	CollisionRadii* closest = this->collisionBounds.at(closestCollisionRadii);
	radius = closest->getInterpolatedRadiusAt(angleToOther);

	//drawLine(myX, myY, myX + radius, myY);
	//drawLine(myX, myY, otherX, otherY);

	//GameObject::debugger->addMessage(utils::doubleToString((double)myX));
	//GameObject::debugger->addMessage(utils::doubleToString((double)myY));

	//GameObject::debugger->addMessage(utils::doubleToString((double)getScreenPosition()->x));
	//GameObject::debugger->addMessage(utils::doubleToString((double)getScreenPosition()->y));

	//GameObject::debugger->addMessage(utils::doubleToString((double)this->getZAngle()));


	//GameObject::debugger->addMessage(this->name);
	//GameObject::debugger->addMessage(utils::doubleToString((double)angleToOther));
	//GameObject::debugger->addMessage(utils::doubleToString((double)radius));

	//drawLine(myX, myY, (myX+radius) + cosf(angleToOther*(3.1415926f/ 180.0f)), (myY+radius) * sinf(angleToOther*(3.1415926f / 180.0f)));
	//drawLine(myX, myY, otherX, otherY);

	return radius;

}

void GameObject::drawCircle(float x, float y, float radius, Color4f* col)
{
	glDisable(GL_TEXTURE_2D);

	glColor4f(col->r, col->g, col->b, col->a);
	glBegin(GL_LINE_LOOP);
	for (float i = 0; i<360; i += 5)
	{
		float xcoord = (x + radius) * cosf(i*(3.1415926f / 180.0f));
		float ycoord = (y + radius) * sinf(i*(3.1415926f / 180.0f));
		glVertex2f(xcoord, ycoord);
	}

	/*if (GameObject::debugger != NULL) {
		GameObject::debugger->addMessage("Drawing Circle:");
		GameObject::debugger->addMessage(utils::doubleToString((double)radius));
	}*/

	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void GameObject::drawLine(float x1, float y1, float x2, float y2) {
	glDisable(GL_TEXTURE_2D);

	//Earthquake test!
	//glRotatef(10.0f, 0.0f, 0.0f, 1.0f);
	
	glPushMatrix();

	if (GameObject::worldToCamera != NULL) {
		glMultMatrixf(GameObject::worldToCamera->values);
	}

	glBegin(GL_LINES);
		glLineWidth(10.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

bool GameObject::hasCollidedWith(string name) {
	for (string collidedWith : this->collidedWithThisFrame) {
		if (name == collidedWith) return true;
	}
	return false;
}

void GameObject::setHasCollidedWith(string name) {
	this->collidedWithThisFrame.push_back(name);
}

void GameObject::resetCollisionFlags() {
	this->collidedWithThisFrame.clear();
}