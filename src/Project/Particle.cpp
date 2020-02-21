#include "Particle.h"

Particle::Particle(glm::vec3 _p,glm::vec3 _v, float _ttl) {
	position = _p;
	setVelocity(_v);
	color = glm::vec4(1.0, 0.6, 0, 1.0);
	setTtl(_ttl); // Time to live
	origin = _p; // original position of the particle
	dead = false;
	size = 0.3;
}

void Particle::update(float dt, glm::vec3 wind) {

	if (!dead) {
		// reduce time to live
		ttl -= dt;

		// die if time to live is expired
		if (ttl < 0.0f) {
			dead = true;
		}

		// alive and well
		else {
			position += velocity * dt; // move
			size -= 0.1 * dt; // size decreases
			// color shift
			color.a -= dt * (((rand() % 11) / 100) + 1); 
			if (color.y <= 0)
				color.y = 0;
			else
				color.y -= dt;
			// velocity affected by wind
			velocity += wind;
		}
	}
}

void Particle::draw() {
	if (!dead) {
		// Translate to particle position to draw it & translate back
		glTranslatef(position.x, position.y, position.z);
		glColor4f(color.x, color.y, color.z, color.a);
		glutSolidSphere(size, 10, 10);
		glTranslatef(-position.x, -position.y, -position.z);
	}
}

glm::vec3 Particle::randomVelocity() {
	int maxVelocity = 7;
	int yVelocity = rand() % maxVelocity * 2;
	while (yVelocity < 4)
	{
		yVelocity = rand() % maxVelocity * 2;
	}
	return glm::vec3(
		maxVelocity - rand() % maxVelocity*2,
		yVelocity,
		maxVelocity - rand() % maxVelocity*2
	);
}

void Particle::setVelocity(glm::vec3 _v) {
	if (_v == glm::vec3(0)) {
		velocity = randomVelocity();
	}
	else {
		velocity = _v;
	}
}

void Particle::setTtl(float _ttl) {
	int maxTtl = 60; // 10th of seconds
	int minTtl = 10; // 10th of seconds
	if (_ttl == 0) {
		ttl = ( (minTtl / 10 ) + (float)(rand() % (maxTtl - minTtl)) / 10 );
	}
	else {
		ttl = _ttl;
	}
}

void Particle::respawn(glm::vec3 _p, glm::vec3 _v, glm::vec4 _c, float _ttl) {
	position = _p;
	setVelocity(_v);
	color = glm::vec4(1.0, 0.6, 0, 1.0);
	setTtl(_ttl); // Time to live
	dead = false;
	size = 0.3;
}