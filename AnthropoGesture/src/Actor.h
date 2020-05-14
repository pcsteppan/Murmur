#pragma once
#include <glm/vec3.hpp>
#include "ofMain.h"
#include <memory>

// forward declaration
class Octree;
//class Network;

class Actor
{
	public:
	void represent(ofColor c);
	void difference(const vector<Actor*> &relations);
	void boundInSpace();
	void applyForce(glm::vec3 force);
	void updateFactors(float* forceWeights, float* forceRadii);
	glm::vec3 seek(glm::vec3 targer);
	

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	//Network* network;
	Octree* spatialImage;
	float forceWeights[4] = { 1.0f, 1.2f, 1.5f, 0.0f };
	float forceRadii[3] = { 150.0f, 270.0f, 60.0f };
	const float maxSpeed = 24.0f;
	const float maxForce = 0.13f;
	const int cube_size = 1910;
	const int cube_size_o = 1900;
	Actor();
};

