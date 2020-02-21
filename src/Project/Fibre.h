#pragma once
#include <gl/glut.h>
#include "../Dependencies/glm/glm.hpp"
#include "GameObject.h"
#include "IgnitionPoint.h"

/*
Thread or line between 2 ignition points on our grid
*/
class Fibre : public GameObject
{
public:
	Fibre(IgnitionPoint* p1, IgnitionPoint* p2);
	~Fibre();

	// Properties
	IgnitionPoint* leftPtr;
	IgnitionPoint* rightPtr;
	glm::vec3 left;
	glm::vec3 right;
	glm::vec3 fallVelocity;
	float length;

	// From GameObject
	void draw();
	void update(float dt, glm::vec3 wind);

	// For the fibre movement
	float calcDist(glm::vec3 start, glm::vec3 end);
	glm::vec3 setFallVelo();

};