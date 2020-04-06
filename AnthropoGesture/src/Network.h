#pragma once
#include "ofMain.h" // we need to include this to have a reference to the openFrameworks framework
#include "Actor.h"
#include "Octree.h"

class Network {
	public:
	void difference();
	void represent();

	
	// variables
	void Network::fixedRadiusNearestActorSearch(Actor* a, vector<Actor*> &results);
	/*
	float min[3] = { 0.0f, 0.0f, 0.0f };
	float max[3] = { 1.0f + EPSIOLON, 1.0f + EPSIOLON, 1.0f + EPSIOLON };
	float cellSize[3] = { 0.1, 0.1, 0.1 };
	Octree octree(min, max, cellSize);
	*/
	shared_ptr<Octree> root = make_shared<Octree>(weak_ptr<Octree>(), glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0), glm::vec1(ofGetWidth() / 2));
	int pop = 100;
	vector<Actor*> actors;
	// align, cohere, separate
	float forceWeights[3];
	float forceThresholds[3];
	Network();
};

