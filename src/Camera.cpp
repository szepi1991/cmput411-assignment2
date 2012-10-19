/*
 * Camera.cpp
 *
 *  Created on: 2012-10-17
 *      Author: david
 */

#include "Camera.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


Camera::Camera() {
	//set the default camera: looking at origin from +5 on z axis
	eyeXP = 8.0, eyeYP = 5.0, eyeZP = 10.0;
	centerXP = 0, centerYP = 0, centerZP = 0.0;
	upXP = 0, upYP = 1, upZP = 0;

}

void Camera::moveX(double dx) { eyeXP += dx;}
void Camera::moveZ(double dz) { eyeZP += dz;}


/* Sets up the view. To be called before any drawing! */
void Camera::view() {
	// TODO finalize later..
	glLoadIdentity();

	gluLookAt(eyeXP, eyeYP, eyeZP, centerXP, centerYP,
		centerZP, upXP, upYP, upZP);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 1.0, 100);
	glMatrixMode(GL_MODELVIEW);
}

Camera::~Camera() {
}

