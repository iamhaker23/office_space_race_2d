#pragma once
#include "DebugInfo.h"
#include "kgeUtils.h"
#include "freetype.h"		// Header for font library.
#include <windows.h>
#include "win32_window.h"
#include "GameObject.h"
#include "Camera.h"

#ifndef GAMELOOP_H

	#define GAMELOOP_H
	using freetype::font_data;

	class GameLoop {
		

		private:
			static DebugInfo* debugger;
			static HDC hDC;
			static int frame;
			static vector<font_data*> fonts;
			static nv::Image* backgroundPNG;
			static InputStates* inputs;

			static void drawTextBox(freetype::font_data _font, string _str, float ssOffsetX, float ssOffsetY, float boxXSize, float boxYSize, Color4f textColor, Color4f boxColor);
			static void drawBackground(float repeat, float tintColor[]);

		public:
			static void freeData();
			static Camera* camera;
			static void init(HDC _hDC, DebugInfo* _debugger);
			static void display();

	};

#endif
