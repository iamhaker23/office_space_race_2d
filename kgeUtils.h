#pragma once

#include <windows.h>		// Header File For Windows
#include <string>			// used for strings
#include <sstream>			// used for streaming text
#include "Image_Loading\nvImage.h"
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "freetype.h"

#ifndef KGE_UTILS

	#define KGE_UTILS
	using std::string;
	using std::fixed;
	using std::stringstream;
	using std::vector;
	using freetype::font_data;
	
	class utils {
		private:
			static GLuint base;

		public:

			static string doubleToString(double val);
			static string intToString(int val);
			static string floatToString(float val);

			static string floatLabel(char* prefix, float val, char* suffix);
			static string intLabel(char* prefix, int val, char* suffix);
			static string lapsLabel(int completed, int laps);

			static GLvoid BuildFont(HDC hDC);
			static GLvoid KillFont(GLvoid);
			static GLvoid glPrint(string fmt);

			static nv::Image* loadPNG(std::string name);

			static void bindTexture(GLuint tex);
			//static void utils::freeTexture(GLuint tex);

			static void enableTextureBlending();
			static vector<string> getFileContents(char* filename);
			static vector<string> readLines(FILE* fptr);
			static vector<string> splitString(string input, char delimiter);
			static string getPositionLabel(int val);


	};

#endif