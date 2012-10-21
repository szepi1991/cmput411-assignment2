/*
 * Camera.cpp
 *
 *  Created on: 2012-10-17
 *      Author: david
 */

#include "Camera.h"

#include <cassert>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265359

// TODO make sure we update position - matrix pair.. always

Camera::Camera() {
	//set the default camera: looking at origin from +5 on z axis
	eyeXP = 8.0, eyeYP = 10.0, eyeZP = 15.0;
	centerXP = 0, centerYP = 0, centerZP = 0.0;
	upXP = 0, upYP = 1, upZP = 0;
}

// this resets the camera to the origin facing in default things blabla
// NEEDS TO BE CALLED IN SETUP!!
void Camera::reset() {
	glMatrixMode(GL_MODELVIEW); // make sure we have the right matrix selected
	glPushMatrix(); // make a copy
	glLoadIdentity(); // do nothing
	glGetDoublev(GL_MODELVIEW_MATRIX, cameraTrans); // save nothing
	glPopMatrix(); // reset modelview to original settings

}

/* Sets up the view. To be called before any drawing! */
void Camera::view() {
	// TODO finalize later..
	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();

//	std::cout << std::fixed;
//	std::cout.precision(6);
//	for (int i = 0; i < 4; ++i) {
//		for (int j = 0; j < 4; ++j) {
//			std::cout << "\t" << cameraTrans[i*4+j];
//		}
//		std::cout << std::endl;
//	}
	glMultMatrixd(cameraTrans);
//	gluLookAt(eyeXP, eyeYP, eyeZP, centerXP, centerYP,
//		centerZP, upXP, upYP, upZP);

}


//// rotate the cam ccw around its own position about the x axis (tilt)
//void Camera::rotX(double deg) {
//	double rad = deg * PI / 180.0;
//	double centerYPold = centerYP - eyeYP, centerZPold = centerZP - eyeZP;
//	centerYP = cos(rad) * centerYPold - sin(rad) * centerZPold;
//	centerZP = sin(rad) * centerYPold + cos(rad) * centerZPold;
//	centerYP += eyeYP; centerZP += eyeZP;
//}
//
//// rotate the cam ccw around its own position about the y axis (pan)
//void Camera::rotY(double deg) {
//	double rad = deg * PI / 180.0;
//	double centerXPold = centerXP -eyeXP, centerZPold = centerZP - eyeZP;
//	centerXP = cos(rad) * centerXPold - sin(rad) * centerZPold;
//	centerZP = sin(rad) * centerXPold + cos(rad) * centerZPold;
//	centerXP += eyeXP; centerZP += eyeZP;
//}
//
//// rotate the cam ccw around its own position about the z axis (roll)
//void Camera::rotZ(double deg) {
//	double rad = deg * PI / 180.0;
//	double centerXPold = centerXP -eyeXP, centerYPold = centerYP - eyeYP;
//	centerXP = cos(rad) * centerXPold - sin(rad) * centerYPold;
//	centerYP = sin(rad) * centerXPold + cos(rad) * centerYPold;
//	centerXP += eyeXP; centerYP += eyeYP;
//}

// translate the camera by the given amounts (with respect to its local frame)
void Camera::translateCamera(double x, double y, double z) {
	updateMatrix();

	glMatrixMode(GL_MODELVIEW); // make sure we have the right matrix selected
	glPushMatrix(); // make a copy
	glLoadIdentity(); // do nothing
	glTranslated(-x, -y, -z); // add the translation
	glMultMatrixd(cameraTrans); // now add already existing camera transformations, from the left!
	glGetDoublev(GL_MODELVIEW_MATRIX, cameraTrans); // save new rotation
	glPopMatrix(); // reset modelview to original settings

	updateState();
}

// rotate the camera (with respect to its local frame) by the given amount, about the given vector
void Camera::rotateCamera(double angle, double x, double y, double z) {
	updateMatrix();

	glMatrixMode(GL_MODELVIEW); // make sure we have the right matrix selected
	glPushMatrix(); // make a copy
	glLoadIdentity(); // do nothing
	glRotated(-angle, x, y, z);
	glMultMatrixd(cameraTrans); // now add already existing camera transformations, from the left!
	glGetDoublev(GL_MODELVIEW_MATRIX, cameraTrans); // save new rotation
	glPopMatrix(); // reset modelview to original settings

	updateState();
}

// updates the position, lookAt position and up vector based on the transform matrix
void Camera::updateState() {
	/* matrix has form:
		R 0
		t 1
	*/
	eyeXP = cameraTrans[12];
	eyeYP = cameraTrans[14];
	eyeZP = cameraTrans[14];
}

// updates the transform matrix based on the position, lookAt position and up vector
void Camera::updateMatrix() {

}


// returns true iff the key had some effect
bool Camera::controlSpec(int key) {
	switch (key) {
	case GLUT_KEY_UP:
		translateCamera(0, 0.1, 0);
		break;
	case GLUT_KEY_DOWN:
		translateCamera(0, -0.1, 0);
		break;
	case GLUT_KEY_LEFT:
		translateCamera(-0.1, 0, 0);
		break;
	case GLUT_KEY_RIGHT:
		translateCamera(0.1, 0, 0);
		break;
	default:
		return false;
	}
	return true;
}

bool Camera::control(unsigned char key) {
	switch (key) {
	// TODO needs to be camera z direction!!
	case 'I':
		translateCamera(0, 0, 0.1);
		break;
	case 'i':
		translateCamera(0, 0, -0.1);
		break;

	case 't':
		rotateCamera(-10.0, 1.0, 0.0, 0.0);
		break;
	case 'T':
		rotateCamera(10.0, 1.0, 0.0, 0.0);
		break;

	case 'a':
		rotateCamera(-10.0, 0.0, 1.0, 0.0);
		break;
	case 'A':
		rotateCamera(10.0, 0.0, 1.0, 0.0);
		break;

	case 'c':
		rotateCamera(-10.0, 0.0, 0.0, 1.0);
		break;
	case 'C':
		rotateCamera(10.0, 0.0, 0.0, 1.0);
		break;

	default:
		return false;
	}

	return true;
}

Camera::~Camera() {
}

