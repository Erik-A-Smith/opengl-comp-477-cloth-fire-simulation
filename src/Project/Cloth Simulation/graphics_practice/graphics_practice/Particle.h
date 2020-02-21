#pragma once
#include "glm.hpp"
#include "GameObject.h"
#include <gl/glut.h>
#include <iostream>


class Particle: public GameObject
{
	public:
		Particle(glm::vec3 _p, glm::vec3 _v, glm::vec4 _c, float _ttl);

		//properties
		glm::vec3 velocity;
		glm::vec4 color;
		float ttl;
		glm::vec3 origin;
		bool dead;

		glm::vec3 randomVelocity();
		void setVelocity(glm::vec3 _v);
		void setTtl(float _ttl);
		void respawn(glm::vec3 _p, glm::vec3 _v, glm::vec4 _c, float _ttl);

		// from GameObject
		void update(float dt);
		void draw();
};

