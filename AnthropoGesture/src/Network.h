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
	void Network::octreeNearestActorSearch(Actor* a, vector<Actor*> &results);

	/*
	float min[3] = { 0.0f, 0.0f, 0.0f };
	float max[3] = { 1.0f + EPSIOLON, 1.0f + EPSIOLON, 1.0f + EPSIOLON };
	float cellSize[3] = { 0.1, 0.1, 0.1 };
	Octree octree(min, max, cellSize);
	*/
	unique_ptr<Octree> root;
	int pop = 512;
	vector<Actor*> actors;
	// align, cohere, separate
	float forceWeights[3];
	float forceThresholds[3];
	Network();
};

