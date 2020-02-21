#pragma once
#include "../Dependencies/glm/glm.hpp"
#include "GameObject.h"
#include "Particle.h"
#include <gl/glut.h>
#include <iostream>
#include <vector>

/*
The emitter for our fire particle system
*/
class Fire : public GameObject
{
public:
	Fire(glm::vec3 position);
	~Fire();

	// Trickle fire particles (avoid boom of initial particles)
	void trickle(float dt);

	// From GameObject
	void draw();
	void update(float dt, glm::vec3 wind);

	// Properties
	bool dead = false;
	float flowRate = .05f;	// time between next released particle (until max intensity)
	float lastTrickle = 0;	// time since last released particle
	int intensity = 400;	// number of particles to create
	std::vector<Particle*> particles;
};

