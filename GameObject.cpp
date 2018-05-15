#include "GameObject.h"


DebugInfo* GameObject::debugger = NULL;
Matrix3f GameObject::worldToCamera = Matrix3f();
bool GameObject::drawDebug = false;
GLuint GameObject::defaultSprite = (GLuint)0;


GameObject::~GameObject() {
	this->freeData();
	
}

/*
void GameObject::freeTextures() {
	for (GLuint tex : this->sprites) {
		utils::freeTexture(tex);
	}
}*/

void GameObject::freeData() {
	for (CollisionRadii* c : this->collisionBounds) {
		delete c;
	}
	this->collisionBounds.clear();

	this->worldSpaceTransform->freeData();
	this->boundsTransform->freeData();


}

GameObject::GameObject() {
	this->visible = true;
	this->canFall = false;

	this->sprites = {};
	this->sprites.push_back(GameObject::defaultSprite);
	this->objectColor = new Color4f();

	this->activeSpriteIndex = 0;
	this->pingPongDirection = 1;
	this->name = "DEFAULT_OBJECT";

	this->worldSpaceTransform = new Matrix3f();
	this->boundsTransform = new Matrix3f();

	this->forces[0] = 0.0f;
	this->forces[1] = 0.0f;
	this->forces[2] = 0.0f;
	this->zTorque = 0.0f;

	this->friction = 0.0f;
	this->angularDamping = 0.0f;

	this->scales[0] = 1.0f;
	this->scales[1] = 1.0f;
	this->scales[2] = 1.0f;
	this->boundScales[0] = 1.0f;
	this->boundScales[1] = 1.0f;
	this->boundScales[2] = 1.0f;

	this->colliderFlag = false;

	this->ignoreContainers = false;
	this->physics = false;
	this->physicsContainer = false;
	this->ghost = false;
	topSpeed = 0.0f;
	
	this->mesh = {};
	this->collisionBounds = {};
	this->collided = false;
	this->collidedWithThisFrame = {};
}


GameObject::GameObject(const GameObject &copy) {
	this->visible = copy.visible;
	this->canFall = copy.canFall;
	this->worldSpaceTransform = new Matrix3f(*copy.worldSpaceTransform);
	this->boundsTransform = new Matrix3f(*copy.boundsTransform);
	for (int i = 0; i < 3; i++) {
		this->forces[i] = copy.forces[i];
	}

	this->objectColor = new Color4f(copy.objectColor->r, copy.objectColor->g, copy.objectColor->b, copy.objectColor->a);

	this->friction = copy.friction;
	this->angularDamping = copy.angularDamping;

	this->activeSpriteIndex = copy.activeSpriteIndex;

	this->sprites = copy.sprites;
	this->mesh = copy.mesh;
	this->children = copy.children;
	/*this->sprites = {};
	for (GLuint i : copy.sprites) {
		this->sprites.push_back(i);
	}

	this->mesh = {};
	for (Vertex v : copy.mesh) {
		this->mesh.push_back(v);
	}*/


	this->name = copy.name;

	this->scales[0] = copy.scales[0];
	this->scales[1] = copy.scales[1];
	this->scales[2] = copy.scales[2];
	this->boundScales[0] = copy.boundScales[0];
	this->boundScales[1] = copy.boundScales[1];
	this->boundScales[2] = copy.boundScales[2];

	this->colliderFlag = copy.colliderFlag;
	this->ignoreContainers = copy.ignoreContainers;
	this->physics = copy.physics;
	this->physicsContainer = copy.physicsContainer;
	this->ghost = copy.ghost;
	topSpeed = copy.topSpeed;

	this->zTorque = copy.zTorque;
	this->physics = copy.physics;
	this->collidedWithThisFrame = {};
	this->collided = false;
	this->setCollisionBounds(copy.collisionBounds);
	this->pingPongDirection = copy.pingPongDirection;

}

void GameObject::addChild(GameObject &o) {
	this->children.push_back(GameObject(o));
}

GameObject::GameObject(string name, vector<GLuint> &sprites, vector<Vertex> &mesh, int activeSprite,  Color4f &objectColor) {
	this->visible = true;
	this->canFall = false;
	this->name = name;

	this->sprites = sprites;
	this->mesh = mesh;
	this->children = {};
	/*this->sprites = {};
	for (GLuint i : sprites) {
	this->sprites.push_back(i);
	}
	this->mesh = {};
	for (Vertex v : mesh) {
		this->mesh.push_back(v);
	}*/

	this->activeSpriteIndex = 0;

	this->objectColor = new Color4f(objectColor);


	this->scales[0] = 1.0f;
	this->scales[1] = 1.0f;
	this->scales[2] = 1.0f;
	this->boundScales[0] = 1.0f;
	this->boundScales[1] = 1.0f;
	this->boundScales[2] = 1.0f;

	this->pingPongDirection = 1;

	this->worldSpaceTransform = new Matrix3f();
	this->boundsTransform = new Matrix3f();

	this->forces[0] = 0.0f;
	this->forces[1] = 0.0f;
	this->forces[2] = 0.0f;
	this->zTorque = 0.0f;

	this->ignoreContainers = false;
	this->friction = 0.0f;
	this->angularDamping = 0.0f;
	this->colliderFlag = false;
	this->physics = false;
	this->physicsContainer = false;
	this->ghost = false;
	topSpeed = 0.0f;

	this->collidedWithThisFrame = {};
	this->collided = false;
	this->collisionBounds = {};
}

void GameObject::setVisibility(bool vis) {
	this->visible = vis;
}

void GameObject::setCanFall(bool fall) {
	this->canFall = fall;
}

bool GameObject::getCanFall() {
	return this->canFall;
}

void GameObject::draw() {

	if (!this->visible) return;
	//identity matrix
	draw(Matrix3f());

}

void GameObject::draw(Matrix3f &parentTransform) {

	if (!this->visible) return;
	//TODO: could this ever be a feature? Need justification to ruin the matrix at the top of the stack
	//glRotatef(10.0f, 0.0f, 0.0f, 1.0f);

	if ((int)this->sprites.size() > 0) {
		GLuint tex = this->sprites[this->activeSpriteIndex];

		GLboolean b = glIsTexture(tex);
		utils::bindTexture(tex);

		glEnable(GL_TEXTURE_2D);
	}
	else {
		glDisable(GL_TEXTURE_2D);
	}

	glMatrixMode(GL_MODELVIEW);

	glColor4f(this->objectColor->r, this->objectColor->g, this->objectColor->b, this->objectColor->a);


	glPushMatrix();

	glMultMatrixf(GameObject::worldToCamera.values);

	//TODO: setWorldSpaceMatrix(MAtrix3f &mat);
	Matrix3f* nM = new Matrix3f(this->getNewPosition());
	delete this->worldSpaceTransform;
	this->worldSpaceTransform = nM;

	glMultMatrixf(parentTransform.Multiply(*this->worldSpaceTransform).values);


	resetModifiers();

	glBegin(GL_QUADS);

	for (Vertex v : this->mesh) {
		glTexCoord2f(v.u, v.v);
		glVertex2f(v.x, v.y);
	}

	glEnd();


	if (GameObject::drawDebug) {
		//if ((int)this->collisionBounds.size() != 0) {
		glPushMatrix();
		glLoadIdentity();

		glMultMatrixf(GameObject::worldToCamera.values);
		if (this->hasPhysics()) glMultMatrixf(this->worldSpaceTransform->values);

		glDisable(GL_TEXTURE_2D);
		for (int i = 0; i < (int)this->collisionBounds.size(); i++) {

			CollisionRadii* tmp = this->collisionBounds.at(i);
			
			glColor4f(0.6f, 0.6f, 1.0f, 1.0f);
			for (int j = 0; j <= 360; j++){
				glBegin(GL_LINES);
				Vect4f centre = boundSpaceToObjectSpace(Vect4f(tmp->centreX, tmp->centreY, 0.0f));

				glVertex3f(centre.x, centre.y, 0.0f);
				float ang = (float)j;
				float rad = tmp->getInterpolatedRadiusAt(ang);

				Vect4f toDraw = boundSpaceToObjectSpace(Vect4f((rad*cosf(ang*(3.1415926f / 180.0f))), (rad*sinf(ang*(3.1415926f / 180.0f))), 0.0f));
				
				glVertex3f(toDraw.getX() + centre.getX(), toDraw.getY() + centre.getY(), 0.0f);
				glEnd();
			}
		}

		for (int i = 0; i < (int)this->collisionBounds.size(); i++) {

			CollisionRadii* tmp = this->collisionBounds.at(i);
			glBegin(GL_LINES);
			for (int j = 0; j < (int)tmp->radii.size(); j++) {
				Vect4f centre = boundSpaceToObjectSpace(Vect4f(tmp->centreX, tmp->centreY, 0.0f));
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				glVertex3f(centre.x, centre.y, 0.0f);


				float ang = tmp->angles.at(j);
				float rad = this->radius2DToWorldSpace(tmp->radii.at(j), ang);

				Vect4f toDraw = Vect4f((rad*cosf(ang*(3.1415926f / 180.0f))), (rad*sinf(ang*(3.1415926f / 180.0f))), 0.0f);
				glVertex3f(toDraw.getX() + centre.getX(), toDraw.getY() + centre.getY(), 0.0f);
			}
			glEnd();
		}
		glBegin(GL_LINE_LOOP);
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		for (int i = 0; i < (int)this->collisionBounds.size(); i++) {
			
			CollisionRadii* tmp = this->collisionBounds.at(i);
			
			for (int j = 0; j < (int)tmp->radii.size(); j++) {
				Vect4f centre = boundSpaceToObjectSpace(Vect4f(tmp->centreX, tmp->centreY, 0.0f));
				float ang = tmp->angles.at(j);
				float rad = this->radius2DToWorldSpace(tmp->radii.at(j), ang);
				Vect4f toDraw = Vect4f((rad*cosf(ang*(3.1415926f / 180.0f))), (rad*sinf(ang*(3.1415926f / 180.0f))), 0.0f);
				glVertex3f(toDraw.getX() + centre.getX(), toDraw.getY() + centre.getY(), 0.0f);
			}
		}
		glEnd();

		glEnable(GL_TEXTURE_2D);
		glPopMatrix();
		//}
	}

	glPopMatrix();

	for (GameObject o : this->children) {
		glPushMatrix();
		o.draw(parentTransform.Multiply(*this->worldSpaceTransform));
		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);
}

void GameObject::scale(float uniformScaleFactor, bool scaleCollider) {
	this->scales[0] = uniformScaleFactor;
	this->scales[1] = uniformScaleFactor;
	this->scales[2] = uniformScaleFactor;
	if (scaleCollider) {
		this->boundScales[0] = uniformScaleFactor;
		this->boundScales[1] = uniformScaleFactor;
		this->boundScales[2] = uniformScaleFactor;
		applyBoundsScale();
	}
}

void GameObject::nuScale(float x, float y, float z, bool scaleCollider) {
	this->scales[0] = x;
	this->scales[1] = y;
	this->scales[2] = z;
	if (scaleCollider) {
		this->boundScales[0] = x;
		this->boundScales[1] = y;
		this->boundScales[2] = z;
		applyBoundsScale();
	}
}

void GameObject::nuScaleBounds(float x, float y, float z) {
	this->boundScales[0] = x;
	this->boundScales[1] = y;
	this->boundScales[2] = z;

	applyBoundsScale();
}

void GameObject::applyBoundsScale() {
	Matrix3f* nM = new Matrix3f(this->boundsTransform->Multiply(Matrix3f(0.0f, 0.0f, 0.0f, 0.0f, this->boundScales[0], this->boundScales[1], this->boundScales[2])));
	delete this->boundsTransform;
	this->boundsTransform = nM;
}

void GameObject::setPhysicalAttributes(float friction, float angularDamping, float topSpeed) {
	this->friction = friction;
	this->angularDamping = angularDamping;
	this->topSpeed = topSpeed;
}

int GameObject::countCollisionRadii() {
	return (int)this->collisionBounds.size();
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



Vect4f GameObject::getWorldPosition() {
	return this->worldSpaceTransform->getPosition();
}

Vect4f GameObject::localToWorldSpace(Vect4f &localCoords) {
	return localCoords.transform(*this->worldSpaceTransform);
}

Vect4f GameObject::boundSpaceToObjectSpace(Vect4f &localCoords) {
	//Matrix3f* boundsSpace = new Matrix3f(0.0f, this->boundsTransform->values[12], this->boundsTransform->values[13], this->boundsTransform->values[14], 0.0f);
	//boundsSpace = boundsSpace->Multiply(new Matrix3f(this->boundsTransform->getZAngle(), 0.0f, 0.0f, 0.0f, 1.0f));
	//boundsSpace = boundsSpace->Multiply(new Matrix3f(0.0f, 0.0f, 0.0f, 0.0f, this->boundScales[0], this->boundScales[1], this->boundScales[2]));

	//return localCoords.transform(this->boundsTransform);
	//Bounds space is local to object and only has scaling applied
	return localCoords.scale(this->boundScales[0], this->boundScales[1], this->boundScales[2])
		.transform(
			Matrix3f(0.0f, this->boundsTransform->values[12], this->boundsTransform->values[13], this->boundsTransform->values[14], 1.0f)
		);
		
	//return new Vect4f(localCoords);
	//return output;
}

void GameObject::setWorldToCameraTransform(Matrix3f &wtc){
	//STATIC
	GameObject::worldToCamera = wtc;
}

void GameObject::setDebugger(DebugInfo &_debugger) {
	if (GameObject::debugger == NULL) GameObject::debugger = &_debugger;
}

void GameObject::setDebugMode(bool flag) {
	GameObject::drawDebug = flag;
}

bool GameObject::getDebugMode(){
	return GameObject::drawDebug;
}

float GameObject::getZAngle() {
	//0 rotation is at [1,0] 
	//0..PI = anticlockwise 180 (facing left)
	//0..-PI = clockwise 180 (facing right)
	return this->worldSpaceTransform->getZAngle();
}


void GameObject::setZAngle(float angle) {
	Matrix3f* nM = new Matrix3f(this->worldSpaceTransform->RotateRadians((angle + 270.0f) * (3.1415926f / 180.0f)));
	delete this->worldSpaceTransform;
	this->worldSpaceTransform = nM;

}

float GameObject::getAngleFromX() {
	float output = this->getZAngle() * (180.0f / 3.1415926f);

	if (output < 0.0f) {
		output += 360.0f;
	}

	return output;

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

void GameObject::translateBounds(float x, float y, float z) {
	//todo
	//Non-force related translate
	//this->forces[0] = x;
	//this->forces[1] = y;
	//this->forces[2] = z;
	this->boundsTransform->values[12] += x;
	this->boundsTransform->values[13] += y;
	this->boundsTransform->values[14] += z;
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

void GameObject::setCollisionBounds(const vector<CollisionRadii> &bounds) {
	this->collisionBounds = {};
	for (CollisionRadii b : bounds) {
		this->collisionBounds.push_back(new CollisionRadii(b));
	}

}
void GameObject::setCollisionBounds(const vector<CollisionRadii*> &bounds) {
	this->collisionBounds = {};
	for (CollisionRadii* b : bounds) {
		this->collisionBounds.push_back(new CollisionRadii(*b));
	}
}

void GameObject::setCollisionBounds(const CollisionRadii &bounds) {
	this->collisionBounds = {};
	this->collisionBounds.push_back(new CollisionRadii(bounds));
}

bool GameObject::isCollider() {
	return this->colliderFlag;
}

void GameObject::setCollider(bool flag) {
	this->colliderFlag = flag;
}

vector<CollisionResult> GameObject::resolveCollisions(const vector<GameObject*> &others) {
	
	vector<CollisionResult> results = {};

	if ((int)this->collisionBounds.size() == 0) {
		if (GameObject::debugger != NULL) {
			GameObject::debugger->addMessage("ERROR - no bounds for object:");
			GameObject::debugger->addMessage(this->name);
		}
		return {};
	}

	if (this->isCollider()) {

		for (GameObject* other : others) {

			if ((int)other->collisionBounds.size() == 0) {
				if (GameObject::debugger != NULL) {
					GameObject::debugger->addMessage("ERROR - no bounds for object:");
					GameObject::debugger->addMessage(other->name);
				}
				continue;
			}

			if (other != NULL && other != this) {
				if (other->isCollider() && !other->hasResolvedWith(this->name) && (this->hasPhysics() || other->hasPhysics())) {
										
					CollisionResult result = CollisionResult();

					//Account for movement before doing collisions
					//Or post-collision set forces to zero (feels too sticky tho)

					CollisionRadii* myClosest = this->getClosestRadiiTo(other->getNewPosition().getPosition());
					//CollisionRadii* myClosest = this->getClosestRadiiTo(other->getWorldPosition());
					
					//bounds are transformed seperately to the object
					//only apply object transform in the case of dynamic or translated objects (e.g. physics)
					Vect4f myCentreWo = 
						(this->hasPhysics()) ?
						this->localToWorldSpace(this->boundSpaceToObjectSpace(Vect4f(myClosest->centreX, myClosest->centreY, 0.0f)))
						: (this->boundSpaceToObjectSpace(Vect4f(myClosest->centreX, myClosest->centreY, 0.0f)));
					
					CollisionRadii* theirClosest = other->getClosestRadiiTo(myCentreWo);

					Vect4f theirCentreWo = other->localToWorldSpace(other->boundSpaceToObjectSpace(Vect4f(theirClosest->centreX, theirClosest->centreY, 0.0f)));
					
					float distX = (myCentreWo.x - theirCentreWo.x);
					float distY = (myCentreWo.y - theirCentreWo.y);

					float angleToOther = GameObject::getAngleBetweenPositions(myCentreWo, theirCentreWo) - this->getAngleFromX();
					while (angleToOther > 360.0f) {
						angleToOther -= 360.0f;
					}
					while (angleToOther < 0.0f) {
						angleToOther += 360.0f;
					}


					float myRad = this->radius2DToWorldSpace(myClosest->getInterpolatedRadiusAt(angleToOther), angleToOther);
					
					float otherAngleToMe = abs((((angleToOther >= 180.0f) ? angleToOther - 180.0f : 360.0f - (180.0f - angleToOther))) - other->getAngleFromX());

					while (otherAngleToMe > 360.0f) {
						otherAngleToMe -= 360.0f;
					}

					float theirRad = other->radius2DToWorldSpace(theirClosest->getInterpolatedRadiusAt(otherAngleToMe), otherAngleToMe);

					float distanceSqrd = (distX * distX) + (distY * distY);

					float combRad = theirRad + myRad;

					result.distanceSqrd = distanceSqrd - (combRad * combRad);

					bool iContain = this->isPhysicsContainer();
					bool theyContain = other->isPhysicsContainer();

					if ((!iContain && !theyContain) && combRad*combRad > distanceSqrd) {
						

						this->setCollidedWith(other->name);
						other->setCollidedWith(this->name);
						this->collided = true;
						other->collided = true;
						result.other = other;


						//regular collision
						if (GameObject::drawDebug) {
							//collision is detected, even if either object is "ghost"
							delete this->objectColor; 
							delete other->objectColor;
							this->objectColor = new Color4f(0.0f, 0.0f, 1.0f, 1.0f);
							other->objectColor = new Color4f(0.0f, 0.0f, 1.0f, 1.0f);
						}

						if (!this->isGhost() && !other->isGhost()) {
							float factor = 0.06f;
							float torque = (angleToOther <= 270.0f && angleToOther >= 90.0f) ? -1.4f : 1.4f;
							bool bothPhysics = this->hasPhysics() && other->hasPhysics();

							//short circuit optimisation?
							if (bothPhysics || other->hasPhysics()) {
								if (!bothPhysics) other->zTorque += torque;
								other->translate(-distX*factor, -distY*factor, 0.0f);
								//other->forces[0] = 0.0f;
								//other->forces[1] = 0.0f;
								//other->forces[2] = 0.0f;
								//this->zTorque /= this->angularDamping;
							}
							if (bothPhysics || this->hasPhysics()) {
								if (!bothPhysics) this->zTorque += -torque;
								this->translate(distX*factor, distY*factor, 0.0f);
								//this->forces[0] = 0.0f;
								//this->forces[1] = 0.0f;
								//this->forces[2] = 0.0f;
								//this->zTorque /= this->angularDamping;
							}
						}

					}
					else if ((iContain && !other->ignoreContainers && ((myRad*myRad) < (distanceSqrd ))) ||
							(theyContain && !this->ignoreContainers && ((theirRad*theirRad) < (distanceSqrd )))) {
						float factor = 0.15f;
						float torque = (angleToOther <= 270.0f && angleToOther >= 90.0f) ? -1.4f : 1.4f;
						bool bothPhysics = this->hasPhysics() && other->hasPhysics();

						this->setCollidedWith(other->name);
						other->setCollidedWith(this->name);
						this->collided = true;
						other->collided = true;
						result.other = other;

						//short circuit optimisation?
						if (bothPhysics || (other->hasPhysics() && !this->isGhost())) {
							other->translate(distX*factor, distY*factor, 0.0f);
							//other->forces[0] = 0.0f;
							//other->forces[1] = 0.0f;
							//other->forces[2] = 0.0f;
							//other->zTorque /= this->angularDamping;
						}

						if (bothPhysics || (this->hasPhysics() && !other->isGhost())) {
							this->translate(-distX*factor, -distY*factor, 0.0f);
							//this->forces[0] = 0.0f;
							//this->forces[1] = 0.0f;
							//this->forces[2] = 0.0f;
							//this->zTorque /= this->angularDamping;
						}
					}
					else {

						this->collided = false;
						other->collided = false;
						result.other = NULL;

						if (GameObject::drawDebug) {
							delete this->objectColor;
							delete other->objectColor;
							this->objectColor = new Color4f(1.0f, 1.0f, 1.0f, 1.0f);
							other->objectColor = new Color4f(1.0f, 1.0f, 1.0f, 1.0f);
						}
					}
					other->setCollisionResolvedWith(this->name);
					this->setCollisionResolvedWith(other->name);
					results.push_back(result);
				}
			}

		}
	}

	return results;
}

float GameObject::radius2DToWorldSpace(float radius, float angle) {
	//return this->boundsTransform->scalex;
	float output = radius2DToObjectSpace(radius, angle);

	float adj = (radius * (cosf(angle * (3.1415926f / 180.0f))))*this->scales[0] * this->boundScales[0];// *0.5f;
	float opp = (radius * (sinf(angle * (3.1415926f / 180.0f))))*this->scales[1] * this->boundScales[1];// *0.5f;

	Vect4f* newRad = (new Vect4f(adj, opp, 0.0f));

	return newRad->getXYMagnitude();
}

float GameObject::radius2DToObjectSpace(float radius, float angle) {
	
	//no XY scaling of radius
	//if (this->boundScales[1] == 1.0f && this->boundScales[1] == 1.0f) return radius; 
	
	float output = radius;

	//float adj = (radius * (cosf(angle * (3.1415926f / 180.0f))));// *0.5f;
	//float opp = (radius * (sinf(angle * (3.1415926f / 180.0f))));// *0.5f;


	float adj = (radius * (cosf(angle * (3.1415926f / 180.0f))));
	float opp = (radius * (sinf(angle * (3.1415926f / 180.0f))));

	Vect4f newRad = Vect4f(adj, opp, 0.0f);
	//newRad = (Vect4f(adj, opp, 0.0f)).transform(this->boundsTransform);

	newRad = this->boundSpaceToObjectSpace(newRad);
	//newRad = this->localToWorldSpace(*newRad);
	//newRad = newRad->scale(this->worldSpaceTransform->scalex, this->worldSpaceTransform->scaley, this->worldSpaceTransform->scalez);
	//return radius * this->boundScales[0] * this->boundScales[1] * this->boundScales[2];
	//return radius * 5.0f;
	return newRad.getXYMagnitude();
	//return sqrt((opp*opp) + (adj*adj));
}

void GameObject::setGhost(bool flag) {
	this->ghost = flag;
}

bool GameObject::isGhost() {
	return this->ghost;
}

void GameObject::setPhysicsContainer(bool flag) {
	this->physicsContainer = flag;
}

bool GameObject::isPhysicsContainer() {
	return this->physicsContainer;
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

float GameObject::getAngleToOther(GameObject &other) {

	return this->getAngleToPosition(other.getWorldPosition());

}

float GameObject::getAngleToPosition(Vect4f &position) {
	return getAngleBetweenPositions(this->getWorldPosition(), position);
}

float GameObject::getAngleBetweenPositions(Vect4f &a, Vect4f &b) {

	float myX = a.x;
	float myY = a.y;

	float otherX = b.x;
	float otherY = b.y;

	return ((atan2f(myY - otherY, myX - otherX) * (180.0f / 3.1415926f)) + 180.0f);
}

CollisionRadii* GameObject::getNextCollisionRadiiFor(Vect4f &otherPosition, int step) {
	int closestCollisionRadii = getIndexOfClosestRadiiTo(otherPosition);

	closestCollisionRadii = getNextIndexStep(closestCollisionRadii, step);
	
	return this->collisionBounds.at(closestCollisionRadii);
}

int GameObject::getNextIndexStep(int segIndex, int step) {
	int size = (int)this->collisionBounds.size();
	int output = segIndex + step;
	
	if (output <= size*-1) output += size;
	output = (output < 0) ? (size + (output - 1)) : output;
	output = (output > (int)this->collisionBounds.size() - 1) ? 0 : output;
	return output;
}

CollisionRadii* GameObject::getClosestRadiiTo(Vect4f &otherPosition) {
	return this->collisionBounds.at(this->getIndexOfClosestRadiiTo(otherPosition));
}

int GameObject::getIndexOfClosestRadiiTo(Vect4f &otherPosition) {
	int closestCollisionRadii = 0;

	float smallestDistSqrd = 10000.0f;

	for (int i = 0; i < (int)this->collisionBounds.size(); i++) {

		CollisionRadii* tmp = this->collisionBounds.at(i);
		Vect4f tmpWoPos = (!this->hasPhysics()) ? (this->boundSpaceToObjectSpace(Vect4f(tmp->centreX, tmp->centreY, 0.0f))) 
			: this->localToWorldSpace(this->boundSpaceToObjectSpace(Vect4f(tmp->centreX, tmp->centreY, 0.0f)));

		float distSqrd = ((tmpWoPos.x - otherPosition.x)*(tmpWoPos.x - otherPosition.x)) + ((tmpWoPos.y - otherPosition.y)*(tmpWoPos.y - otherPosition.y));
		
		if (distSqrd < smallestDistSqrd) {
			smallestDistSqrd = distSqrd;
			closestCollisionRadii = i;
		}
	}

	return closestCollisionRadii;
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

	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void GameObject::drawLine(float x1, float y1, float x2, float y2) {
	glDisable(GL_TEXTURE_2D);
		
	glPushMatrix();

	glMultMatrixf(GameObject::worldToCamera.values);

	glBegin(GL_LINES);
		glLineWidth(10.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

bool GameObject::hasResolvedWith(string name) {
	for (string collidedWith : this->resolvedWithThisFrame) {
		if (name == collidedWith) return true;
	}
	return false;
}
bool GameObject::hasCollidedWith(string name) {
	for (string collidedWith : this->collidedWithThisFrame) {
		if (name == collidedWith) return true;
	}
	return false;
}

void GameObject::setCollisionResolvedWith(string name) {
	this->resolvedWithThisFrame.push_back(name);
}
void GameObject::setCollidedWith(string name) {
	this->collidedWithThisFrame.push_back(name);
}

void GameObject::resetCollisionFlags() {
	this->collidedWithThisFrame.clear();
	this->resolvedWithThisFrame.clear();
}

void GameObject::toggleDebugMode() {
	GameObject::drawDebug = !GameObject::drawDebug;
}

Matrix3f GameObject::getNewPosition() {
	Matrix3f transform = (Matrix3f(0.0f, this->forces[0], this->forces[1], this->forces[2], 1.0f)).Multiply(
		((Matrix3f(0.0f, 0.0f, 0.0f, 0.0f, this->scales[0], this->scales[1], this->scales[2])
			).Multiply(Matrix3f(this->zTorque*(3.1415926f / 180.0f), 0.0f, 0.0f, 0.0f, 1.0f)
			)
			));

	return this->worldSpaceTransform->Multiply(transform);
}

//getProgressAcrossTrackSegment() Function used for track only
float GameObject::getProgressAcrossTrackSegment(int segIndex, Vect4f &worldPosition, int step) {
	int size = (int)this->collisionBounds.size();
	int tmp = segIndex;
	int prev = -1;
	int next = -1;
	int curr = -1;

	//reverse the forward direction for reverse (i.e. step < 0)
	/*if (step > 0) {
		prev = (segIndex - step < 0) ? size - 1 : segIndex - step;
		next = (segIndex + step >= size) ? 0 : segIndex + step;
	}
	else {
		next = (segIndex + step < 0) ? size - 1 : segIndex + step;
		prev = (segIndex - step >= size) ? 0 : segIndex - step;
	}*/

	curr = getNextIndexStep(segIndex, 0);
	prev = getNextIndexStep(segIndex, -step);
	next = getNextIndexStep(segIndex, step);


	Vect4f currSeg = boundSpaceToObjectSpace(Vect4f(this->collisionBounds.at(curr)->centreX, this->collisionBounds.at(segIndex)->centreY, 0.0f));
	Vect4f prevSeg = boundSpaceToObjectSpace(Vect4f(this->collisionBounds.at(prev)->centreX, this->collisionBounds.at(prev)->centreY, 0.0f));
	Vect4f nextSeg = boundSpaceToObjectSpace(Vect4f(this->collisionBounds.at(next)->centreX, this->collisionBounds.at(next)->centreY, 0.0f));

	float total = (nextSeg.subtract(currSeg).getXYMagnitude()) + (currSeg.subtract(prevSeg).getXYMagnitude());
	float current = (nextSeg.subtract(worldPosition).getXYMagnitude());

	float angle = this->getAngleBetweenPositions(nextSeg, worldPosition);

	while (angle >= 90.0f) {
		angle -= 90.0f;
	}

	//TODO: more accurate calculation would be to project the worldPosition onto the axis between prevSeg and nextSeg
	//use trig to get the distance along the relative x of vector "current"
	float relativeXComponent = current * (((angle >= 90.0f && angle <= 180.0f) || (angle <= 360.0f && angle >= 270.0f)) ? sinf(angle*(3.1415926f / 180.0f)) : cosf(angle*(3.1415926f / 180.0f)));

	//approaches 1.0 as worldPosition approaches nextSeg
	return (total - relativeXComponent) / total;

}

void GameObject::setIgnoreContainers(bool flag) {
	this->ignoreContainers = flag;
}

CollisionRadii* GameObject::getRadiiAt(int index) {
	if (((int)this->collisionBounds.size()) <= index) return NULL;
	return this->collisionBounds.at(index);
}

float GameObject::getXYScale() {
	return ((this->scales[0] + this->scales[1]) / 2.0f);
}