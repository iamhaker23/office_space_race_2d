#include "gameloop.h"


//Initialise static members
GLuint gameloop::myTexture = NULL;
int gameloop::frame = 0;
HDC gameloop::hDC = NULL;
DebugInfo* gameloop::debugger = NULL;
vector<font_data*> gameloop::fonts;
GameObject* cameraTarget;
float slowParentFactor[] = { 0.15f, 0.99f };
float cameraZRads = 0.0f;

//IMPORTANT
//scene as vector<GameObject> instead of vector<GameObject*> resulted in temporary copies bug!

vector<GameObject*> scene;

void gameloop::init(HDC _hDC, DebugInfo* _debugger)
{
	hDC = _hDC;
	debugger = _debugger;

	glClearColor(0.0, 0.0, 0.0, 0.0);

	utils::BuildFont(hDC);

	font_data* font1 = new font_data();
	font1->init("BKANT.TTF", 16);
	fonts.push_back(font1);

	vector<Vertex> planeMesh = {
		Vertex(0.0, 0.0, -0.5, -0.5),
		Vertex(0.0, 1.0, -0.5, 0.5),
		Vertex(1.0, 1.0, 0.5, 0.5),
		Vertex(1.0, 0.0, 0.5, -0.5)
	};

	vector<string> carSprites{ "car/1.png","car/2.png","car/3.png" };
	vector<string> boxSprites{ "box/1.png","box/2.png","box/3.png" };
	vector<string> trackSprites{ "track.png" };
	vector<string> treeSprites{
		"tree/1.png",
		"tree/2.png",
		"tree/3.png",
		"tree/4.png",
		"tree/5.png",
		"tree/6.png",
		"tree/7.png",
		"tree/8.png",
		"tree/9.png",
		"tree/10.png",
		"tree/11.png",
		"tree/12.png",
		"tree/13.png",
		"tree/14.png",
		"tree/15.png",
		"tree/16.png"
	};

	GameObject* car = new GameObject("Player", carSprites, planeMesh, 0, new float[4]{ 1.0f, 1.0f, 1.0f, 1.0f });
	car->scale(0.3f);
	car->setPhysicalAttributes(1.2f, 1.2f, 5.0f);
	car->setPlayerControl(true);

	//GameObject* box = new GameObject("Box", boxSprites, planeMesh, 0, new float[4]{ 1.0f, 0.5f, 0.5f, 1.0f });
	//box->scale(0.75f);
	//box->translate(1.0f, 1.0f, 0.0f);

	GameObject* track = new GameObject("Track", trackSprites, planeMesh, 0, new float[4]{ 1.0f, 1.0f, 1.0f, 1.0f });
	track->scale(10.0f);

	//GameObject* tree = new GameObject("Tree", treeSprites, planeMesh, 0, new float[4]{ 0.8f, 1.0f, 0.6f, 1.0f });
	//tree->nuScale(0.5f, 1.0f, 1.0f);

	scene.push_back(track);
	//scene.push_back(tree);
	//scene.push_back(box);
	scene.push_back(car);
	

	cameraTarget = car;
}

void gameloop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_STENCIL_BUFFER_BIT);

	utils::enableTextureBlending();


	float bgColor[] = { 0.6f, 0.9f, 0.9f, 0.5f };
	drawBackground("bg.png", 2, bgColor);
	
	glPushMatrix();

	if (cameraTarget != NULL) {
		vector<float> follow = cameraTarget->getPosition();
		glTranslatef(-follow[0] * slowParentFactor[0], -follow[1] * slowParentFactor[0], 0.0f);

		float followAngle = cameraTarget->getZAngle();
		if (abs(cameraZRads - followAngle) > (0.15f)) {
			cameraZRads = -(followAngle*slowParentFactor[1]);
			glRotatef(cameraZRads, 0.0f, 0.0f, 1.0f);
		}

	}

	InputStates* inputs = win32_window::getInputs();
	
	for (GameObject* obj : scene) {

		if (obj->getName() == "Tree") {
			obj->animate(AnimationLogic::LOOPEND);
		}
		
		obj->processInputs(inputs);

		//win32_window::printMessage(obj->toString());

		obj->draw();
	}

	glPopMatrix();


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
	glTexCoord2f(0.0, 0.0); glVertex2f(-repeat, -repeat);
	glTexCoord2f(0.0, repeat); glVertex2f(-repeat, repeat);
	glTexCoord2f(repeat, repeat); glVertex2f(repeat, repeat);
	glTexCoord2f(repeat, 0.0); glVertex2f(repeat, -repeat);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}
