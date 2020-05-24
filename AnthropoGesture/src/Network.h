#pragma once
#include "ofMain.h" // we need to include this to have a reference to the openFrameworks framework
#include "Actor.h"
#include "Octree.h"

class Network {
	public:
	void difference();
	void represent(bool bDrawOctree);

	//void fixedRadiusNearestActorSearch(Actor* a, vector<Actor*> &results);
	void octreeNearestActorSearch(Actor* a, vector<Actor*> &results);
	void updateFactors(float wa, float wc, float ws, float wcp, float ra, float rc, float rs);
	
	/*
	float min[3] = { 0.0f, 0.0f, 0.0f };
	float max[3] = { 1.0f + EPSIOLON, 1.0f + EPSIOLON, 1.0f + EPSIOLON };
	float cellSize[3] = { 0.1, 0.1, 0.1 };
	Octree octree(min, max, cellSize);
	*/
	// variables
	unique_ptr<Octree> root;
	int pop = 8196*3;
	vector<Actor*> actors;
	// align, cohere, separate
	float forceWeights[4];
	float forceRadii[3];
	Network();
};
