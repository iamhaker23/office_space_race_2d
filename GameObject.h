#pragma once

#include <vector>
#include <string>

#include "kgeUtils.h"
#include "kgeGeometry.h"

#include "InputStates.h"
#include "DebugInfo.h"

#ifndef GAME_OBJECT_H

#define GAME_OBJECT_H

using std::string;
using std::vector;


enum AnimationLogic {
	PINGPONG = 1, LOOPEND = 2
};

class GameObject {
	private:

		static nv::Image* defaultSprite;
		int activeSpriteIndex;
		vector<nv::Image*> sprites;

		Color4f* objectColor;

		// openGL Matrix indices
		//|0, 4, 8,  12|
		//|1, 5, 9,  13|
		//|2, 6, 10, 14|
		//|3, 7, 11, 15|
		Matrix3f* worldSpaceTransform;
		Matrix3f* boundsTransform;

		GLfloat forces[3];
		GLfloat zTorque;
		GLfloat scales[3];
		GLfloat boundScales[3];
		
		GLfloat friction;
		GLfloat angularDamping;
				
		vector<Vertex> mesh;

		//A list of meshes representing radiuses from centres
		vector<CollisionRadii*> collisionBounds;

		int pingPongDirection;
		string name;

		bool playerControl;
		float topSpeed;
		bool colliderFlag;
		bool ghost;
		bool physicsContainer;

		static DebugInfo* debugger;
		//Matrix to convert world to camera space
		static Matrix3f* worldToCamera;
		static bool drawDebug;
		
		vector<string> collidedWithThisFrame;

		bool physics;
		bool steering;

	public:
		static void setDebugMode(bool flag);
		static void setDebugger(DebugInfo* _debugger);
		static void setWorldToCameraTransform(Matrix3f* wtc);
		static void freeData();
		
		~GameObject();
		GameObject();
		GameObject(const GameObject &copy);
		GameObject(string name, vector<nv::Image*> sprites, vector<Vertex> mesh, int activeSprite, Color4f* objectColor);
		void setSprite(int index);
		void animate(AnimationLogic al);
		string toString();

		void draw();
		void processInputs(InputStates* inputs);

		void scale(float uScale, bool scaleCollider);
		void nuScale(float x, float y, float z, bool scaleCollider);
		void nuScaleBounds(float x, float y, float z);
		void translate(float x, float y, float z);
		void translateBounds(float x, float y, float z);

		void setPhysicalAttributes(float friction, float angularDamping, float topSpeed);
		Vect4f* getWorldPosition();
		Vect4f* getScreenPosition();
		float getZAngle();
		void setPlayerControl(bool playerControl);
		string getName();
		void resetModifiers();
		void setCollisionBounds(vector<CollisionRadii*> bounds);
		float getRadiusToObj(GameObject* other);
		void resolveCollisions(vector<GameObject*> others);

		bool isCollider();
		void setCollider(bool flag);

		static void drawCircle(float x, float y, float radius, Color4f* col);
		static void drawLine(float x1, float y1, float x2, float y2);

		bool hasCollidedWith(string name);
		void setHasCollidedWith(string name);
		void resetCollisionFlags();
		CollisionRadii* getClosestRadiiTo(Vect4f* otherPosition);

		float getAngleToOther(GameObject* other);
		//float getRadiusAtAngle(float angleToOther, Vect4f* otherPosition);
		Vect4f* localToWorldSpace(Vect4f localCoords);
		Vect4f* boundSpaceToObjectSpace(Vect4f localCoords);

		void addForce(float x, float y, float z);
		bool hasPhysics();
		void setPhysics(bool flag);

		void setGhost(bool flag);
		bool isGhost();
		void setPhysicsContainer(bool flag);
		bool isPhysicsContainer();

		float radius2DToObjectSpace(float radius, float angle);
		float radius2DToWorldSpace(float radius, float angle);
		void applyBoundsScale();

};


#endif