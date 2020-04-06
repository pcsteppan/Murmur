#pragma once
#include "ofMain.h"
#include <memory>

// forward declaration
class Actor;

class Octree
	: public enable_shared_from_this<Octree>
{
	glm::vec3 center;
	glm::vec1 halfLength;
	weak_ptr<Octree> parent;
	vector<shared_ptr<Octree>> children;
	vector<Actor*> actors;
	//static int nodes;

public:
	Octree(weak_ptr<Octree> parent, glm::vec3 center, glm::vec1 halfLength);
	Octree();
	weak_ptr<Octree> add(const Actor &a);
	void deconstruct();
	void reconstruct(const vector<Actor*> &actors);
	void represent();
};