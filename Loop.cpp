#include "Loop.h"

//init static members
HDC* Loop::hDC = NULL;
DebugInfo* Loop::debugger = NULL;
InputStates* Loop::inputs = NULL;
Camera* Loop::camera = NULL;
vector<font_data*> Loop::fonts = {};
float Loop::screenHeight = -1;
float Loop::screenWidth = -1;
int Loop::requestedActiveLoop = -1;
GameData* Loop::globals = NULL;
TextureCache Loop::cache = TextureCache();

void Loop::resetData(bool retainGlobals) {
	
	Loop::cache.clear();
	
	GameObject::defaultSprite = Loop::getTexture("resources/images/default-sprite.png");

	Loop::requestedActiveLoop = -1;
	for (UIElement* u : this->UI) {
		delete u;
	}
	this->UI.clear();
	for (font_data* f : Loop::fonts) {
		f->clean();
		delete f;
	}
	Loop::fonts.clear();

	if (Loop::camera != NULL) {
		Loop::camera->freeData();
		delete Loop::camera;
		Loop::camera = new Camera();
	}
	if (!retainGlobals) {
		if (Loop::globals != NULL) {
			Loop::globals->freeData();
			delete Loop::globals;
			Loop::globals = new GameData();
		}
	}
}

void Loop::freeData() {
	for (UIElement* u : this->UI) {
		delete u;
	}
	this->UI.clear();
}

void Loop::freeAllStaticData() {
	for (font_data* f : Loop::fonts) {
		f->clean();
		delete f;
	}
	Loop::fonts.clear();

	Loop::camera->freeData();
	Loop::globals->freeData();
	delete Loop::camera;
	delete Loop::globals;
}

void Loop::writeMessage(string str) {
	Loop::writeMessage(str, 200.0f, 200.0f);
}
void Loop::writeMessage(string str, float h, float w) {
	Color4f textColor = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
	Color4f boxColor = Color4f(0.1f, 0.1f, 0.1f, 0.8f);
	Loop::writeMessage(str, h, w, textColor, boxColor);
}

void Loop::writeMessage(string str, float h, float w, Color4f &textColor, Color4f &boxColor) {
	if (fonts.size() > 0) {
		drawTextBox(*fonts.front(), str, 0.0f, 0.0f, w, h, textColor, boxColor);
	}
}

void Loop::drawTextBox(freetype::font_data &_font, string _str, float ssOffsetX, float ssOffsetY, float boxXSize, float boxYSize, Color4f &textColor, Color4f &boxColor) {
	glPushMatrix();

	glTranslatef(ssOffsetX, ssOffsetY, 0);
	float centreX = Loop::screenWidth / 2.0f;
	float centreY = Loop::screenHeight / 2.0f;


	freetype::pushScreenCoordinateMatrix();
		glPushMatrix();
			glDisable(GL_TEXTURE_2D);

			
			float boxXFactor = (boxXSize / 2);
			float boxYFactor = (boxYSize / 2);
			glColor4f(boxColor.r, boxColor.g, boxColor.b, boxColor.a);
			glBegin(GL_QUADS);

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

void Loop::drawBackground(GLuint image, float repeat, Color4f &tintColor) {
	Loop::drawBackground(image, repeat, tintColor, Vect4f(0.0f, 0.0f, 0.0f));
}

void Loop::drawBackground(GLuint image, float repeat, Color4f &tintColor, Vect4f &ss_offset) {

	if (image == NULL) return;

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);

	utils::bindTexture(image);

	glColor4f(tintColor.r, tintColor.g, tintColor.b, tintColor.a);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0+ss_offset.getX(), 0.0 + ss_offset.getY()); glVertex2f(-repeat, -repeat);
	glTexCoord2f(0.0 + ss_offset.getX(), repeat + ss_offset.getY()); glVertex2f(-repeat, repeat);
	glTexCoord2f(repeat + ss_offset.getX(), repeat + ss_offset.getY()); glVertex2f(repeat, repeat);
	glTexCoord2f(repeat + ss_offset.getX(), 0.0 + ss_offset.getY()); glVertex2f(repeat, -repeat);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}

void Loop::addUI(Vect4f &location, Vect4f &size, string value, const UIType &type, font_data &font) {
	this->UI.push_back(new UIElement(location, size, value, type, font));
}

void Loop::processUI() {
	
	if (inputs != NULL) {

		vector<int> keys = {};

		for (int key = 0; key < 256; key++) {
			if (inputs->keys[key]) {
				keys.push_back(key);
			}
		}

		for (UIElement* e : this->UI) {

			//no input handling for a label
			if (e->type == UIType::LABEL) continue;

			bool keyPressed = ((int)keys.size() > 0);

			if (e->type == UIType::TEXTBOX && e->focused && keyPressed) {
				for (int key : keys) {
					if (key == VK_RETURN) {
						e->focused = false;
					}
					else if (key == VK_SPACE && (e->value.back() != ' ')) {
						e->setValue(e->value+' ');
					}
					else if (key == VK_BACK) {
						if ((int)e->value.length() > 0) e->setValue(e->value.substr(0, e->value.length() - 1));
					}
						
				}
			}
			 else if (e->type == UIType::TEXTBOX && e->focused) {

				if (inputs->characterIsNew && inputs->character != NULL) {
					e->setValue(e->value+inputs->character);
					inputs->characterIsNew = false;
				}
			}
			bool clicked = false;
			if (inputs->LeftPressed) {
				if ((inputs->mouse_x - (e->location->getX() - (e->size->getX() / 2.0f)) > 0.0f)
					&& (inputs->mouse_y - (e->location->getY() - (e->size->getY() / 2.0f)) > 0.0f)) {
					if ((inputs->mouse_x - (e->location->getX() + (e->size->getX() / 2.0f)) < 0.0f)
						&& (inputs->mouse_y - (e->location->getY() + (e->size->getY() / 2.0f)) < 0.0f)) {
						clicked = true;
					}
				}
			}
			if (clicked && e->enabled) {
				e->focused = true;
				e->clickedAt = debugger->getTime();
				//We read the character in the inputstate object, and we do nothing with it
				//since the input wasn't focused
				//without this, the last character typed before focus will be entered in the case of textbox
				inputs->characterIsNew = false;
				mciSendString("play btn from 0", NULL, 0, NULL);
			}
			else if ((e->type == UIType::BUTTON) || (!clicked && inputs->LeftPressed) || (!e->enabled)){
				e->focused = false;
			}
			e->focusedColor = e->clicked = e->focused;
		}
	}
}

void Loop::drawUI() {
	this->processUI();
	for (UIElement* e : this->UI) {
		Loop::drawTextBox(*e->font, e->value, e->location->getX(), e->location->getY(), e->size->getX(), e->size->getY(), *e->textColor, (!e->focusedColor)?*e->boxColor:*e->altColor);
	}
}

void Loop::init(HDC &_hDC, DebugInfo &_debugger, InputStates &inputs) {
	
	if (Loop::hDC == NULL) Loop::hDC = &_hDC;
	if (Loop::debugger == NULL) Loop::debugger = &_debugger;
	if (Loop::inputs == NULL) Loop::inputs = &inputs;

	//if (Loop::camera != NULL) delete Loop::camera; 
	//if (Loop::globals != NULL) delete Loop::globals;

	if (Loop::camera == NULL) Loop::camera = new Camera();
	if (Loop::globals == NULL) Loop::globals = new GameData();

	GameObject::setDebugger(_debugger);
	
}

Loop::Loop() {
	this->UI = {};
	this->loopStarted = 0.0;
}

void Loop::addFont(const char* name, unsigned int size) {

	font_data* font1 = new font_data();
	font1->init(name, size);
	Loop::fonts.push_back(font1);
}

GLuint Loop::getTexture(std::string name) {
	int idx = 0;
	for (std::string s : Loop::cache.textures) {
		if (s == name) {
			//cached
			return Loop::cache.textureIDs.at(idx);
		}
		else idx++;
	}
	return initTexture(name);
}

GLuint Loop::initTexture(std::string name) {

	GLuint myTextureID = (int)Loop::cache.textureIDs.size();
	//This makes sure we aren't using a handle that is already used
	while ((int)myTextureID == 0 || glIsTexture(myTextureID)) {
		myTextureID = (GLuint)((int)myTextureID + 1);
	}

	nv::Image* img = utils::loadPNG(name);

	if (img != NULL) {

		glBindTexture(GL_TEXTURE_2D, myTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img->getInternalFormat(), img->getWidth(), img->getHeight(), 0, img->getFormat(), img->getType(), img->getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		img->~Image();

	}

	Loop::cache.textures.push_back(name);
	Loop::cache.textureIDs.push_back(myTextureID);

	return myTextureID;
}