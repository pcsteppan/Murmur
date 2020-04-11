#include "Network.h"


Network::Network() 
{
	root = make_unique<Octree>();
	for (int i = 0; i < pop; i++)
	{
		Actor* a = new Actor();
		actors.push_back(a);
		//auto l = root->add(*a);
		//a->spatialImage = l;
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
	{
		cout << "hmmm\n";
	}
	else
	{
		vector<Actor*>* neighbors = &((a->spatialImage)->actors);
	
		for (int i = 0; i < neighbors->size(); i++)
		{
			results.push_back(neighbors->at(i));
		}
	}
}

void Network::difference() 
{
	for (int i = 0; i < actors.size(); i++)
	{
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

void Network::represent() 
{
	for (int i = 0; i < actors.size(); i++)
	{
		actors[i]->represent();
	}
	ofSetColor(102, 166, 199, 10);
	root->represent();
}
