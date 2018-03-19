#include "gameloop.h"


//Initialise static members
GLuint gameloop::myTexture = NULL;
int gameloop::frame = 0;
HDC gameloop::hDC = NULL;
DebugInfo* gameloop::debugger = NULL;
vector<font_data*> gameloop::fonts;
vector<GameObject> scene;

void gameloop::init(HDC _hDC, DebugInfo* _debugger)
{
	hDC = _hDC;
	debugger = _debugger;

	glClearColor(0.0, 0.0, 0.0, 0.0);

	utils::BuildFont(hDC);

	font_data* font1 = new font_data();
	font1->init("BKANT.TTF", 16);
	fonts.push_back(font1);

	vector<string> truckSprites{ "truck/1.png","truck/2.png","truck/3.png","truck/4.png","truck/5.png","truck/6.png",
		"truck/6.png","truck/5.png","truck/4.png","truck/3.png","truck/2.png","truck/1.png" };
	GameObject truck = GameObject(truckSprites, 0, new float[4] {1.0f, 0.3f, 0.3f, 1.0f});
	scene.push_back(truck);

}

void gameloop::display() {

	
	glClear(GL_COLOR_BUFFER_BIT);
	utils::enableTextureBlending();

	float bgColor[] = { 0.6f, 0.9f, 0.9f, 0.5f };
	drawBackground("bg.png", 2,  bgColor);

	
	for (GameObject obj : scene) {
		obj.draw();
	}

	if (win32_window::isLeftPressed()) {
		scene.at(0).pingPongSprite();
	}

	if (debugger != NULL) {

		float textColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float boxColor[] = { 0.1f, 0.1f, 0.1f, 0.8f };
		drawTextBox(*fonts.front(), debugger->getOutput(), -200.0f, -200.0f, 200.0f, 200.0f, textColor, boxColor);
		
	}
	
}


void gameloop::drawTextBox(freetype::font_data _font, string _str, float ssOffsetX, float ssOffsetY, float boxXSize, float boxYSize, float textColor[], float boxColor[]) {
	glPushMatrix();
	glTranslatef(ssOffsetX, ssOffsetY, 0);
	float centreX = (float)win32_window::getScreenWidth() / 2.0f;
	float centreY = (float)win32_window::getScreenHeight() / 2.0f;


	freetype::pushScreenCoordinateMatrix();
	glPushMatrix();
	glColor4f(boxColor[0], boxColor[1], boxColor[2], boxColor[3]);
	glBegin(GL_QUADS);

	float boxXFactor = boxXSize / 2;
	float boxYFactor = boxYSize / 2;
	glVertex2f(centreX - boxXFactor, centreY - boxYFactor);
	glVertex2f(centreX - boxXFactor, centreY + boxYFactor);
	glVertex2f(centreX + boxXFactor, centreY + boxYFactor);
	glVertex2f(centreX + boxXFactor, centreY - boxYFactor);

	glEnd();

	glColor4f(textColor[0], textColor[1], textColor[2], textColor[3]);
	freetype::print(_font, centreX - boxXFactor, centreY+boxYFactor-(_font.h), _str.c_str());


	glPopMatrix();
	freetype::pop_projection_matrix();
	glPopMatrix();
}

void gameloop::drawBackground(string png, float repeat, float tintColor[]) {
	glPushMatrix();

	myTexture = utils::loadPNG(png);

	glEnable(GL_TEXTURE_2D);

	glColor4f(tintColor[0], tintColor[1], tintColor[2], tintColor[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-1, -1);
	glTexCoord2f(0.0, repeat); glVertex2f(-1, 1);
	glTexCoord2f(repeat, repeat); glVertex2f(1, 1);
	glTexCoord2f(repeat, 0.0); glVertex2f(1, -1);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}