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

	void moveX(double);
	void moveZ(double);
	void view();
};

#endif /* CAMERA_H_ */
