#pragma once
#include <gl/glut.h>

/*
Standard camera for our scene
*/
class Camera
{
	public:
		Camera();
		void eventMouse(int button, int state, int x, int y);
		void eventMouseMotion(int x, int y);
		float x_diff, y_diff, x_rot, y_rot;
};

