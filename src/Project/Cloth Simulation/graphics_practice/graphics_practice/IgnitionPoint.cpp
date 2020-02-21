#include "main.h"
#include "IgnitionPoint.h"

IgnitionPoint::IgnitionPoint(glm::vec3 _position)
{
	position = _position;
	ttl = 1;
	//ttl = rand() % (10 - 5 + 1) + 5; // between 5 and 10 seconds
	dead = false;
	spreadTime = 1;
	//spreadTime  = rand() % (5 - 3 + 1) + 3; // between 3 and 5 seconds
}


IgnitionPoint::~IgnitionPoint()
{
	if (fire) {
		delete(fire);
	}
}

void IgnitionPoint::draw() {

	if (!dead) {
		//program.Use(); {
		//	glUniformMatrix4fv(program("ModelView"), 1, GL_TRUE, model_view);
		//		// Draw the ignition point (for debugging)
		glPointSize(5);
		glBegin(GL_POINTS);
		glColor4f(0.7, 0.7, 0.7, 1);
		glVertex3f(position.x, position.y, position.z);
		glEnd();

		//	const int size = sizeof(glm::vec3);
		//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[0]);
		//	glBufferData(GL_SHADER_STORAGE_BUFFER, size, &position.x, GL_DYNAMIC_DRAW);

		//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[1]);
		//	glBufferData(GL_SHADER_STORAGE_BUFFER, size, &position.y, GL_DYNAMIC_DRAW);

		//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[2]);
		//	glBufferData(GL_SHADER_STORAGE_BUFFER, size, &position.z, GL_DYNAMIC_DRAW);
		//}
	}

	if (fire) {
		fire->draw();
	}
}

void IgnitionPoint::update(float dt) {

	if (fire) {
		ttl -= dt;
		spreadTime -= dt;
		fire->update(dt);
		fire->position = position; // update the fire to follow the ignition point
	}

	if (ttl <= 0 && !dead)
	{
		dead = true;
		fire->dead = true; // update the fire to follow the ignition point
	}
}
void IgnitionPoint::ignite() {
	fire = new Fire(position);
}

void IgnitionPoint::spread(std::vector<IgnitionPoint*> burnPoints)
{
	if (!hasSpread && spreadTime <= 0) {
		hasSpread = true;

		for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object) {
			if (inRange(*object) && (*object)->fire == NULL && (*object) != this) {
				(*object)->ignite();
			}
		}
	}
}

bool IgnitionPoint::inRange(IgnitionPoint* _other) {
	return distance(this, _other) <= spreadRadius;
}

float IgnitionPoint::distance(IgnitionPoint* _p1, IgnitionPoint* _p2) {
	float dx = _p2->position.x - _p1->position.x;
	float dy = _p2->position.y - _p1->position.y;
	float dz = _p2->position.z - _p1->position.z;
	return sqrt((dx*dx) + (dy*dy) + (dz*dz));
}