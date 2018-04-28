#pragma once

#include <string>
#include <vector>

#include "kgeMatrix.h"
#include "kgeVect.h"
#include "kgeUtils.h"

#ifndef KGE_GEOMETRY

#define KGE_GEOMETRY

using std::vector;
using std::string;

class GameObject;

struct CollisionResult {

public:
	inline CollisionResult() {
		this->distanceSqrd = 0.0f;
		this->point = Vect4f();
		this->other = NULL;
	}

	inline ~CollisionResult() {
		//no longer reference the given object and leave it in memory
		this->other = NULL;
	}

	inline CollisionResult(const CollisionResult &copy){
		this->distanceSqrd = copy.distanceSqrd;
		this->other = copy.other;
		this->point = copy.point;
	}

	GameObject* other;
	float distanceSqrd;
	Vect4f point;

};

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

	~CollisionRadii() {
		this->radii.clear();
		this->angles.clear();
		delete drawColor;
	}
	
	inline CollisionRadii(const CollisionRadii &copy) {
		
		this->radii = {};
		this->angles = {};

		if (copy.radii.size() == copy.angles.size()) {
			for (int i = 0; i < (int)copy.radii.size(); i++) {
				this->angles.push_back(copy.angles.at(i));
				this->radii.push_back(copy.radii.at(i));
			}
		}

		this->drawColor = new Color4f(copy.drawColor->r, copy.drawColor->g, copy.drawColor->b, copy.drawColor->a);
		this->centreX = copy.centreX;
		this->centreY = copy.centreY;
		this->trigonometricInterpolation = copy.trigonometricInterpolation;

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
		//float myAngle = angle;

		//while (myAngle > 360.0f) {
		//	myAngle -= 360.0f;
		//}

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
		
		float radius = 1.0f;
		
		//if (firstAngle - secondAngle >= 90.0f) {
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


				int smallest = (this->angles.at(first) <= this->angles.at(second)) ? first : second;
				int largest = (smallest == first) ? second : first;

				float rl = this->radii.at(largest);
				float rs = this->radii.at(smallest);

				float al_rads = this->angles.at(largest)*(3.1415926f / 180.0f);
				float as_rads = this->angles.at(smallest)*(3.1415926f / 180.0f);
				float al_x_ratio = cosf(al_rads);
				float al_y_ratio = sinf(al_rads);
				float as_x_ratio = cosf(as_rads);
				float as_y_ratio = sinf(as_rads);

				float x_2 = (rl * al_x_ratio);
				float y_2 = (rl * al_y_ratio);

				float x_1 = (rs * as_x_ratio);
				float y_1 = (rs * as_y_ratio);

				float largestAngle = this->angles.at(largest);
				float smallestAngle = this->angles.at(smallest);

				if (largest == first) {
					//if the largest is the first then we have to wrap so we interpolate correctly
					if (angle >= largestAngle) angle = angle - 360.0f;
					largestAngle = largestAngle - 360.0f;
				}

				float interpolationPercentage = ((angle - smallestAngle) / (largestAngle - smallestAngle));

				radius = (
					Vect4f(
					((x_2*interpolationPercentage) + (x_1*(1.0f - interpolationPercentage)))
						, ((y_2*interpolationPercentage) + (y_1*(1.0f - interpolationPercentage)))
						, 0.0f
						, 1.0f
					)
					).getXYMagnitude();
			}
			else {
				//linear interpolation results in a circular edge
				//float interpolationPercentage = angle / (((this->angles.at(second) + this->angles.at(first))));

				int smallest = (this->angles.at(first) <= this->angles.at(second)) ? first : second;
				int largest = (smallest == first) ? second : first;
				float largestAngle = this->angles.at(largest);
				float smallestAngle = this->angles.at(smallest);
				if (largest == first) {
					//if the largest is the first then we have to wrap so we interpolate correctly
					if (angle >= largestAngle) angle = angle - 360.0f;
					largestAngle = largestAngle - 360.0f;
				}
				float interpolationPercentage = ((angle - smallestAngle) / (largestAngle - smallestAngle));

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

		//cache?
		//if close-to-between two other radii
		//if (abs(difference) - 0.5f < 0.05f) {
		//	this->addRadius(radius, angle);
		//}


		return radius;
	}
};


enum UIType {
	BUTTON = 1, TEXTBOX = 2, LABEL = 3
};
struct UIElement {
public:
	Vect4f* location;
	Vect4f* size;
	string value;
	Color4f* textColor;
	Color4f* boxColor;
	Color4f* altColor;
	bool enabled;
	bool focused;
	UIType type;
	bool focusedColor;
	font_data* font;
	bool clicked;
	double clickedAt;
	double changedAt;

	inline ~UIElement() {
		
		delete location;
		delete size;
		delete textColor;
		delete boxColor;
		delete altColor;
	}

	inline UIElement(const UIElement &copy) {
		this->location = new Vect4f(copy.location->x, copy.location->y, copy.location->z, copy.location->w);
		Vect4f* size = new Vect4f(copy.size->x, copy.size->y, copy.size->z, copy.size->w);
		string value = copy.value;

		this->textColor = new Color4f(copy.textColor->r, copy.textColor->g, copy.textColor->b, copy.textColor->a);
		this->boxColor = new Color4f(copy.boxColor->r, copy.boxColor->g, copy.boxColor->b, copy.boxColor->a);
		this->altColor = new Color4f(copy.altColor->r, copy.altColor->g, copy.altColor->b, copy.altColor->a);
		this->font = copy.font;
		
		
	}

	//font is not copy-assigned so must be a persistent object that UIElement can store reference to, i.e. an element in Loop::fonts
	UIElement(Vect4f &location, Vect4f &size, string value, UIType type, font_data &font) {
		this->location = new Vect4f(location);
		this->size = new Vect4f(size);
		this->value = value;
		this->type = type;

		this->textColor = new Color4f(0.9f, 0.9f, 0.9f, 1.0f);
		this->boxColor = new Color4f(0.1f, 0.1f, 0.1f, 0.4f);
		this->altColor = new Color4f(0.5f, 0.5f, 0.5f, 0.4f);
		this->enabled = true;
		this->focused = false;
		this->focusedColor = false;

		//Don't copy font because it is should be referenced, e.g. from the static Loop::font vector
		this->font = &font;

		this->clicked = false;
		this->clickedAt = -1.0;
		this->changedAt = -1.0;
	}
	void setValue(string value) {
		this->value = value;
	}
};

#endif