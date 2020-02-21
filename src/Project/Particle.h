#pragma once
#include "../Dependencies/glm/glm.hpp"
#include "GameObject.h"
#include <gl/glut.h>
#include <iostream>

/*
A particle of the fire
*/
class Particle: public GameObject
{
	public:
		Particle(glm::vec3 _p, glm::vec3 _v, float _ttl);

		// Properties
		glm::vec3 velocity;
		glm::vec4 color;
		glm::vec3 wind;
		glm::vec3 origin;
		float ttl;
		bool dead;
		float size;

		// for the particle movement
		glm::vec3 randomVelocity();
		void setVelocity(glm::vec3 _v);
		void setTtl(float _ttl);
		void respawn(glm::vec3 _p, glm::vec3 _v, glm::vec4 _c, float _ttl);

		// from GameObject
		void update(float dt, glm::vec3 wind);
		void draw();
};

