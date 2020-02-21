#include "../../../Dependencies/glm/glm.hpp"
#pragma once
class PartObject
{
	public:
		PartObject();
		glm::vec3 position;
		bool dead;
		virtual void draw();
		virtual void update(float dt);
		virtual void eventMouse(int button, int state, int x, int y);
		virtual void eventMouseMotion(int x, int y);
};

