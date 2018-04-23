#pragma once

#include <vector>
#include <string>

#include "kgeUtils.h"
#include "kgeGeometry.h"

#include "InputStates.h"
#include "DebugInfo.h"
#include "RaceData.h"

#ifndef GAME_OBJECT_H

#define GAME_OBJECT_H

using std::string;
using std::vector;


enum AnimationLogic {
	PINGPONG = 1, LOOPEND = 2
};

class GameObject {
	protected:

		static DebugInfo* debugger;
		//Matrix to convert world to camera space
		static Matrix3f worldToCamera;
		static bool drawDebug;

		string name;

		int pingPongDirection;
		int activeSpriteIndex;
		vector<GLuint> sprites;

		Color4f* objectColor;

		// openGL Matrix indices
		//|0, 4, 8,  12|
		//|1, 5, 9,  13|
		//|2, 6, 10, 14|
		//|3, 7, 11, 15|
		Matrix3f* worldSpaceTransform;
		Matrix3f* boundsTransform;

		GLfloat scales[3];
		
		vector<Vertex> mesh;

		//A list of meshes representing radiuses from centres
		vector<CollisionRadii*> collisionBounds;
		GLfloat boundScales[3];
		bool collided;
		bool colliderFlag;
		bool ghost;
		bool physicsContainer;
		vector<string> collidedWithThisFrame;

		bool physics;
		GLfloat forces[3];
		GLfloat zTorque;
		GLfloat friction;
		GLfloat angularDamping;
		float topSpeed;

		void setCollisionBounds(const vector<CollisionRadii*> &bounds);

	public:
		static GLuint defaultSprite;
		static void setDebugMode(bool flag);
		static bool getDebugMode();
		static void setDebugger(DebugInfo &_debugger);
		static void setWorldToCameraTransform(Matrix3f &wtc);
		static void toggleDebugMode();
		static float getAngleBetweenPositions(Vect4f &a, Vect4f &b);
		static void drawCircle(float x, float y, float radius, Color4f* col);
		static void drawLine(float x1, float y1, float x2, float y2);
		
		~GameObject();
		GameObject();
		GameObject(const GameObject &copy);
		GameObject(string name, vector<GLuint> &sprites, vector<Vertex> &mesh, int activeSprite, Color4f &objectColor);
		virtual void freeData();
		//void freeTextures();
		
		void animate(AnimationLogic al);
		virtual void draw();
		virtual vector<CollisionResult> resolveCollisions(const vector<GameObject*> &others);

		string toString();
		string getName();
		void setPhysicalAttributes(float friction, float angularDamping, float topSpeed);
		bool hasPhysics();
		void setPhysics(bool flag); 
		void setSprite(int index);

		Matrix3f getNewPosition();
		Vect4f getWorldPosition();
		float getZAngle();
		void setZAngle(float angle);
		Vect4f localToWorldSpace(Vect4f &localCoords);
		Vect4f boundSpaceToObjectSpace(Vect4f &localCoords);
		float radius2DToObjectSpace(float radius, float angle);
		float radius2DToWorldSpace(float radius, float angle);
		void applyBoundsScale();
		void scale(float uScale, bool scaleCollider);
		void nuScale(float x, float y, float z, bool scaleCollider);
		void nuScaleBounds(float x, float y, float z);
		void translate(float x, float y, float z);
		void translateBounds(float x, float y, float z);

		int countCollisionRadii(); 
		void setCollisionBounds(const vector<CollisionRadii> &bounds);
		void setCollisionBounds(const CollisionRadii &bounds);


		bool isCollider();
		void resetModifiers(); 
		void setCollider(bool flag);
		bool hasResolvedWith(string name);
		void setCollisionResolvedWith(string name);
		void resetCollisionFlags();
		CollisionRadii* getClosestRadiiTo(Vect4f &otherPosition);
		int getIndexOfClosestRadiiTo(Vect4f &otherPosition);
		CollisionRadii* getNextCollisionRadiiFor(Vect4f &otherPosition, int step);
		float getAngleToOther(GameObject &other);
		float getAngleToPosition(Vect4f &position);
		float getAngleFromX();

		void addForce(float x, float y, float z);
		void setGhost(bool flag);
		bool isGhost();
		void setPhysicsContainer(bool flag);
		bool isPhysicsContainer();

		float getProgressAcrossTrackSegment(int segIndex, Vect4f &worldPosition, int step);
		
};



#endif