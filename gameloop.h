#pragma once
#include "DebugInfo.h"
#include "kgeUtils.h"
#include "freetype.h"		// Header for font library.
#include <windows.h>
#include "win32_window.h"
#include "GameObject.h"

#ifndef GAMELOOP_H

	#define GAMELOOP_H
	using freetype::font_data;

	class gameloop {
		

		private:
			static DebugInfo* debugger;
			static HDC hDC;
			static GLuint myTexture;
			static int frame;
			static vector<font_data*> fonts;
			
			static void drawTextBox(freetype::font_data _font, string _str, float ssOffsetX, float ssOffsetY, float boxXSize, float boxYSize, float textColor[], float boxColor[]);
			static void drawBackground(string png, float repeat, float tintColor[]);

		public:
			
			static void init(HDC _hDC, DebugInfo* _debugger);
			static void display();

	};

#endif
