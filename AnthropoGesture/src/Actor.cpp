#include <iostream>
#include "ofApp.h"
#include "Actor.h"
#include "Octree.h"

Actor::Actor() {
	/*
	position = glm::vec3((rand() % (ofGetWidth() - 20)) + 10,
		(rand() % (ofGetHeight() - 20)) + 10,
		(rand() % (600 - 20)) - 300 + 10);
		*/
	position = glm::vec3(ofRandom(-cube_size_o, cube_size_o),
		ofRandom(-cube_size_o, cube_size_o),
		ofRandom(-cube_size_o, cube_size_o));

	int x = ofMap(position.x, -cube_size, cube_size, 0, 1919, true);
	int y = ofMap(position.y, -cube_size, cube_size, 0, 1079, true);

	this->color = palettePixels.getColor(x, y);

	//position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(rand() % 4 - 2, rand() % 4 - 2, (rand() % 4) - 2);

	//printf("init\nx:%.1f, y:%.1f, v:%.1f\n\n", position.x, position.y, glm::length(velocity));
	//cout << "Initialized\n";
	acceleration = glm::vec3(0,0,0);
}

void Actor::colorSetup() {
	int x = ofMap(position.x, -cube_size, cube_size, 0, 1919, true);
	int y = ofMap(position.y, -cube_size, cube_size, 0, 1079, true);

	this->color = palettePixels.getColor(x, y);
}

void Actor::difference(const vector<Actor*> &relations) {
	// find better way to do this
	float maxV = maxVelocity;
	float maxF = maxForce;
	//
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
			if (distanceBetweenActors < forceRadii[0])
			{
				forces[0] += relations[i]->velocity;
				counts[0]++;
			}
			//cohere
			if (distanceBetweenActors < forceRadii[1])
			{
				// force to cohere is inverse to distance?
				//glm::vec3 inverseDifference = glm::scale(differenceBetweenActors, 1./distanceBetweenActors);
				forces[1] += relations[i]->position;
				counts[1]++;
			}
			//separate
			if (distanceBetweenActors < forceRadii[2])
			{
				forces[2] += glm::normalize(differenceBetweenActors)/distanceBetweenActors;
				counts[2]++;
			}
		}

		if (counts[0] > 0 && forces[0] != glm::vec3(0, 0, 0))
		{
			forces[0] /= counts[0];
			forces[0] = glm::normalize(forces[0]);
			forces[0] *= maxV;
			glm::vec3 steer = forces[0] - velocity;
			if (glm::length(steer) > maxForce)
				steer = glm::normalize(steer) * maxF;
			applyForce(steer * forceWeights[0]);
		}
		if (counts[1] > 0 && forces[1] != glm::vec3(0,0,0))
		{
			forces[1] /= counts[1];
			applyForce(seek(forces[1]) * forceWeights[1]);
		}
		if (counts[2] > 0 && forces[2] != glm::vec3(0, 0, 0))
		{
			forces[2] /= counts[2];
			forces[2] = glm::normalize(forces[2]);
			forces[2] = forces[2] * maxV - velocity;
			if (glm::length(forces[2]) > maxForce)
				forces[2] = glm::normalize(forces[2]) * maxF;
			applyForce(forces[2] * forceWeights[2]);
		}

		if (glm::isnan(forces[0]).x
			|| glm::isnan(forces[1]).x
			|| glm::isnan(forces[2]).x)
		{
			cout << "isnan error\n";
		}
	}

	//float f_mx = ofMap(ofGetMouseX() , 0, ofGetWidth(), 0.0f, 1.0f); 
	float inverse_distance = glm::length(position) / cube_size;
	applyForce(forceWeights[3] * inverse_distance * seek(glm::vec3(0,0,0)));

	velocity += acceleration;
	if (glm::length(velocity) > maxVelocity)
		velocity = glm::normalize(velocity) * maxV;
	position += velocity;
	acceleration *= 0;
	this->boundInSpace();
}

glm::vec3 Actor::seek(glm::vec3 target)
{
	// find better way to do this
	float maxV = maxVelocity;
	float maxF = maxForce;
	//
	glm::vec3 desired = target - position;
	desired = glm::normalize(desired) * maxV;
	glm::vec3 steer = desired - velocity;
	if (glm::length(steer) > maxForce)
	{
		steer = glm::normalize(steer) * maxF;
	}
	return steer;
}

void Actor::represent() {
	float size = ofMap(position.z, -300, 300, 0.1f, 4.0f);

	float camDist = ofMap(glm::distance(cam.getPosition(), this->position), 0, 2000, 0.0f, 1.0f, true);
	//c.lerp(farColorBoid, camDist);
	ofColor c = nearColorBoid;
	c.r = color.r;
	c.b = color.b;
	c.g = color.g;
	//ofColor c = ofColor(color.r,color.g,color.b);

	//ofColor c = palettePixels.getColor((int)position.x, (int)position.y);
	//c = ofColor(glm::distance(this->position, p), 0, 0);
	ofSetColor(color);
	ofFill();
	//ofDrawEllipse(position.x, position.y, size, size);
	//ofDrawCone(position, 3, 4);
	//ofDrawArrow(position, position + velocity, 0.5f);
	//ofDrawArrow(position - velocity*5, position, 0.3f);
	//ofDrawLine(position - velocity * 5, position);
	//ofDrawPlane(position, 30, 10);
	
	ofDrawLine(position, position+(velocity+velocity));
}

void Actor::updateFactors(float* pForceWeights, float* pForceRadii) {
	forceWeights[0] = pForceWeights[0];
	forceWeights[1] = pForceWeights[1];
	forceWeights[2] = pForceWeights[2];
	forceWeights[3] = pForceWeights[3];

	forceRadii[0] = pForceRadii[0];
	forceRadii[1] = pForceRadii[1];
	forceRadii[2] = pForceRadii[2];
}

void Actor::boundInSpace() {
	/*
	if (position.x > ofGetWidth())
		position.x = 10;
	if (position.x < 0)
		position.x = ofGetWidth() - 10;
	if (position.y > ofGetHeight())
		position.y = 10;
	if (position.y < 0)
		position.y = ofGetHeight() - 10;
	if (position.z > ofGetHeight()/2)
		position.z = -ofGetHeight()/2 + 10;
	if (position.z < -ofGetHeight()/2)
		position.z = ofGetHeight()/2 - 10;
		*/
	

	if (position.x > cube_size*2)
		position.x = -cube_size_o * 2;
	if (position.x < -cube_size * 2)
		position.x = cube_size_o * 2;
	/*
	if (position.x > 600)
		position.x = -590;
	if (position.x < -560)
		position.x = 560 - 10;
	*/
	if (position.y > cube_size * 2)
		position.y = -cube_size_o * 2;
	if (position.y < -cube_size * 2)
		position.y = cube_size_o * 2;
	if (position.z > cube_size * 2)
		position.z = -cube_size_o * 2;
	if (position.z < -cube_size * 2)
		position.z = cube_size_o * 2;
}

void Actor::applyForce(glm::vec3 force)
{
	acceleration += force;
}
