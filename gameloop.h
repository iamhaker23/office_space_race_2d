#pragma once
#include "DebugInfo.h"
#include "Sprites.h"
#include "kgeUtils.h"
#include "freetype.h"		// Header for font library.
#include <windows.h>

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

		public:
			
			static void init(HDC _hDC, DebugInfo* _debugger);
			static void display();

	};

#endif
