#pragma once
#include "GameObject.h"
#include "Fire.h"
#include <GL/glut.h>
#include <vector>

/*
A point on our grid mesh which can instantiate a fire emitter
*/
class IgnitionPoint : public GameObject
{
public:
	IgnitionPoint(glm::vec3 position);
	~IgnitionPoint();

	// from GameObject
	void draw();
	void update(float dt, glm::vec3 wind);

	// Properties
	float ttl;
	bool dead;
	bool hasSpread = false;		// has it already spread
	float spreadTime;			// how long until it spreads
	float spreadRadius = 1;		// how far it will spreads
	float constructRad = 1.5;	//approx. sqrt(2), allows mesh construction to reach diagonal
	Fire* fire = 0;
	std::vector<IgnitionPoint*> neighbours; // for constructing the threads

	// For fire spreading
	void spread(std::vector<IgnitionPoint*> burnPoints);
	void spreadOutsideGrid(std::vector<IgnitionPoint*> burnPoints);
	bool inRange(IgnitionPoint* _other);
	float distance(IgnitionPoint* _p1, IgnitionPoint* _p2);
	bool noDuplicates(IgnitionPoint* _other);
	void findNeighbors(std::vector<IgnitionPoint*> burnPoints);
	void ignite();
};

