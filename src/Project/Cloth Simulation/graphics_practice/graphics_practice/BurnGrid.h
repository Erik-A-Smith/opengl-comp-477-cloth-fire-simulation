
#pragma once
#include "Angel.h"

#include "glm.hpp"
#include "GameObject.h"
#include "Particle.h"
#include "IgnitionPoint.h"
#include <vector>

/*
This would be your cloth
*/
class BurnGrid : public GameObject
{
public:
	BurnGrid(glm::vec3 position, std::vector<vec4> points);
	~BurnGrid();
	void setPoints(std::vector<vec4> points); // dynamically generate the grid of ignition points

	// from GameObject
	void draw();
	void update(float dt);

	// Needed for burning
	void ignite();
	std::vector<IgnitionPoint*> burnPoints;
};

