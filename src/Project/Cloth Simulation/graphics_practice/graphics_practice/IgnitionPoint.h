#pragma once
#include "GameObject.h"
#include "Fire.h"
#include <GL/glut.h>

class IgnitionPoint : public GameObject
{
public:
	IgnitionPoint(glm::vec3 position);
	~IgnitionPoint();
	void draw();
	void update(float dt);
	void ignite();
	float ttl;
	bool dead;
	Fire* fire = 0;

	bool hasSpread = false;		// has it already spread
	float spreadTime;			// how long until it spreads
	float spreadRadius = 0.1f;		// how far it will spreads
	void spread(std::vector<IgnitionPoint*> burnPoints);
	bool inRange(IgnitionPoint* _other);
	float distance(IgnitionPoint* _p1, IgnitionPoint* _p2);
};

