#include "kgeUtils.h"

GLuint utils::base = NULL;

string utils::doubleToString(double val) {
	stringstream ss;
	ss.precision(2);
	ss << fixed << val;
	return ss.str();
}

string utils::intToString(int val) {
	stringstream ss;
	ss.precision(0);
	ss << fixed << val;
	return ss.str();
}

GLvoid utils::BuildFont(HDC hDC)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	utils::base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(-30,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_NORMAL,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		"Times New Roman");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, utils::base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid utils::KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(utils::base, 96);							// Delete All 96 Characters
}

GLvoid utils::glPrint(string fmt)					// Custom GL "Print" Routine
{
	stringstream stream;
	stream << fmt;

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(utils::base - 32);								// Sets The Base Character to 32
														// first 32 chars not used
	glCallLists(stream.str().size(), GL_UNSIGNED_BYTE, stream.str().c_str());	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}


nv::Image* utils::loadPNG(std::string name)
{
	// Texture loading object
	nv::Image* img = new nv::Image();
	
	// Return true on success
	if (img->loadImageFromFile(name.c_str()))
	{
		return img;
	}
	else
		return NULL;
}


void utils::bindPNG(nv::Image* img) {

	GLuint myTextureID;

	if (img != NULL) {
		//I once removed this line
		//And created an odd bug where all my font changed to tiny image textures
		//THAT was a headache!
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img->getInternalFormat(), img->getWidth(), img->getHeight(), 0, img->getFormat(), img->getType(), img->getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	//return myTextureID;
}

void utils::enableTextureBlending() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}
