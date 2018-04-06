#include "gameloop.h"


//Initialise static members
int GameLoop::frame = 0;
HDC GameLoop::hDC = NULL;
DebugInfo* GameLoop::debugger = NULL;
vector<font_data*> GameLoop::fonts;
Camera* GameLoop::camera = new Camera();

nv::Image* GameLoop::backgroundPNG = utils::loadPNG("bg.png");
vector<nv::Image*> GameLoop::bgSprites;
int GameLoop::bgSpriteIndex = -1;

double LastFrameTime = 0.0;

InputStates* GameLoop::inputs = NULL;
vector<GameObject*> scene;

void GameLoop::freeData() {
	GameLoop::fonts.clear();
	delete GameLoop::camera;
	delete GameLoop::backgroundPNG;
	GameObject::freeData();
}

void GameLoop::init(HDC _hDC, DebugInfo* _debugger)
{
	hDC = _hDC;
	debugger = _debugger;

	GameObject::setDebugger(_debugger);
	GameObject::setDebugMode(true);
	GameLoop::inputs = win32_window::getInputs();

	glClearColor(0.0, 0.0, 0.0, 0.0);

	//utils::BuildFont(hDC);

	font_data* font1 = new font_data();
	font1->init("BKANT.TTF", 16);
	fonts.push_back(font1);

	vector<Vertex> planeMesh = {
		Vertex(0.0, 0.0, -0.5, -0.5),
		Vertex(0.0, 1.0, -0.5, 0.5),
		Vertex(1.0, 1.0, 0.5, 0.5),
		Vertex(1.0, 0.0, 0.5, -0.5)
	};

	GameLoop::bgSprites = { utils::loadPNG("animbg/bg_1.png") };

	vector<nv::Image*> carSprites = { utils::loadPNG("chair/1.png") ,utils::loadPNG("chair/2.png") ,utils::loadPNG("chair/3.png") };
	vector<nv::Image*> boxSprites = { utils::loadPNG("box/1.png"), utils::loadPNG("box/2.png"), utils::loadPNG("box/3.png") };
	vector<nv::Image*> trackSprites = { utils::loadPNG("office_1.png") };

	GameObject* car = new GameObject("Player", carSprites, planeMesh, 0, new Color4f( 1.0f, 1.0f, 1.0f, 1.0f ));
	car->scale(0.8f, false);
	car->setPhysicalAttributes(1.6f, 1.5f, 6.0f);
	car->translate(-1.0f, 1.0f, 0.0f);
	car->setPlayerControl(true);
	car->setCollider(true);
	car->setPhysics(true);
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
	
	GameObject* track = new GameObject("Track", trackSprites, planeMesh, 0, new Color4f(1.0f,1.0f,1.0f, 1.0f));
	track->setCollisionBounds(generateTrackBounds("office_container.txt"));
	track->setCollider(true);
	track->setPhysicsContainer(true);
	track->scale(10.0f, false);
	track->nuScaleBounds(0.5f, 0.5f, 1.0f);

	scene.push_back(track);

	CollisionRadii* bradii2 = new CollisionRadii(0.0f, 0.0f);
	bradii2->addRadius(0.29f, 0.0f);
	bradii2->addRadius(0.32f, 90.0f);
	bradii2->addRadius(0.29f, 180.0f);
	bradii2->addRadius(0.25f, 270.0f);
	bradii2->addRadius(0.4f, 45.0f);
	bradii2->addRadius(0.4f, 135.0f);
	bradii2->addRadius(0.35f, 225.0f);
	bradii2->addRadius(0.35f, 315.0f);
	vector<CollisionRadii*> bbounds2 = {
		bradii2
	};
	GameObject* box2 = new GameObject("Box2", boxSprites, planeMesh, 0, new Color4f(1.0f, 0.5f, 0.5f, 1.0f ));
	box2->scale(0.5f, false);
	box2->translate(-1.0f, 1.5f, 0.0f);
	box2->setCollider(true);
	box2->setCollisionBounds(bbounds2);
	box2->setPhysics(true);
	
	scene.push_back(box2);
	scene.push_back(car);

	camera->setCameraTarget(car);
	camera->setCameraSlowParentFactors(0.15f, 0.5f);
}

void GameLoop::display() {

	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_STENCIL_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	drawBackground(4, Color4f());


	for (GameObject* obj : scene) {
		obj->resetCollisionFlags();
	}

	glPushMatrix();

	GameObject* camTarget = camera->getCameraTarget();

	if (camTarget != NULL) {
		Vect4f* follow = camTarget->getWorldPosition();
		Vect4f* camPos = camera->getCameraPosition();

		float movDamp = camera->getSlowFactorMov();
		camera->setCameraPosition(camPos->x + ((follow->x - camPos->x) * movDamp), camPos->y + ((follow->y - camPos->y) * movDamp), 0.0f);

		//TODO: account for damping
		//float rotDamp = camera->getSlowFactorRot();
		//camera->setCameraZAngle( camera->getCameraZAngle() +  ((camTarget->getZAngle() - camera->getCameraZAngle())* rotDamp));

	}

	GameObject::setWorldToCameraTransform(camera->getTransformation());

	for (GameObject* obj : scene) {

		//double now = debugger->getTime();
		//double frameDelay = 1.0 / 100.0;
		//if ((debugger->getTime() - LastFrameTime) >= frameDelay) {
			//LastFrameTime = (double)now- (frameDelay/1.0);
			if (inputs != NULL) obj->processInputs(GameLoop::inputs);
			obj->draw();
			obj->resolveCollisions(scene);
		//}

	}

	glPopMatrix();

	if (debugger != NULL) {

		Color4f textColor = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
		Color4f boxColor = Color4f(0.1f, 0.1f, 0.1f, 0.8f);
		drawTextBox(*fonts.front(), debugger->getOutput(), -200.0f, -200.0f, 200.0f, 200.0f, textColor, boxColor);

	}

}


void GameLoop::drawTextBox(freetype::font_data _font, string _str, float ssOffsetX, float ssOffsetY, float boxXSize, float boxYSize, Color4f textColor, Color4f boxColor) {
	glPushMatrix();
	glTranslatef(ssOffsetX, ssOffsetY, 0);
	float centreX = (float)win32_window::getScreenWidth() / 2.0f;
	float centreY = (float)win32_window::getScreenHeight() / 2.0f;


	freetype::pushScreenCoordinateMatrix();
	glPushMatrix();
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
	freetype::print(_font, centreX - boxXFactor, centreY+boxYFactor-(_font.h), _str.c_str());

	glPopMatrix();
	freetype::pop_projection_matrix();
	glPopMatrix();
}

void GameLoop::drawBackground(float repeat, Color4f tintColor) {
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);

	if ((int)GameLoop::bgSprites.size() > 0) {
		GameLoop::bgSpriteIndex = (GameLoop::bgSpriteIndex == -1 || GameLoop::bgSpriteIndex == (int)GameLoop::bgSprites.size() - 1) ? 0 : GameLoop::bgSpriteIndex + 1;
		utils::bindPNG(GameLoop::bgSprites.at(GameLoop::bgSpriteIndex));
	}
	else {
		utils::bindPNG(backgroundPNG);
	}

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

vector<CollisionRadii*> GameLoop::generateTrackBounds(char* filename){

	vector<string> lines = utils::getFileContents(filename);

	//centrex, centrey, radius, angle, radius, angle, radius, angle...

	vector<CollisionRadii*> tbounds;

	for (int i = 0; i < (int)lines.size(); i++) {
		vector<string> radii = utils::splitString(lines.at(i), ',');

		CollisionRadii* curr = new CollisionRadii(stof(radii.at(0)), stof(radii.at(1)));
		for (int j = 2; j < (int)radii.size(); j += 2) {
			curr->addRadius(stof(radii.at(j)), stof(radii.at(j + 1)));
		}
		curr->setInterpolationTrigO(true);
		tbounds.push_back(curr);
	}
	
	return tbounds;

}
