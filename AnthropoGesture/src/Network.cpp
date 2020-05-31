#include "Network.h"
#include "ofApp.h"

Network::Network() 
{
	root = make_unique<Octree>();
	actors.reserve(pop);
	for (int i = 0; i < pop; i++)
	{
		Actor* a = new Actor();
		actors.push_back(a);
	}
	root->reconstruct(actors);
}

void Network::octreeNearestActorSearch(Actor* a, vector<Actor*> &results)
{
	if (a->spatialImage == nullptr)
		cout << "hmmm\n";
	else
	{
		vector<Actor*>* neighbors = &((a->spatialImage)->actors);
	
		results.reserve(results.size() + neighbors->size());
		results.insert(results.begin(), neighbors->begin(), neighbors->end());
	}
}

void Network::difference() 
{
	for (int i = 0; i < actors.size(); i++)
	{
		actors[i]->updateFactors(forceWeights, forceRadii);
		vector<Actor*> results;
		results.reserve(64);
		actors[i]->spatialImage->fixedRadiusNearestActorSearch(actors[i], results);
		/*
		for (int j = 1; j < results.size(); j++)
		{
			ofDrawLine(results[j - 1]->position, results[j]->position);
		}
		*/
		//octreeNearestActorSearch(actors[i], results);
		actors[i]->difference(results);
	}
	//root.reset(new Octree);
	//root = make_shared<Octree>(weak_ptr<Octree>(), glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0), glm::vec1(ofGetWidth() / 2));
	root->deconstruct();
	//root.reset(new Octree);
	root->reconstruct(actors);
}

void Network::updateFactors(float wa, float wc, float ws, float wcp, float ra, float rc, float rs)
{
	forceWeights[0] = wa;
	forceWeights[1] = wc;
	forceWeights[2] = ws;
	forceWeights[3] = wcp;

	forceRadii[0] = ra;
	forceRadii[1] = rc;
	forceRadii[2] = rs;
}


void Network::represent(bool bDrawOctree) 
{

	ofSetColor(colorGrid);
	if (bDrawOctree) root->represent();

	float w = (sin(ofMap(ofGetFrameNum() % 240, 0, 240, 0, TWO_PI))+1)*127.5f;
	ofColor c(255,255,255,160);

	
	//ofMesh mesh;
	//mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	for (int i = 0; i < actors.size(); i++)
	{
		// change color to be a map of distance to camera
		actors[i]->represent();
		//mesh.addVertex(actors[i]->position);
		//mesh.addTexCoord(glm::vec2(actors[i]->position.x, actors[i]->position.y));
	}
	//mesh.draw();

	//ofSetColor(102, 166, 199, 10);
	
}
