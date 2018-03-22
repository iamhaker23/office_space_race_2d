#include "GameObject.h"

GameObject::GameObject() {
	this->sprites.push_back("default-sprite.png");
	this->activeSpriteIndex = 0;
	this->pingPongDirection = 1;
	this->name = "DEFAULT_OBJECT";
	
	glGetFloatv(GL_MODELVIEW_MATRIX, this->worldTransformMatrix);

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
	
	this->oldForces[0] = 0.0f;
	this->oldForces[1] = 0.0f;
	this->oldForces[2] = 0.0f;
}


GameObject::GameObject(const GameObject &copy) {
	for (int i = 0; i < 16; i++) {
		this->worldTransformMatrix[i] = copy.worldTransformMatrix[i];
	}
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

	this->oldForces[0] = 0.0f;
	this->oldForces[1] = 0.0f;
	this->oldForces[2] = 0.0f;
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



	this->pingPongDirection = 1;

	glGetFloatv(GL_MODELVIEW_MATRIX, this->worldTransformMatrix);

	this->forces[0] = 0.0f;
	this->forces[1] = 0.0f;
	this->forces[2] = 0.0f;
	this->zTorque = 0.0f;

	this->friction = 0.0f;
	this->angularDamping = 0.0f;
	this->playerControl = false;
}

void GameObject::draw() {

	//Problem: updates texture every frame!
	GLuint myTexture = utils::loadPNG(this->sprites[this->activeSpriteIndex]);

	glEnable(GL_TEXTURE_2D);

	glColor4f(this->objectColor[0], this->objectColor[1], this->objectColor[2], this->objectColor[3]);
	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
	
	glMultMatrixf(this->worldTransformMatrix);
	glScalef(scales[0], scales[1], scales[2]);
	glRotatef(this->zTorque, 0.0f, 0.0f, 1.0f);
	glTranslatef(this->forces[0], this->forces[1], this->forces[2]);
	

	//TODO: Verify and optimise, bugfix (does not act in worldspace!!!)
	float diffXForce = (this->forces[1] - this->oldForces[0]);
	if (diffXForce > 0.05f) {
		this->oldForces[0] *= 0.85f;
		glTranslatef(0.0f, this->oldForces[0], 0.0f);
	}
	float diffYForce = (this->forces[1] - this->oldForces[1]);
	if (diffYForce > 0.05f) {
		this->oldForces[1] *= 0.85f;
		glTranslatef(0.0f, this->oldForces[1], 0.0f);
	}

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

	glGetFloatv(GL_MODELVIEW_MATRIX, this->worldTransformMatrix);

	glBegin(GL_QUADS);
	
	for (Vertex v : this->mesh) {
		glTexCoord2f(v.u, v.v); 
		glVertex2f(v.x, v.y);
	}

	glEnd();
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


void GameObject::processInputs(InputStates* inputs) {
	
	if (!playerControl) return;

	bool backwards = false;

	if (inputs->keys[0x53]) {
		//S Key
		if (this->forces[1] > -1*this->topSpeed) {

			if ((this->oldForces[1] - (this->forces[1] - 0.02f)) > 0.05f) {
				this->oldForces[1] = this->forces[1];
			}
			this->forces[1] -= 0.02f;
		}
		backwards = true;
	}
	if (inputs->keys[0x57]) {
		//W Key
		backwards = false;
		if (this->forces[1] < this->topSpeed) {

			if ((this->oldForces[1] - (this->forces[1] - 0.02f)) > 0.05f) {
				this->oldForces[1] = this->forces[1];
			}
			this->forces[1] += 0.03f;
		}
		
	}
	int sprite = 0;

	if (inputs->keys[0x41]) {
		//A Key
		sprite = 1;
		if (this->forces[1] != 0.0f) {
			this->zTorque += 1.1f;
		}
	}

	if (inputs->keys[0x44]) {
		//D Key
		sprite = 2;
		if (this->forces[1] != 0.0f) {
			this->zTorque += -1.1f;
		}
	}
	
	this->setSprite(sprite);
	//Animations after auto-animation
	if (inputs->LeftPressed) {
		this->animate(AnimationLogic::LOOPEND);
	}

	if (backwards) this->zTorque = this->zTorque * -1.0f;
}

vector<float> GameObject::getPosition() {
	return vector<float>{ this->worldTransformMatrix[12], this->worldTransformMatrix[13], this->worldTransformMatrix[14]};
}

float GameObject::getZAngle() {
	return atan2f(this->worldTransformMatrix[1], this->worldTransformMatrix[0]);
}


void GameObject::translate(float x, float y, float z) {
	//todo
	//Non-force related translate
	this->forces[0] = x;
	this->forces[1] = y;
	this->forces[2] = z;

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