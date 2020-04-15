#pragma once
#include "ofMain.h"
#include <memory>

// forward declaration
class Actor;

class Octree
{
public:
	glm::vec3 center;
	glm::vec1 halfLength;
	Octree* parent;
	vector<unique_ptr<Octree>> children;
	vector<Actor*> actors;
	short int kidNumber;
	//static int nodes;


	Octree(Octree* parent, glm::vec3 center, glm::vec1 halfLength, short int _kidNumber);
	Octree();
	Octree* add(const Actor &a);
	void deconstruct();
	void reconstruct(const vector<Actor*> &actors);
	void represent();
	void fixedRadiusNearestActorSearch(Actor* a, vector<Actor*> &results);
	void fixedRadiusNearestActorSearchHelper(Actor* a, vector<Actor*> &results, short int kidWhoAsked);
};