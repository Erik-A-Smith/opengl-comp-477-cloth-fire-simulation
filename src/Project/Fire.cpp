#include "Fire.h"

Fire::Fire(glm::vec3 _position)
{
	position = _position;

	// single initial particle to signal fire is lit
	particles.push_back(
		new Particle(
			_position,
			glm::vec3(0), // will generate random velocity
			2
		)
	);
}

void Fire::trickle(float dt) {
	// increment time since last added particle
	lastTrickle += dt;

	if (particles.size() != intensity && lastTrickle >= flowRate) {

		// Add new particle
		lastTrickle = 0;
		particles.push_back(
			new Particle(
				position,
				glm::vec3(0), // will generate random velocity
				2
			)
		);

	}
}

Fire::~Fire()
{

}


void Fire::draw()
{
	for (std::vector<Particle*>::iterator object = particles.begin(); object != particles.end(); ++object) {
		(*object)->draw();
	}
}

void Fire::update(float dt, glm::vec3 wind)
{
	trickle(dt);

	for (std::vector<Particle*>::iterator object = particles.begin(); object != particles.end(); ++object) {
		// If the current particle died but the fire is still alive, respawn it at the bottom (emitter)
		// this keeps the total number of particle for a fire at a max limit
		if ((*object)->dead && !dead) {
			(*object)->respawn(
				position,
				glm::vec3(0), // will generate random velocity
				glm::vec4(1.0, .3, .0, 1),
				5
			);
		}

		(*object)->update(dt, wind);
	}
}