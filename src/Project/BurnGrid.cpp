#include "BurnGrid.h"
#include <time.h>

BurnGrid::BurnGrid(glm::vec3 _position, int _pointsSquared)
{
	position = _position;		// center of the grid
	setPoints(_pointsSquared);	// create grid
	weaveFibres(burnPoints);
	ignite();					// light a random point
}

BurnGrid::~BurnGrid() {
	// Delete all the pointers to avoid memory leak
	for (IgnitionPoint* p : burnPoints) {
		delete(p);
	}
	for (Fibre* f : threads) {
		delete(f);
	}
}

void BurnGrid::setPoints(int pointsSquared)
{
	int index = 0;
	int dist = 1;

	// Create ignition points grid
	for (size_t i = 0; i < pointsSquared; i++)
	{
		for (size_t j = 0; j < pointsSquared; j++)
		{
			int mid = (pointsSquared / 2);
			float tmp2 = ((float)i - mid);
			float tmp0 = ((i - mid) * dist);
			float tmp1 = position.x + tmp0;
			glm::vec3 pPosition = glm::vec3(
				position.x + ((-mid + (float)i) * dist),
				position.y + ((-mid + (float)j) * dist),
				0
			);
			IgnitionPoint* newPoint = new IgnitionPoint(pPosition);
			burnPoints.push_back(newPoint);
			newPoint->findNeighbors(burnPoints);
		}
	}
}

void BurnGrid::draw() {
	// Draw every ignition points
	for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object) {
		if (!(*object)->dead) {
			(*object)->draw();
			
		}
	}
	// Draw every fibre between the points
	for (std::vector<Fibre*>::iterator object = threads.begin(); object != threads.end(); ++object) {
		(*object)->draw();
	}
}

void BurnGrid::update(float dt, glm::vec3 wind, std::vector<IgnitionPoint*> otherPoints) {
	// For every ignition point on the grid, update it and check if it can spread
	for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object) {
		(*object)->update(dt, wind);
		(*object)->spread(burnPoints);
		(*object)->spreadOutsideGrid(otherPoints);
	}
	// Update all fibre threads
	for (std::vector<Fibre*>::iterator object = threads.begin(); object != threads.end(); ++object) {
		(*object)->update(dt, wind);
	}
}

void BurnGrid::ignite() {
	// Choose a random point on the grid to ignite first
	std::vector<IgnitionPoint*>::iterator randIt = burnPoints.begin();
	srand(time(NULL));
	std::advance(randIt, std::rand() % burnPoints.size());
	
	(*randIt)->ignite();
}

void BurnGrid::weaveFibres(std::vector<IgnitionPoint*> burnPoints)
{
	// Creates the threads by adding a new fibre for each 2 neighboring ignition points on the grid
	for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object)
	{
		for (std::vector<IgnitionPoint*>::iterator otherObj = (*object)->neighbours.begin(); otherObj != (*object)->neighbours.end(); ++otherObj)
		{
			threads.push_back(new Fibre((*object), (*otherObj)));
		}
	}
}