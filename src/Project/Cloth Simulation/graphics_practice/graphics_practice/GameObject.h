#include "glm.hpp"
#pragma once
class GameObject
{
	public:
		GameObject();
		glm::vec3 position;
		virtual void draw();
		virtual void update(float dt);
		virtual void eventMouse(int button, int state, int x, int y);
		virtual void eventMouseMotion(int x, int y);
};

