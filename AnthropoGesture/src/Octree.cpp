#pragma once
#include "Octree.h"
#include "Actor.h"

Octree::Octree(Octree* _parent, glm::vec3 _center, glm::vec1 _halfLength) {
	parent = _parent;
	center = _center;
	halfLength = _halfLength;
	//nodes++;
	//actors = vector<Actor*>(8);
}


Octree::Octree() 
{
	parent = nullptr;
	center = glm::vec3(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 0);
	halfLength = glm::vec1(ofGetWidth() / 2.0f);
}


Octree* Octree::add(const Actor &a) {
	if (this == nullptr)
	{
		cout << "aaaaah\n";
	}
	if (a.position.x <= center.x + halfLength.x &&
		a.position.x >= center.x - halfLength.x &&
		a.position.y <= center.y + halfLength.x &&
		a.position.y >= center.y - halfLength.x &&
		a.position.z <= center.z + halfLength.x &&
		a.position.z >= center.z - halfLength.x
		)
	{
		if(children.size() == 0)
		{
			if (actors.size() == 8)
			{
				glm::vec1 quarterLength = halfLength / 2.0f;
				// subdivide
				for (int x = -1; x <= 1; x += 2)
				{
					for (int y = -1; y <= 1; y += 2)
					{
						for (int z = -1; z <= 1; z += 2)
						{
							glm::vec3 childCenter = center +
								glm::vec3(
									x*quarterLength.x,
									y*quarterLength.x,
									z*quarterLength.x
								);
							children.push_back(make_unique<Octree>(this, childCenter, quarterLength));
						}
					}
				}

				// actors in current node are added to children nodes
				for (int i = 0; i < children.size(); i++)
				{
					auto child = this->add(*actors[i]);
					actors[i]->spatialImage = child;
					/*
					if (child != nullptr)
					{
						actors[i]->spatialImage = child;
						break;
					}
					*/
				}
				actors.clear();
				return this->add(a);
			}
			else if (true)
			{
				// less than 8 actors in this node so far
				Actor* b = (Actor*)&a;
				actors.push_back(b);
				return this;
			}
		}
		else
		{
			// children nodes exist
			for (int i = 0; i < children.size(); i++)
			{
				Octree* child_return = children[i]->add(a);
				if(child_return != nullptr)
					return child_return;
			}
			cout << "uh oh\n";
		}
	}
	else
		return nullptr;
}

void Octree::deconstruct() 
{
	
	for (int i = 0; i < children.size(); i++)
	{
		//children[i]->deconstruct();
		children[i].reset();
	}
	children = vector<unique_ptr<Octree>>();

	/*
	else
	{
		cout << "leaf destroyed\n";
	}
	*/
}

// should only be called from root
void Octree::reconstruct(const vector<Actor*> &actors)
{
	//if (!parent.expired()) return;
	for each (Actor* a in actors)
	{
		auto l = this->add(*a);
		a->spatialImage = l;
	}
}

void Octree::represent()
{
	ofNoFill();
	ofDrawBox(center, halfLength.x*2, halfLength.x * 2, halfLength.x * 2);

	
	//ofDrawRectangle(center.x-halfLength.x, center.y-halfLength.x, halfLength.x*2, halfLength.x*2);
	if (children.size() > 0)
	{
		for (int i = 0; i < children.size(); i++)
		{
			printf("child index: %d\n", i);
			children[i]->represent();
		}
	}
}