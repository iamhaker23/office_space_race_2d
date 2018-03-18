#include "gameloop.h"


//Initialise static members
GLuint gameloop::myTexture = NULL;
int gameloop::frame = 0;
HDC gameloop::hDC = NULL;
DebugInfo* gameloop::debugger = NULL;
vector<font_data*> gameloop::fonts;

void gameloop::init(HDC _hDC, DebugInfo* _debugger)
{
	hDC = _hDC;
	debugger = _debugger;

	glClearColor(0.0, 0.0, 0.0, 0.0);

	utils::BuildFont(hDC);

	font_data* font1 = new font_data();
	font1->init("BKANT.TTF", 12);
	fonts.push_back(font1);

}

void gameloop::display() {

	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPushMatrix();

	myTexture = utils::loadPNG("sky.png");
	glEnable(GL_TEXTURE_2D);
	//glColor3f(1,0,0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-1, -1);
	glTexCoord2f(0.0, 5.0); glVertex2f(-1, 1);
	glTexCoord2f(5.0, 5.0); glVertex2f(1, 1);
	glTexCoord2f(5.0, 0.0); glVertex2f(1, -1);
	glEnd();
	glPopMatrix();

	//Start Character
	//Using string to minimise overflow risk
	//std::string character_sprite = (++frame%100 < 50) ? "character_alpha.png" : "character_alpha_2.png";

	std::string character_sprite = sprites[(frame++) % 960];

	myTexture = utils::loadPNG(character_sprite);
	glEnable(GL_TEXTURE_2D);

	glColor3f(1, 1, 1);
	glPushMatrix();
	glScalef(1.2f, 1.2f, 1.2f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.5);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.5);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.5);

	glEnd();
	glPopMatrix();
	//End Character
	glDisable(GL_TEXTURE_2D);


	if (debugger != NULL) {

		freetype::print(*fonts.front(), 100.0f, 100.0f, debugger->getOutput().c_str());
		
		glPushMatrix();
		glColor4f(0, 0, 0, 0.5);
		glBegin(GL_QUADS);

		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.5, -0.5);

		glEnd();
		glPopMatrix();
		
	}
	
}
