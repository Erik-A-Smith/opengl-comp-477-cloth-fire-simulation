
#pragma once
#include <gl/glut.h>
#include "../Dependencies/glm/glm.hpp"
#include "GameObject.h"
#include "Particle.h"
#include "IgnitionPoint.h"
#include "Fibre.h"
#include <vector>

/*
This would be a cloth mesh
*/
class BurnGrid : public GameObject
{
public:
	BurnGrid(glm::vec3 position, int pointsSquared);
	~BurnGrid();
	void setPoints(int pointsSquared); // dynamically generate the grid of ignition points
	void weaveFibres(std::vector<IgnitionPoint*> burnPoints); // constructs thread-like connections between points

	// from GameObject
	void draw();
	void update(float dt, glm::vec3 wind, std::vector<IgnitionPoint*> otherPoints);

	// Needed for burning
	void ignite();
	std::vector<IgnitionPoint*> burnPoints;
	std::vector<Fibre*> threads;
};

