#include "Cube.h"

Cube::Cube(glm::vec3 _p) {
	position = _p;
	color = glm::vec4(0, 0, 1, 1); // blue
	origin = _p; // original position of the particle
	size = 1;
	// place one ignition point on each side of the cube
	burnPoints.push_back(new IgnitionPoint(position + glm::vec3(0, 0, size / 2)));
	burnPoints.push_back(new IgnitionPoint(position + glm::vec3(0, 0, -size / 2)));
	burnPoints.push_back(new IgnitionPoint(position + glm::vec3(size / 2, 0, 0)));
	burnPoints.push_back(new IgnitionPoint(position + glm::vec3(-size / 2, 0, 0)));
	burnPoints.push_back(new IgnitionPoint(position + glm::vec3(0, size / 2, 0)));
	burnPoints.push_back(new IgnitionPoint(position + glm::vec3(0, -size / 2, 0)));
}

Cube::~Cube(){
	for (IgnitionPoint* p : burnPoints) {
		delete(p);
	}
}

void Cube::draw() {
	// Draw the cube
	glTranslatef(position.x, position.y, position.z);
	glColor4f(color.x, color.y, color.z, color.a);
	glutSolidCube(size);
	glTranslatef(-position.x, -position.y, -position.z);	
	// Draw the ignition points for visibility
	for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object) {
		(*object)->draw();
	}
}

void Cube::move(glm::vec3 _dir) {
	position = position + _dir;
	setBurnPoints();
}

void Cube::setBurnPoints() {
	// set ignition points position on sides of the cube based on the position of the cube (center)
	burnPoints[0]->position = position + glm::vec3(0, 0, size/2);
	burnPoints[1]->position = position + glm::vec3(0, 0, -size/2);
	burnPoints[2]->position = position + glm::vec3(size/2, 0, 0);
	burnPoints[3]->position = position + glm::vec3(-size/2, 0, 0);
	burnPoints[4]->position = position + glm::vec3(0, size/2, 0);
	burnPoints[5]->position = position + glm::vec3(0, -size/2, 0);
}

void Cube::update(float dt, glm::vec3 wind) {
	for (std::vector<IgnitionPoint*>::iterator object = burnPoints.begin(); object != burnPoints.end(); ++object) {
		(*object)->update(dt, wind);
	}
}

void Cube::ignite() {
	for (IgnitionPoint* p : burnPoints) {
		p->ignite();
	}
}

