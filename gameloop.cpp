#include "gameloop.h"

GameLoop::GameLoop() : Loop() {

	this->backgrounds = {};
	this->scene = RaceScene();
	this->sprites = {};

	this->startTime = 0.0;
	this->bestPlayerPos = 1;
	this->renderedFirstFrame = false;

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
			//first time this finish routine has been called
			Loop::globals->updated = true;
			if (Loop::globals->player != NULL) delete Loop::globals->player;
			Loop::globals->player = scene.getPlayer()->getRaceData();
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

	//Calculate Camera Transform
	GameObject::setWorldToCameraTransform(camera->getTransformation());
	
	GameObject* track = scene.getTrack();

	int NUM_TRACK_SEGMENTS = track->countCollisionRadii()-1;
	int TRACK_SEGMENT_STEP = (Loop::globals->reverse) ? -1 : 1;
	int TOTAL_LAPS = this->globals->laps;

	glPushMatrix();

	for (int i = 0; i < Loop::globals->collisionResolution; i++) {
		for (GameObject* obj : objs) {
			obj->resetCollisionFlags();
		}
		for (GameObject* obj : objs) {
			obj->resolveCollisions(objs);
		}
	}

	for (GameObject* obj : objs) {
		if (obj->getCanFall() && !obj->hasCollidedWith(scene.getWorld()->getName())){
			//outside world plane
			//object will fall and die
			if (obj->getXYScale() < 0.1f) {
				//disable collisions and don't draw
				//TODO: elegant object kill in Scene class
				obj->setGhost(true);
			}
			else {
				obj->scale(0.9f, true);
				obj->draw();
			}
		}
		else {
			obj->draw();
		}
	}

	glPopMatrix();

	if (startTime != 0.0 && debugger != NULL && (debugger->getTime() - this->startTime) < 3.0) {
		string message = utils::floatLabel("START IN ", 3.0f - (static_cast<float>(debugger->getTime() - this->startTime)), "");
		Loop::writeMessage(message, 50.0f, 250.0f, Color4f(), Color4f(0.6f, 0.0f, 0.0f, 0.5f));
	}
	else {
		scene.startRace();
	}
	//no need to do the rest of the logic unless the race has started
	//TODO: refactor, currently procedural and brittle implementation of pre-race
	if (!scene.hasRaceStarted()) {
		debugger->reset();
		return;
	}

	if (inputs != NULL) scene.getPlayer()->processInputs(inputs);
	vector<GO_Racer*> racers = scene.getRacers();
	for (GO_Racer* competitor : racers) {
		if (competitor->isAI()) competitor->doAIControl(track, TRACK_SEGMENT_STEP);
		if (competitor->getRaceData() != NULL) {

			
			int segmentOn = track->getIndexOfClosestRadiiTo(competitor->getWorldPosition());
			int segIndex = segmentOn;
			if (TRACK_SEGMENT_STEP < 0) {
				//segmentOn is the number of segments progressed and must account for track step
				//segIndex is the index of the current segment in track bounds (i.e. vector<CollisionRadii*>)
				segmentOn = NUM_TRACK_SEGMENTS - segmentOn;
			}

			RaceData* rd = competitor->getRaceData();

			int SKIPPABLE = 4;
			
			int segDelta = (segmentOn - rd->getCurrentSegment());
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
							//used to prevent player overtaking opponent who has already finished
							this->bestPlayerPos += 1;
						}
						else {
							finished = true;
						}
					}
				}

				rd->setCurrentSegment(segmentOn);
			}
			else if (segDelta > SKIPPABLE) {
				if (competitor->getName() == this->globals->playerName) Loop::writeMessage("CHEAT ALERT", 30.0f, 150.0f, Color4f(), Color4f(0.6f, 0.0f, 0.0f, 0.5f));

			}
			else if (segDelta == 0) {
				//important note: the racers can move to the next segment and have an arbitrary progress across a track segment
				//i.e. after 70% across the current segment, the algorithm could validly say the racer is now on the next segment
				//similarly the racer might enter the new segment at 40% progress across the length
				//it is bounded 0.0 to 1.0 but not guaranteed (or even likely) to use the full range
				//because it is a measurement that does not inform the algorithm's workings, only statistics

				//NOTE: segIndex NOT segmentOn
				float prog = track->getProgressAcrossTrackSegment(segIndex, competitor->getWorldPosition(), TRACK_SEGMENT_STEP);
				
				rd->setProgressOnCurrentSegment(prog);
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

	//best player pos variable accounts for the edge case:
	//if the opponent wins and triggers the DNF timer
	//the player may cross the line very shortly after, e.g. in 2nd
	//the player might have further "progressAcrossCurrentSegment"
	//even though the opponent already finished
	//the player still would place 1st because they have more overall progress
	rd->setPosition(max(playerPosition, this->bestPlayerPos));

	
	debugger->addMessage(utils::getPositionLabel(playerPosition));
	debugger->addMessage(utils::lapsLabel(playerLaps, TOTAL_LAPS));
	debugger->addMessage(utils::intLabel("", (int)(playerProgress*100.0f), "%%"));
	
	// Uncomment to see player position
	//debugger->addMessage(utils::floatLabel("X:", scene.getPlayer()->getWorldPosition().getX(), ""));
	//debugger->addMessage(utils::floatLabel("Y:", scene.getPlayer()->getWorldPosition().getY(), ""));
	

	//after everything else is drawn, print the DNF timer
	double MAX_TIME = 10.0;
	if (startTimeOutClock >= 0.0) {
		double timeout = debugger->getTime() - startTimeOutClock;
		if (timeout >= MAX_TIME) {
			scene.getPlayer()->raceData->setDNF(true);
			finished = true;
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
			Loop::drawTextBox(*fonts.front(), debugger->getOutput(), -200.0f, -200.0f, 200.0f, 200.0f, textColor, boxColor);
		}
		else {
			Loop::drawTextBox(*fonts.front(), debugger->getMessages(), -200.0f, -200.0f, 200.0f, 200.0f, textColor, boxColor);
		}

	}
	racers.clear();
	objs.clear();
}

vector<CollisionRadii> GameLoop::loadBoundsFromFile(char* filename){

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

	if (this->startTime == 0.0 && debugger != NULL) this->startTime = debugger->getTime();

	this->startTimeOutClock = -1.0;
	this->finished = false;

	this->backgrounds.push_back(Loop::getTexture("resources/images/backgrounds/aerial_city.png"));
	this->backgrounds.push_back(Loop::getTexture("resources/images/backgrounds/clouds.png"));

	Loop::addFont("resources/fonts/HANDA.TTF", 20);

	string trackName = (Loop::globals->track == 1) ? "resources/images/tracks/office_1.png" : "resources/images/tracks/office_2.png";
	char* trackBounds = (Loop::globals->track == 1) ? "resources/data/office_container_1.txt" : "resources/data/office_container_2.txt";
	char* worldBounds = (Loop::globals->track == 1) ? "resources/data/world_plane_1.txt" : "resources/data/world_plane_2.txt";

	vector<GLuint> trackSprites = { Loop::getTexture(trackName) };
	GO_Racer player = generatePlayer(this->globals->playerName);
	vector<GameObject> desks = GameLoop::generateDesks();
	vector<GameObject> objects = GameLoop::generateObjects();

	GameObject track = GameObject("Track", trackSprites, generatePlaneMesh(), 0, Color4f(1.0f, 1.0f, 1.0f, 1.0f));
	track.setCollisionBounds(loadBoundsFromFile(trackBounds));
	track.setCollider(true);
	track.setPhysicsContainer(true);
	track.scale(10.0f, false);
	track.nuScaleBounds(0.5f, 0.5f, 1.0f);

	GameObject world = GameObject("WorldPlane", vector<GLuint>{}, vector<Vertex>{}, 0, Color4f(0.0f, 0.0f, 0.0f, 0.0f));
	world.setCollisionBounds(loadBoundsFromFile(worldBounds));
	world.setCollider(true);
	world.setGhost(true);
	world.nuScaleBounds(0.45f, 0.45f, 1.0f);
	

	//Uncomment to allow player to go anywhere
	//track.setGhost(true)
	
	scene.setTrack(track);
	scene.setWorld(world); 
	
	CollisionRadii* startSegment = track.getNextCollisionRadiiFor(player.getWorldPosition(), (Loop::globals->reverse||Loop::globals->track==2)?1:-1);
	Vect4f oppositionLoc = 
		track.localToWorldSpace(
			track.boundSpaceToObjectSpace(
				Vect4f(startSegment->centreX, startSegment->centreY, 0.0f)
			)
		);
	
	vector<GO_Racer> racers = GameLoop::generateRacers(this->globals->ai, oppositionLoc);

	for (GO_Racer o : racers) {
		scene.addCompetitor(o);
	}

	scene.setPlayer(player);

	for (GameObject o : objects) {
		scene.addObject(o);
	}

	for (GameObject o : desks) {
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

	vector<GLuint> chairSprites = { Loop::getTexture(string("resources/images/chairs/").append(Loop::globals->pushee).append("/1.png"))
		,Loop::getTexture(string("resources/images/chairs/").append(Loop::globals->pushee).append("/2.png"))
		,Loop::getTexture(string("resources/images/chairs/").append(Loop::globals->pushee).append("/3.png"))
	};

	GO_Racer player = GO_Racer(name, carSprites, generatePlaneMesh(), 0, Color4f(1.0f, 1.0f, 1.0f, 1.0f), chairSprites, (Loop::globals->difficulty == 2));
	player.scale(0.6f, true);
	player.setPhysicalAttributes(1.6f, 1.3f, 4.0f);
	
	if (Loop::globals->track == 1) {
		player.setZAngle((Loop::globals->reverse) ? 0.0f : 180.0f);
	}
	else if (Loop::globals->track == 2){
		player.setZAngle((Loop::globals->reverse) ? 180.0f : 90.0f);
	}
	
	player.translate(-1.8f, -1.8f, 0.0f);
	player.setPlayerControl(true);
	player.setCollider(true);

	player.setPhysics(true); 
	CollisionRadii radii = CollisionRadii(0.0f, 0.0f);
	radii.addRadius(0.5f, 0.0f);
	radii.addRadius(1.3f, 90.0f);
	radii.addRadius(0.5f, 180.0f);
	radii.addRadius(0.5f, 270.0f);
	radii.addRadius(1.2f, 80.0f);
	radii.addRadius(1.2f, 100.0f);
	radii.addRadius(0.5f, 225.0f);
	radii.addRadius(0.5f, 315.0f);
	radii.setInterpolationTrigO(true);
	player.setCollisionBounds(radii);

	return player;

}
vector<GO_Racer> GameLoop::generateRacers(int numAI, Vect4f &startPos) {

	vector<GO_Racer> output = {};

	//effectively instancing for bounds (i.e. change one, change all).
	CollisionRadii radii = CollisionRadii(0.0f, 0.0f);
	radii.addRadius(0.5f, 0.0f);
	radii.addRadius(1.3f, 90.0f);
	radii.addRadius(0.5f, 180.0f);
	radii.addRadius(0.5f, 270.0f);
	radii.addRadius(1.2f, 80.0f);
	radii.addRadius(1.2f, 100.0f);
	radii.addRadius(0.5f, 225.0f);
	radii.addRadius(0.5f, 315.0f);
	radii.setInterpolationTrigO(true);

	string characters[] = { "Carlos", "Suzanne", "Barry" };
	string chairs[] = { "IT Guy", "Office Dog", "Sally" };

	for (int i = 0; i < numAI; i++){

		string name = characters[i % 3];
		string chair = chairs[i % 3];

		vector<GLuint> chairSprites = { Loop::getTexture(string("resources/images/chairs/").append(chair).append("/1.png"))
			,Loop::getTexture(string("resources/images/chairs/").append(chair).append("/2.png"))
			,Loop::getTexture(string("resources/images/chairs/").append(chair).append("/3.png"))
		};

		vector<GLuint> carSprites = { Loop::getTexture(string("resources/images/characters/").append(name).append("/1.png"))
			, Loop::getTexture(string("resources/images/characters/").append(name).append("/2.png"))
			, Loop::getTexture(string("resources/images/characters/").append(name).append("/3.png"))
			, Loop::getTexture(string("resources/images/characters/").append(name).append("/4.png"))
			, Loop::getTexture(string("resources/images/characters/").append(name).append("/5.png"))
			, Loop::getTexture(string("resources/images/characters/").append(name).append("/6.png"))
			, Loop::getTexture(string("resources/images/characters/").append(name).append("/7.png"))
			, Loop::getTexture(string("resources/images/characters/").append(name).append("/8.png"))
		};

		GO_Racer ai = GO_Racer(utils::intLabel("AI_RACER_", i, ""), carSprites, generatePlaneMesh(), 0, Color4f(1.0f, 1.0f, 1.0f, 1.0f), chairSprites, (Loop::globals->difficulty == 0));
		
		ai.setPhysicalAttributes(1.4f - ((float)i*0.1f), 1.3f, 6.0f+((float)i));
		//ai.translate(-0.5f, -1.0f, 0.0f);

		ai.scale(0.6f, true);
		ai.setZAngle((Loop::globals->reverse)?0.0f:180.0f); 

		ai.translate(startPos.getX(), startPos.getY(), startPos.getZ() );
		
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
		
	CollisionRadii boxRadii = CollisionRadii(0.0f, 0.0f);
	boxRadii.addRadius(0.7f, 45.0f);
	boxRadii.addRadius(0.7f, 135.0f);
	boxRadii.addRadius(0.7f, 225.0f);
	boxRadii.addRadius(0.7f, 315.0f);
	boxRadii.setInterpolationTrigO(true);

	for (int i = 0; i < 5; i++) {
		string name = utils::intToString(i % 3);
		vector<GLuint> boxSprites = { Loop::getTexture(string("resources/images/obstacles/").append(name).append(".png")) };

		GameObject box = GameObject(utils::intLabel("Box", i, ""), boxSprites, generatePlaneMesh(), 0, Color4f(1.0f, 1.0f, 1.0f, 1.0f));
		
		//hack to make the printer bounds larger
		if (name == "1") box.nuScaleBounds(2.0f, 2.0f, 1.0f);

		box.translate(-1.0f, 1.5f + (i*0.1f), 0.0f);
		box.scale(0.8f, true);
		box.setCanFall(true);
		box.setCollider(true);
		box.setCollisionBounds(boxRadii);
		box.setPhysics(true);
		box.setPhysicalAttributes(1.6f, 1.5f, 6.0f);
		box.setIgnoreContainers(true);

		output.push_back(box);
	}

	return output;

}

vector<GameObject> GameLoop::generateDesks() {

	vector<GameObject> output = {};
		
	vector<Vect4f> deskPositions = {};
	
	if (Loop::globals->track == 1) {
		deskPositions = {
			Vect4f(-2.4f, -1.2f, 0.0f)
			,Vect4f(-3.78f, -0.21f, 0.0f)
			, Vect4f(-3.15f, 1.18f, 0.0f)
			, Vect4f(-3.15f, 1.75f, 0.0f)
			, Vect4f(-0.92f, -0.19f, 0.0f)
			, Vect4f(-0.35f, 1.55f, 0.0f)
			, Vect4f(0.6f, 1.5f, 0.0f)
			, Vect4f(1.6f, 1.5f, 0.0f)
			, Vect4f(3.89f, 0.24f, 0.0f)
			, Vect4f(3.91f, -1.02f, 0.0f)
			,Vect4f(-2.6f, 1.35f, 0.0f)
			,Vect4f(2.6f, 1.0f, 0.0f)
			,Vect4f(3.9f, -0.9f, 0.0f)
			,Vect4f(3.0f, 0.6f, 0.0f)
			,Vect4f(3.0f, 0.2f, 0.0f)
		};
	}
	else {
		deskPositions = {
			Vect4f(-4.2f, -0.69f, 0.0f)
			, Vect4f(-4.3f, 0.25f, 0.0f)
			, Vect4f(-3.0f, 1.5f, 0.0f)
			, Vect4f(-3.1f, 2.9f, 0.0f)
			, Vect4f(-3.5f, 3.7f, 0.0f)
			, Vect4f(-2.7f, 3.5f, 0.0f)
			, Vect4f(-2.7f, 2.8f, 0.0f)
			, Vect4f(0.0f, 3.25f, 0.0f)
			, Vect4f(1.1f, 2.9f, 0.0f)
			, Vect4f(4.17f, -0.43f, 0.0f)
			, Vect4f(2.85f, -0.4f, 0.0f)
			, Vect4f(-0.71f, -2.5f, 0.0f)
			, Vect4f(-3.17f, -3.49f, 0.0f)
			, Vect4f(-3.2f, -2.36f, 0.0f)
			, Vect4f(-1.13f, -1.32f, 0.0f)
		};
	}

	CollisionRadii deskRadii = CollisionRadii(0.0f, 0.0f);
	deskRadii.addRadius(0.7f, 45.0f);
	deskRadii.addRadius(0.7f, 135.0f);
	deskRadii.addRadius(0.7f, 225.0f);
	deskRadii.addRadius(0.7f, 315.0f);
	deskRadii.setInterpolationTrigO(true);

	vector<GLuint> deskSprites = { Loop::getTexture("resources/images/obstacles/desk.png") };

	for (int i = 0; i < (int)deskPositions.size() - 1; i++) {
		GameObject desk = GameObject(utils::intLabel("Desk", i, ""), deskSprites, generatePlaneMesh(), 0, Color4f(1.0f, 1.0f, 1.0f, 1.0f));

		desk.setZAngle(((i % 2) == 0) ? 0.0f : 180.0f); 
		desk.translate(deskPositions.at(i).getX(), deskPositions.at(i).getY(), deskPositions.at(i).getZ() );
		desk.setCanFall(true);
		desk.scale(0.8f, true);
		desk.setCollider(true);
		desk.setCollisionBounds(deskRadii);
		desk.setPhysics(true);
		desk.setPhysicalAttributes(1.6f, 1.5f, 6.0f);
		desk.setIgnoreContainers(true);
		output.push_back(desk);
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