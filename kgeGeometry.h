#pragma once

#include <string>
#include <vector>

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

};

struct Vect4f {
	public:
		
		float x;
		float y;
		float z;
		float w;
		Vect4f(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Vect4f() {
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
			this->w = 0.0f;
		}
		Vect4f* add(Vect4f* other) {
			return new Vect4f(other->x + this->x, other->y + this->y, other->z + this->z, other->w + this->w);
		}
};

struct Matrix3f {

	public:
		// openGL Matrix indices
		//|0, 4, 8,  12|
		//|1, 5, 9,  13|
		//|2, 6, 10, 14|
		//|3, 7, 11, 15|
		GLfloat values[16];
		float scalex = 1.0f;
		float scaley = 1.0f;
		float scalez = 1.0f;
		
		~Matrix3f() {
			delete [] (GLfloat*)values;
		}

		Matrix3f() {
			initValues();
		}

		Matrix3f(const Matrix3f &copy) {
			this->scalex = copy.scalex;
			this->scaley = copy.scaley;
			this->scalez = copy.scalez;
			for (int i = 0; i < 16; i++) {
				this->values[i] = copy.values[i];
			}
		}
		
		void initValues() {
			for (int i = 0; i < 16; i++) {
				this->values[i] = 0.0f;
			}
			this->values[0] = 1.0f;
			this->values[5] = 1.0f;
			this->values[10] = 1.0f;
			this->values[15] = 1.0f;

		}

		Matrix3f(float radians, float x, float y, float z, float scale) {
			initValues();
			float cosRads = cos(radians);
			float sinRads = sin(radians);
			this->values[0] = cosRads;
			this->values[4] = -1.0f * sinRads;
			this->values[1] = sinRads;
			this->values[5] = cosRads;
			this->values[12] = x;
			this->values[13] = y;
			this->values[14] = z;

			values[10] = 1.0f;
			this->values[0] *= scale;
			this->values[5] *= scale;
			this->values[10] *= scale;
			this->scalex = scale;
			this->scaley = scale;
			this->scalez = scale;

		}
		Matrix3f(float radians, float x, float y, float z, float scalex, float scaley, float scalez ) {
			initValues();
			float cosRads = cos(radians);
			float sinRads = sin(radians);
			this->values[0] = cosRads;
			this->values[4] = -1.0f * sinRads;
			this->values[1] = sinRads;
			this->values[5] = cosRads;
			this->values[12] = x;
			this->values[13] = y;
			this->values[14] = z;

			values[10] = 1.0f;
			this->values[0] = scalex;
			this->values[5] = scaley;
			this->values[10] = scalez;
			this->scalex = scalex;
			this->scaley = scaley;
			this->scalez = scalez;

		}

		Vect4f* getPosition() {
			return new Vect4f(this->values[12], this->values[13], this->values[14], this->values[15]);
		}
		
		Matrix3f* Add(Matrix3f* other) {
			Vect4f* myPos = this->getPosition();
			Vect4f* otherPos = other->getPosition();
			return
				new Matrix3f(
					this->getZAngle() + other->getZAngle(),
					myPos->x + otherPos->x,
					myPos->y + otherPos->y,
					myPos->z + otherPos->z,
					this->scalex + other->scalex,
					this->scaley + other->scaley,
					this->scalez + other->scalez
					);
		}

		Matrix3f* Multiply(Matrix3f* other) {
			Matrix3f* output = new Matrix3f();

			for (int i = 0; i < 4; i++) {
				//|0, 4, 8,  12|
				//|1, 5, 9,  13|
				//|2, 6, 10, 14|
				//|3, 7, 11, 15|
				output->values[i] = 
					(this->values[i] * other->values[0]) +
					(this->values[i + 4] * other->values[1]) +
					(this->values[i + 8] * other->values[2]) +
					(this->values[i + 12] * other->values[3]);
				output->values[i + 4] =
					(this->values[i] * other->values[4]) +
					(this->values[i + 4] * other->values[5]) +
					(this->values[i + 8] * other->values[6]) +
					(this->values[i + 12] * other->values[7]);
				output->values[i + 8] =
				(this->values[i] * other->values[8]) +
					(this->values[i + 4] * other->values[9]) +
					(this->values[i + 8] * other->values[10]) +
					(this->values[i + 12] * other->values[11]);
				output->values[i + 12] =
					(this->values[i] * other->values[12]) +
					(this->values[i + 4] * other->values[13]) +
					(this->values[i + 8] * other->values[14]) +
					(this->values[i + 12] * other->values[15]);
			}

			return output;
		}

		/*Matrix3f* RotateRadians(float angle) {
			Vect4f* myPos = this->getPosition();
			return
				(new Matrix3f(
					angle,
					0.0f,
					0.0f,
					0.0f,
					1.0f))->Multiply(this);
		}
		Matrix3f* Translate(float x, float y, float z) {
			return
				(new Matrix3f(
					0.0f,
					x,
					y,
					z,
					1.0f))->Multiply(this);
		}
		Matrix3f* Scale(float x, float y, float z) {
			return
				(this)->Multiply(new Matrix3f(
					0.0f, 0.0f, 0.0f, 0.0f,
					x,
					y,
					z));
		}*/

		Matrix3f* RotateRadians(float angle) {
			Vect4f* myPos = this->getPosition();
			return
				(new Matrix3f(
					angle,
					0.0f,
					0.0f,
					0.0f,
					1.0f))->Add(this);
		}
		Matrix3f* Translate(float x, float y, float z) {
			return
				(this)->Add(new Matrix3f(
					0.0f,
					x,
					y,
					z,
					0.0f));
		}
		Matrix3f* Scale(float x, float y, float z) {
			return
				(this)->Add(new Matrix3f(
					0.0f, 0.0f, 0.0f, 0.0f,
					x,
					y,
					z));
		}


		Matrix3f* inverse() {
			Vect4f* pos = this->getPosition();
			return new Matrix3f(this->getZAngle()*-1.0f, pos->x*-1.0f, pos->y*-1.0f, pos->z*-1.0f, 1.0f / this->scalex, 1.0f / this->scaley, 1.0f / this->scalez);
			
		}

		float getZAngle() {
			//tanA = opp(y)/adj(x)
			return atan2f(this->values[1], this->values[0]);
		}

		void loadMatrix(GLfloat* _values) {
			for (int i = 0; i < 16; i++) {
				this->values[i] = _values[i];
			}
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

	//TODO: should strictly be an ordered map
	vector<float> radii;
	vector<float> angles;
	Color4f* drawColor;
	int lastSelected[2] = {-1, -1};

	~CollisionRadii() {
	}

	CollisionRadii(float x, float y) {
		this->centreX = x;
		this->centreY = y;
		this->radii = {};
		this->angles = {};
		drawColor = new Color4f();
	}

	//IMPORTANT: MUST PROVIDE ANGLES IN ASC ORDER, AND THEIR RADII AT CORRESPONDING INDEXES
	CollisionRadii(float x, float y, vector<float> angles, vector<float> radii) {
		this->centreX = x;
		this->centreY = y;
		this->angles = angles;
		this->radii = radii;

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

		vector<float> newRadii;
		vector<float> newAngles;

		bool added = false;

		for (int i = 0; i < (int)this->angles.size(); i++) {
			float currentRadius = this->radii.at(i);
			float currentAngle = this->angles.at(i);
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
			else {
				//insert new ones first
				added = true;
				newRadii.push_back(radius);
				newAngles.push_back(angle);
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

		int first = (index == 0) ? this->angles.size() - 1 : index - 1;
		int second = (first == this->angles.size() - 1) ? 0 : index;
		float interpolationPercentage = abs((this->angles.at(second) - this->angles.at(first))) / (angle * 100.0f);

		float firstRadius = this->radii.at(first);
		float secondRadius = this->radii.at(second);
		
		float difference = secondRadius - firstRadius;

		//enable drawing this radius as green
		this->lastSelected[0] = first;
		this->lastSelected[1] = second;
		
		//cache?
		//if close-to-between two other radii
		//if (abs(difference) - 0.5f < 0.05f) {
		//	this->addRadius(radius, angle);
		//}
		float radius = ((difference > 0.0f) ? firstRadius : secondRadius) + (difference * interpolationPercentage);

		return radius;
	}


};


#endif