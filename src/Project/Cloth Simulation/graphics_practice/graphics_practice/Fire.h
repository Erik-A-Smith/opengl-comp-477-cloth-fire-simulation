#pragma once
#include "glm.hpp"
#include "GameObject.h"
#include "Particle.h"
#include <gl/glut.h>
#include <iostream>
#include <vector>

class Fire : public GameObject
{
public:
	Fire(glm::vec3 position);
	~Fire();

	// Trickle fire particles (avoid boom of initial particles)
	void trickle(float dt);

	// From GameObject
	void draw();
	void update(float dt);

	// Properties
	bool dead = false;
	float flowRate = .05f;	// time between next released particle (until max intensity)
	float lastTrickle = 0;	// time since last released particle
	int intensity = 100;	// number of particles to create
	std::vector<Particle*> particles;
};

