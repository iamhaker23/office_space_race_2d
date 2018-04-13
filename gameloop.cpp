#include "gameloop.h"

GameLoop::GameLoop() {
	
	this->backgroundPNG = utils::loadPNG("resources/images/backgrounds/aerial_city.png");
	this->scene = {};

	//racers
	//track
	//objects

}

GameLoop::~GameLoop() {
	this->freeData();
	delete this->backgroundPNG;

}

void GameLoop::freeData() {
	this->scene.clear();
	Loop::freeStaticData();
	GameObject::freeData();
}

void GameLoop::init(HDC _hDC, DebugInfo* _debugger, InputStates* inputs)
{
	hDC = _hDC;
	debugger = _debugger;

	GameObject::setDebugger(_debugger);
	
	Loop::inputs = inputs;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	this->initGame();
}

void GameLoop::display() {


	//before clearing the on screen frame
	if (finished) {
		Loop::requestedActiveLoop = 0;
		return;
	}
		
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgroundPNG, 4, Color4f());
	
	for (GameObject* obj : scene) {
		obj->resetCollisionFlags();
	}

	glPushMatrix();

	//Calculate Camera Transform
	GameObject::setWorldToCameraTransform(camera->getTransformation());
	
	int NUM_TRACK_SEGMENTS = scene.at(0)->countCollisionRadii()-1;
	int TRACK_SEGMENT_STEP = 1;
	int TOTAL_LAPS = 1;

	for (GameObject* obj : scene) {

		//double now = debugger->getTime();
		//double frameDelay = 1.0 / 100.0;
		//if ((debugger->getTime() - LastFrameTime) >= frameDelay) {
			//LastFrameTime = (double)now- (frameDelay/1.0);
		if (inputs != NULL) obj->processInputs(GameLoop::inputs);
		obj->draw();
		obj->resolveCollisions(scene);
		if (obj->isAI()) {
			GameObject::doAIControl(obj, scene.at(0), TRACK_SEGMENT_STEP);
		}

		if (obj->isRacer()){
			if (obj->getRaceData() != NULL) {

				//TODO: better way of accessing track object
				//get the index of the closest segment
				int segmentOn = scene.at(0)->getIndexOfClosestRadiiTo(obj->getWorldPosition());

				RaceData* rd = obj->getRaceData();
				//index-currentsegment <= 2 to tolerate some skipping forward
				//70 - 1
				//69 - 70 - 1

				int SKIPPABLE = 4;

				int segDelta = segmentOn - rd->getCurrentSegment();
				
				//wrap forwards
				segDelta = (segDelta == -NUM_TRACK_SEGMENTS) ? segmentOn + 1 : segDelta;
				
				//will give cheat alert if you SKIP TOO MANY SEGMENTS
				//e.g. exploiting a hole in the track boundaries
				//e.g. trying to reverse and go forwards over the finish line

				if (segDelta > 0 && segDelta <= SKIPPABLE) {
					if (rd->hasCompletedSegments(NUM_TRACK_SEGMENTS)) {
						rd->incrementLaps();
						if (rd->getLaps() == TOTAL_LAPS) {
							if (obj->getName() != "Player") {
								startTimeOutClock = debugger->getTime();
							}
							else {
								finished = true;
							}
						}
					}
					rd->setCurrentSegment(segmentOn);
				}
				else if (segDelta > 0 && segDelta > SKIPPABLE) {
					//unecessary to check >0 and >SKIPPABLE but just in case SKIPPABLE gets given a negative value
					if (obj->getName() == "Player") Loop::writeMessage("CHEAT ALERT", 30.0f, 150.0f, Color4f(), Color4f(0.6f, 0.0f, 0.0f, 0.5f));

				}
				else if (segDelta == 0) {
					//Player is on the same segment, but perhaps some progress has been made

					//NOTE:
					//progress on current segment is 0.0f - 1.0f but as the player moves forwards there are two possibilities
					//we have reached 100% of the track segment OR we progress to the next segment because we closer to it's CollisionRadii centre
					//hence the player may start a segment at an arbitrary progress (e.g. 40%)
					//and the player may leave a segment on an arbitrary progress (e.g. 75%)
					//Entry and Exit percentages are not combinant because they are ratios between different segment lengths
					//on a uniform track they would be combinant

					rd->setProgressOnCurrentSegment(scene.at(0)->getProgressAcrossTrackSegment(segmentOn, obj->getWorldPosition(), TRACK_SEGMENT_STEP));
				}
				else if (segDelta < -1) {
					if (obj->getName() == "Player") Loop::writeMessage("Wrong way!", 30.0f, 150.0f, Color4f(), Color4f(0.6f, 0.4f, 0.0f, 0.5f));
				}

			}
			else {
				obj->initRaceData();
			}
		}
		//}

	}


	glPopMatrix();

	int playerPosition = 0;
	float playerDistance = 0.0f;
	int playerLaps = 0;
	float playerProgress = 0.0f;

	for (GameObject* obj : scene) {
		if (obj->getName() == "Player" && obj->getRaceData() != NULL) {

			RaceData* rd = obj->getRaceData();
			playerDistance = (float)(rd->getLaps() * NUM_TRACK_SEGMENTS) + (float)rd->getCurrentSegment() + rd->getProgressOnCurrentSegment();
			playerLaps = rd->getLaps();
			playerProgress = (float)rd->getCurrentSegment() / ((float)NUM_TRACK_SEGMENTS);
		}
	}

	for (GameObject* obj : scene) {
		if (obj->isRacer() && obj->getRaceData() != NULL) {
			//racer with race data

			if (obj->getName() != "Player") {
				//competitor
				RaceData* rd = obj->getRaceData();
				float competitorDistance = (float)(rd->getLaps() * NUM_TRACK_SEGMENTS) + (float)rd->getCurrentSegment() + rd->getProgressOnCurrentSegment();
				if (playerDistance <= competitorDistance) {
					playerPosition += 1;
				}
			}

		}
	}

	vector<string> positions = {"1st", "2nd", "3rd", "4th"};

	debugger->addMessage(positions.at(playerPosition));
	debugger->addMessage(utils::lapsLabel(playerLaps, TOTAL_LAPS));
	//escape percent because string is actually a format string
	//can pass format string and variables if needed
	debugger->addMessage(utils::intLabel("", (int)(playerProgress*100.0f), "%%"));

	//after everything else is drawn, print the DNF timer
	double MAX_TIME = 10.0;
	if (startTimeOutClock >= 0.0) {
		double timeout = debugger->getTime() - startTimeOutClock;
		if (timeout >= MAX_TIME) {
			finished = true;
			return;
		}
		else {
			Loop::drawTextBox(*fonts.at(0), utils::floatLabel("DNF in ", (float)(MAX_TIME - timeout), "s"), 0.0f, 80.0f, 180.0f, 50.0f, Color4f(), Color4f(1.0f, 0.2f, 0.2f, 0.8f));
		}
	}
	
	if (debugger != NULL) {

		Color4f textColor = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
		Color4f boxColor = Color4f(0.1f, 0.1f, 0.1f, 0.8f);
		if (GameObject::getDebugMode()) {
			drawTextBox(*fonts.front(), debugger->getOutput(), -200.0f, -200.0f, 200.0f, 200.0f, textColor, boxColor);
		}
		else {
			drawTextBox(*fonts.front(), debugger->getMessages(), -200.0f, -200.0f, 200.0f, 200.0f, textColor, boxColor);
		}

	}
	/*
	END BLOCK COMMENT
	*/
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


void GameLoop::handleActivation() {
	//LoopManager has activated this scene

	this->initGame();

}


void GameLoop::initGame() {

	this->freeData();

	this->startTimeOutClock = -1.0;
	this->finished = false;
	Loop::camera = new Camera();
	
	font_data* font1 = new font_data();
	font1->init("resources/fonts/HANDA.TTF", 20);
	Loop::fonts.push_back(font1);

	vector<Vertex> planeMesh = {
		Vertex(0.0, 0.0, -0.5, -0.5),
		Vertex(0.0, 1.0, -0.5, 0.5),
		Vertex(1.0, 1.0, 0.5, 0.5),
		Vertex(1.0, 0.0, 0.5, -0.5)
	};

	vector<nv::Image*> carSprites = { utils::loadPNG("resources/images/chair/1.png") ,utils::loadPNG("resources/images/chair/2.png") ,utils::loadPNG("resources/images/chair/3.png") };
	vector<nv::Image*> boxSprites = { utils::loadPNG("resources/images/box/1.png"), utils::loadPNG("resources/images/box/2.png"), utils::loadPNG("resources/images/box/3.png") };
	vector<nv::Image*> trackSprites = { utils::loadPNG("resources/images/tracks/office_1.png") };

	GameObject* car = new GameObject("Player", carSprites, planeMesh, 0, new Color4f(1.0f, 0.0f, 0.0f, 1.0f));
	car->scale(0.6f, true);
	car->setPhysicalAttributes(1.4f, 1.3f, 7.5f);
	car->setZAngle(180.0f);
	car->translate(-1.8f, -1.8f, 0.0f);
	car->setPlayerControl(true);
	car->setCollider(true);
	car->setPhysics(true);

	GameObject* ai = new GameObject("AI", carSprites, planeMesh, 0, new Color4f(0.0f, 0.5f, 1.0f, 1.0f));
	ai->scale(0.6f, true);
	ai->setPhysicalAttributes(1.4f, 1.3f, 7.5f);
	ai->translate(-0.5f, -1.0f, 0.0f);
	ai->setZAngle(170.0f);
	ai->setCollider(true);
	ai->setPhysics(true);
	ai->setAIControl(true);

	CollisionRadii* radii = new CollisionRadii(0.0f, 0.0f);
	radii->addRadius(0.3f, 0.0f);
	radii->addRadius(0.32f, 90.0f);
	radii->addRadius(0.3f, 180.0f);
	radii->addRadius(0.33f, 270.0f);
	radii->addRadius(0.3f, 45.0f);
	radii->addRadius(0.3f, 135.0f);
	radii->addRadius(0.3f, 225.0f);
	radii->addRadius(0.3f, 315.0f);
	vector<CollisionRadii*> bounds = {
		radii
	};
	car->setCollisionBounds(bounds);
	ai->setCollisionBounds(bounds);

	GameObject* track = new GameObject("Track", trackSprites, planeMesh, 0, new Color4f(1.0f, 1.0f, 1.0f, 1.0f));
	track->setCollisionBounds(generateTrackBounds("resources/data/office_container.txt"));
	track->setCollider(true);
	track->setPhysicsContainer(true);
	track->scale(10.0f, false);
	track->nuScaleBounds(0.5f, 0.5f, 1.0f);

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
	GameObject* box2 = new GameObject("Box2", boxSprites, planeMesh, 0, new Color4f(1.0f, 0.5f, 0.5f, 1.0f));
	box2->translate(-1.0f, 1.5f, 0.0f);
	box2->setCollider(true);
	box2->setCollisionBounds(bbounds2);
	box2->setPhysics(true);
	box2->setPhysicalAttributes(1.6f, 1.5f, 6.0f);

	//TODO: select track by more flexible means than hard-coding it's index
	scene.push_back(track);
	scene.push_back(box2);
	scene.push_back(car);
	scene.push_back(ai);

	camera->setCameraTarget(car);
	camera->setCameraSlowParentFactors(0.15f, 0.5f);
}