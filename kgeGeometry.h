#pragma once

#include <string>
#include <vector>

#include "kgeMatrix.h"
#include "kgeVect.h"

#ifndef KGE_GEOMETRY

#define KGE_GEOMETRY

using std::vector;
using std::string;

struct Color4f {
public:
	float r;
	float g;
	float b;
	float a;
	Color4f() {
		this->r = 1.0f;
		this->g = 1.0f;
		this->b = 1.0f;
		this->a = 1.0f;
	}
	Color4f(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	Color4f(const Color4f &copy) {
		this->r = copy.r;
		this->g = copy.g;
		this->b = copy.b;
		this->a = copy.a;
	}

};

struct Vertex {
public:
	float x;
	float y;
	float u;
	float v;
	Vertex(float u, float v, float x, float y) {
		this->u = u;
		this->v = v;
		this->x = x;
		this->y = y;
	}
};

struct CollisionRadii {
public:
	float centreX;
	float centreY;
	bool trigonometricInterpolation;

	//TODO: should strictly be an ordered map
	vector<float> radii;
	vector<float> angles;
	Color4f* drawColor;
	int lastSelected[2] = { -1, -1 };

	~CollisionRadii() {
	}

	CollisionRadii(float x, float y) {
		this->centreX = x;
		this->centreY = y;
		this->radii = {};
		this->angles = {};
		drawColor = new Color4f();
		trigonometricInterpolation = false;
	}

	//IMPORTANT: MUST PROVIDE ANGLES IN ASC ORDER, AND THEIR RADII AT CORRESPONDING INDEXES
	CollisionRadii(float x, float y, vector<float> angles, vector<float> radii) {
		this->centreX = x;
		this->centreY = y;
		this->angles = angles;
		this->radii = radii;

		trigonometricInterpolation = false;
		drawColor = new Color4f();
	}
	inline void setLastSelected(int lasta, int lastb) {
		this->lastSelected[0] = lasta;
		this->lastSelected[1] = lastb;
	}

	inline int* getLastSelected() {
		return this->lastSelected;
	}

	inline void CollisionRadii::setDrawColor(float r, float g, float b, float a) {
		drawColor = new Color4f(r, g, b, a);
	}

	inline void setInterpolationTrigO(bool flag) {
		this->trigonometricInterpolation = flag;
	}

	inline Color4f* CollisionRadii::getDrawColor() {
		return this->drawColor;
	}

	inline void CollisionRadii::addRadius(float radius, float angle) {

		if ((int)this->angles.size() == 0) {
			//first addition
			this->radii.push_back(radius);
			this->angles.push_back(angle);
			return;
		}

		vector<float> newRadii = {};
		vector<float> newAngles = {};

		bool added = false;

		for (int i = 0; i < (int)this->angles.size(); i++) {
			float currentRadius = this->radii.at(i);
			float currentAngle = this->angles.at(i);
			if (!added) {
				if (currentAngle < angle) {
					//still before insert position
					newRadii.push_back(currentRadius);
					newAngles.push_back(currentAngle);
				}
				else if (currentAngle == angle) {
					//update, don't include the old
					added = true;
					newRadii.push_back(radius);
					newAngles.push_back(currentAngle);
				}
				else if (currentAngle > angle){
					//insert new ones first
					added = true;
					newRadii.push_back(radius);
					newAngles.push_back(angle);
					newRadii.push_back(currentRadius);
					newAngles.push_back(currentAngle);
				}
			}
			else {
				//inserted new radius, so simply add the remaining to the new list
				newRadii.push_back(currentRadius);
				newAngles.push_back(currentAngle);
			}
		}

		if (!added) {
			//larger than all so, not added in loop
			newRadii.push_back(radius);
			newAngles.push_back(angle);
		}

		this->radii = newRadii;
		this->angles = newAngles;

	}

	inline void CollisionRadii::removeRadius(float radius, float angle) {
		vector<float> newRadii;
		vector<float> newAngles;

		for (int i = 0; i < (int)this->radii.size(); i++) {
			float currRadius = this->radii.at(i);
			float currAngle = this->angles.at(i);
			if (currRadius != radius && currAngle != angle) {
				newRadii.push_back(currRadius);
				newAngles.push_back(currAngle);
			}
		}

		//TODO: do I actually need to clear these first?
		this->radii.clear();
		this->angles.clear();

		this->radii = newRadii;
		this->angles = newAngles;
	}
	/*
	inline float CollisionRadii::getRadiusAtOLD(float angle) {

	if (this->angles.size() == 0) return 0.2f;

	//bound angle
	float myAngle = angle;

	while (myAngle > 360.0f) {
	myAngle -= 360.0f;
	}

	bool found = false;
	int index = 0;
	int maxIndex = this->angles.size() - 1;
	while (!found && index <= maxIndex) {
	float tmpAngle = this->angles.at(index);
	if (tmpAngle >= angle) {
	this->lastSelected[0] = index;
	this->lastSelected[1] = -1;
	return this->radii.at(index);
	}
	else index++;
	}

	//fallback - has radius values but none that are bigger than (i.e. capture in minor arc) my angle
	this->lastSelected[0] = 0;
	this->lastSelected[1] = -1;
	return this->radii.at(0);
	}
	*/
	inline float CollisionRadii::getInterpolatedRadiusAt(float angle) {

		if (this->angles.size() == 0) return 0.2f;

		//bound angle
		float myAngle = angle;

		while (myAngle > 360.0f) {
			myAngle -= 360.0f;
		}

		bool found = false;
		int index = 0;
		int maxIndex = this->angles.size() - 1;
		while (!found && index <= maxIndex) {
			float tmpAngle = this->angles.at(index);
			if (tmpAngle >= angle) {
				if (tmpAngle == angle) {
					return this->radii.at(index);
				}
				found = true;
			}
			else index++;
		}

		//wrap around, first is the largest angle available (this->angles.size - 1), otherwise the smaller of the two (index - 1)
		int first = (index == 0) ? this->angles.size() - 1 : index - 1;
		int second = (first == this->angles.size() - 1) ? 0 : index;


		float firstAngle = this->angles.at(first) * (3.1415926f / 180.0f);
		float secondAngle = this->angles.at(second) * (3.1415926f / 180.0f);

		float radius = 1.0f;
		
		//if (firstAngle - secondAngle >= 90.0f) {
			//use trigonometric interpolation
			float smallest = (firstAngle >= secondAngle) ? secondAngle : firstAngle;
			float absSinAngle = abs(sinf(angle * (3.1415926f / 180.0f)));
			float absCosAngle = abs(cosf(angle * (3.1415926f / 180.0f)));
			//interpolationPercentage = 0.0f;
			//if (abs(sinf(smallest * (3.1415926f / 180.0f))) > abs(cosf(smallest * (3.1415926f / 180.0f)))) {
			/*if (absSinAngle > absCosAngle){
				float Y_1 = abs(this->radii.at(first) * sinf(firstAngle));
				float Y_2 = abs(this->radii.at(second) * sinf(secondAngle));

				//float difference = secondRadius - firstRadius;
				float difference = Y_1 - Y_2;
				radius = ((interpolationPercentage * (abs(difference))) + ((difference > 0.0f) ? Y_2 : Y_1)) / absSinAngle;
				//float radius = (interpolationPercentage * (Y_1 + Y_2)) / sinf(angle * (3.1415926f / 180.0f));
			}
			else {

				float X_1 = abs(this->radii.at(first) * cosf(firstAngle));
				float X_2 = abs(this->radii.at(second) * cosf(secondAngle));

				//float difference = secondRadius - firstRadius;
				float difference = X_1 - X_2;
				radius = ((interpolationPercentage * (abs(difference))) + ((difference > 0.0f) ? X_2 : X_1)) / absCosAngle;
			}*/
			if (this->trigonometricInterpolation) {

				float interpolationPercentage = angle / (((this->angles.at(second) + this->angles.at(first)))/2.0f);

				float Y_1 = abs(this->radii.at(first) * sinf(firstAngle));
				float Y_2 = abs(this->radii.at(second) * sinf(secondAngle));
				float differenceY = Y_1 - Y_2;
				float radiusS = ((interpolationPercentage * (abs(differenceY))) + ((differenceY > 0.0f) ? Y_2 : Y_1)) / absSinAngle;
				float X_1 = abs(this->radii.at(first) * cosf(firstAngle));
				float X_2 = abs(this->radii.at(second) * cosf(secondAngle));
				float differenceX = X_1 - X_2;
				float radiusC = ((interpolationPercentage * (abs(differenceX))) + ((differenceX > 0.0f) ? X_2 : X_1)) / absCosAngle;

				radius = min(radiusC, radiusS);
				radius = max(0.2f, radius);
				//radius = (radiusC > this->radii.at(first)) ? radiusS : radiusC;
			}
			else {

				return this->radii.at(first);
				
				float interpolationPercentage = angle / (((this->angles.at(second) + this->angles.at(first)))/2.0f);
				float firstRadius = this->radii.at(first);
				float secondRadius = this->radii.at(second);

				float difference = firstRadius - secondRadius;

				radius = ((difference < 0.0f) ? firstRadius : secondRadius) + (abs(difference) * interpolationPercentage);
				
			}

			
		/*}
		else {
			//use linear interpolation
			float firstRadius = this->radii.at(first);
			float secondRadius = this->radii.at(second);
			
			float difference = firstRadius - secondRadius;

			radius = ((difference > 0.0f) ? firstRadius : secondRadius) + (abs(difference) * interpolationPercentage);
		}*/

		//enable drawing this radius as green

		/*float absSinAngle = abs(sinf(angle * (3.1415926f / 180.0f)));
		float absCosAngle = abs(cosf(angle * (3.1415926f / 180.0f)));

		float mutingS = (absSinAngle <= 0.2f) ? 0.0f : 1.0f;
		float mutingC = (mutingS != 0.0f && absCosAngle <= 0.2f) ? 0.0f : 1.0f;

		float Y_1 = abs(this->radii.at(first) * sinf(firstAngle));
		float Y_2 = abs(this->radii.at(second) * sinf(secondAngle));

		//float difference = secondRadius - firstRadius;
		float differenceY = Y_1 - Y_2;

		float X_1 = abs(this->radii.at(first) * cosf(firstAngle));
		float X_2 = abs(this->radii.at(second) * cosf(secondAngle));

		//float difference = secondRadius - firstRadius;
		float differenceX = X_1 - X_2;

		radius = (
				((((interpolationPercentage * (abs(differenceY))) + ((differenceY > 0.0f) ? Y_2 : Y_1)) / absSinAngle)*mutingS)
				+
				((((interpolationPercentage * (abs(differenceX))) + ((differenceX > 0.0f) ? X_2 : X_1)) / absCosAngle)*mutingC)
			)
			/ (mutingC + mutingS);

			*/
		this->lastSelected[0] = first;
		this->lastSelected[1] = second;

		//cache?
		//if close-to-between two other radii
		//if (abs(difference) - 0.5f < 0.05f) {
		//	this->addRadius(radius, angle);
		//}


		return radius;
	}
};



#endif