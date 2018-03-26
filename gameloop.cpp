#include "gameloop.h"


//Initialise static members
GLuint GameLoop::myTexture = NULL;
int GameLoop::frame = 0;
HDC GameLoop::hDC = NULL;
DebugInfo* GameLoop::debugger = NULL;
vector<font_data*> GameLoop::fonts;
Camera* GameLoop::camera = new Camera();

//IMPORTANT
//scene as vector<GameObject> instead of vector<GameObject*> resulted in temporary copies bug!
vector<GameObject*> scene;

void GameLoop::init(HDC _hDC, DebugInfo* _debugger)
{
	hDC = _hDC;
	debugger = _debugger;

	GameObject::setDebugger(_debugger);
	//GameObject::setDebugMode(true);


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

	GameObject* car = new GameObject("Player", carSprites, planeMesh, 0, new float[4]{ 1.0f, 1.0f, 1.0f, 1.0f });
	car->scale(0.3f);
	car->setPhysicalAttributes(1.15f, 1.35f, 10.0f);
	car->setPlayerControl(true);
	car->setCollider(true);
	car->setPhysics(true);

	//TODO: read from file
	CollisionRadii* radii = new CollisionRadii(0.0f, 0.0f);
	radii->addRadius(0.1f, 0.0f);
	radii->addRadius(0.38f, 90.0f);
	radii->addRadius(0.1f, 180.0f);
	radii->addRadius(0.39f, 270.0f);
	radii->addRadius(0.1f, 45.0f);
	radii->addRadius(0.1f, 135.0f);
	radii->addRadius(0.1f, 225.0f);
	radii->addRadius(0.1f, 315.0f);
	vector<CollisionRadii*> bounds = {
		radii
	};
	car->setCollisionBounds(bounds);


	CollisionRadii* bradii = new CollisionRadii(0.0f, 0.0f);
	bradii->addRadius(0.29f, 0.0f);
	bradii->addRadius(0.32f, 90.0f);
	bradii->addRadius(0.29f, 180.0f);
	bradii->addRadius(0.25f, 270.0f);
	bradii->addRadius(0.4f, 45.0f);
	bradii->addRadius(0.4f, 135.0f);
	bradii->addRadius(0.35f, 225.0f);
	bradii->addRadius(0.35f, 315.0f);
	vector<CollisionRadii*> bbounds = {
		bradii
	};
	GameObject* box = new GameObject("Box", boxSprites, planeMesh, 0, new float[4]{ 1.0f, 0.5f, 0.5f, 1.0f });
	box->translate(0.3f, 0.3f, 0.0f);
	box->setCollider(true);
	box->setCollisionBounds(bbounds);

	GameObject* track = new GameObject("Track", trackSprites, planeMesh, 0, new float[4]{ 1.0f, 1.0f, 1.0f, 1.0f });
	track->scale(10.0f);
	scene.push_back(track);

	scene.push_back(box);

	scene.push_back(car);

	camera->setCameraTarget(car);
	camera->setCameraSlowParentFactors(0.98f, 0.95f);
}

void GameLoop::display() {


	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_STENCIL_BUFFER_BIT);

	utils::enableTextureBlending();


	float bgColor[] = { 0.6f, 0.9f, 0.9f, 0.5f };
	drawBackground("bg.png", 2, bgColor);

	for (GameObject* obj : scene) {
		obj->resetCollisionFlags();
	}

	glPushMatrix();

	GameObject* camTarget = camera->getCameraTarget();
	
	if (camTarget != NULL) {
		//Vect4f* follow = camTarget->getScreenPosition();
		Vect4f* follow = camTarget->getWorldPosition();

		float movDamp = camera->getSlowFactorMov();
		float rotDamp = camera->getSlowFactorRot();
		camera->setCameraPosition(follow->x * movDamp, follow->y * movDamp, 0.0f);
		//TODO: account for damping
		camera->setCameraZAngle(camTarget->getZAngle());

	}

	//Before object rendering prevents GameObject::worldToCamera from being NULL
	GameObject::setWorldToCameraTransform(camera->getTransformation());

	InputStates* inputs = win32_window::getInputs();
	for (GameObject* obj : scene) {
		obj->processInputs(inputs);
		obj->resolveCollisions(scene);
		obj->draw();
	}

	glPopMatrix();

	if (debugger != NULL) {

		float textColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float boxColor[] = { 0.1f, 0.1f, 0.1f, 0.8f };
		drawTextBox(*fonts.front(), debugger->getOutput(), -200.0f, -200.0f, 200.0f, 200.0f, textColor, boxColor);
		
	}

	
}


void GameLoop::drawTextBox(freetype::font_data _font, string _str, float ssOffsetX, float ssOffsetY, float boxXSize, float boxYSize, float textColor[], float boxColor[]) {
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

void GameLoop::drawBackground(string png, float repeat, float tintColor[]) {
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
