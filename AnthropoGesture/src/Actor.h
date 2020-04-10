#pragma once
#include <glm/vec3.hpp>
#include "ofMain.h"
#include <memory>

// forward declaration
class Octree;

class Actor
{
	public:
	void represent();
	void difference(const vector<Actor*> &relations);
	void boundInSpace();

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	Octree* spatialImage;
	const float forceWeights[3] = { 1.00, 1.10, 1.29 };
	const float forceThresholds[3] = { 200.0, 200.0, 40.0 };
	const float maxSpeed = 12.0;
	const float maxForce = 0.4;
	Actor();
};

