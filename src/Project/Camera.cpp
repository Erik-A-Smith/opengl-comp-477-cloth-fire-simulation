#include "Camera.h"

Camera::Camera() {
	// angle
	x_rot = 0.0f;
	y_rot = 0.0f;

	// positions
	x_diff = 0.0f;
	y_diff = 0.0f;
}

void Camera::eventMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_diff = x - y_rot;
		y_diff = -y + x_rot;
	}
}

void Camera::eventMouseMotion(int x, int y) {
	x_rot = y + y_diff;
	y_rot = x - x_diff;
}
