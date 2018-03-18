#pragma once

#include <windows.h>		// Header File For Windows
#include <string>			// used for strings
#include <sstream>			// used for streaming text
#include "Image_Loading\nvImage.h"
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#ifndef KGE_UTILS

	#define KGE_UTILS
	using std::string;
	using std::fixed;
	using std::stringstream;

	class utils {
		private:
			static GLuint base;

		public:
			static string doubleToString(double val);
			static GLvoid BuildFont(HDC hDC);
			static GLvoid KillFont(GLvoid);
			static GLvoid glPrint(string fmt);
			static GLuint loadPNG(std::string name);
	};

#endif