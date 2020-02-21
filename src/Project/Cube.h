#pragma once
#include "../Dependencies/glm/glm.hpp"
#include "GameObject.h"
#include <gl/glut.h>
#include <iostream>
#include "IgnitionPoint.h"

/*
Simple object to move around the scene
*/
class Cube : public GameObject
{
public:
	Cube(glm::vec3 _p);
	~Cube();

	// Properties
	float speed;
	glm::vec3 velocity;
	glm::vec3 origin;
	glm::vec4 color;
	float size;

	// Needed for burning
	std::vector<IgnitionPoint*> burnPoints;
	void setBurnPoints();
	void ignite();
	void update(float dt, glm::vec3 wind);

	// from GameObject
	void move(glm::vec3 _dir);
	void draw();
};