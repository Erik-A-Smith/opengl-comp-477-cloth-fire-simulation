#include "../Dependencies/glm/glm.hpp"
#pragma once

/*
Abstract class for all our objects
*/
class GameObject
{
	public:
		GameObject();
		glm::vec3 position;
		virtual void draw();
		virtual void update(float dt, glm::vec3 wind);
		virtual void eventMouse(int button, int state, int x, int y);
		virtual void eventMouseMotion(int x, int y);
};

