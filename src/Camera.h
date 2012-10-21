/*
 * Camera.h
 *
 *  Created on: 2012-10-17
 *      Author: david
 */


#ifndef CAMERA_H_
#define CAMERA_H_

class Camera {
private:
	double eyeXP, eyeYP, eyeZP;
	double centerXP, centerYP, centerZP;
	double upXP, upYP, upZP;

public:
	Camera();
	virtual ~Camera();

	bool controlSpec(int key);
	bool control(unsigned char key);
	void rotX(double);
	void rotY(double);
	void rotZ(double);
	void view();
};

#endif /* CAMERA_H_ */
