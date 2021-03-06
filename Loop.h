#pragma once

#pragma once
#include "DebugInfo.h"
#include "kgeUtils.h"
#include <windows.h>
#include "Camera.h"
#include "GameData.h"
//#include "win32_window.h"

#ifndef LOOP_H

#define LOOP_H

struct TextureCache {

public:

	//TODO: consider refactoring startTime into loop, as most loops use it!

	vector<GLuint> textureIDs;
	vector<std::string> textures;
	TextureCache() {
		this->textureIDs = {};
		this->textures = {};
	}
	void clear() {
		for (GLuint i : this->textureIDs) {
			glDeleteTextures(1, &i);
		}
		glFinish();
		this->textureIDs.clear();
		this->textures.clear();
	}
};

class Loop {
		
	public:

		static TextureCache cache;
		static DebugInfo* debugger;
		static HDC* hDC;
		static InputStates* inputs;
		static Camera* camera;
		static vector<font_data*> fonts;
		static float screenHeight;
		static float screenWidth;
		static int requestedActiveLoop;
		static GameData* globals;
	
		static void writeMessage(string str);
		static void writeMessage(string str, float h, float w);
		static void writeMessage(string str, float h, float w, Color4f &textColor, Color4f &boxColor);
		static void drawTextBox(freetype::font_data &_font, string _str, float ssOffsetX, float ssOffsetY, float boxXSize, float boxYSize, Color4f &textColor, Color4f &boxColor);
		static void drawBackground(GLuint image, float repeat, Color4f &tintColor);
		static void drawBackground(GLuint image, float repeat, Color4f &tintColor, Vect4f &ss_offset);

		static GLuint initTexture(std::string name);
		static GLuint getTexture(std::string name);

		virtual void freeData();
		static void Loop::freeAllStaticData();
		void resetData(bool retainGlobals);
		virtual void resetData() = 0;
		virtual void handleActivation() = 0;
		

		double loopStarted;

		static void Loop::addFont(const char* name, unsigned int size);
		static void init(HDC &_hDC, DebugInfo &_debugger, InputStates &inputs);
		virtual void display() = 0;

		vector<UIElement*> UI;
		void addUI(Vect4f &location, Vect4f &size, string value, const UIType &type, font_data &font); //called in subclass
		void processUI();//called in window
		void drawUI(); //called in window

		Loop();

};

#endif
