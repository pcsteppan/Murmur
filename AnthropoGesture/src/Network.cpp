#include "Network.h"


Network::Network() 
{
	for (int i = 0; i < pop; i++)
	{
		Actor* a = new Actor();
		actors.push_back(a);
		auto l = root->add(*a);
		a->spatialImage = l;
	}
}

void Network::fixedRadiusNearestActorSearch(Actor* a, vector<Actor*> &results)
{
	for (int i = 0; i < actors.size(); i++)
	{
		results.push_back(actors[i]);
	}
}

void Network::difference() 
{
	for (int i = 0; i < actors.size(); i++)
	{
		vector<Actor*> results;
		fixedRadiusNearestActorSearch(actors[i], results);
		actors[i]->difference(results);
	}
	//root.reset(new Octree);
	//root = make_shared<Octree>(weak_ptr<Octree>(), glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0), glm::vec1(ofGetWidth() / 2));
	root->deconstruct();
	root.reset(new Octree);
	root->reconstruct(actors);
}

void Network::represent() 
{
	for (int i = 0; i < actors.size(); i++)
	{
		actors[i]->represent();
	}
	root->represent();
}
