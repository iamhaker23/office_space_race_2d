#pragma once

#pragma once
#include "DebugInfo.h"
#include "kgeUtils.h"
#include "freetype.h"		// Header for font library.
#include <windows.h>
#include "GameObject.h"
#include "Camera.h"
//#include "win32_window.h"

#ifndef LOOP_H

#define LOOP_H
using freetype::font_data;

class Loop {
		
	public:

		static DebugInfo* debugger;
		static HDC hDC;
		static InputStates* inputs;
		static Camera* camera;
		static vector<font_data*> fonts;

		static float screenHeight;
		static float screenWidth;
		static int requestedActiveLoop;
	
		static void writeMessage(string str);
		static void writeMessage(string str, float h, float w);
		static void writeMessage(string str, float h, float w, Color4f textColor, Color4f boxColor);
		static void drawTextBox(freetype::font_data _font, string _str, float ssOffsetX, float ssOffsetY, float boxXSize, float boxYSize, Color4f textColor, Color4f boxColor);
		static void drawBackground(nv::Image* image, float repeat, Color4f tintColor);

		static void freeStaticData();

		virtual void freeData() = 0;
		virtual void init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs) = 0;
		virtual void display() = 0;


};

#endif
