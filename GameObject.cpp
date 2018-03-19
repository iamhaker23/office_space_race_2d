#include "GameObject.h"

GameObject::GameObject() {
	this->sprites.push_back("default-sprite.png");
	this->activeSpriteIndex = 0;
	this->pingPongDirection = 1;
}

GameObject::GameObject(vector<string> sprites, int activeSprite, float objectColor[]) {

	this->sprites = sprites;

	this->activeSpriteIndex = 0;

	this->objectColor[0] = objectColor[0];
	this->objectColor[1] = objectColor[1];
	this->objectColor[2] = objectColor[2];
	this->objectColor[3] = objectColor[3];


	this->pingPongDirection = 1;

}

void GameObject::draw() {

	//Problem: updates texture every frame!
	GLuint myTexture = utils::loadPNG(this->sprites[this->activeSpriteIndex]);

	glEnable(GL_TEXTURE_2D);

	glColor4f(this->objectColor[0], this->objectColor[1], this->objectColor[2], this->objectColor[3]);
	glPushMatrix();

	//Do Transformations in reverse order HERE

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.5);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.5);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.5);

	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void GameObject::pingPongSprite() {
	//TODO:Refactor
	int incr = this->activeSpriteIndex + this->pingPongDirection;
	if (incr >= 0 && incr <= this->sprites.size() - 1) {
		this->activeSpriteIndex = incr;
	}
	else {
		this->pingPongDirection = this->pingPongDirection * -1;
		this->activeSpriteIndex = this->activeSpriteIndex + this->pingPongDirection;
	}
}

void GameObject::setSprite(int index) {
	if (index >= 0 && index <= this->sprites.size() - 1) {
		this->activeSpriteIndex = index;
	}
}
