#include "Loop.h"

//init static members
HDC Loop::hDC = NULL;
DebugInfo* Loop::debugger = NULL;
InputStates* Loop::inputs = NULL;
Camera* Loop::camera = new Camera();
vector<font_data*> Loop::fonts = {};
float Loop::screenHeight = -1;
float Loop::screenWidth = -1;
int Loop::requestedActiveLoop = -1;

void Loop::writeMessage(string str) {
	Loop::writeMessage(str, 200.0f, 200.0f);
}
void Loop::writeMessage(string str, float h, float w) {
	Color4f textColor = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
	Color4f boxColor = Color4f(0.1f, 0.1f, 0.1f, 0.8f);
	Loop::writeMessage(str, h, w, textColor, boxColor);
}

void Loop::writeMessage(string str, float h, float w, Color4f textColor, Color4f boxColor) {
	if (fonts.size() > 0) {
		drawTextBox(*fonts.front(), str, 0.0f, 0.0f, w, h, textColor, boxColor);
	}
}

void Loop::drawTextBox(freetype::font_data _font, string _str, float ssOffsetX, float ssOffsetY, float boxXSize, float boxYSize, Color4f textColor, Color4f boxColor) {
	glPushMatrix();

	glTranslatef(ssOffsetX, ssOffsetY, 0);
	float centreX = Loop::screenWidth / 2.0f;
	float centreY = Loop::screenHeight / 2.0f;


	freetype::pushScreenCoordinateMatrix();
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glColor4f(boxColor.r, boxColor.g, boxColor.b, boxColor.a);
	glBegin(GL_QUADS);

	float boxXFactor = boxXSize / 2;
	float boxYFactor = boxYSize / 2;
	glVertex2f(centreX - boxXFactor, centreY - boxYFactor);
	glVertex2f(centreX - boxXFactor, centreY + boxYFactor);
	glVertex2f(centreX + boxXFactor, centreY + boxYFactor);
	glVertex2f(centreX + boxXFactor, centreY - boxYFactor);

	glEnd();

	glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
	freetype::print(_font, centreX - boxXFactor, centreY + boxYFactor - (_font.h), _str.c_str());

	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	freetype::pop_projection_matrix();
	glPopMatrix();
}

void Loop::drawBackground(nv::Image* image, float repeat, Color4f tintColor) {
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);

	utils::bindPNG(image);

	glColor4f(tintColor.r, tintColor.g, tintColor.b, tintColor.a);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-repeat, -repeat);
	glTexCoord2f(0.0, repeat); glVertex2f(-repeat, repeat);
	glTexCoord2f(repeat, repeat); glVertex2f(repeat, repeat);
	glTexCoord2f(repeat, 0.0); glVertex2f(repeat, -repeat);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}

void Loop::freeStaticData() {

	Loop::fonts.clear();
	delete Loop::camera;
	GameObject::freeData();

}
