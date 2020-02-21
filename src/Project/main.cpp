#include <gl/glut.h>
#include "../Dependencies/glm/glm.hpp"
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
#include "GameObject.h"
#include "BurnGrid.h"
#include "Camera.h"
#include <iostream>
#include "Cube.h"

using namespace std;

// All objects in the scene
BurnGrid* burngrid;
Camera* camera = new Camera();
glm::vec3 windForce;
Cube* cube;

// Time calculators
float lastFrameTime, dt;

// Setting Variables
int fullscreen = 0;
int mouseDown = 0;

// Perspective settings
float grow_shrink = 70.0f;
float resize_f = 1.0f;

/*
Populate scene with objects
*/
void populateScene() {
	burngrid = new BurnGrid(glm::vec3(0), 12);
	cube = new Cube(glm::vec3(0, 0, 0));
	windForce = glm::vec3(0, 0, 0);
}

/*
Delete scene objects and respawn them
*/
void resetScene() {
	//delete pointers
	delete(burngrid);
	delete(cube);
	populateScene();
}

/*
Calculate the delta time between frames
*/
void calculateDeltaTime() {
	dt = (std::clock() - lastFrameTime) / 1000;
	lastFrameTime = std::clock();
}

/*
Update all the objects on the scene
*/
void updateObjects() {
	burngrid->update(dt, windForce * dt, cube->burnPoints);
	cube->update(dt, windForce * dt);
}

/*
Draw all the objects on the scene
*/
void drawObjects(){
	burngrid->draw();
	cube->draw();
}

/*
Display run instructions to the console
*/
void information() {
	cout << "Welcome to Team 6's spreading fire particle system!" << endl << "Controls:" << endl;
	cout << "Holding down the left mouse button, move the mouse to change camera position" << endl;
	cout << "Use WASD keys to move around the blue cube" << endl;
	cout << "'q'    Activate wind force" << endl;
	cout << "'r'    To reset the scene" << endl;
	cout << "'ESC'  Exit the application" << endl;
}

/*
Initialize  openGL (GLut)
*/
int init(void)
{
	// Set clock
	lastFrameTime = std::clock();
	dt = std::clock();

	// Skybox color
	glClearColor(0, 0, .1, 1);

	glEnable(GL_DEPTH_TEST);

	// allow see through
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	return 1;
}


/*
Update function ran every frame render
*/
void display(void)
{
	calculateDeltaTime();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Camera look position
	gluLookAt(
		0.0f, 10.0f, 20.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	// Camera Rotation
	glRotatef((*camera).x_rot, 1.0f, 0.0f, 0.0f);
	glRotatef((*camera).y_rot, 0.0f, 1.0f, 0.0f);	
	
	// Draw all scene objects
	drawObjects();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

/*
Windows resized event listener
*/
void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(grow_shrink, resize_f * w / h, resize_f, 100 * resize_f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
Keyboard event listener
*/
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	// Wind force
	case 'q':
		if (windForce != glm::vec3(0))
			windForce = glm::vec3(0);	
		else
			windForce = glm::vec3(0, 0, -50);
		break;
	// Reset
	case 'r':
		resetScene();
		break;
	// Cube movement
	case 'w':
		cube->move(glm::vec3(0, 0.5, 0));
		break;
	case 's':
		cube->move(glm::vec3(0,-0.5, 0));
		break;
	case 'd':
		cube->move(glm::vec3(0.5, 0, 0));
		break;
	case 'a':
		cube->move(glm::vec3(-0.5, 0, 0));
		break;
	//Escape Key
	case 27:
		exit(1);
		break;
	}
	
	glutPostRedisplay();
}

/*
Mouse click event listenr
*/
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = 1;
		// Call event listeners on the burngrid
		burngrid->eventMouse(button, state, x, y);

		// Call event listeners for the camera
		camera->eventMouse(button, state, x, y);
	}
	else
		mouseDown = 0;
}

/*
Mouse movement event listener
*/
void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		//Call event listeners on the burngrid
		burngrid->eventMouseMotion(x, y);

		//Call event listeners for the camera
		camera->eventMouseMotion(x, y);

		glutPostRedisplay();
	}
}

/*
Main application call
*/
int main(int argc, char* argv[])
{
	// Innitialize App
	glutInit(&argc, argv);

	// Create the window
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	// Render mode
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// Print instructions
	information();

	//Title
	glutCreateWindow("Group 6 - Final Project");

	//Bind Callbacks
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(resize);
	glutIdleFunc(updateObjects);

	// Error check if initialiation failed
	if (!init())
		return 1;

	// Set scene Objects
	populateScene();

	// FreeGlut game loop call
	// Calls the bound callbacks
	glutMainLoop();

	return 0;
}