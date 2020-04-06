#pragma once
#include "Octree.h"
#include "Actor.h"

Octree::Octree(weak_ptr<Octree> _parent, glm::vec3 _center, glm::vec1 _halfLength) {
	parent = _parent;
	center = _center;
	halfLength = _halfLength;
	//nodes++;
	//actors = vector<Actor*>(8);
}


Octree::Octree() 
{
	parent = weak_ptr<Octree>();
	center = glm::vec3(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 0);
	halfLength = glm::vec1(ofGetWidth() / 2.0f);
}


weak_ptr<Octree> Octree::add(const Actor &a) {
	if (a.position.x < center.x + halfLength.x &&
		a.position.x > center.x - halfLength.x &&
		a.position.y < center.y + halfLength.x &&
		a.position.y > center.y - halfLength.x &&
		a.position.z < center.z + halfLength.x &&
		a.position.z > center.z - halfLength.x
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
							auto child = make_shared<Octree>(weak_from_this(), childCenter, quarterLength);
							children.push_back(child);
						}
					}
				}

				// children now go to new leaves
				for (int i = 0; i < 8; i++)
				{
					auto child = this->add(*actors[i]);
					if (!child.expired())
					{
						actors[i]->spatialImage = child;
						break;
					}
				}
				actors.clear();

				return this->add(a);
			}
			else if (true)
			{
				// push_back
				Actor* b = (Actor*)&a;
				actors.push_back(b);
				//actors.push_back(&a);
				return weak_from_this();
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				auto child_return = children[i]->add(a);
				// channges this?
				if (!child_return.expired())
					return child_return;
			}
		}
	}
	else
		return weak_ptr<Octree>();
}

void Octree::deconstruct() 
{
	if (children.size() > 0)
	{
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->deconstruct();
			children[i].reset();
		}
	}
	else
	{
		cout << "leaf destroyed\n";
	}
}

// should only be called from root
void Octree::reconstruct(const vector<Actor*> &actors)
{
	if (!parent.expired()) return;
	children = vector<shared_ptr<Octree>>();
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
		for(int i = 0; i < children.size(); i++)
			children[i]->represent();
	}
}