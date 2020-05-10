#include "Network.h"


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

void Network::fixedRadiusNearestActorSearch(Actor* a, vector<Actor*> &results)
{
	for (int i = 0; i < actors.size(); i++)
	{
		results.push_back(actors[i]);
	}
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
		//fixedRadiusNearestActorSearch(actors[i], results);
		octreeNearestActorSearch(actors[i], results);
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
	float w = (sin(ofMap(ofGetFrameNum() % 240, 0, 240, 0, TWO_PI))+1)*127.5f;
	ofColor c(0,0,0,255);

	
	//ofMesh mesh;
	//mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	for (int i = 0; i < actors.size(); i++)
	{
		// change color to be a map of distance to camera
		actors[i]->represent(c);
		//mesh.addVertex(actors[i]->position);
		//mesh.addTexCoord(glm::vec2(actors[i]->position.x, actors[i]->position.y));
	}
	//mesh.draw();

	//ofSetColor(102, 166, 199, 10);
	ofSetColor(255,255,255,100);
	if(bDrawOctree) root->represent();
}
