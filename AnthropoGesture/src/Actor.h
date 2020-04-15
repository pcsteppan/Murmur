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
	void applyForce(glm::vec3 force);
	glm::vec3 seek(glm::vec3 targer);
	

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	Octree* spatialImage;
	const float forceWeights[3] = { 1.0f, 1.2f, 1.5f };
	const float forceThresholds[3] = { 50.0f, 60.0f, 20.0f };
	const float maxSpeed = 12.0f;
	const float maxForce = 0.1f;
	Actor();
};

