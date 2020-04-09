#include <iostream>
#include "Actor.h"
#include "Octree.h"

Actor::Actor() {
	position = glm::vec3(rand() % ofGetWidth(), rand() % ofGetHeight(), rand() % 600 - 300);
	velocity = glm::vec3(rand() % 3, rand() % 3, rand() % 3);
	//printf("init\nx:%.1f, y:%.1f, v:%.1f\n\n", position.x, position.y, glm::length(velocity));
	//cout << "Initialized\n";
}

void Actor::difference(vector<Actor*> &relations) {
	glm::vec3 forces[] = {glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };
	int counts[] = {0, 0, 0};

	//printf("align: %.1f, cohere:%.1f, separate: %.1f\n", glm::length(forces[0]), glm::length(forces[1]), glm::length(forces[2]));
	//printf("velocity: %.1f\n", glm::length(velocity));

	for (int i = 0; i < relations.size(); i++)
	{
		glm::vec3 differenceBetweenActors = position - relations[i]->position;
		float distanceBetweenActors = glm::length(differenceBetweenActors);
		if (distanceBetweenActors == 0) continue;

		//align
		if (distanceBetweenActors < forceThresholds[0])
		{
			forces[0] += glm::normalize(relations[i]->velocity);
			counts[0]++;
		}
		//cohere
		if (distanceBetweenActors < forceThresholds[1])
		{
			// force to cohere is inverse to distance?
			//glm::vec3 inverseDifference = glm::scale(differenceBetweenActors, 1./distanceBetweenActors);
			forces[1] -= glm::normalize(differenceBetweenActors);
			counts[1]++;
		}
		//separate
		if (distanceBetweenActors < forceThresholds[2])
		{
			forces[2] += glm::normalize(differenceBetweenActors);
			counts[2]++;
		}
	}

	//printf("align: %.1f, cohere:%.1f, separate: %.1f\n", glm::length(forces[0]), glm::length(forces[1]), glm::length(forces[2]));


	if (counts[0] > 0)
	{
		forces[0] /= counts[0];
		forces[0] *= forceWeights[0];
		forces[0] *= maxSpeed;
		//cout << "align\n";
	}
	if (counts[1] > 0)
	{
		forces[1] /= counts[1];
		forces[1] *= forceWeights[1];
		//cout << "cohere\n";
	}
	if (counts[2] > 0)
	{
		forces[2] /= counts[2];
		forces[2] *= forceWeights[2];
		//cout << "separate\n";
	}

	//printf("align: %.1f, cohere:%.1f, separate: %.1f\n", glm::length(forces[0]), glm::length(forces[1]), glm::length(forces[2]));


	glm::vec3 desired(0, 0, 0);
	desired = forces[0] + forces[1] + forces[2];
	glm::vec3 steer(0, 0, 0); 
	steer = desired - velocity;
	if (glm::length(steer) > maxForce)
	{
		steer = glm::normalize(steer) * maxForce;
	}
	velocity += steer;
	if (glm::length(velocity) > maxSpeed)
	{
		velocity = glm::normalize(velocity) * maxSpeed;
	}
	//printf("desired: %.1f, steer:%.1f, velocity: %.1f\n\n", glm::length(desired), glm::length(steer), glm::length(velocity));
	position += velocity;
	this->boundInSpace();
}

void Actor::represent() {
	
	float size = ofMap(position.z, -300, 300, 0.1f, 4.0f);
	ofSetColor(55, 70, 90);
	ofFill();
	//ofDrawEllipse(position.x, position.y, size, size);
	ofDrawCone(position, 2, 3);
	//printf("x:%.1f, y:%.1f\n", position.x, position.y);
}

void Actor::boundInSpace() {
	/*
	if (position.x < 0 || position.x > ofGetWidth() ||
		position.y < 0 || position.y > ofGetHeight())
	{
		position = glm::vec3(ofGetWidth()/2, ofGetHeight()/2, 0);
		//cout << "repositioned\n";
	}
	*/
	if (position.x > ofGetWidth())
		position.x = 10;
	if (position.x < 0)
		position.x = ofGetWidth() - 10;
	if (position.y > ofGetHeight())
		position.y = 10;
	if (position.y < 0)
		position.y = ofGetHeight() - 10;
	if (position.z > 300)
		position.z = -290;
	if (position.z < -300)
		position.z = 290;
}