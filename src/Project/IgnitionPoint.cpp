#include "IgnitionPoint.h"

IgnitionPoint::IgnitionPoint(glm::vec3 _position)
{
	position = _position;
	ttl = rand() % (10 - 5 + 1) + 5; // between 5 and 10 seconds
	dead = false;
	spreadTime  = rand() % (5 - 3 + 1) + 3; // between 3 and 5 seconds
}


IgnitionPoint::~IgnitionPoint()
{
	if (fire) {
		delete(fire);
	}
}

void IgnitionPoint::draw() {

	if (!dead) {

		// Draw the ignition point
		glPointSize(5);
		glBegin(GL_POINTS);
		glColor4f(0.7, 0.7, 0.7, 1);
		glVertex3f(position.x, position.y, position.z);
		glEnd();
	}
	// If fire is ignited, draw it
	if (fire) {
		fire->draw();
	}
}

void IgnitionPoint::update(float dt, glm::vec3 wind) {

	if (fire) {
		ttl -= dt; // decreate time to live
		spreadTime -= dt; // decrease time before the fire spreads
		fire->update(dt, wind);
		fire->position = position; // update the fire to follow the ignition point (currently points don't move)
	}

	if (ttl <= 0 && !dead)
	{
		dead = true;
		fire->dead = true; // update the fire to follow the ignition point
	}
}
void IgnitionPoint::ignite() {
	fire = new Fire(position); // creates the emitter for the fire particle system
}

void IgnitionPoint::spread(std::vector<IgnitionPoint*> burnPoints)
{
	if (!hasSpread && spreadTime <= 0) {
		hasSpread = true; // time to spread to neighboring points

		// Check for all points on the burn grid if they are in range and not already on fire
		for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object) {
			if (inRange(*object) && (*object)->fire == NULL && (*object) != this) {
				(*object)->ignite();
			}
		}
	}
}

void IgnitionPoint::spreadOutsideGrid(std::vector<IgnitionPoint*> burnPoints) {
	// If current ignition point is on fire it can spread to additional ignition points passed to this function
	for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object) {
		if (inRange(*object) && (*object)->fire == NULL && (*object) != this && fire != 0) {
			(*object)->ignite();
		}
	}
}


bool IgnitionPoint::inRange(IgnitionPoint* _other) {
	return distance(this, _other) <= spreadRadius;
}

float IgnitionPoint::distance(IgnitionPoint* _p1, IgnitionPoint* _p2) {
	// Calculates the euclidean distance in 3D
	float dx = _p2->position.x - _p1->position.x;
	float dy = _p2->position.y - _p1->position.y;
	float dz = _p2->position.z - _p1->position.z;
	return sqrt((dx*dx) + (dy*dy) + (dz*dz));
}

bool IgnitionPoint::noDuplicates(IgnitionPoint* _other)
{
	if (!neighbours.empty())
	{
		for (std::vector<IgnitionPoint*>::iterator object = _other->neighbours.begin(); object != _other->neighbours.end(); ++object)
		{
			if ((*object) == this)
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return true;
	}
}

void IgnitionPoint::findNeighbors(std::vector<IgnitionPoint*> burnPoints)
{
	// links the neighboring ignition points of the burn grid
	for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object)
	{
		if ((*object) != this && distance(this, (*object)) <= constructRad && noDuplicates(*object))
		{
			neighbours.push_back(*object);
		}
	}

}