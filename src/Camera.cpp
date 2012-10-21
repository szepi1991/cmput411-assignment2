/*
 * Camera.cpp
 *
 *  Created on: 2012-10-17
 *      Author: david
 */

#include "Camera.h"
#include <cassert>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265359

// TODO just use my solution from last time .. ?

Camera::Camera() {
	//set the default camera: looking at origin from +5 on z axis
	eyeXP = 8.0, eyeYP = 5.0, eyeZP = 15.0;
	centerXP = 0, centerYP = 0, centerZP = 0.0;
	upXP = 0, upYP = 1, upZP = 0;

}

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

// returns true iff the key had some effect
bool Camera::controlSpec(int key) {
	switch (key) {
	// TODO needs to be camera x-y direction!!
	case GLUT_KEY_UP:
		eyeYP += 0.1;
		break;
	case GLUT_KEY_DOWN:
		eyeYP -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		eyeXP -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		eyeXP += 0.1;
		break;
	default:
		return false;
	}
	return true;
}

// rotate the cam ccw around its own position about the x axis (tilt)
void Camera::rotX(double deg) {
	double rad = deg * PI / 180.0;
	double centerYPold = centerYP - eyeYP, centerZPold = centerZP - eyeZP;
	centerYP = cos(rad) * centerYPold - sin(rad) * centerZPold;
	centerZP = sin(rad) * centerYPold + cos(rad) * centerZPold;
	centerYP += eyeYP; centerZP += eyeZP;
}

// rotate the cam ccw around its own position about the y axis (pan)
void Camera::rotY(double deg) {
	double rad = deg * PI / 180.0;
	double centerXPold = centerXP -eyeXP, centerZPold = centerZP - eyeZP;
	centerXP = cos(rad) * centerXPold - sin(rad) * centerZPold;
	centerZP = sin(rad) * centerXPold + cos(rad) * centerZPold;
	centerXP += eyeXP; centerZP += eyeZP;
}

// rotate the cam ccw around its own position about the z axis (roll)
void Camera::rotZ(double deg) {
	double rad = deg * PI / 180.0;
	double centerXPold = centerXP -eyeXP, centerYPold = centerYP - eyeYP;
	centerXP = cos(rad) * centerXPold - sin(rad) * centerYPold;
	centerYP = sin(rad) * centerXPold + cos(rad) * centerYPold;
	centerXP += eyeXP; centerYP += eyeYP;
}

bool Camera::control(unsigned char key) {
	switch (key) {
	// TODO needs to be camera z direction!!
	case 'I':
		eyeZP += 0.1;
		break;
	case 'i':
		eyeZP -= 0.1;
		break;

	case 't':
		rotX(-10.0);
		break;
	case 'T':
		rotX(10.0);
		break;

	case 'a':
		rotY(-10.0);
		break;
	case 'A':
		rotY(10.0);
		break;

	case 'c':
		rotZ(-10.0);
		break;
	case 'C':
		rotZ(10.0);
		break;

	default:
		return false;
	}

	return true;
}

Camera::~Camera() {
}

