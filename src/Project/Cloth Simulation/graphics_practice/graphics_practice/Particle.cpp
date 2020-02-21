#include "main.h"
#include "Particle.h"

Particle::Particle(glm::vec3 _p,glm::vec3 _v, glm::vec4 _c, float _ttl) {
	position = _p;
	setVelocity(_v);
	color = _c;
	setTtl(_ttl); // Time to live
	origin = _p; // original position of the particle
	dead = false;
}

void Particle::update(float dt) {

	if (!dead) {
		// reduce time to live
		ttl -= dt;

		// die
		if (ttl < 0.0f) {
			dead = true;
		}

		// alive and well
		else {
			position += velocity * dt; // move
			color.a -= dt * (((rand() % 11) / 100) + 1); // color shift
		}
	}
}

void Particle::draw() {
	if (!dead) {
		/*mvstack.push(model_view);
		program.Use(); {
			glUniformMatrix4fv(program("ModelView"), 1, GL_TRUE, model_view);

			// Draw Lines
			glBindBuffer(GL_ARRAY_BUFFER, g_verticesBuffer[2]);
			glVertexAttribPointer(program["vPosition"], 4, GL_FLOAT, GL_FALSE, 5, 0);
	
			glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, &(indices[7 * 3]));

			const int size = sizeof(glm::vec3);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[0]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, &position.x, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[1]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, &position.y, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[2]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, &position.z, GL_DYNAMIC_DRAW);

		}
		model_view = mvstack.pop();*/

		glUniformMatrix4fv(program("ModelView"), 1, GL_TRUE, model_view);
		glTranslatef(position.x, position.y, position.z);
		glColor4f(color.x, color.y, color.z, color.a);
		glutSolidSphere(0.05, 10, 10);
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
	color = _c;
	setTtl(_ttl); // Time to live
	dead = false;
}