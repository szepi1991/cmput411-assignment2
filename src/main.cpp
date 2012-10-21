/*
 * main.cpp
 *
 *  Created on: 2012-10-16
 *      Author: david
 */

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


#include <iostream>
#include <boost/shared_ptr.hpp>

#include "Animation.h"
#include "ParseException.h"
#include "Camera.h"
#include "tools.h"

using namespace std;


unsigned SkeletonNode::nodeCounter=0;
static boost::shared_ptr<Animation> anim;
static Camera cam;

void setup(int argc, char **argv) throw (int) {

	if (argc != 2) {
		cerr << "ERROR: this program takes exactly 1 argument: the .obj file to load." << endl;
		throw 1;
	}

	cout << "Reading in file now." << endl;
	try {
		anim.reset(new Animation(argv[1]));
		cout << "The name of the loaded file is " << anim->getFileName() << endl;

	} catch (ParseException& e) {
		cerr << e.what() << endl;
		throw 2;
	}

}

/* OpenGL window reshape routine.
 * Don't have to do much, since the projection is set before each drawing
 */
void resize(int w, int h)
{
	// Set viewport size to be entire OpenGL window.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void drawAxes(float length) {
	// draw the 3 axis (x, y, z) <--> (red, green, blue)
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(length, 0, 0);
	glEnd();
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);
	glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, length);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
}

// Drawing (display) routine.
void drawScene(void)
{
	// Clear screen to background color.
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cam.view();

	drawAxes(10);

	// Set foreground (or drawing) color.
	glColor3f(0.0, 0.0, 0.0);

	anim->display();

	glutSwapBuffers();
}


void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		exit(0);
		break;
	case 'p':
		anim->startAnim();
		break;
	case 'P':
		anim->stopAnim();
		break;
	case 's':
		anim->reset();
		break;
	}
}

// Special keyboard input processing routine.
void specialKeyInput(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		cam.moveZ(-1);
	if (key == GLUT_KEY_DOWN)
		cam.moveZ(1);
	if (key == GLUT_KEY_LEFT)
		cam.moveX(-1);
	if (key == GLUT_KEY_RIGHT)
		cam.moveX(1);
//	glutPostRedisplay();
}

void animate(int arg) {

	glutPostRedisplay();
	glutTimerFunc((unsigned) (anim->getFrameTime()*SECtoMSEC), animate, 0);
}



int main(int argc, char **argv) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("box.cpp");

	// Set background (or clearing) color.
	glClearColor(1.0, 1.0, 1.0, 0.0);
	// Initialize.
	try {
		setup(argc, argv);
	} catch (int e) {
		cerr << "The program is going to terminate now." << endl;
		return 1;
	}

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
//	glutIdleFunc(doWhenIdle); // we'll prob have to use this now
	glutTimerFunc((unsigned) anim->getFrameTime()*1000000, animate, 0);
	glutMainLoop();


	cout << "Exit!" << endl;
	return 0;
}



