#include <iostream>
#include "Actor.h"
#include "Octree.h"

Actor::Actor() {
	position = glm::vec3((rand() % (ofGetWidth()-20))+10, 
		(rand() % (ofGetHeight()-20))+10,
		(rand() % (600-20)) - 300 + 10);
	//position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(rand() % 10, rand() % 10, rand() % 10);
	//printf("init\nx:%.1f, y:%.1f, v:%.1f\n\n", position.x, position.y, glm::length(velocity));
	//cout << "Initialized\n";
}

void Actor::difference(const vector<Actor*> &relations) {
	if (relations.size() > 0)
	{
		glm::vec3 forces[] = { glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };
		int counts[] = { 0, 0, 0 };

		for (int i = 0; i < relations.size(); i++)
		{
			glm::vec3 differenceBetweenActors = position - relations[i]->position;
			float distanceBetweenActors = glm::length(differenceBetweenActors);
			if (distanceBetweenActors == 0) continue;

			//align
			if (distanceBetweenActors < forceThresholds[0])
			{
				if (relations[i]->velocity != glm::vec3(0,0,0))
				{
					forces[0] += glm::normalize(relations[i]->velocity);
					counts[0]++;
				}
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

		glm::vec3 desired(0, 0, 0);
		desired = forces[0] + forces[1] + forces[2];
		glm::vec3 steer(0, 0, 0);
		steer = desired - velocity;
		if (glm::length(steer) > maxForce)
		{
			steer = glm::normalize(steer) * maxForce;
		}
		if (steer.x != steer.x)
		{
			cout << "what\n";
		}
		velocity += steer;
	}
	if (velocity.x != velocity.x)
	{
		cout << "what\n";
	}
	if (glm::length(velocity) > maxSpeed)
	{
		velocity = glm::normalize(velocity) * maxSpeed;
		
	}
	if (velocity.x != velocity.x)
	{
		cout << "what\n";
	}
	position += velocity;
	this->boundInSpace();
}

void Actor::represent() {
	float size = ofMap(position.z, -300, 300, 0.1f, 4.0f);
	ofSetColor(214, 95, 87);
	ofFill();
	//ofDrawEllipse(position.x, position.y, size, size);
	//ofDrawCone(position, 3, 4);
	ofDrawArrow(position, position + velocity, 0.05f);
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