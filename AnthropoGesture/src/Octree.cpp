#pragma once
#include "Octree.h"
#include "Actor.h"

Octree::Octree(Octree* _parent, glm::vec3 _center, glm::vec1 _halfLength, short int _kidNumber)
{
	parent = _parent;
	center = _center;
	halfLength = _halfLength;
	kidNumber = _kidNumber;
	actors.reserve(8);
}

Octree::Octree() 
{
	parent = nullptr;
	center = glm::vec3(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 0);
	halfLength = glm::vec1(ofGetWidth() / 2.0f);
	kidNumber = -1;
	actors.reserve(8);
}

Octree* Octree::add(const Actor &a)
{
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
				for (int z = -1; z <= 1; z += 2)
				{
					for (int y = -1; y <= 1; y += 2)
					{
						for (int x = -1; x <= 1; x += 2)
						{
							glm::vec3 childCenter = center +
								glm::vec3(
									x*quarterLength.x,
									y*quarterLength.x,
									z*quarterLength.x
								);
							children.push_back(make_unique<Octree>(this, childCenter, quarterLength, 4*z+2*y+x));
						}
					}
				}

				// actors in current node are added to children nodes
				for (int i = 0; i < children.size(); i++)
				{
					auto child = this->add(*(actors[i]));
					actors[i]->spatialImage = child;
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
			return nullptr;
		}
	}
	else
		return nullptr;
}

void Octree::deconstruct() 
{
	for (int i = 0; i < children.size(); i++)
		children[i].reset();
	children = vector<unique_ptr<Octree>>();
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
	//ofSetColor(102, 166, 199, 10);
	ofDrawBox(center, halfLength.x*2, halfLength.x * 2, halfLength.x * 2);
	if (children.size() > 0)
	{
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->represent();
		}
	}
	/*
	else
	{
		ofSetColor(0, 0, 0);
		ofFill();
		vector<glm::vec3> verts;
		if (actors.size() >= 1)
		{
			for each(Actor* a in actors)
				verts.push_back(a->position);
			ofMesh mesh(OF_PRIMITIVE_TRIANGLE_STRIP, verts);
			ofSetColor(0, 255, 255);
			mesh.drawWireframe();
			//ofSetColor(100, 100, 100, 20);
			//ofFill();
			//mesh.drawFaces();
			
			//for (int i = 1; i < actors.size(); i++)
			//	ofDrawLine(actors[i - 1]->position, actors[i]->position);
			//ofDrawLine(actors[0]->position, actors[actors.size() - 1]->position);
			
		}
	}
	*/
}

void Octree::fixedRadiusNearestActorSearch(Actor* a, vector<Actor*> &results)
{
	// return all actors in current node
	// ask parent for possibly relevant children
	// current plan: only go up one level

}

void Octree::fixedRadiusNearestActorSearchHelper(Actor* a, vector<Actor*> &results, short int kidWhoAsked)
{
	// find vector that represents difference of midpoint and actor pos
	glm::vec3 d = center - a->position;
	for (int i = 0; i < children.size(); i++)
	{
		// mask it's 3 component vectors with an XOR operation
		short int relevantComponentVectorMask = i ^ kidWhoAsked;
		d.x *= relevantComponentVectorMask & 1;
		d.y *= (relevantComponentVectorMask >> 1) & 1;
		d.z *= (relevantComponentVectorMask >> 2) & 1;
		if (glm::length(d) <= 60.0f)
		{
			results.insert(results.end(), (children[i]->actors).begin(), (children[i]->actors).end());
		}
	}
}