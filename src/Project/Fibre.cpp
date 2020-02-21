#include "Fibre.h"

Fibre::Fibre(IgnitionPoint* p1, IgnitionPoint* p2)
{
	leftPtr = p1;
	rightPtr = p2;
	left = p1->position;
	right = p2->position;
	length = calcDist(left, right);
	fallVelocity = glm::vec3(0);
}

Fibre::~Fibre()
{
}

void Fibre::draw()
{
	// Color the fibre according to its current status (attached or not to the ignition points)
	if(!leftPtr->dead && !rightPtr->dead)
		glColor4f(0.4, 1, 0.8, 1);
	else if(!leftPtr->dead || !rightPtr->dead)
		glColor4f(0.4, 0.6, 0.4, 1);
	else
		glColor4f(0.7, 0.4, 0.0, 1);
	if (left.y > -20)
	{
		glBegin(GL_LINES);
		glVertex3f(left.x, left.y, left.z);
		glVertex3f(right.x, right.y, right.z);
		glEnd();
	}
}

float Fibre::calcDist(glm::vec3 start, glm::vec3 end)
{
	// Euclidean distance in 3D between the 2 positions
	float dx = end.x - start.x;
	float dy = end.y - start.y;
	float dz = end.z - start.z;
	return sqrt((dx * dx) + (dy * dy) + (dz * dz));
}

void Fibre::update(float dt, glm::vec3 wind)
{
	float gravity = -0.1f;
	// Move the fibre position a bit if left end is detached
	if (leftPtr->dead)
	{
		glm::vec3 newLeft = glm::vec3(left.x, left.y + gravity, left.z);
		if (calcDist(right, newLeft) <= length + 0.05)
		{
			left = newLeft;
		}
	}
	// Move the fibre position a bit if right end is detached
	if (rightPtr->dead)
	{
		glm::vec3 newRight = glm::vec3(right.x, right.y + gravity, right.z);
		if (calcDist(left, newRight) <= length + 0.05)
		{
			right = newRight;
		}
	}

	// If both ignition points are dead, fibre starts falling
	if (leftPtr->dead && rightPtr->dead && fallVelocity == glm::vec3(0))
	{
		fallVelocity = setFallVelo();
		// velocity affected by wind
		fallVelocity += wind;
	}
	else
	{
		left += fallVelocity * dt;
		right += fallVelocity * dt;		
	}
}

glm::vec3 Fibre::setFallVelo()
{
	// Set a random fall velocity if fibre is not attached anymore
	int maxVelocity = 5;
	int yVelocity = rand() % maxVelocity * 2;
	while (yVelocity < 4)
	{
		yVelocity = rand() % maxVelocity * 2;
	}
	return glm::vec3(
		maxVelocity - rand() % maxVelocity * 2,
		yVelocity * -1,
		maxVelocity - rand() % maxVelocity * 2
	);
}
