#include "BurnGrid.h"

BurnGrid::BurnGrid(glm::vec3 _position, std::vector<vec4> points)
{
	position = _position;		// center of the grid
	setPoints(points);	// create grid
	ignite();					// light a random point
}

void BurnGrid::setPoints(std::vector<vec4> points)
{
	int index = 0;
	int dist = 1;

	for (std::vector<vec4>::iterator object = points.begin(); object != points.end(); ++object) {
		

		glm::vec3 pPosition = glm::vec3(
			object->x,
			object->y,
			object->z
		);
		burnPoints.push_back(new IgnitionPoint(pPosition));
	}


	// Create point grid
	/*for (size_t i = 0; i < pointsSquared; i++)
	{
		for (size_t j = 0; j < pointsSquared; j++)
		{
			int mid = (pointsSquared / 2);
			float tmp2 = ((float)i - mid);
			float tmp0 = ((i - mid) * dist);
			float tmp1 = position.x + tmp0;
			glm::vec3 pPosition = glm::vec3(
				position.x + ((-mid + (float)i) * dist),
				0,
				position.y + ((-mid + (float)j) * dist)
			);
			burnPoints.push_back(new IgnitionPoint(pPosition));
		}
	}*/
}

BurnGrid::~BurnGrid()
{
}

void BurnGrid::draw() {
	for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object) {
		if (!(*object)->dead) {
			(*object)->draw();
		}
	}
}

void BurnGrid::update(float dt) {
	for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object) {
		(*object)->update(dt);
		(*object)->spread(burnPoints);
	}
}

void BurnGrid::ignite() {
	std::vector<IgnitionPoint*>::iterator randIt = burnPoints.begin();
	std::advance(randIt, std::rand() % burnPoints.size());
	(*randIt)->ignite();
}