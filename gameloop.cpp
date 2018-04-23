#include "gameloop.h"

GameLoop::GameLoop() : Loop() {

	this->backgrounds = {};
	this->scene = RaceScene();
	this->sprites = {};
}

GameLoop::~GameLoop() {
	this->freeData();
}

void GameLoop::freeData() {
	scene.freeData();
	Loop::freeData();
}

void GameLoop::resetData() {
	Loop::resetData(true);
	
	scene.freeData();

	this->loopStarted = debugger->getTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GameLoop::display() {

	//before clearing the on screen frame
	if (finished) {
		Loop::requestedActiveLoop = 3;
		if (!Loop::globals->updated) {
			Loop::globals->updated = true;
			Loop::globals->player = scene.getPlayer()->getRaceData();
			/*vector<RaceData*> racers = {};
			for (GO_Racer* r : scene.getRacers()) {
				if (r != NULL && r->getRaceData() != NULL) {
					racers.push_back(r->getRaceData());
				}
			}
			Loop::globals->others = racers;*/
		}

		return;
	}
		
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Loop::drawBackground(this->backgrounds.at(0), 4, Color4f());
	float factor = (float)debugger->getTime();
	Loop::drawBackground(this->backgrounds.at(1), 4, Color4f(), Vect4f(factor/10.0f, factor/12.0f, 0.0f));

	vector<GameObject*> objs = scene.getObjectList();

	for (GameObject* obj : objs) {
		obj->resetCollisionFlags();
	}

	//Calculate Camera Transform
	GameObject::setWorldToCameraTransform(camera->getTransformation());
	
	GameObject* track = scene.getTrack();

	int NUM_TRACK_SEGMENTS = track->countCollisionRadii()-1;
	int TRACK_SEGMENT_STEP = 1;
	int TOTAL_LAPS = this->globals->laps;

	glPushMatrix();

	for (GameObject* obj : objs) {
		obj->draw(); 
		obj->resolveCollisions(objs);
	}

	glPopMatrix();

	if (inputs != NULL) scene.getPlayer()->processInputs(inputs);
	vector<GO_Racer*> racers = scene.getRacers();
	for (GO_Racer* competitor : racers) {
		competitor->doAIControl(track, TRACK_SEGMENT_STEP);
		if (competitor->getRaceData() != NULL) {

			int segmentOn = track->getIndexOfClosestRadiiTo(competitor->getWorldPosition());

			RaceData* rd = competitor->getRaceData();

			int SKIPPABLE = 4;
			int segDelta = segmentOn - rd->getCurrentSegment();
			segDelta = (segDelta == -NUM_TRACK_SEGMENTS) ? segmentOn + 1 : segDelta;
			//will give cheat alert if you SKIP TOO MANY SEGMENTS
			//e.g. exploiting a hole in the track boundaries
			//e.g. trying to reverse and go forwards over the finish line

			if (segDelta > 0 && segDelta <= SKIPPABLE) {
				if (rd->hasCompletedSegments(NUM_TRACK_SEGMENTS)) {
					rd->incrementLaps();
					if (rd->getLaps() == TOTAL_LAPS) {
						if (competitor->getName() != this->globals->playerName){
							if (startTimeOutClock == -1.0) {
								startTimeOutClock = debugger->getTime();
							}
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
				if (competitor->getName() == this->globals->playerName) Loop::writeMessage("CHEAT ALERT", 30.0f, 150.0f, Color4f(), Color4f(0.6f, 0.0f, 0.0f, 0.5f));

			}
			else if (segDelta == 0) {
				rd->setProgressOnCurrentSegment(track->getProgressAcrossTrackSegment(segmentOn, competitor->getWorldPosition(), TRACK_SEGMENT_STEP));
			}
			else if (segDelta < -1) {
				if (competitor->getName() == this->globals->playerName) Loop::writeMessage("Wrong way!", 30.0f, 150.0f, Color4f(), Color4f(0.6f, 0.4f, 0.0f, 0.5f));
			}

		}
	}

	int playerPosition = 1;
	float playerDistance = 0.0f;
	int playerLaps = 0;
	float playerProgress = 0.0f;

	RaceData* rd = scene.getPlayer()->getRaceData();
	if (rd != NULL) {
		playerDistance = (float)(rd->getLaps() * NUM_TRACK_SEGMENTS) + (float)rd->getCurrentSegment() + rd->getProgressOnCurrentSegment();
		playerLaps = rd->getLaps();
		playerProgress = (float)rd->getCurrentSegment() / ((float)NUM_TRACK_SEGMENTS);
	}

	for (GO_Racer* competitor : racers) {
		if (competitor->getRaceData() != NULL && competitor->getName() != this->globals->playerName) {
			//competitor
			RaceData* rd = competitor->getRaceData();
			float competitorDistance = (float)(rd->getLaps() * NUM_TRACK_SEGMENTS) + (float)rd->getCurrentSegment() + rd->getProgressOnCurrentSegment();
			if (playerDistance <= competitorDistance) {
				playerPosition += 1;
			}
		}
	}
	rd->setPosition(playerPosition);

	debugger->addMessage(utils::getPositionLabel(playerPosition));
	debugger->addMessage(utils::lapsLabel(playerLaps, TOTAL_LAPS));
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
	debugger->addMessage(this->globals->playerName);
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
	racers.clear();
	objs.clear();
}

vector<CollisionRadii> GameLoop::generateTrackBounds(char* filename){

	vector<string> lines = utils::getFileContents(filename);

	//centrex, centrey, radius, angle, radius, angle, radius, angle...

	vector<CollisionRadii> tbounds;

	for (int i = 0; i < (int)lines.size(); i++) {
		vector<string> radii = utils::splitString(lines.at(i), ',');

		CollisionRadii curr = CollisionRadii(stof(radii.at(0)), stof(radii.at(1)));
		for (int j = 2; j < (int)radii.size(); j += 2) {
			curr.addRadius(stof(radii.at(j)), stof(radii.at(j + 1)));
		}
		curr.setInterpolationTrigO(true);
		tbounds.push_back(curr);
	}
	
	return tbounds;

}


void GameLoop::handleActivation() {
	//LoopManager has activated this scene

	this->resetData();
	this->initGame();

}


void GameLoop::initGame() {

	
	this->startTimeOutClock = -1.0;
	this->finished = false;
	
	this->backgrounds.push_back(Loop::getTexture("resources/images/backgrounds/aerial_city.png"));
	this->backgrounds.push_back(Loop::getTexture("resources/images/backgrounds/clouds.png"));
	
	Loop::addFont("resources/fonts/HANDA.TTF", 20);

	vector<GLuint> trackSprites = { Loop::getTexture("resources/images/tracks/office_1.png") };

	vector<GO_Racer> racers = GameLoop::generateRacers(this->globals->ai);
	vector<GameObject> objects = GameLoop::generateObjects();

	GameObject track = GameObject("Track", trackSprites, generatePlaneMesh(), 0, Color4f(1.0f, 1.0f, 1.0f, 1.0f));
	track.setCollisionBounds(generateTrackBounds("resources/data/office_container.txt"));
	track.setCollider(true);
	track.setPhysicsContainer(true);
	track.scale(10.0f, false);
	track.nuScaleBounds(0.5f, 0.5f, 1.0f);

	scene.setTrack(track);

	for (GO_Racer o : racers) {
		scene.addCompetitor(o);
	}

	//setting after ensures player is rendered after ai
	scene.setPlayer(generatePlayer(this->globals->playerName));

	for (GameObject o : objects) {
		scene.addObject(o);
	}

	camera->setCameraTarget(scene.getPlayer());
	camera->setCameraSlowParentFactors(0.15f, 0.5f);
}

GO_Racer GameLoop::generatePlayer(string &name) {

	vector<GLuint> carSprites = { Loop::getTexture(string("resources/images/characters/").append(Loop::globals->pusher).append("/1.png"))
		, Loop::getTexture(string("resources/images/characters/").append(Loop::globals->pusher).append("/2.png"))
		, Loop::getTexture(string("resources/images/characters/").append(Loop::globals->pusher).append("/3.png"))
		, Loop::getTexture(string("resources/images/characters/").append(Loop::globals->pusher).append("/4.png"))
		, Loop::getTexture(string("resources/images/characters/").append(Loop::globals->pusher).append("/5.png"))
		, Loop::getTexture(string("resources/images/characters/").append(Loop::globals->pusher).append("/6.png"))
		, Loop::getTexture(string("resources/images/characters/").append(Loop::globals->pusher).append("/7.png"))
		, Loop::getTexture(string("resources/images/characters/").append(Loop::globals->pusher).append("/8.png"))
	};

	vector<GLuint> chairSprites = { Loop::getTexture("resources/images/chair/1.png")
		,Loop::getTexture("resources/images/chair/2.png")
		,Loop::getTexture("resources/images/chair/3.png") };

	GO_Racer player = GO_Racer(name, carSprites, generatePlaneMesh(), 0, Color4f(1.0f, 1.0f, 1.0f, 1.0f), chairSprites, (Loop::globals->difficulty == 2));
	player.scale(0.6f, true);
	player.setPhysicalAttributes(1.6f, 1.3f, 4.0f);
	player.setZAngle(180.0f);
	player.translate(-1.8f, -1.8f, 0.0f);
	player.setPlayerControl(true);
	player.setCollider(true);

	player.setPhysics(true); 
	CollisionRadii radii = CollisionRadii(0.0f, 0.0f);
	radii.addRadius(0.3f, 0.0f);
	radii.addRadius(0.65f, 90.0f);
	radii.addRadius(0.3f, 180.0f);
	radii.addRadius(0.33f, 270.0f);
	radii.addRadius(0.4f, 45.0f);
	radii.addRadius(0.4f, 135.0f);
	radii.addRadius(0.3f, 225.0f);
	radii.addRadius(0.3f, 315.0f);

	player.setCollisionBounds(radii);

	return player;

}
vector<GO_Racer> GameLoop::generateRacers(int numAI) {

	vector<GO_Racer> output = {};

	vector<GLuint> carSprites = { Loop::getTexture("resources/images/characters/carlos/1.png")
		,Loop::getTexture("resources/images/characters/carlos/2.png")
		,Loop::getTexture("resources/images/characters/carlos/3.png")
		,Loop::getTexture("resources/images/characters/carlos/4.png")
		,Loop::getTexture("resources/images/characters/carlos/5.png")
		,Loop::getTexture("resources/images/characters/carlos/6.png")
		,Loop::getTexture("resources/images/characters/carlos/7.png")
		,Loop::getTexture("resources/images/characters/carlos/8.png")
	};

	vector<GLuint> chairSprites = { Loop::getTexture("resources/images/chair/1.png")
		,Loop::getTexture("resources/images/chair/2.png")
		,Loop::getTexture("resources/images/chair/3.png") };

	//effectively instancing for bounds (i.e. change one, change all).
	CollisionRadii radii = CollisionRadii(0.0f, 0.0f);
	radii.addRadius(0.3f, 0.0f);
	radii.addRadius(0.65f, 90.0f);
	radii.addRadius(0.3f, 180.0f);
	radii.addRadius(0.33f, 270.0f);
	radii.addRadius(0.4f, 45.0f);
	radii.addRadius(0.4f, 135.0f);
	radii.addRadius(0.3f, 225.0f);
	radii.addRadius(0.3f, 315.0f);

	for (int i = 0; i < numAI; i++){
		GO_Racer ai = GO_Racer(utils::intLabel("AI_RACER_", i, ""), carSprites, generatePlaneMesh(), 0, Color4f(0.1f, 0.2f, 1.0f, 1.0f), chairSprites, (Loop::globals->difficulty == 0));
		ai.scale(0.6f, true);
		ai.setPhysicalAttributes(1.4f - ((float)i*0.1f), 1.3f, 6.0f+((float)i));
		ai.translate(-0.5f, -1.0f, 0.0f);
		ai.setZAngle(170.0f);
		ai.setCollider(true);
		ai.setPhysics(true);
		ai.setAIControl(true);

		ai.setCollisionBounds(radii);

		output.push_back(ai);
	}

	return output;
}

vector<GameObject> GameLoop::generateObjects() {

	vector<GameObject> output = {};

	vector<GLuint> boxSprites = { Loop::getTexture("resources/images/box/1.png") };
	
	CollisionRadii boxRadii = CollisionRadii(0.0f, 0.0f);
	boxRadii.addRadius(0.29f, 0.0f);
	boxRadii.addRadius(0.32f, 90.0f);
	boxRadii.addRadius(0.29f, 180.0f);
	boxRadii.addRadius(0.25f, 270.0f);
	boxRadii.addRadius(0.4f, 45.0f);
	boxRadii.addRadius(0.4f, 135.0f);
	boxRadii.addRadius(0.35f, 225.0f);
	boxRadii.addRadius(0.35f, 315.0f);

	for (int i = 0; i < 5; i++) {
		GameObject box = GameObject(utils::intLabel("Box", i, ""), boxSprites, generatePlaneMesh(), 0, Color4f(1.0f, 0.5f, 0.5f, 1.0f));
		box.translate(-1.0f, 1.5f + (i*0.1f), 0.0f);
		box.scale(0.5f, true);
		box.setCollider(true);
		box.setCollisionBounds(boxRadii);
		box.setPhysics(true);
		box.setPhysicalAttributes(1.6f, 1.5f, 6.0f);

		output.push_back(box);
	}

	return output;

}

vector<Vertex> GameLoop::generatePlaneMesh() {
	vector<Vertex> planeMesh = {
		Vertex(0.0, 0.0, -0.5, -0.5),
		Vertex(0.0, 1.0, -0.5, 0.5),
		Vertex(1.0, 1.0, 0.5, 0.5),
		Vertex(1.0, 0.0, 0.5, -0.5)
	};
	return planeMesh;
}